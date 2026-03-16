#pragma once
#include <string>
#include <optional>

namespace llm {

/// LLM 推理请求参数
struct ChatRequest {
    std::string model       = "gpt-4o-mini"; // 模型名称（可替换）
    std::string userMessage;
    std::string systemPrompt = "You are a helpful assistant.";
    float       temperature  = 0.7f;
    int         maxTokens    = 512;
};

/// LLM 推理结果
struct ChatResponse {
    bool        success = false;
    std::string content;
    std::string errorMessage;
};

/// LLM 客户端抽象接口
/// 实现者可对接：OpenAI / DeepSeek / 本地 llama.cpp HTTP 服务
class LLMClient {
public:
    virtual ~LLMClient() = default;
    virtual ChatResponse chat(const ChatRequest& request) = 0;
};

} // namespace llm
