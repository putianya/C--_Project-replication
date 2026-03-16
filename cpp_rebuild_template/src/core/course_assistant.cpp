#include "core/course_assistant.h"
#include <string>

namespace core {

std::string StubCourseAssistant::chat(const std::string& question) {
    // TODO: 接入 LLMClient 并实现真正的课程问答逻辑
    return "[STUB] 收到问题: " + question + " | 请替换为真实 LLM 调用。";
}

std::string StubCourseAssistant::reviewAssignment(const std::string& text) {
    // TODO: 实现基于规则或 LLM 的作业评审
    if (text.empty()) {
        return "{\"status\":\"error\",\"message\":\"提交内容为空\"}";
    }
    return "{\"status\":\"ok\",\"score\":0,\"comment\":\"[STUB] 评审占位，请实现真实逻辑\"}";
}

bool StubCourseAssistant::isHealthy() const {
    return true;
}

} // namespace core
