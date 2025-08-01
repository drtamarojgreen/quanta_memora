#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <cstddef>

namespace fs = std::filesystem;

/**
 * Comprehensive Build System for Quanta Memora Project
 * 
 * This utility builds all components of the project:
 * - Template Generator
 * - Test Framework and Unit Tests
 * - CBT Applications
 * - Documentation
 */

struct BuildTarget {
    std::string name;
    std::string source_file;
    std::string output_name;
    std::string description;
    std::vector<std::string> dependencies;
};

class ProjectBuilder {
private:
    std::vector<BuildTarget> targets;
    std::string compiler = "g++";
    std::string flags = "-std=c++17 -Wall -Wextra -O2";
    
    void initializeTargets() {
        targets = {
            {
                "template_generator",
                "template_generator.cpp",
                "template_generator",
                "Advanced C++ Template Generator",
                {}
            },
            {
                "test_framework",
                "tests/test_framework.cpp",
                "test_framework",
                "Comprehensive Testing Framework",
                {}
            },
            {
                "unit_tests",
                "tests/test_template_generator.cpp",
                "run_tests",
                "Unit Tests for Template Generator",
                {"tests/test_framework.cpp"}
            },
            {
                "thought_record",
                "cbt_apps/thought_record_journal.cpp",
                "thought_record_journal",
                "CBT Thought Record Journal",
                {}
            },
            {
                "mood_tracker",
                "cbt_apps/mood_emotion_tracker.cpp",
                "mood_emotion_tracker",
                "Advanced Mood & Emotion Tracker",
                {}
            },
            {
                "distortion_identifier",
                "cbt_apps/cognitive_distortion_identifier.cpp",
                "cognitive_distortion_identifier",
                "Cognitive Distortion Identifier",
                {}
            }
        };
    }
    
    bool fileExists(const std::string& path) {
        return fs::exists(path);
    }
    
    bool buildTarget(const BuildTarget& target) {
        std::cout << "Building " << target.name << " (" << target.description << ")..." << std::endl;
        
        // Check if source file exists
        if (!fileExists(target.source_file)) {
            std::cerr << "Error: Source file not found: " << target.source_file << std::endl;
            return false;
        }
        
        // Build command
        std::string command = compiler + " " + flags + " " + target.source_file;
        
        // Add dependencies
        for (const auto& dep : target.dependencies) {
            if (fileExists(dep)) {
                command += " " + dep;
            }
        }
        
        command += " -o " + target.output_name;
        
        std::cout << "Command: " << command << std::endl;
        
        int result = std::system(command.c_str());
        if (result == 0) {
            std::cout << "✅ Successfully built " << target.name << std::endl;
            return true;
        } else {
            std::cerr << "❌ Failed to build " << target.name << std::endl;
            return false;
        }
    }

public:
    ProjectBuilder() {
        initializeTargets();
    }
    
