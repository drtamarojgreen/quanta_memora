#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <sstream>
#include <map>
#include <cstddef>
#include <fstream>
#include <filesystem>

/**
 * Comprehensive Testing Framework for C++ Template Generator
 * Supports both Unit Testing and Behavior-Driven Development (BDD) patterns
 */

namespace TestFramework {

// Test result tracking
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
    double duration_ms;
    std::string category;
};

// BDD Context for Given-When-Then patterns
struct BDDContext {
    std::string scenario;
    std::string given;
    std::string when;
    std::string then;
    bool setup_complete = false;
    bool action_complete = false;
};

class TestRunner {
private:
    std::vector<TestResult> results;
    std::vector<std::function<void()>> test_functions;
    std::vector<std::function<void()>> setup_functions;
    std::vector<std::function<void()>> teardown_functions;
    std::map<std::string, std::vector<TestResult>> categories;
    BDDContext bdd_context;
    
    // Statistics
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    double total_duration = 0.0;

public:
    // Test registration
    void addTest(const std::string& name, std::function<void()> test_func, const std::string& category = "General");
    void addSetup(std::function<void()> setup_func);
    void addTeardown(std::function<void()> teardown_func);
    
    // BDD Methods
    void scenario(const std::string& description);
    void given(const std::string& context);
    void when(const std::string& action);
    void then(const std::string& expectation);
    
    // Assertion methods
    void assertTrue(bool condition, const std::string& message = "");
    void assertFalse(bool condition, const std::string& message = "");
    void assertEqual(const std::string& expected, const std::string& actual, const std::string& message = "");
    void assertNotEqual(const std::string& expected, const std::string& actual, const std::string& message = "");
    void assertContains(const std::string& haystack, const std::string& needle, const std::string& message = "");
    void assertThrows(std::function<void()> func, const std::string& message = "");
    void assertNoThrow(std::function<void()> func, const std::string& message = "");
    
    // Template-specific assertions
    void assertFileExists(const std::string& filepath, const std::string& message = "");
    void assertFileContains(const std::string& filepath, const std::string& content, const std::string& message = "");
    void assertDirectoryExists(const std::string& dirpath, const std::string& message = "");
    
    // Test execution
    void runAllTests();
    void runCategory(const std::string& category);
    void runTest(const std::string& name);
    
    // Reporting
    void printResults();
    void printDetailedResults();
    void printBDDReport();
    void exportResults(const std::string& filename);
    
    // Utilities
    void clearResults();
    int getPassedCount() const { return passed_tests; }
    int getFailedCount() const { return failed_tests; }
    int getTotalCount() const { return total_tests; }
    double getTotalDuration() const { return total_duration; }
    
private:
    void recordResult(const std::string& name, bool passed, const std::string& message, double duration, const std::string& category);
    std::string getCurrentTimestamp();
    void runSetup();
    void runTeardown();
};

// Macros for easier test writing
#define TEST(name, category) \
    void test_##name(); \
    struct TestRegistrar_##name { \
        TestRegistrar_##name() { \
            TestFramework::getGlobalRunner().addTest(#name, test_##name, category); \
        } \
    }; \
    static TestRegistrar_##name registrar_##name; \
    void test_##name()

#define SETUP() \
    void setup_function(); \
    struct SetupRegistrar { \
        SetupRegistrar() { \
            TestFramework::getGlobalRunner().addSetup(setup_function); \
        } \
    }; \
    static SetupRegistrar setup_registrar; \
    void setup_function()

#define TEARDOWN() \
    void teardown_function(); \
    struct TeardownRegistrar { \
        TeardownRegistrar() { \
            TestFramework::getGlobalRunner().addTeardown(teardown_function); \
        } \
    }; \
    static TeardownRegistrar teardown_registrar; \
    void teardown_function()

// BDD Macros
#define SCENARIO(desc) TestFramework::getGlobalRunner().scenario(desc)
#define GIVEN(context) TestFramework::getGlobalRunner().given(context)
#define WHEN(action) TestFramework::getGlobalRunner().when(action)
#define THEN(expectation) TestFramework::getGlobalRunner().then(expectation)

// Assertion Macros
#define ASSERT_TRUE(condition) TestFramework::getGlobalRunner().assertTrue(condition, #condition)
#define ASSERT_FALSE(condition) TestFramework::getGlobalRunner().assertFalse(condition, #condition)
#define ASSERT_EQ(expected, actual) TestFramework::getGlobalRunner().assertEqual(expected, actual, #expected " == " #actual)
#define ASSERT_NE(expected, actual) TestFramework::getGlobalRunner().assertNotEqual(expected, actual, #expected " != " #actual)
#define ASSERT_CONTAINS(haystack, needle) TestFramework::getGlobalRunner().assertContains(haystack, needle, #haystack " contains " #needle)
#define ASSERT_THROWS(func) TestFramework::getGlobalRunner().assertThrows(func, #func " should throw")
#define ASSERT_NO_THROW(func) TestFramework::getGlobalRunner().assertNoThrow(func, #func " should not throw")
#define ASSERT_FILE_EXISTS(path) TestFramework::getGlobalRunner().assertFileExists(path, "File should exist: " #path)
#define ASSERT_FILE_CONTAINS(path, content) TestFramework::getGlobalRunner().assertFileContains(path, content, #path " should contain " #content)
#define ASSERT_DIR_EXISTS(path) TestFramework::getGlobalRunner().assertDirectoryExists(path, "Directory should exist: " #path)

// Global test runner instance
TestRunner& getGlobalRunner();

// Performance testing utilities
class PerformanceTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string operation_name;
    
public:
    PerformanceTimer(const std::string& name);
    ~PerformanceTimer();
    double getElapsedMs();
};

// Mock utilities for testing
class MockFileSystem {
private:
    std::map<std::string, std::string> mock_files;
    std::vector<std::string> mock_directories;
    
public:
    void addFile(const std::string& path, const std::string& content);
    void addDirectory(const std::string& path);
    bool fileExists(const std::string& path) const;
    bool directoryExists(const std::string& path) const;
    std::string getFileContent(const std::string& path) const;
    void clear();
};

// Test data generators
class TestDataGenerator {
public:
    static std::string generateRandomString(size_t length);
    static std::string generateValidProjectName();
    static std::string generateInvalidProjectName();
    static std::vector<std::string> generateTestFileList();
};

} // namespace TestFramework

#endif // TEST_FRAMEWORK_H
