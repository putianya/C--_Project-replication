# C++ 课程助手复刻模板

> 面向"熟悉 C++ 基础语法与 STL"的学习者，帮助你系统打通现代 C++、工程化与大模型接入能力。

## 目录结构

```
cpp_rebuild_template/
├── CMakeLists.txt          # CMake 构建配置（C++17，含 GoogleTest）
├── include/                # 公共头文件（接口抽象）
│   ├── core/course_assistant.h
│   ├── llm/llm_client.h
│   └── server/http_server.h
├── src/
│   ├── main.cpp            # 最小可运行入口
│   ├── core/               # 业务核心占位实现
│   ├── llm/                # LLM 客户端接口 + OpenAI-style Stub
│   └── server/             # HTTP 服务占位实现
├── tests/
│   └── test_core.cpp       # GoogleTest 单元测试
└── docs/
    ├── 需求分析说明书.md
    ├── 开发路线图.md
    ├── 技术选型.md
    ├── 接口设计.md
    └── 测试与质量保障.md
```

## 快速开始

### 前置依赖

| 工具 | 最低版本 | 说明 |
|------|---------|------|
| CMake | 3.16 | 构建系统 |
| GCC / Clang | 支持 C++17 | Linux/macOS |
| MSVC | 2019+ | Windows |
| Git | 2.x | 拉取 GoogleTest（首次联网） |

### 编译与运行

```bash
# 进入模板目录
cd cpp_rebuild_template

# 配置（默认 Debug；加 -DCMAKE_BUILD_TYPE=Release 切换）
cmake -S . -B build

# 编译
cmake --build build

# 运行主程序
./build/CppRebuildTemplate

# 运行测试
cd build && ctest --output-on-failure
```

### 开启 Sanitizer（排查内存/未定义行为，需手动启用）

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON
cmake --build build
./build/CppRebuildTemplate
```

### 升级到 C++20

在 `CMakeLists.txt` 中将：

```cmake
set(CMAKE_CXX_STANDARD 17)
```

改为：

```cmake
set(CMAKE_CXX_STANDARD 20)
```

C++20 新特性可逐步引入：`concepts`、`ranges`、`coroutines`、`std::format`。

## 核心能力学习路径

```
C++ 基础/STL
    ↓
现代 C++ 特性（11/14/17/20）
    ↓
CMake 工程化 + 单元测试
    ↓
HTTP 服务 + JSON 解析
    ↓
LLM 云 API 接入
    ↓
并发改造（线程池）
    ↓
本地推理 / RAG 演进
```

详见 [`docs/开发路线图.md`](docs/开发路线图.md)。

## 文档索引

| 文档 | 说明 |
|------|------|
| [需求分析说明书](docs/需求分析说明书.md) | 背景、功能需求、非功能需求、架构设计 |
| [开发路线图](docs/开发路线图.md) | 8~12 周按周可执行计划 |
| [技术选型](docs/技术选型.md) | HTTP 框架、JSON、日志、测试框架对比 |
| [接口设计](docs/接口设计.md) | API 规范、数据结构定义 |
| [测试与质量保障](docs/测试与质量保障.md) | 单元/集成/压力测试、CI 基线 |

## 贡献与扩展

替换占位实现的步骤：

1. 选择 HTTP 框架（推荐先用 [Crow](https://github.com/CrowCpp/Crow)）
2. 添加 `nlohmann/json` 依赖，解析真实请求体
3. 实现 `OpenAIClient::chat()` 中的真实 HTTP 调用
4. 逐步用真实业务逻辑替换 `StubCourseAssistant`

详见 [`docs/技术选型.md`](docs/技术选型.md)。
