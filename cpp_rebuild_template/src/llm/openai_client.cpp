#include "llm/openai_client.h"
#include <string>

namespace llm {

OpenAIClient::OpenAIClient(std::string apiKey, std::string baseUrl)
    : apiKey_(std::move(apiKey)), baseUrl_(std::move(baseUrl)) {}

ChatResponse OpenAIClient::chat(const ChatRequest& request) {
    // TODO: 替换为真实 HTTP POST 到 baseUrl_ + "/v1/chat/completions"
    // 示例请求体（nlohmann/json）：
    // json body = {
    //     {"model", request.model},
    //     {"temperature", request.temperature},
    //     {"max_tokens", request.maxTokens},
    //     {"messages", {
    //         {{"role","system"},{"content", request.systemPrompt}},
    //         {{"role","user"},  {"content", request.userMessage}}
    //     }}
    // };

    (void)request; // 消除 unused 警告（Stub 阶段）
    ChatResponse resp;
    resp.success = true;
    resp.content = "[STUB] OpenAI 响应占位 | 接入真实 API 后删除此行";
    return resp;
}

std::unique_ptr<LLMClient> createOpenAIClient(const std::string& apiKey,
                                              const std::string& baseUrl) {
    return std::make_unique<OpenAIClient>(apiKey, baseUrl);
}

} // namespace llm
