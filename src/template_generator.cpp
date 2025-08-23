#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <map>
#include <cstddef>
#include <cctype>

#include "TemplateLoader.h"

namespace fs = std::filesystem;

namespace StringUtils {
// Helper functions
std::string capitalize(const std::string& input) {
    if (input.empty()) return input;
    std::string result = input;
    result[0] = std::toupper(result[0]);
    return result;
}

std::string toUpper(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string camelToSnake(const std::string& input) {
    std::string result;
    for (std::size_t i = 0; i < input.length(); ++i) {
        if (i > 0 && std::isupper(input[i])) {
            result += '_';
        }
        result += std::tolower(input[i]);
    }
    return result;
}

std::string escapeSql(const std::string& input) {
    std::string result;
    result.reserve(input.length());
    for (char c : input) {
        if (c == '\'') {
            result += "''";
        } else {
            result += c;
        }
    }
    return result;
}
} // namespace StringUtils

// Project template types
enum class ProjectType {
    CONSOLE_APP,
    STATIC_LIBRARY,
    SHARED_LIBRARY,
    HEADER_ONLY,
    GUI_APP,
    UNIT_TEST
};

struct ProjectConfig {
    std::string name;
    std::string className;
    std::string description;
    std::string goal;
    std::string author;
    std::string version;
    ProjectType type;
    bool useCMake;
    bool includeTests;
    bool includeGitIgnore;
    bool includeLikertScale;
    bool includeDataDictionary;
    bool includePrivacyPolicy;
    std::vector<std::string> dependencies;
};

class TemplateGenerator {
private:
    const ProjectConfig& config_;
    const TemplateLoader& loader_;
    
    void createDirectory(const std::string& path) {
        if (!fs::exists(path)) {
            fs::create_directories(path);
            std::cout << "Created directory: " << path << std::endl;
        }
    }

    void writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            std::cout << "Generated: " << filename << std::endl;
        } else {
            std::cerr << "Error: Could not create " << filename << std::endl;
        }
    }

public:
    TemplateGenerator(const ProjectConfig& config, const TemplateLoader& loader)
        : config_(config), loader_(loader) {}

    void generateProject() {
        std::cout << "\n=== Generating Project Structure ===\n";
        
        // Create project directory
        createDirectory(config_.name);
        
        // Create subdirectories based on project type
        if (config_.type != ProjectType::HEADER_ONLY) {
            createDirectory(config_.name + "/src");
        }
        createDirectory(config_.name + "/include");
        
        if (config_.includeTests) {
            createDirectory(config_.name + "/tests");
        }
        
        if (config_.useCMake) {
            createDirectory(config_.name + "/build");
        }
        
        // Generate files
        generateMainFile();
        generateHeaderFile();
        
        if (config.type != ProjectType::HEADER_ONLY) {
            generateImplementationFile();
        }
        
        if (config.useCMake) {
            generateCMakeFile();
        } else {
            generateMakefile();
        }
        
        if (config.includeTests) {
            generateTestFile();
        }
        
        if (config.includeGitIgnore) {
            generateGitIgnore();
        }
        
        if (config.includeLikertScale) {
            writeFile(config.name + "/include/LikertScale.h", generateLikertScaleHeader());
            writeFile(config.name + "/src/LikertScale.cpp", generateLikertScaleImplementation());
        }

        if (config.includeDataDictionary) {
            writeFile(config.name + "/data_dictionary.md", generateDataDictionary());
        }

        if (config.includePrivacyPolicy) {
            writeFile(config.name + "/PRIVACY_POLICY.md", generatePrivacyPolicy());
        }

        generateReadme();
        generateLicense();
        
        std::cout << "\nProject '" << config_.name << "' generated successfully!\n";
        std::cout << "Navigate to the project directory: cd " << config_.name << std::endl;
    }
    
