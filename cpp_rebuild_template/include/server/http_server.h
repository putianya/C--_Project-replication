#pragma once
#include <string>
#include <functional>
#include <unordered_map>

namespace server {

/// HTTP 路由处理器：接收请求体，返回响应体（JSON 字符串）
using Handler = std::function<std::string(const std::string& requestBody)>;

/// HTTP 服务器抽象接口
/// 具体实现可替换为 Drogon / oat++ / Crow
class HttpServer {
public:
    virtual ~HttpServer() = default;

    /// 注册 POST 路由
    virtual void addPostRoute(const std::string& path, Handler handler) = 0;

    /// 注册 GET 路由
    virtual void addGetRoute(const std::string& path, Handler handler) = 0;

    /// 启动监听（阻塞）
    virtual void listen(int port) = 0;

    /// 停止服务器
    virtual void stop() = 0;
};

/// Stub 实现（开发期 + 单元测试用）
class StubHttpServer : public HttpServer {
public:
    void addPostRoute(const std::string& path, Handler handler) override;
    void addGetRoute(const std::string& path, Handler handler) override;
    void listen(int port) override;
    void stop() override;

    /// 测试辅助：手动触发一次请求
    std::string dispatchPost(const std::string& path, const std::string& body);
    std::string dispatchGet(const std::string& path);

private:
    std::unordered_map<std::string, Handler> postRoutes_;
    std::unordered_map<std::string, Handler> getRoutes_;
};

} // namespace server
