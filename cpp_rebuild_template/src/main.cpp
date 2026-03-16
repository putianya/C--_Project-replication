#include "core/course_assistant.h"
#include "server/http_server.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== C++ 课程助手模板启动 ===\n";

    // 1. 初始化业务核心（Stub 版）
    auto assistant = std::make_unique<core::StubCourseAssistant>();

    // 2. 初始化 HTTP 服务器（Stub 版；生产时替换为 Drogon/oat++/Crow）
    auto httpServer = std::make_unique<server::StubHttpServer>();

    // 3. 注册路由
    httpServer->addGetRoute("/health", [&](const std::string& /*body*/) {
        bool ok = assistant->isHealthy();
        return ok ? "{\"status\":\"ok\"}" : "{\"status\":\"degraded\"}";
    });

    httpServer->addPostRoute("/chat", [&](const std::string& body) {
        // TODO: 用 nlohmann/json 解析 body 中的 "question" 字段
        std::string reply = assistant->chat(body);
        return "{\"answer\":\"" + reply + "\"}";
    });

    httpServer->addPostRoute("/review", [&](const std::string& body) {
        return assistant->reviewAssignment(body);
    });

    // 4. 启动服务（Stub 不真正监听；替换框架后此处阻塞）
    httpServer->listen(8080);

    // 5. 演示一次本地请求（开发期自测）
    std::cout << "GET /health -> "
              << httpServer->dispatchGet("/health") << "\n";
    std::cout << "POST /chat  -> "
              << httpServer->dispatchPost("/chat", "什么是递归？") << "\n";
    std::cout << "POST /review -> "
              << httpServer->dispatchPost("/review", "int main(){}") << "\n";

    std::cout << "=== 模板运行完毕 ===\n";
    return 0;
}