private:
    void generateMainFile() {
        if (config.type == ProjectType::STATIC_LIBRARY || 
            config.type == ProjectType::SHARED_LIBRARY ||
            config.type == ProjectType::HEADER_ONLY) {
            return; // Libraries don't need main files
        }
        
        std::string content;
        std::string filename = config_.name + "/src/main.cpp";
        
        switch (config_.type) {
            case ProjectType::CONSOLE_APP:
                content = generateConsoleMain();
                break;
            case ProjectType::GUI_APP:
                content = generateGuiMain();
                break;
            case ProjectType::UNIT_TEST:
                content = generateTestMain();
                break;
            default:
                content = generateConsoleMain();
        }
        
        writeFile(filename, content);
    }
    
    std::string generateConsoleMain() {
        std::string content = "#include <iostream>\n"
                             "#include \"../include/" + config_.name + ".h\"\n";

        if (config_.includeLikertScale) {
            content += "#include \"../include/LikertScale.h\"\n"
                       "#include <vector>\n";
        }

        content += "\nint main() {\n"
                   "    std::cout << \"Launching " + config_.className + " Application...\" << std::endl;\n\n";

        if (config_.includeLikertScale) {
            content += "    // Example usage of LikertScale\n"
                       "    std::vector<std::string> options = {\n"
                       "        \"Strongly Disagree\",\n"
                       "        \"Disagree\",\n"
                       "        \"Neutral\",\n"
                       "        \"Agree\",\n"
                       "        \"Strongly Agree\"\n"
                       "    };\n"
                       "    LikertScale scale(\"The software was easy to use.\", options);\n"
                       "    int response = scale.displayAndGetResponse();\n"
                       "    std::cout << \"\\nYou selected option: \" << response << \" - \" << options[response - 1] << std::endl;\n\n";
        }

        content += "    " + config_.className + " app;\n"
                   "    return app.run();\n"
                   "}\n";

        return content;
    }
    
    std::string generateGuiMain() {
        return "#include <iostream>\n"
               "#include \"../include/" + config_.name + ".h\"\n\n"
               "// TODO: Include GUI framework headers (Qt, GTK, etc.)\n\n"
               "int main(int argc, char* argv[]) {\n"
               "    std::cout << \"Launching " + config_.className + " GUI Application...\" << std::endl;\n\n"
               "    // TODO: Initialize GUI framework\n"
               "    " + config_.className + " app;\n"
               "    return app.run();\n"
               "}\n";
    }
    
    std::string generateTestMain() {
        return "#include <iostream>\n"
               "#include \"../include/" + config_.name + ".h\"\n\n"
               "// Simple test runner - consider using Google Test, Catch2, or similar\n"
               "int main() {\n"
               "    std::cout << \"Running " + config_.className + " Tests...\" << std::endl;\n\n"
               "    " + config_.className + "Test test;\n"
               "    return test.runAllTests();\n"
               "}\n";
    }
    
    void generateHeaderFile() {
        std::string guard = StringUtils::toUpper(config_.name) + "_H";
        std::string filename = config_.name + "/include/" + config_.name + ".h";
        
        std::string content = "#ifndef " + guard + "\n"
                             "#define " + guard + "\n\n"
                             "#include <string>\n"
                             "#include <vector>\n"
                             "#include <memory>\n\n";
        
        if (!config_.description.empty()) {
            content += "/**\n * " + config_.description + "\n";
            if (!config_.author.empty()) {
                content += " * @author " + config_.author + "\n";
            }
            if (!config_.version.empty()) {
                content += " * @version " + config_.version + "\n";
            }
            content += " */\n";
        }
        
        switch (config_.type) {
            case ProjectType::CONSOLE_APP:
            case ProjectType::GUI_APP:
                content += generateAppClass();
                break;
            case ProjectType::STATIC_LIBRARY:
            case ProjectType::SHARED_LIBRARY:
            case ProjectType::HEADER_ONLY:
                content += generateLibraryClass();
                break;
            case ProjectType::UNIT_TEST:
                content += generateTestClass();
                break;
        }
        
        content += "\n#endif // " + guard + "\n";
        writeFile(filename, content);
    }
    
    std::string generateAppClass() {
        return "class " + config_.className + " {\n"
               "private:\n"
               "    std::string appName;\n"
               "    bool isRunning;\n\n"
               "public:\n"
               "    " + config_.className + "();\n"
               "    ~" + config_.className + "();\n\n"
               "    int run();\n"
               "    void shutdown();\n"
               "    \n"
               "    // Core functionality\n"
               "    void initialize();\n"
               "    void update();\n"
               "    void cleanup();\n"
               "};\n";
    }
    
    std::string generateLibraryClass() {
        return "class " + config_.className + " {\n"
               "private:\n"
               "    // Private members\n\n"
               "public:\n"
               "    " + config_.className + "();\n"
               "    ~" + config_.className + "();\n\n"
               "    // Public API\n"
               "    void process();\n"
               "    std::string getVersion() const;\n"
               "};\n\n"
               "// Utility functions\n"
               "namespace " + StringUtils::toLower(config_.className) + "_utils {\n"
               "    void helperFunction();\n"
               "}\n";
    }
    
    std::string generateTestClass() {
        return "class " + config_.className + "Test {\n"
               "private:\n"
               "    int passedTests;\n"
               "    int failedTests;\n\n"
               "    void assert(bool condition, const std::string& testName);\n\n"
               "public:\n"
               "    " + config_.className + "Test();\n"
               "    ~" + config_.className + "Test();\n\n"
               "    int runAllTests();\n"
               "    void testBasicFunctionality();\n"
               "    void testEdgeCases();\n"
               "    void printResults();\n"
               "};\n";
    }
    
    void generateImplementationFile() {
        std::string filename = config_.name + "/src/" + config_.name + ".cpp";
        std::string content = "#include \"../include/" + config_.name + ".h\"\n"
                             "#include <iostream>\n"
                             "#include <stdexcept>\n\n";
        
        switch (config_.type) {
            case ProjectType::CONSOLE_APP:
            case ProjectType::GUI_APP:
                content += generateAppImplementation();
                break;
            case ProjectType::STATIC_LIBRARY:
            case ProjectType::SHARED_LIBRARY:
                content += generateLibraryImplementation();
                break;
            case ProjectType::UNIT_TEST:
                content += generateTestImplementation();
                break;
        }
        
        writeFile(filename, content);
    }
    
    std::string generateAppImplementation() {
        return config_.className + "::" + config_.className + "()\n"
               "    : appName(\"" + config_.className + "\"), isRunning(false) {\n"
               "    // Constructor implementation\n"
               "}\n\n"
               + config_.className + "::~" + config_.className + "() {\n"
               "    cleanup();\n"
               "}\n\n"
               "int " + config_.className + "::run() {\n"
               "    try {\n"
               "        initialize();\n"
               "        isRunning = true;\n"
               "        \n"
               "        std::cout << \"Running " + config_.description + "...\" << std::endl;\n"
               "        \n"
               "        // Main application loop\n"
               "        while (isRunning) {\n"
               "            update();\n"
               "            // TODO: Implement main logic: " + config_.goal + "\n"
               "            break; // Remove this for continuous running\n"
               "        }\n"
               "        \n"
               "        return 0;\n"
               "    } catch (const std::exception& e) {\n"
               "        std::cerr << \"Error: \" << e.what() << std::endl;\n"
               "        return 1;\n"
               "    }\n"
               "}\n\n"
               "void " + config_.className + "::initialize() {\n"
               "    std::cout << \"Initializing \" << appName << \"...\" << std::endl;\n"
               "    // TODO: Add initialization logic\n"
               "}\n\n"
               "void " + config_.className + "::update() {\n"
               "    // TODO: Add update logic\n"
               "}\n\n"
               "void " + config_.className + "::cleanup() {\n"
               "    std::cout << \"Cleaning up \" << appName << \"...\" << std::endl;\n"
               "    // TODO: Add cleanup logic\n"
               "}\n\n"
               "void " + config_.className + "::shutdown() {\n"
               "    isRunning = false;\n"
               "}\n";
    }
    
    std::string generateLibraryImplementation() {
        return config_.className + "::" + config_.className + "() {\n"
               "    // Constructor implementation\n"
               "}\n\n"
               + config_.className + "::~" + config_.className + "() {\n"
               "    // Destructor implementation\n"
               "}\n\n"
               "void " + config_.className + "::process() {\n"
               "    std::cout << \"Processing with " + config_.className + "...\" << std::endl;\n"
               "    // TODO: Implement core functionality: " + config_.goal + "\n"
               "}\n\n"
               "std::string " + config_.className + "::getVersion() const {\n"
               "    return \"" + config_.version + "\";\n"
               "}\n\n"
               "// Utility functions implementation\n"
               "namespace " + StringUtils::toLower(config_.className) + "_utils {\n"
               "    void helperFunction() {\n"
               "        // TODO: Implement helper functionality\n"
               "    }\n"
               "}\n";
    }
    
    std::string generateTestImplementation() {
        return config_.className + "Test::" + config_.className + "Test()\n"
               "    : passedTests(0), failedTests(0) {\n"
               "    // Test constructor\n"
               "}\n\n"
               + config_.className + "Test::~" + config_.className + "Test() {\n"
               "    // Test destructor\n"
               "}\n\n"
               "int " + config_.className + "Test::runAllTests() {\n"
               "    std::cout << \"Running all tests for " + config_.className + "...\" << std::endl;\n"
               "    \n"
               "    testBasicFunctionality();\n"
               "    testEdgeCases();\n"
               "    \n"
               "    printResults();\n"
               "    return (failedTests == 0) ? 0 : 1;\n"
               "}\n\n"
               "void " + config_.className + "Test::testBasicFunctionality() {\n"
               "    // TODO: Implement basic functionality tests\n"
               "    assert(true, \"Basic functionality test\");\n"
               "}\n\n"
               "void " + config_.className + "Test::testEdgeCases() {\n"
               "    // TODO: Implement edge case tests\n"
               "    assert(true, \"Edge cases test\");\n"
               "}\n\n"
               "void " + config_.className + "Test::assert(bool condition, const std::string& testName) {\n"
               "    if (condition) {\n"
               "        std::cout << \"[PASS] \" << testName << std::endl;\n"
               "        passedTests++;\n"
               "    } else {\n"
               "        std::cout << \"[FAIL] \" << testName << std::endl;\n"
               "        failedTests++;\n"
               "    }\n"
               "}\n\n"
               "void " + config_.className + "Test::printResults() {\n"
               "    std::cout << \"\\nTest Results:\" << std::endl;\n"
               "    std::cout << \"Passed: \" << passedTests << std::endl;\n"
               "    std::cout << \"Failed: \" << failedTests << std::endl;\n"
               "    std::cout << \"Total: \" << (passedTests + failedTests) << std::endl;\n"
               "}\n";
    }
    
    void generateCMakeFile() {
        std::string filename = config_.name + "/CMakeLists.txt";
        std::string content = "cmake_minimum_required(VERSION 3.12)\n"
                             "project(" + config_.name + " VERSION " + config_.version + ")\n\n"
                             "set(CMAKE_CXX_STANDARD 17)\n"
                             "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
                             "# Include directories\n"
                             "include_directories(include)\n\n";
        
        std::string sources;
        if (config_.type == ProjectType::CONSOLE_APP || config_.type == ProjectType::GUI_APP) {
            sources = "    src/main.cpp\n    src/" + config_.name + ".cpp\n";
            if (config_.includeLikertScale) {
                sources += "    src/LikertScale.cpp\n";
            }
            content += "# Executable\n"
                      "add_executable(" + config_.name + "\n" + sources + ")\n\n";
        } else if (config_.type == ProjectType::STATIC_LIBRARY) {
            sources = "    src/" + config_.name + ".cpp\n";
            if (config_.includeLikertScale) {
                sources += "    src/LikertScale.cpp\n";
            }
            content += "# Static Library\n"
                      "add_library(" + config_.name + " STATIC\n" + sources + ")\n\n";
        } else if (config_.type == ProjectType::SHARED_LIBRARY) {
            sources = "    src/" + config_.name + ".cpp\n";
            if (config_.includeLikertScale) {
                sources += "    src/LikertScale.cpp\n";
            }
            content += "# Shared Library\n"
                      "add_library(" + config_.name + " SHARED\n" + sources + ")\n\n";
        }
        
        if (config_.includeTests) {
            content += "# Tests\n"
                      "enable_testing()\n"
                      "add_executable(" + config_.name + "_tests\n"
                      "    tests/test_" + config_.name + ".cpp\n"
                      ")\n"
                      "add_test(NAME " + config_.name + "_tests COMMAND " + config_.name + "_tests)\n\n";
        }
        
        content += "# Compiler flags\n"
                  "target_compile_options(" + config_.name + " PRIVATE\n"
                  "    -Wall -Wextra -Wpedantic\n"
                  ")\n\n"
                  "# Installation\n"
                  "install(TARGETS " + config_.name + " DESTINATION bin)\n"
                  "install(FILES include/" + config_.name + ".h DESTINATION include)\n";
        
        writeFile(filename, content);
    }
    
    void generateMakefile() {
        std::string filename = config_.name + "/Makefile";
        std::string content = "# Makefile for " + config_.name + "\n\n"
                             "CXX = g++\n"
                             "CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude\n"
                             "SRCDIR = src\n"
                             "OBJDIR = obj\n"
                             "SOURCES = $(wildcard $(SRCDIR)/*.cpp)\n"
                             "OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)\n"
                             "TARGET = " + config_.name + "\n\n"
                             ".PHONY: all clean debug release\n\n"
                             "all: $(TARGET)\n\n"
                             "$(TARGET): $(OBJECTS)\n"
                             "\t$(CXX) $(OBJECTS) -o $@\n\n"
                             "$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)\n"
                             "\t$(CXX) $(CXXFLAGS) -c $< -o $@\n\n"
                             "$(OBJDIR):\n"
                             "\tmkdir -p $(OBJDIR)\n\n"
                             "debug: CXXFLAGS += -g -DDEBUG\n"
                             "debug: $(TARGET)\n\n"
                             "release: CXXFLAGS += -O3 -DNDEBUG\n"
                             "release: $(TARGET)\n\n"
                             "clean:\n"
                             "\trm -rf $(OBJDIR) $(TARGET)\n\n"
                             "install: $(TARGET)\n"
                             "\tcp $(TARGET) /usr/local/bin/\n";
        
        writeFile(filename, content);
    }
    
    void generateTestFile() {
        if (config_.type == ProjectType::UNIT_TEST) return; // Already handled in main files
        
        std::string filename = config_.name + "/tests/test_" + config_.name + ".cpp";
        std::string content = "#include \"../include/" + config_.name + ".h\"\n"
                             "#include <iostream>\n"
                             "#include <cassert>\n\n"
                             "// Simple test framework - consider using Google Test, Catch2, or similar\n\n"
                             "void test_" + StringUtils::toLower(config_.className) + "_creation() {\n"
                             "    " + config_.className + " obj;\n"
                             "    std::cout << \"[PASS] " + config_.className + " creation test\" << std::endl;\n"
                             "}\n\n"
                             "void test_" + StringUtils::toLower(config_.className) + "_functionality() {\n"
                             "    " + config_.className + " obj;\n"
                             "    // TODO: Add specific functionality tests\n"
                             "    std::cout << \"[PASS] " + config_.className + " functionality test\" << std::endl;\n"
                             "}\n\n"
                             "int main() {\n"
                             "    std::cout << \"Running tests for " + config_.className + "...\" << std::endl;\n\n"
                             "    test_" + toLower(config_.className) + "_creation();\n"
                             "    test_" + toLower(config_.className) + "_functionality();\n\n"
                             "    std::cout << \"All tests passed!\" << std::endl;\n"
                             "    return 0;\n"
                             "}\n";
        
        writeFile(filename, content);
    }
    
    void generateGitIgnore() {
        std::string filename = config_.name + "/.gitignore";
        std::string content = "# Compiled Object files\n"
                             "*.slo\n*.lo\n*.o\n*.obj\n\n"
                             "# Precompiled Headers\n"
                             "*.gch\n*.pch\n\n"
                             "# Compiled Dynamic libraries\n"
                             "*.so\n*.dylib\n*.dll\n\n"
                             "# Fortran module files\n"
                             "*.mod\n*.smod\n\n"
                             "# Compiled Static libraries\n"
                             "*.lai\n*.la\n*.a\n*.lib\n\n"
                             "# Executables\n"
                             "*.exe\n*.out\n*.app\n\n"
                             "# Build directories\n"
                             "build/\n"
                             "obj/\n"
                             "bin/\n\n"
                             "# IDE files\n"
                             ".vscode/\n"
                             ".vs/\n"
                             "*.vcxproj*\n"
                             "*.sln\n\n"
                             "# CMake\n"
                             "CMakeCache.txt\n"
                             "CMakeFiles/\n"
                             "cmake_install.cmake\n"
                             "Makefile\n\n"
                             "# Debug files\n"
                             "*.dSYM/\n"
                             "*.su\n"
                             "*.idb\n"
                             "*.pdb\n\n"
                             "# Project specific\n"
                             + config_.name + "\n"
                             + config_.name + "_tests\n";
        
        writeFile(filename, content);
    }
    
    void generateLicense() {
        std::string filename = config_.name + "/LICENSE";
        std::map<std::string, std::string> substitutions = {
            {"{{author}}", config_.author}
        };
        std::string content = loader_.getAndSubstitute("LICENSE", substitutions);
        writeFile(filename, content);
    }
    
    void generateReadme() {
        std::string filename = config_.name + "/README.md";
        std::map<std::string, std::string> substitutions = {
            {"{{project_name}}", config_.className},
            {"{{description}}", config_.description},
            {"{{goal}}", config_.goal},
            {"{{author}}", config_.author}
        };
        // For now, we only load the README from the template loader.
        // The rest of the README is still hardcoded to avoid having to create
        // an extremely complex template for it.
        std::string content = loader_.getAndSubstitute("README.md", substitutions);
        
        // The rest of the README generation can be added here if needed,
        // for example, to append the project structure dynamically.
        
        writeFile(filename, content);
    }

