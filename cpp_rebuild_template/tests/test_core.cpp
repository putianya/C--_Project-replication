#include <gtest/gtest.h>
#include "core/course_assistant.h"

// ─── 测试：StubCourseAssistant::isHealthy ────────────────────────────────────
TEST(StubCourseAssistantTest, IsHealthyReturnsTrue) {
    core::StubCourseAssistant assistant;
    EXPECT_TRUE(assistant.isHealthy());
}

// ─── 测试：chat 返回非空字符串 ────────────────────────────────────────────────
TEST(StubCourseAssistantTest, ChatReturnsNonEmpty) {
    core::StubCourseAssistant assistant;
    std::string reply = assistant.chat("什么是递归？");
    EXPECT_FALSE(reply.empty());
}

// ─── 测试：chat 回复包含原始问题（Stub 行为验证） ────────────────────────────
TEST(StubCourseAssistantTest, ChatReplyContainsQuestion) {
    core::StubCourseAssistant assistant;
    std::string question = "什么是多态";
    std::string reply    = assistant.chat(question);
    EXPECT_NE(reply.find(question), std::string::npos);
}

// ─── 测试：空作业返回 error JSON ──────────────────────────────────────────────
TEST(StubCourseAssistantTest, ReviewEmptyReturnsError) {
    core::StubCourseAssistant assistant;
    std::string result = assistant.reviewAssignment("");
    EXPECT_NE(result.find("error"), std::string::npos);
}

// ─── 测试：非空作业返回 ok JSON ───────────────────────────────────────────────
TEST(StubCourseAssistantTest, ReviewNonEmptyReturnsOk) {
    core::StubCourseAssistant assistant;
    std::string result = assistant.reviewAssignment("int main(){}");
    EXPECT_NE(result.find("ok"), std::string::npos);
}
