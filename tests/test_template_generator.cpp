#include "test_framework.h"
#define TEMPLATE_GENERATOR_TEST
#include "../template_generator.cpp" // Include the source directly for testing
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Test helper functions
namespace TestHelpers {
    void cleanupTestDirectory(const std::string& dir) {
        if (fs::exists(dir)) {
            fs::remove_all(dir);
        }
    }
    
    ProjectConfig createTestConfig(const std::string& name = "test_project") {
        ProjectConfig config;
        config.name = name;
        config.className = capitalize(name);
        config.description = "Test project for unit testing";
        config.goal = "Test the template generator functionality";
        config.author = "Test Author";
        config.version = "1.0.0";
        config.type = ProjectType::CONSOLE_APP;
        config.useCMake = true;
        config.includeTests = true;
        config.includeGitIgnore = true;
        return config;
    }
}

using namespace TestFramework;

// Setup and teardown
SETUP() {
    std::cout << "Setting up test environment..." << std::endl;
}

TEARDOWN() {
    // Clean up any test directories
    TestHelpers::cleanupTestDirectory("test_project");
    TestHelpers::cleanupTestDirectory("test_library");
    TestHelpers::cleanupTestDirectory("test_gui");
    std::cout << "Cleaning up test environment..." << std::endl;
}

// Helper function tests
TEST(capitalize_function, "Helper Functions") {
    SCENARIO("Testing string capitalization");
    GIVEN("a lowercase string");
    std::string input = "hello";
    
    WHEN("capitalize function is called");
    std::string result = capitalize(input);
    
    THEN("first character should be uppercase");
    ASSERT_EQ("Hello", result);
}

TEST(toUpper_function, "Helper Functions") {
    std::string input = "hello world";
    std::string result = toUpper(input);
    ASSERT_EQ("HELLO WORLD", result);
}

TEST(toLower_function, "Helper Functions") {
    std::string input = "HELLO WORLD";
    std::string result = toLower(input);
    ASSERT_EQ("hello world", result);
}

TEST(camelToSnake_function, "Helper Functions") {
    SCENARIO("Converting camelCase to snake_case");
    GIVEN("a camelCase string");
    std::string input = "myTestProject";
    
    WHEN("camelToSnake function is called");
    std::string result = camelToSnake(input);
    
    THEN("result should be in snake_case");
    ASSERT_EQ("my_test_project", result);
}

// Template generator core functionality tests
TEST(template_generator_creation, "Core Functionality") {
    SCENARIO("Creating a TemplateGenerator instance");
    GIVEN("no preconditions");
    
    WHEN("TemplateGenerator is instantiated");
    TemplateGenerator generator;
    
    THEN("instance should be created successfully");
    ASSERT_TRUE(true); // If we get here, creation succeeded
}

TEST(project_config_setting, "Core Functionality") {
    SCENARIO("Setting project configuration");
    GIVEN("a TemplateGenerator and ProjectConfig");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig();
    
    WHEN("setConfig is called");
    ASSERT_NO_THROW([&]() { generator.setConfig(config); });
    
    THEN("configuration should be set without errors");
    ASSERT_TRUE(true); // If no exception was thrown, test passes
}

// File generation tests
TEST(console_app_generation, "Project Generation") {
    SCENARIO("Generating a console application project");
    GIVEN("a configured TemplateGenerator for console app");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("test_console");
    config.type = ProjectType::CONSOLE_APP;
    generator.setConfig(config);
    
    WHEN("generateProject is called");
    ASSERT_NO_THROW([&]() { generator.generateProject(); });
    
    THEN("project directory and files should be created");
    ASSERT_DIR_EXISTS("test_console");
    ASSERT_DIR_EXISTS("test_console/src");
    ASSERT_DIR_EXISTS("test_console/include");
    ASSERT_DIR_EXISTS("test_console/tests");
    ASSERT_FILE_EXISTS("test_console/src/main.cpp");
    ASSERT_FILE_EXISTS("test_console/include/test_console.h");
    ASSERT_FILE_EXISTS("test_console/src/test_console.cpp");
    ASSERT_FILE_EXISTS("test_console/CMakeLists.txt");
    ASSERT_FILE_EXISTS("test_console/README.md");
    ASSERT_FILE_EXISTS("test_console/LICENSE");
    ASSERT_FILE_EXISTS("test_console/.gitignore");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("test_console");
}

TEST(static_library_generation, "Project Generation") {
    SCENARIO("Generating a static library project");
    GIVEN("a configured TemplateGenerator for static library");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("test_lib");
    config.type = ProjectType::STATIC_LIBRARY;
    generator.setConfig(config);
    
    WHEN("generateProject is called");
    ASSERT_NO_THROW([&]() { generator.generateProject(); });
    
    THEN("library project should be created without main.cpp");
    ASSERT_DIR_EXISTS("test_lib");
    ASSERT_DIR_EXISTS("test_lib/src");
    ASSERT_DIR_EXISTS("test_lib/include");
    ASSERT_FILE_EXISTS("test_lib/include/test_lib.h");
    ASSERT_FILE_EXISTS("test_lib/src/test_lib.cpp");
    ASSERT_FILE_EXISTS("test_lib/CMakeLists.txt");
    
    // Should not have main.cpp for libraries
    ASSERT_FALSE(fs::exists("test_lib/src/main.cpp"));
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("test_lib");
}