private:
    std::string generateLikertScaleHeader() {
        return "#ifndef LIKERT_SCALE_H\n"
               "#define LIKERT_SCALE_H\n\n"
               "#include <string>\n"
               "#include <vector>\n\n"
               "/**\n"
               " * @class LikertScale\n"
               " * @brief A simple class to display a Likert scale question and get a response.\n"
               " */\n"
               "class LikertScale {\n"
               "public:\n"
               "    /**\n"
               "     * @brief Construct a new Likert Scale object\n"
               "     * @param question The question to ask the user.\n"
               "     * @param options The list of options for the scale (e.g., \"Strongly Disagree\" to \"Strongly Agree\").\n"
               "     */\n"
               "    LikertScale(const std::string& question, const std::vector<std::string>& options);\n\n"
               "    /**\n"
               "     * @brief Displays the question and options, then waits for and validates user input.\n"
               "     * @return The user's choice as an integer (1-based index).\n"
               "     */\n"
               "    int displayAndGetResponse();\n\n"
               "private:\n"
               "    std::string question_;\n"
               "    std::vector<std::string> options_;\n"
               "};\n\n"
               "#endif // LIKERT_SCALE_H\n";
    }

    std::string generateLikertScaleImplementation() {
        return "#include \"../include/LikertScale.h\"\n"
               "#include <iostream>\n"
               "#include <limits>\n\n"
               "LikertScale::LikertScale(const std::string& question, const std::vector<std::string>& options)\n"
               "    : question_(question), options_(options) {}\n\n"
               "int LikertScale::displayAndGetResponse() {\n"
               "    std::cout << \"\\n\" << question_ << std::endl;\n"
               "    for (size_t i = 0; i < options_.size(); ++i) {\n"
               "        std::cout << \"  \" << (i + 1) << \". \" << options_[i] << std::endl;\n"
               "    }\n\n"
               "    int response = 0;\n"
               "    while (true) {\n"
               "        std::cout << \"Enter your choice (1-\" << options_.size() << \"): \";\n"
               "        std::cin >> response;\n\n"
               "        if (std::cin.good() && response >= 1 && response <= static_cast<int>(options_.size())) {\n"
               "            // Clear the input buffer\n"
               "            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\\n');\n"
               "            return response;\n"
               "        } else {\n"
               "            std::cin.clear();\n"
               "            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\\n');\n"
               "            std::cout << \"Invalid input. Please enter a number between 1 and \" << options_.size() << \".\" << std::endl;\n"
               "        }\n"
               "    }\n"
               "}\n";
    }

    std::string generateDataDictionary() {
        return "# Data Dictionary for " + config.name + "\n\n"
               "This document describes the format of the data files generated by this application.\n\n"
               "## File Naming Convention\n\n"
               "Data files are named using the following convention:\n"
               "`<participant_id>_<session_id>_<timestamp>.csv`\n\n"
               "## Data Columns\n\n"
               "| Column Name      | Data Type | Description                                        |\n"
               "|------------------|-----------|----------------------------------------------------|\n"
               "| `participant_id` | string    | Unique identifier for the participant.             |\n"
               "| `timestamp`      | string    | ISO 8601 timestamp of the event.                   |\n"
               "| `event_type`     | string    | The type of event being logged (e.g., 'response'). |\n"
               "| `event_data`     | JSON      | A JSON object containing event-specific data.      |\n\n"
               "--- \n"
               "*This is a template. Please update it to reflect the actual data your application will generate.*";
    }

    std::string generatePrivacyPolicy() {
        return "# Privacy Policy for " + config.name + "\n\n"
               "**Last Updated:** [Date]\n\n"
               "This privacy policy explains how " + config.author + " collects, uses, and protects your data in relation to the " + config.name + " application.\n\n"
               "## 1. Data We Collect\n\n"
               "This application collects the following data:\n"
               "- **[List of data collected, e.g., survey responses, reaction times, etc.]**\n"
               "- **[Specify if any personally identifiable information (PII) is collected]**\n\n"
               "## 2. How We Use Your Data\n\n"
               "Your data is used for the following research purposes:\n"
               "- **[Primary research goal, e.g., To study the effects of X on Y]**\n"
               "- **[Secondary research goals]**\n\n"
               "## 3. Data Storage and Security\n\n"
               "All data is stored locally on your device in the following location: `[Specify data storage path]`. "
               "We take reasonable measures to protect your data, including **[mention any security measures like encryption, if applicable]**.\n\n"
               "## 4. Data Sharing\n\n"
               "We will not share your personally identifiable data with third parties without your explicit consent. Anonymized, aggregated data may be shared with the broader scientific community or published in academic journals.\n\n"
               "## 5. Your Rights\n\n"
               "You have the right to:\n"
               "- Access your data at any time.\n"
               "- Request the deletion of your data.\n"
               "- Withdraw from the study at any time without penalty.\n\n"
               "## 6. Contact Us\n\n"
               "If you have any questions about this privacy policy, please contact us at **[Your Contact Email or Information]**.\n";
    }
};

