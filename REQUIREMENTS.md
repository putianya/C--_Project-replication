# 📄 C++ 复刻「计算思维课程助手」需求文档

> **设计原则：每个模块都对应一个必学的 C++ 核心知识点，做项目的同时系统性地补全技术栈。**

---

## 一、项目概览

| 项 | 内容 |
|---|---|
| 项目名 | `cpp-thinking-assistant` |
| 目标 | 用 C++ 后端复刻原 Flask 项目，前端直接复用原 Vue.js |
| 架构 | C++ HTTP 后端 + SQLite 数据库 + 向量检索 + LLM API |
| 构建工具 | CMake + vcpkg |
| 编译标准 | **C++17** |

---

## 二、目录结构设计

```
cpp-thinking-assistant/
│
├── CMakeLists.txt                  ← 构建入口
├── vcpkg.json                      ← 依赖声明
├── .env                            ← 环境变量
│
├── src/
│   ├── main.cpp                    ← 程序入口
│   ├── config/
│   │   └── Config.hpp              ← 配置类
│   │
│   ├── models/                     ← 数据模型层
│   │   ├── User.hpp / .cpp
│   │   ├── ChatSession.hpp / .cpp
│   │   ├── ChatMessage.hpp / .cpp
│   │   ├── KnowledgeChunk.hpp / .cpp
│   │   ├── LearningRecord.hpp / .cpp
│   │   ├── ErrorPattern.hpp / .cpp
│   │   └── LearningReport.hpp / .cpp
│   │
│   ├── controllers/                ← 路由控制器层
│   │   ├── AuthController.hpp
│   │   ├── ChatController.hpp
│   │   ├── KnowledgeController.hpp
│   │   ├── CodeController.hpp
│   │   ├── AnalyticsController.hpp
│   │   └── AdminController.hpp
│   │
│   ├── services/                   ← 业务逻辑层
│   │   ├── AuthService.hpp / .cpp
│   │   ├── ChatService.hpp / .cpp
│   │   ├── LLMService.hpp / .cpp
│   │   ├── VectorService.hpp / .cpp
│   │   ├── CodeService.hpp / .cpp
│   │   └── analytics/
│   │       ├── DataCollector.hpp / .cpp
│   │       ├── StatsCalculator.hpp / .cpp
│   │       └── WeaknessAnalyzer.hpp / .cpp
│   │
│   ├── middleware/                 ← 中间件
│   │   ├── AuthMiddleware.hpp
│   │   └── CorsMiddleware.hpp
│   │
│   └── utils/                     ← 工具库
│       ├── JwtHelper.hpp
│       ├── PasswordHelper.hpp
│       ├── Validators.hpp
│       └── ThreadPool.hpp          ← ⭐ 自己实现
│
├── tests/                          ← 单元测试
│   ├── test_auth.cpp
│   ├── test_vector.cpp
│   └── test_code_service.cpp
│
└── frontend/                       ← 直接复用原 Vue.js 前端
```

---

## 三、功能模块 × C++ 知识点映射

> **这是文档最核心的部分。每个功能模块都对应你需要学习的 C++ 知识。**

---

### 模块 1：构建系统与工程化
**对应功能：** 整个项目的基础  
**需要学习的 C++ 知识：**

```
✅ CMake 现代写法（target_link_libraries、find_package、FetchContent）
✅ vcpkg 依赖管理（vcpkg.json manifest 模式）
✅ 头文件与实现分离（.hpp / .cpp）
✅ 编译单元与链接原理
✅ 预处理器与条件编译（#pragma once、#ifdef）
```

**具体任务：**
- 编写 `CMakeLists.txt`，用 vcpkg 引入所有第三方依赖
- 配置 Debug / Release 两种构建模式
- 配置 GTest 测试框架

---

### 模块 2：配置管理
**对应功能：** 原 `config.py`  
**需要学习的 C++ 知识：**

```
✅ 单例模式（Singleton）的正确 C++ 实现（Meyer's Singleton）
✅ std::string 与环境变量读取（getenv）
✅ std::optional<T> 处理可能不存在的配置项
✅ constexpr 常量与 const 的区别
✅ 结构化绑定（C++17）
```

