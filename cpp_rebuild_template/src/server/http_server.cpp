#include "server/http_server.h"
#include <iostream>
#include <unordered_map>

namespace server {

void StubHttpServer::addPostRoute(const std::string& path, Handler handler) {
    postRoutes_[path] = std::move(handler);
}

void StubHttpServer::addGetRoute(const std::string& path, Handler handler) {
    getRoutes_[path] = std::move(handler);
}

void StubHttpServer::listen(int port) {
    // TODO: 替换为真实 HTTP 框架（Drogon / oat++ / Crow）
    std::cout << "[StubServer] 监听端口 " << port
              << " （Stub 模式，未实际绑定端口）\n";
}

void StubHttpServer::stop() {
    std::cout << "[StubServer] 停止。\n";
}

std::string StubHttpServer::dispatchPost(const std::string& path,
                                         const std::string& body) {
    auto it = postRoutes_.find(path);
    if (it == postRoutes_.end()) {
        return "{\"error\":\"route not found\"}";
    }
    return it->second(body);
}

std::string StubHttpServer::dispatchGet(const std::string& path) {
    auto it = getRoutes_.find(path);
    if (it == getRoutes_.end()) {
        return "{\"error\":\"route not found\"}";
    }
    return it->second("");
}

} // namespace server