// Interactive configuration function
ProjectConfig getProjectConfig() {
    ProjectConfig config;
    std::string input;
    
    std::cout << "=== Advanced C++ Template Generator ===\n\n";
    
    // Project name
    std::cout << "Enter project name (e.g., graph_analyzer, json_parser): ";
    std::getline(std::cin, config.name);
    config.className = StringUtils::capitalize(config.name);
    
    // Description
    std::cout << "Enter project description: ";
    std::getline(std::cin, config.description);
    
    // Goal
    std::cout << "Enter project goal: ";
    std::getline(std::cin, config.goal);
    
    // Author
    std::cout << "Enter author name: ";
    std::getline(std::cin, config.author);
    if (config.author.empty()) {
        config.author = "Unknown Author";
    }
    
    // Version
    std::cout << "Enter version (default: 1.0.0): ";
    std::getline(std::cin, config.version);
    if (config.version.empty()) {
        config.version = "1.0.0";
    }
    
    // Project type
    std::cout << "\nSelect project type:\n";
    std::cout << "1. Console Application\n";
    std::cout << "2. Static Library\n";
    std::cout << "3. Shared Library\n";
    std::cout << "4. Header-Only Library\n";
    std::cout << "5. GUI Application\n";
    std::cout << "6. Unit Test Framework\n";
    std::cout << "Choice (1-6): ";
    std::getline(std::cin, input);
    
    switch (input[0]) {
        case '2': config.type = ProjectType::STATIC_LIBRARY; break;
        case '3': config.type = ProjectType::SHARED_LIBRARY; break;
        case '4': config.type = ProjectType::HEADER_ONLY; break;
        case '5': config.type = ProjectType::GUI_APP; break;
        case '6': config.type = ProjectType::UNIT_TEST; break;
        default: config.type = ProjectType::CONSOLE_APP; break;
    }
    
    // Build system
    std::cout << "\nUse CMake? (y/N): ";
    std::getline(std::cin, input);
    config.useCMake = (input == "y" || input == "Y" || input == "yes");
    
    // Tests
    std::cout << "Include unit tests? (y/N): ";
    std::getline(std::cin, input);
    config.includeTests = (input == "y" || input == "Y" || input == "yes");
    
    // Git ignore
    std::cout << "Include .gitignore? (Y/n): ";
    std::getline(std::cin, input);
    config.includeGitIgnore = !(input == "n" || input == "N" || input == "no");

    // Likert Scale
    std::cout << "Include Likert Scale module for surveys? (y/N): ";
    std::getline(std::cin, input);
    config.includeLikertScale = (input == "y" || input == "Y" || input == "yes");

    // Data Dictionary
    std::cout << "Include a Data Dictionary file? (y/N): ";
    std::getline(std::cin, input);
    config.includeDataDictionary = (input == "y" || input == "Y" || input == "yes");

    // Privacy Policy
    std::cout << "Include a PRIVACY_POLICY.md file? (y/N): ";
    std::getline(std::cin, input);
    config.includePrivacyPolicy = (input == "y" || input == "Y" || input == "yes");
    
    return config;
}

#ifndef TEMPLATE_GENERATOR_TEST
int main() {
    try {
        ProjectConfig config = getProjectConfig();
        
        // Load templates from the SQL data file
        TemplateLoader loader("SQL/data.sql");

        // Create the generator and run it
        TemplateGenerator generator(config, loader);
        generator.generateProject();
        
        std::cout << "\n=== Generation Complete ===\n";
        std::cout << "Your '" << config.description << "' project is ready!\n";
        std::cout << "Next steps:\n";
        std::cout << "1. cd " << config.name << "\n";
        
        if (config.useCMake) {
            std::cout << "2. mkdir build && cd build\n";
            std::cout << "3. cmake ..\n";
            std::cout << "4. make\n";
        } else {
            std::cout << "2. make\n";
        }
        
        if (config.type == ProjectType::CONSOLE_APP || config.type == ProjectType::GUI_APP) {
            std::cout << "5. ./" << config.name << "\n";
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
#endif // TEMPLATE_GENERATOR_TEST
