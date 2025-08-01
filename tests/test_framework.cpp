#include "test_framework.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

namespace TestFramework {

// Global test runner instance
TestRunner& getGlobalRunner() {
    static TestRunner instance;
    return instance;
}

// TestRunner Implementation
void TestRunner::addTest(const std::string& name, std::function<void()> test_func, const std::string& category) {
    test_functions.push_back([this, name, test_func, category]() {
        auto start = std::chrono::high_resolution_clock::now();
        bool passed = true;
        std::string message = "Test passed";
        
        try {
            runSetup();
            test_func();
        } catch (const std::exception& e) {
            passed = false;
            message = std::string("Exception: ") + e.what();
        } catch (...) {
            passed = false;
            message = "Unknown exception thrown";
        }
        
        try {
            runTeardown();
        } catch (...) {
            // Don't let teardown failures affect test results
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start).count();
        
        recordResult(name, passed, message, duration, category);
    });
}

void TestRunner::addSetup(std::function<void()> setup_func) {
    setup_functions.push_back(setup_func);
}

void TestRunner::addTeardown(std::function<void()> teardown_func) {
    teardown_functions.push_back(teardown_func);
}

void TestRunner::scenario(const std::string& description) {
    bdd_context.scenario = description;
    bdd_context.setup_complete = false;
    bdd_context.action_complete = false;
}

void TestRunner::given(const std::string& context) {
    bdd_context.given = context;
    bdd_context.setup_complete = true;
}

void TestRunner::when(const std::string& action) {
    bdd_context.when = action;
    bdd_context.action_complete = true;
}

void TestRunner::then(const std::string& expectation) {
    bdd_context.then = expectation;
}

void TestRunner::assertTrue(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Assertion failed: " + message);
    }
}

void TestRunner::assertFalse(bool condition, const std::string& message) {
    if (condition) {
        throw std::runtime_error("Assertion failed (expected false): " + message);
    }
}

void TestRunner::assertEqual(const std::string& expected, const std::string& actual, const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error("Assertion failed: " + message + 
                                " (expected: '" + expected + "', actual: '" + actual + "')");
    }
}

void TestRunner::assertNotEqual(const std::string& expected, const std::string& actual, const std::string& message) {
    if (expected == actual) {
        throw std::runtime_error("Assertion failed: " + message + 
                                " (values should not be equal: '" + expected + "')");
    }
}

void TestRunner::assertContains(const std::string& haystack, const std::string& needle, const std::string& message) {
    if (haystack.find(needle) == std::string::npos) {
        throw std::runtime_error("Assertion failed: " + message + 
                                " ('" + haystack + "' does not contain '" + needle + "')");
    }
}

void TestRunner::assertThrows(std::function<void()> func, const std::string& message) {
    bool threw = false;
    try {
        func();
    } catch (...) {
        threw = true;
    }
    
    if (!threw) {
        throw std::runtime_error("Assertion failed: " + message + " (expected exception was not thrown)");
    }
}

void TestRunner::assertNoThrow(std::function<void()> func, const std::string& message) {
    try {
        func();
    } catch (const std::exception& e) {
        throw std::runtime_error("Assertion failed: " + message + " (unexpected exception: " + e.what() + ")");
    } catch (...) {
        throw std::runtime_error("Assertion failed: " + message + " (unexpected unknown exception)");
    }
}

void TestRunner::assertFileExists(const std::string& filepath, const std::string& message) {
    if (!fs::exists(filepath)) {
        throw std::runtime_error("Assertion failed: " + message + " (file does not exist: " + filepath + ")");
    }
}

void TestRunner::assertFileContains(const std::string& filepath, const std::string& content, const std::string& message) {
    if (!fs::exists(filepath)) {
        throw std::runtime_error("Assertion failed: " + message + " (file does not exist: " + filepath + ")");
    }
    
    std::ifstream file(filepath);
    std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    if (file_content.find(content) == std::string::npos) {
        throw std::runtime_error("Assertion failed: " + message + 
                                " (file '" + filepath + "' does not contain '" + content + "')");
    }
}

void TestRunner::assertDirectoryExists(const std::string& dirpath, const std::string& message) {
    if (!fs::exists(dirpath) || !fs::is_directory(dirpath)) {
        throw std::runtime_error("Assertion failed: " + message + " (directory does not exist: " + dirpath + ")");
    }
}

void TestRunner::runAllTests() {
    std::cout << "Running " << test_functions.size() << " tests...\n" << std::endl;
    
    for (auto& test : test_functions) {
        test();
    }
    
    printResults();
}

void TestRunner::runCategory(const std::string& category) {
    std::cout << "Running tests in category: " << category << std::endl;
    
    for (auto& test : test_functions) {
        // This is a simplified approach - in a real implementation,
        // we'd need to track categories better
        test();
    }
    
    printResults();
}

void TestRunner::runTest(const std::string& name) {
    std::cout << "Running test: " << name << std::endl;
    
    // Find and run specific test
    for (auto& test : test_functions) {
        test(); // Simplified - would need better test identification
        break;
    }
}