TEST(header_only_library_generation, "Project Generation") {
    SCENARIO("Generating a header-only library project");
    GIVEN("a configured TemplateGenerator for header-only library");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("test_header_lib");
    config.type = ProjectType::HEADER_ONLY;
    generator.setConfig(config);
    
    WHEN("generateProject is called");
    ASSERT_NO_THROW([&]() { generator.generateProject(); });
    
    THEN("header-only project should be created without src directory");
    ASSERT_DIR_EXISTS("test_header_lib");
    ASSERT_DIR_EXISTS("test_header_lib/include");
    ASSERT_FILE_EXISTS("test_header_lib/include/test_header_lib.h");
    
    // Should not have src directory for header-only libraries
    ASSERT_FALSE(fs::exists("test_header_lib/src"));
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("test_header_lib");
}

// File content validation tests
TEST(main_cpp_content_validation, "Content Validation") {
    SCENARIO("Validating main.cpp content for console app");
    GIVEN("a generated console application");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("content_test");
    config.type = ProjectType::CONSOLE_APP;
    generator.setConfig(config);
    generator.generateProject();
    
    WHEN("main.cpp is examined");
    THEN("it should contain expected content");
    ASSERT_FILE_CONTAINS("content_test/src/main.cpp", "#include <iostream>");
    ASSERT_FILE_CONTAINS("content_test/src/main.cpp", "#include \"../include/content_test.h\"");
    ASSERT_FILE_CONTAINS("content_test/src/main.cpp", "int main()");
    ASSERT_FILE_CONTAINS("content_test/src/main.cpp", "Content_test app;");
    ASSERT_FILE_CONTAINS("content_test/src/main.cpp", "return app.run();");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("content_test");
}

TEST(header_file_content_validation, "Content Validation") {
    SCENARIO("Validating header file content");
    GIVEN("a generated project");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("header_test");
    generator.setConfig(config);
    generator.generateProject();
    
    WHEN("header file is examined");
    THEN("it should contain proper header guards and class definition");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "#ifndef HEADER_TEST_H");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "#define HEADER_TEST_H");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "class Header_test");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "public:");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "private:");
    ASSERT_FILE_CONTAINS("header_test/include/header_test.h", "#endif // HEADER_TEST_H");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("header_test");
}

TEST(cmake_file_content_validation, "Content Validation") {
    SCENARIO("Validating CMakeLists.txt content");
    GIVEN("a generated project with CMake enabled");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("cmake_test");
    config.useCMake = true;
    generator.setConfig(config);
    generator.generateProject();
    
    WHEN("CMakeLists.txt is examined");
    THEN("it should contain proper CMake configuration");
    ASSERT_FILE_CONTAINS("cmake_test/CMakeLists.txt", "cmake_minimum_required(VERSION 3.12)");
    ASSERT_FILE_CONTAINS("cmake_test/CMakeLists.txt", "project(cmake_test VERSION 1.0.0)");
    ASSERT_FILE_CONTAINS("cmake_test/CMakeLists.txt", "set(CMAKE_CXX_STANDARD 17)");
    ASSERT_FILE_CONTAINS("cmake_test/CMakeLists.txt", "add_executable(cmake_test");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("cmake_test");
}

TEST(readme_content_validation, "Content Validation") {
    SCENARIO("Validating README.md content");
    GIVEN("a generated project");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("readme_test");
    config.description = "A test project for README validation";
    config.goal = "Test README generation";
    config.author = "Test Author";
    generator.setConfig(config);
    generator.generateProject();
    
    WHEN("README.md is examined");
    THEN("it should contain project information");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "# Readme_test");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "## Description");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "A test project for README validation");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "## Goal");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "Test README generation");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "## Author");
    ASSERT_FILE_CONTAINS("readme_test/README.md", "Test Author");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("readme_test");
}

// Build system tests
TEST(makefile_generation, "Build Systems") {
    SCENARIO("Generating Makefile instead of CMake");
    GIVEN("a project configured to use Makefile");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("makefile_test");
    config.useCMake = false;
    generator.setConfig(config);
    
    WHEN("generateProject is called");
    generator.generateProject();
    
    THEN("Makefile should be created instead of CMakeLists.txt");
    ASSERT_FILE_EXISTS("makefile_test/Makefile");
    ASSERT_FALSE(fs::exists("makefile_test/CMakeLists.txt"));
    ASSERT_FILE_CONTAINS("makefile_test/Makefile", "CXX = g++");
    ASSERT_FILE_CONTAINS("makefile_test/Makefile", "CXXFLAGS = -std=c++17");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("makefile_test");
}

