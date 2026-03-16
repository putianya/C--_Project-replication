#pragma once
#include <string>
#include <vector>

namespace core {

/// 单次对话消息
struct Message {
    std::string role;    // "user" | "assistant" | "system"
    std::string content;
};

/// 课程助手接口（业务核心层）
class CourseAssistant {
public:
    virtual ~CourseAssistant() = default;

    /// 处理学生提问，返回助手回复
    virtual std::string chat(const std::string& question) = 0;

    /// 作业批改（规则版占位）
    virtual std::string reviewAssignment(const std::string& text) = 0;

    /// 健康检查
    virtual bool isHealthy() const = 0;
};

/// 默认 stub 实现（开发期占位）
class StubCourseAssistant : public CourseAssistant {
public:
    std::string chat(const std::string& question) override;
    std::string reviewAssignment(const std::string& text) override;
    bool isHealthy() const override;
};

} // namespace core