    void displayWelcome() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "         QUANTA MEMORA BUILD SYSTEM" << std::endl;
        std::cout << "    Comprehensive CBT Tools & Template Generator" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "\nThis build system compiles all components of the project:" << std::endl;
        std::cout << "• Advanced C++ Template Generator with comprehensive testing" << std::endl;
        std::cout << "• Sophisticated CBT applications for mental health support" << std::endl;
        std::cout << "• Professional-grade testing framework with BDD support" << std::endl;
        std::cout << "\n🚀 Ready to build cutting-edge mental health tools!" << std::endl;
    }
    
    void listTargets() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "AVAILABLE BUILD TARGETS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        for (std::size_t i = 0; i < targets.size(); ++i) {
            const auto& target = targets[i];
            std::cout << (i + 1) << ". " << target.name << std::endl;
            std::cout << "   Description: " << target.description << std::endl;
            std::cout << "   Source: " << target.source_file << std::endl;
            std::cout << "   Output: " << target.output_name << std::endl;
            if (!target.dependencies.empty()) {
                std::cout << "   Dependencies: ";
                for (std::size_t j = 0; j < target.dependencies.size(); ++j) {
                    if (j > 0) std::cout << ", ";
                    std::cout << target.dependencies[j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    
    bool buildAll() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "BUILDING ALL TARGETS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        bool all_success = true;
        int built = 0;
        int failed = 0;
        
        for (const auto& target : targets) {
            if (buildTarget(target)) {
                built++;
            } else {
                failed++;
                all_success = false;
            }
            std::cout << std::endl;
        }
        
        // Summary
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "BUILD SUMMARY" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Successfully built: " << built << std::endl;
        std::cout << "Failed to build: " << failed << std::endl;
        std::cout << "Total targets: " << targets.size() << std::endl;
        
        if (all_success) {
            std::cout << "\n🎉 All targets built successfully!" << std::endl;
            std::cout << "You can now run any of the applications:" << std::endl;
            for (const auto& target : targets) {
                std::cout << "  ./" << target.output_name << " - " << target.description << std::endl;
            }
        } else {
            std::cout << "\n⚠️  Some targets failed to build. Check the errors above." << std::endl;
        }
        
        return all_success;
    }
    
    bool buildSpecific(const std::string& target_name) {
        for (const auto& target : targets) {
            if (target.name == target_name) {
                return buildTarget(target);
            }
        }
        
        std::cerr << "Error: Target '" << target_name << "' not found." << std::endl;
        return false;
    }
    
    void runTests() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "RUNNING UNIT TESTS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // First build the tests if they don't exist
        if (!fileExists("run_tests")) {
            std::cout << "Tests not built. Building now..." << std::endl;
            if (!buildSpecific("unit_tests")) {
                std::cerr << "Failed to build tests." << std::endl;
                return;
            }
        }
        
        // Run the tests
        std::cout << "Running comprehensive test suite..." << std::endl;
        int result = std::system("./run_tests");
        
        if (result == 0) {
            std::cout << "\n🎉 All tests passed!" << std::endl;
        } else {
            std::cout << "\n❌ Some tests failed. Review the output above." << std::endl;
        }
    }
    
    void clean() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "CLEANING BUILD ARTIFACTS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        for (const auto& target : targets) {
            if (fileExists(target.output_name)) {
                fs::remove(target.output_name);
                std::cout << "Removed: " << target.output_name << std::endl;
            }
        }
        
        // Clean up any additional build artifacts
        std::vector<std::string> artifacts = {"build_all", "*.o", "*.obj", "*.exe"};
        for (const auto& artifact : artifacts) {
            if (fileExists(artifact)) {
                fs::remove(artifact);
                std::cout << "Removed: " << artifact << std::endl;
            }
        }
        
        std::cout << "✅ Clean complete!" << std::endl;
    }
    
    void showHelp() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "QUANTA MEMORA BUILD SYSTEM HELP" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << "\n🎯 PURPOSE:" << std::endl;
        std::cout << "This build system compiles all components of the Quanta Memora project," << std::endl;
        std::cout << "including the template generator, testing framework, and CBT applications." << std::endl;
        
        std::cout << "\n📚 USAGE:" << std::endl;
        std::cout << "1. Build All - Compile all project components" << std::endl;
        std::cout << "2. List Targets - Show all available build targets" << std::endl;
        std::cout << "3. Build Specific - Compile a specific component" << std::endl;
        std::cout << "4. Run Tests - Execute the comprehensive test suite" << std::endl;
        std::cout << "5. Clean - Remove all build artifacts" << std::endl;
        
        std::cout << "\n🛠️  REQUIREMENTS:" << std::endl;
        std::cout << "• C++17 compatible compiler (g++, clang++, or MSVC)" << std::endl;
        std::cout << "• Standard C++ library with filesystem support" << std::endl;
        std::cout << "• Make utility (optional, for alternative building)" << std::endl;
        
        std::cout << "\n🚀 QUICK START:" << std::endl;
        std::cout << "1. Run this build system: ./build_all" << std::endl;
        std::cout << "2. Choose 'Build All' to compile everything" << std::endl;
        std::cout << "3. Run tests to verify everything works" << std::endl;
        std::cout << "4. Try the CBT applications!" << std::endl;
        
        std::cout << "\n📱 CBT APPLICATIONS:" << std::endl;
        std::cout << "• Thought Record Journal - Challenge negative thinking patterns" << std::endl;
        std::cout << "• Mood & Emotion Tracker - Monitor emotional well-being" << std::endl;
        std::cout << "• Cognitive Distortion Identifier - Recognize unhelpful thoughts" << std::endl;
        
        std::cout << "\n⚠️  NOTES:" << std::endl;
        std::cout << "• These are educational tools, not replacements for professional help" << std::endl;
        std::cout << "• All applications store data locally for privacy" << std::endl;
        std::cout << "• Source code is available for review and modification" << std::endl;
    }
    
    void run() {
        displayWelcome();
        
        while (true) {
            std::cout << "\n" << std::string(40, '=') << std::endl;
            std::cout << "MAIN MENU" << std::endl;
            std::cout << std::string(40, '=') << std::endl;
            std::cout << "1. Build All Components" << std::endl;
            std::cout << "2. List Available Targets" << std::endl;
            std::cout << "3. Build Specific Target" << std::endl;
            std::cout << "4. Run Unit Tests" << std::endl;
            std::cout << "5. Clean Build Artifacts" << std::endl;
            std::cout << "6. Help & Information" << std::endl;
            std::cout << "7. Exit" << std::endl;
            std::cout << "\nChoose an option (1-7): ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                buildAll();
            } else if (choice == "2") {
                listTargets();
            } else if (choice == "3") {
                listTargets();
                std::cout << "Enter target name to build: ";
                std::string target_name;
                std::getline(std::cin, target_name);
                buildSpecific(target_name);
            } else if (choice == "4") {
                runTests();
            } else if (choice == "5") {
                clean();
            } else if (choice == "6") {
                showHelp();
            } else if (choice == "7") {
                std::cout << "\nThank you for using the Quanta Memora Build System!" << std::endl;
                std::cout << "🌟 Building better mental health tools, one compile at a time! 🌟" << std::endl;
                break;
            } else {
                std::cout << "\nInvalid choice. Please select 1-7." << std::endl;
            }
        }
    }
};

int main() {
    try {
        ProjectBuilder builder;
        builder.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