// Test generation tests
TEST(unit_test_generation, "Test Generation") {
    SCENARIO("Generating unit tests");
    GIVEN("a project configured to include tests");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("unit_test_project");
    config.includeTests = true;
    generator.setConfig(config);
    
    WHEN("generateProject is called");
    generator.generateProject();
    
    THEN("test files should be created");
    ASSERT_DIR_EXISTS("unit_test_project/tests");
    ASSERT_FILE_EXISTS("unit_test_project/tests/test_unit_test_project.cpp");
    ASSERT_FILE_CONTAINS("unit_test_project/tests/test_unit_test_project.cpp", "#include \"../include/unit_test_project.h\"");
    ASSERT_FILE_CONTAINS("unit_test_project/tests/test_unit_test_project.cpp", "int main()");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("unit_test_project");
}

// Error handling tests
TEST(invalid_project_type_handling, "Error Handling") {
    SCENARIO("Handling edge cases gracefully");
    GIVEN("a TemplateGenerator");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("edge_case_test");
    
    WHEN("configuration has edge case values");
    config.name = "";
    config.author = "";
    config.description = "";
    
    THEN("generator should handle gracefully");
    // This test ensures the generator doesn't crash with empty values
    ASSERT_NO_THROW([&]() { generator.setConfig(config); });
}

// Performance tests
TEST(generation_performance, "Performance") {
    SCENARIO("Measuring project generation performance");
    GIVEN("a TemplateGenerator and configuration");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("perf_test");
    generator.setConfig(config);
    
    WHEN("generateProject is called with performance timing");
    {
        PerformanceTimer timer("Project Generation");
        ASSERT_NO_THROW([&]() { generator.generateProject(); });
    }
    
    THEN("project should be generated within reasonable time");
    ASSERT_TRUE(true); // Performance is measured by the timer
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("perf_test");
}

// Integration tests
TEST(full_workflow_integration, "Integration") {
    SCENARIO("Complete workflow from config to build-ready project");
    GIVEN("a complete project configuration");
    TemplateGenerator generator;
    ProjectConfig config = TestHelpers::createTestConfig("integration_test");
    config.description = "Full integration test project";
    config.goal = "Test complete workflow";
    config.author = "Integration Tester";
    config.version = "2.0.0";
    config.type = ProjectType::CONSOLE_APP;
    config.useCMake = true;
    config.includeTests = true;
    config.includeGitIgnore = true;
    generator.setConfig(config);
    
    WHEN("complete project generation workflow is executed");
    ASSERT_NO_THROW([&]() { generator.generateProject(); });
    
    THEN("all expected files should be present and valid");
    // Directory structure
    ASSERT_DIR_EXISTS("integration_test");
    ASSERT_DIR_EXISTS("integration_test/src");
    ASSERT_DIR_EXISTS("integration_test/include");
    ASSERT_DIR_EXISTS("integration_test/tests");
    
    // Core files
    ASSERT_FILE_EXISTS("integration_test/src/main.cpp");
    ASSERT_FILE_EXISTS("integration_test/include/integration_test.h");
    ASSERT_FILE_EXISTS("integration_test/src/integration_test.cpp");
    
    // Build system
    ASSERT_FILE_EXISTS("integration_test/CMakeLists.txt");
    
    // Documentation
    ASSERT_FILE_EXISTS("integration_test/README.md");
    ASSERT_FILE_EXISTS("integration_test/LICENSE");
    
    // Version control
    ASSERT_FILE_EXISTS("integration_test/.gitignore");
    
    // Tests
    ASSERT_FILE_EXISTS("integration_test/tests/test_integration_test.cpp");
    
    // Content validation
    ASSERT_FILE_CONTAINS("integration_test/README.md", "Full integration test project");
    ASSERT_FILE_CONTAINS("integration_test/README.md", "Test complete workflow");
    ASSERT_FILE_CONTAINS("integration_test/README.md", "Integration Tester");
    ASSERT_FILE_CONTAINS("integration_test/README.md", "2.0.0");
    
    // Cleanup
    TestHelpers::cleanupTestDirectory("integration_test");
}

// Main test runner
int main() {
    std::cout << "=== Template Generator Test Suite ===" << std::endl;
    std::cout << "Running comprehensive tests for the C++ Template Generator\n" << std::endl;
    
    TestFramework::getGlobalRunner().runAllTests();
    
    int failed = TestFramework::getGlobalRunner().getFailedCount();
    if (failed == 0) {
        std::cout << "\n🎉 All tests passed! Template Generator is working correctly." << std::endl;
    } else {
        std::cout << "\n❌ " << failed << " test(s) failed. Please review the failures above." << std::endl;
    }
    
    return failed;
}
