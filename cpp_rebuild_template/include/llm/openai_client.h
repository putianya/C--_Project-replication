#pragma once
#include "llm/llm_client.h"
#include <memory>
#include <string>

namespace llm {

/// OpenAI 兼容 HTTP 客户端（Stub 版本）
/// 替换步骤：
///   1. 引入 libcurl 或 cpr 库发送真实 HTTP 请求
///   2. 引入 nlohmann/json 解析响应
///   3. 处理错误重试与超时
class OpenAIClient : public LLMClient {
public:
    explicit OpenAIClient(std::string apiKey,
                          std::string baseUrl = "https://api.openai.com");

    ChatResponse chat(const ChatRequest& request) override;

private:
    std::string apiKey_;
    std::string baseUrl_;
};

/// 工厂函数：通过接口指针创建 OpenAIClient 实例
std::unique_ptr<LLMClient> createOpenAIClient(const std::string& apiKey,
                                              const std::string& baseUrl = "https://api.openai.com");

} // namespace llm