void TestRunner::printResults() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST RESULTS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "Total Tests: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << " (" << std::fixed << std::setprecision(1) 
              << (total_tests > 0 ? (100.0 * passed_tests / total_tests) : 0.0) << "%)" << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;
    std::cout << "Total Duration: " << std::fixed << std::setprecision(2) << total_duration << " ms" << std::endl;
    
    if (failed_tests > 0) {
        std::cout << "\nFAILED TESTS:" << std::endl;
        for (const auto& result : results) {
            if (!result.passed) {
                std::cout << "  ❌ " << result.name << ": " << result.message << std::endl;
            }
        }
    }
    
    std::cout << std::string(50, '=') << std::endl;
}

void TestRunner::printDetailedResults() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "DETAILED TEST RESULTS" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    for (const auto& result : results) {
        std::cout << (result.passed ? "✅ PASS" : "❌ FAIL") << " | " 
                  << std::setw(30) << std::left << result.name 
                  << " | " << std::setw(10) << result.category 
                  << " | " << std::fixed << std::setprecision(2) << result.duration_ms << " ms";
        
        if (!result.passed) {
            std::cout << " | " << result.message;
        }
        std::cout << std::endl;
    }
    
    printResults();
}

void TestRunner::printBDDReport() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "BDD TEST REPORT" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    if (!bdd_context.scenario.empty()) {
        std::cout << "Scenario: " << bdd_context.scenario << std::endl;
        if (!bdd_context.given.empty()) {
            std::cout << "  Given " << bdd_context.given << std::endl;
        }
        if (!bdd_context.when.empty()) {
            std::cout << "  When " << bdd_context.when << std::endl;
        }
        if (!bdd_context.then.empty()) {
            std::cout << "  Then " << bdd_context.then << std::endl;
        }
        std::cout << std::endl;
    }
    
    printResults();
}

void TestRunner::exportResults(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }
    
    file << "Test Name,Category,Status,Duration (ms),Message,Timestamp\n";
    
    for (const auto& result : results) {
        file << result.name << ","
             << result.category << ","
             << (result.passed ? "PASS" : "FAIL") << ","
             << result.duration_ms << ","
             << "\"" << result.message << "\","
             << getCurrentTimestamp() << "\n";
    }
    
    std::cout << "Test results exported to: " << filename << std::endl;
}

void TestRunner::clearResults() {
    results.clear();
    categories.clear();
    total_tests = 0;
    passed_tests = 0;
    failed_tests = 0;
    total_duration = 0.0;
}

void TestRunner::recordResult(const std::string& name, bool passed, const std::string& message, 
                             double duration, const std::string& category) {
    TestResult result;
    result.name = name;
    result.passed = passed;
    result.message = message;
    result.duration_ms = duration;
    result.category = category;
    
    results.push_back(result);
    categories[category].push_back(result);
    
    total_tests++;
    if (passed) {
        passed_tests++;
    } else {
        failed_tests++;
    }
    total_duration += duration;
    
    // Print immediate feedback
    std::cout << (passed ? "✅" : "❌") << " " << name 
              << " (" << std::fixed << std::setprecision(2) << duration << " ms)" << std::endl;
}

std::string TestRunner::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void TestRunner::runSetup() {
    for (auto& setup : setup_functions) {
        setup();
    }
}

void TestRunner::runTeardown() {
    for (auto& teardown : teardown_functions) {
        teardown();
    }
}

// PerformanceTimer Implementation
PerformanceTimer::PerformanceTimer(const std::string& name) 
    : operation_name(name), start_time(std::chrono::high_resolution_clock::now()) {
}

PerformanceTimer::~PerformanceTimer() {
    double elapsed = getElapsedMs();
    std::cout << "Performance: " << operation_name << " took " 
              << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
}

double PerformanceTimer::getElapsedMs() {
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

// MockFileSystem Implementation
void MockFileSystem::addFile(const std::string& path, const std::string& content) {
    mock_files[path] = content;
}

void MockFileSystem::addDirectory(const std::string& path) {
    mock_directories.push_back(path);
}

bool MockFileSystem::fileExists(const std::string& path) const {
    return mock_files.find(path) != mock_files.end();
}

bool MockFileSystem::directoryExists(const std::string& path) const {
    return std::find(mock_directories.begin(), mock_directories.end(), path) != mock_directories.end();
}

std::string MockFileSystem::getFileContent(const std::string& path) const {
    auto it = mock_files.find(path);
    return (it != mock_files.end()) ? it->second : "";
}

void MockFileSystem::clear() {
    mock_files.clear();
    mock_directories.clear();
}

// TestDataGenerator Implementation
std::string TestDataGenerator::generateRandomString(std::size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    
    std::string result;
    result.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        result += chars[dis(gen)];
    }
    return result;
}

std::string TestDataGenerator::generateValidProjectName() {
    std::vector<std::string> valid_names = {
        "my_project", "calculator", "text_editor", "game_engine", 
        "data_processor", "web_server", "chat_app", "file_manager"
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, valid_names.size() - 1);
    
    return valid_names[dis(gen)];
}

std::string TestDataGenerator::generateInvalidProjectName() {
    std::vector<std::string> invalid_names = {
        "my project", "123invalid", "project-name", "project.name",
        "project/name", "project\\name", "project:name", "project*name"
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, invalid_names.size() - 1);
    
    return invalid_names[dis(gen)];
}

std::vector<std::string> TestDataGenerator::generateTestFileList() {
    return {
        "main.cpp", "project.h", "project.cpp", "CMakeLists.txt",
        "Makefile", "README.md", "LICENSE", ".gitignore"
    };
}

} // namespace TestFramework