**具体任务：**
```cpp
// 使用 Meyer's Singleton + std::optional
class Config {
public:
    static Config& getInstance();
    std::string openaiApiKey;
    std::optional<int> maxTokens;
    // ...
private:
    Config();  // 从 .env 文件读取
};
```

---

### 模块 3：HTTP 服务器与路由
**对应功能：** 原 Flask 的 Blueprint 路由系统  
**使用库：** [Drogon](https://github.com/drogonframework/drogon)  
**需要学习的 C++ 知识：**

```
✅ Lambda 表达式（捕获列表、值捕获 vs 引用捕获）
✅ std::function 与回调函数
✅ 右值引用与移动语义（std::move）
✅ 异步编程基础（async/await 等价：Drogon 的 coroutine）
✅ HTTP 协议基础（方法、状态码、Header）
✅ CORS 跨域处理
✅ C++17 结构化绑定解包 JSON 参数
```

**具体任务：**
```cpp
// AuthController.hpp — 对标 auth_bp
app().registerHandler("/api/auth/login",
    [](const HttpRequestPtr& req, Callback&& callback) {
        auto json = req->getJsonObject();
        // ...
    }, {Post});
```

所有路由：
| 原 Flask 路由 | C++ 控制器 |
|---|---|
| `/api/auth/register` `login` `logout` `verify` | `AuthController` |
| `/api/chat/stream` + SSE | `ChatController` |
| `/api/sessions` CRUD | `ChatController` |
| `/api/knowledge/upload` `documents` | `KnowledgeController` |
| `/api/code/analyze` `chat` | `CodeController` |
| `/api/analytics/*` | `AnalyticsController` |
| `/api/admin/users` CRUD | `AdminController` |

---

### 模块 4：数据库 ORM
**对应功能：** 原 SQLAlchemy 模型层（7 张表）  
**使用库：** [SQLiteCpp](https://github.com/SRombauts/SQLiteCpp)  
**需要学习的 C++ 知识：**

```
✅ 类的封装：成员变量私有化、getter/setter
✅ 构造函数：默认构造、拷贝构造、移动构造
✅ 运算符重载（用于模型比较）
✅ std::vector<T> 作为查询结果集
✅ std::unordered_map<K,V> 用于缓存查询结果
✅ RAII：数据库连接的生命周期管理
✅ 异常处理（try/catch/throw，自定义异常类）
✅ 模板函数（泛型查询方法）
```

**需要实现的数据模型（对应原 7 张表）：**

| C++ 类 | 对应数据库表 | 关键字段 |
|---|---|---|
| `User` | `users` | id, username, password_hash, role, is_active |
| `ChatSession` | `chat_sessions` | session_id, user_id, title, is_active |
| `ChatMessage` | `chat_messages` | session_id, role, content, referenced_chunks |
| `KnowledgeChunk` | `knowledge_chunks` | content, source, chapter, vector_id, quality_score |
| `LearningRecord` | `learning_records` | user_id, action_type, content, is_correct |
| `ErrorPattern` | `error_patterns` | user_id, error_type, occurrence_count |
| `LearningReport` | `learning_reports` | user_id, statistics(JSON), overall_score |

---

### 模块 5：JWT 认证与权限中间件
**对应功能：** 原 `utils/decorators.py` 中的 `@login_required` 和 `@require_permission`  
**使用库：** [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) + OpenSSL  
**需要学习的 C++ 知识：**

```
✅ 函数装饰器模式的 C++ 等价：高阶函数 + std::function
✅ 模板元编程基础：函数包装器
✅ std::variant（C++17）处理权限结果（成功/失败/无权限）
✅ OpenSSL HMAC-SHA256（密码哈希）
✅ 字符串处理：std::string_view（零拷贝）
✅ 正则表达式：std::regex 验证 Token 格式
```

**具体任务：**
- 实现 `JwtHelper`：生成/验证 Token
- 实现 `PasswordHelper`：bcrypt 加密/验证密码
- 实现 `AuthMiddleware`：在 Drogon 请求链中验证 JWT
- 实现权限表（对应原 `User.PERMISSIONS` 字典）

---

### 模块 6：流式对话（SSE）
**对应功能：** 原 `/api/chat/stream` + `LLMService`  
**使用库：** libcurl + nlohmann/json  
**需要学习的 C++ 知识：**

```
✅ 多线程：std::thread、std::mutex、std::condition_variable
✅ 生产者-消费者模式（对应 SSE 流式输出）
✅ std::atomic<bool> 控制流停止
✅ 回调函数：libcurl 的 CURLOPT_WRITEFUNCTION
✅ 字符串流：std::ostringstream 拼接响应
✅ 智能指针：shared_ptr 在多线程中的引用计数
✅ RAII 封装 libcurl（防止资源泄漏）
```

**具体任务：**
- 用 libcurl 调用 DeepSeek / OpenAI HTTP API
- 解析 SSE 数据流（`data: {...}` 格式）
- 通过 Drogon 的 `HttpResponse::newStreamResponse` 转发给前端

---

### 模块 7：RAG 向量检索
**对应功能：** 原 `VectorService`（ChromaDB + sentence-transformers）  
**使用库：** [hnswlib](https://github.com/nmslib/hnswlib)（向量检索）+ 外部 Embedding API  
**需要学习的 C++ 知识：**

```
✅ 模板类（泛型容器）：hnswlib 的 HierarchicalNSW<float>
✅ std::vector<float> 向量运算（余弦相似度手动实现）
✅ 文件 I/O：std::fstream 持久化向量索引
✅ 内存布局：理解 float[] 向量在内存中的存储
✅ 单例模式：VectorService 全局唯一实例
✅ 并发安全：读写锁 std::shared_mutex（多线程检索）
✅ RAII + unique_ptr 管理向量索引生命周期
```

**具体任务：**
- 调用 HTTP Embedding API 获取文本向量（libcurl）
- 用 hnswlib 构建 HNSW 索引并持久化到磁盘
- 实现余弦相似度检索 + 阈值过滤
- 实现知识块 CRUD 同步（数据库 ↔ 向量索引）

---

### 模块 8：代码分析服务
**对应功能：** 原 `CodeService`（Python AST 分析 + LLM 分析）  
**需要学习的 C++ 知识：**

```
✅ 正则表达式：std::regex 检测语法模式
✅ 字符串算法：统计行数、函数数量、复杂度估算
✅ std::map<string, int> 统计关键字频次
✅ 策略模式（Strategy Pattern）：不同分析类型的 C++ 实现
✅ nlohmann/json：构造嵌套 JSON 分析报告
✅ 异常安全编程（strong guarantee）
```

**具体任务：**
- 实现 C 代码静态特征提取（替换原 Python AST）：
  - 行数、函数数量、嵌套深度
  - 关键字统计（for/while/if/pointer）
  - 基础语法错误检测（括号匹配、分号缺失）
- 调用 LLM API 进行深度分析
- 输出结构化评分报告（JSON）

---

### 模块 9：学习数据分析
**对应功能：** 原 `analytics/` 模块（DataCollector + StatsCalculator + WeaknessAnalyzer）  
**需要学习的 C++ 知识：**

```
✅ std::chrono：时间点、时长计算（替换 Python datetime）
✅ 算法库：std::sort、std::accumulate、std::transform
✅ 函数式编程风格：std::for_each + lambda
✅ 数值计算：统计学基础（均值、标准差）
✅ nlohmann/json 序列化复杂嵌套结构
✅ 数据库聚合查询的 C++ 封装
```

**具体任务：**
- 实现学习心跳记录（`/api/analytics/heartbeat`）
- 实现学习概览统计（提问次数、代码提交数、学习时长）
- 实现知识点掌握度计算（正确率分析）
- 实现薄弱点分析（高频错误提取）
- 实现学习趋势（按日/周统计）

---

### 模块 10：线程池与并发
**对应功能：** 支撑高并发请求的基础设施  
**需要学习的 C++ 知识：**

```
✅ std::thread 线程创建与管理
✅ std::mutex + std::lock_guard（RAII 锁）
✅ std::condition_variable 线程同步
✅ std::future + std::promise 异步结果传递
✅ std::atomic<T> 无锁编程基础
✅ 移动语义在线程中的应用（任务的 std::move）
```

**具体任务：**
- 实现通用 `ThreadPool`（固定大小线程池）
- 用于：向量化请求的批处理、LLM 请求的并发限制

---

### 模块 11：单元测试
**对应功能：** 原 `tests/` 目录  
**使用库：** Google Test (GTest)  
**需要学习的 C++ 知识：**

```
✅ GTest 基本用法：TEST、EXPECT_EQ、ASSERT_TRUE
✅ Mock 对象（gmock）：模拟数据库/HTTP 调用
✅ 测试夹具（Test Fixture）：setUp/tearDown
✅ 参数化测试
```

**需要编写的测试：**
- `test_auth.cpp`：JWT 生成/验证、密码加密
- `test_vector.cpp`：向量添加/检索/相似度
- `test_code_service.cpp`：代码特征提取、评分计算
- `test_models.cpp`：数据库 CRUD 操作

---

## 四、第三方库依赖清单

```json
// vcpkg.json
{
  "dependencies": [
    "drogon",          // Web 框架（替换 Flask）
    "sqlite3",         // 数据库
    "sqlitecpp",       // SQLite C++ 封装
    "nlohmann-json",   // JSON 处理
    "jwt-cpp",         // JWT 认证
    "libcurl",         // HTTP 客户端（调 LLM API）
    "openssl",         // 密码加密
    "hnswlib",         // 向量检索
    "gtest",           // 单元测试
    "spdlog"           // 日志库
  ]
}
```

---

## 五、开发阶段规划

```
阶段一：工程基础（1周）
├── CMake + vcpkg 构建环境
├── Config 单例
├── spdlog 日志
└── 跑通 Hello World HTTP 服务

阶段二：认证系统（1周）
├── User 模型 + SQLiteCpp
├── JWT 生成/验证
├── 密码 bcrypt 加密
└── /api/auth/* 全部接口

阶段三：对话核心（1.5周）
├── ChatSession / ChatMessage 模型
├── libcurl 调用 LLM API
├── SSE 流式输出
└── /api/chat/* + /api/sessions/* 全部接口

阶段四：知识库 RAG（1.5周）
├── KnowledgeChunk 模型
├── Embedding API 调用
├── hnswlib 向量索引
└── /api/knowledge/* 全部接口

阶段五：代码分析（1周）
├── C 代码特征提取（正则）
├── LLM 分析接口
└── /api/code/* 全部接口

阶段六：数据分析（1周）
├── LearningRecord / ErrorPattern 模型
├── 统计计算
└── /api/analytics/* 全部接口

阶段七：管理功能 + 测试（0.5周）
├── AdminController
├── GTest 单元测试
└── 性能对比报告（vs Python 版）
```

---

## 六、C++ 知识点总览（学习 Checklist）

```
基础语言特性：
□ 现代 C++17 特性（structured bindings、if constexpr、std::optional）
□ RAII 与资源管理
□ 智能指针（unique_ptr / shared_ptr / weak_ptr）
□ 移动语义与右值引用
□ Lambda 表达式与 std::function

并发编程：
□ std::thread / std::mutex / std::lock_guard
□ std::condition_variable
□ std::atomic<T>
□ std::future / std::promise
□ 线程池实现

标准库：
□ std::string / std::string_view
□ std::vector / std::unordered_map
□ std::chrono 时间库
□ std::regex 正则表达式
□ std::filesystem 文件操作
□ STL 算法（sort/transform/accumulate）

设计模式：
□ 单例模式
□ 策略模式
□ 工厂模式（服务注册中心 ServiceRegistry）
□ RAII 模式
□ 观察者模式（SSE 推送）

工程实践：
□ CMake 现代写法
□ vcpkg 依赖管理
□ GTest 单元测试
□ spdlog 结构化日志
□ .env 配置读取
```
