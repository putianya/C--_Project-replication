cmake_minimum_required(VERSION 3.16)
project(CppRebuildTemplate VERSION 0.1.0 LANGUAGES CXX)

# ── 语言标准（C++17；升级 C++20 只需改为 20） ──────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── 编译警告 ───────────────────────────────────────────────────────────────────
# 统一警告选项；Windows/MSVC 用对等选项
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Sanitizer（可通过 -DENABLE_SANITIZERS=ON 手动开启，仅 GCC/Clang） ──────────
option(ENABLE_SANITIZERS "Enable ASan + UBSan in Debug builds" OFF)
if(ENABLE_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# ── 头文件公共路径 ─────────────────────────────────────────────────────────────
include_directories(${PROJECT_SOURCE_DIR}/include)

# ── 依赖（可选；注释掉不影响编译） ──────────────────────────────────────────────
# find_package(nlohmann_json 3.11 REQUIRED)
# find_package(spdlog        1.12 REQUIRED)
# find_package(fmt           10   REQUIRED)

# ── 业务静态库（core / llm / server） ────────────────────────────────────────────
add_library(core_lib STATIC
    src/core/course_assistant.cpp
)
target_include_directories(core_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(llm_lib STATIC
    src/llm/llm_client.cpp
    src/llm/openai_client.cpp
)
target_include_directories(llm_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(server_lib STATIC
    src/server/http_server.cpp
)
target_include_directories(server_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ── 可执行主程序 ───────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE core_lib llm_lib server_lib)

# ── 测试（GoogleTest；可通过 -DBUILD_TESTING=OFF 关闭） ─────────────────────────
include(CTest)
if(BUILD_TESTING)
    # 如果系统已安装 GTest，直接 find_package；否则使用 FetchContent 自动下载
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.14.0
        )
        # 禁止 GTest 污染 install 目标
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    add_executable(unit_tests
        tests/test_core.cpp
    )
    target_link_libraries(unit_tests PRIVATE core_lib llm_lib GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()
