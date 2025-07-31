#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <map>
#include <cstddef>
#include <cctype>

namespace fs = std::filesystem;

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
    std::vector<std::string> dependencies;
};

class TemplateGenerator {
private:
    ProjectConfig config;
    
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
    void setConfig(const ProjectConfig& cfg) {
        config = cfg;
    }
    
    void generateProject() {
        std::cout << "\n=== Generating Project Structure ===\n";
        
        // Create project directory
        createDirectory(config.name);
        
        // Create subdirectories based on project type
        if (config.type != ProjectType::HEADER_ONLY) {
            createDirectory(config.name + "/src");
        }
        createDirectory(config.name + "/include");
        
        if (config.includeTests) {
            createDirectory(config.name + "/tests");
        }
        
        if (config.useCMake) {
            createDirectory(config.name + "/build");
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
        
        generateReadme();
        generateLicense();
        
        std::cout << "\nProject '" << config.name << "' generated successfully!\n";
        std::cout << "Navigate to the project directory: cd " << config.name << std::endl;
    }
    
private:
    void generateMainFile() {
        if (config.type == ProjectType::STATIC_LIBRARY || 
            config.type == ProjectType::SHARED_LIBRARY ||
            config.type == ProjectType::HEADER_ONLY) {
            return; // Libraries don't need main files
        }
        
        std::string content;
        std::string filename = config.name + "/src/main.cpp";
        
        switch (config.type) {
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
        return "#include <iostream>\n"
               "#include \"../include/" + config.name + ".h\"\n\n"
               "int main() {\n"
               "    std::cout << \"Launching " + config.className + " Application...\" << std::endl;\n\n"
               "    " + config.className + " app;\n"
               "    return app.run();\n"
               "}\n";
    }
    
    std::string generateGuiMain() {
        return "#include <iostream>\n"
               "#include \"../include/" + config.name + ".h\"\n\n"
               "// TODO: Include GUI framework headers (Qt, GTK, etc.)\n\n"
               "int main(int argc, char* argv[]) {\n"
               "    std::cout << \"Launching " + config.className + " GUI Application...\" << std::endl;\n\n"
               "    // TODO: Initialize GUI framework\n"
               "    " + config.className + " app;\n"
               "    return app.run();\n"
               "}\n";
    }
    
    std::string generateTestMain() {
        return "#include <iostream>\n"
               "#include \"../include/" + config.name + ".h\"\n\n"
               "// Simple test runner - consider using Google Test, Catch2, or similar\n"
               "int main() {\n"
               "    std::cout << \"Running " + config.className + " Tests...\" << std::endl;\n\n"
               "    " + config.className + "Test test;\n"
               "    return test.runAllTests();\n"
               "}\n";
    }
    
    void generateHeaderFile() {
        std::string guard = toUpper(config.name) + "_H";
        std::string filename = config.name + "/include/" + config.name + ".h";
        
        std::string content = "#ifndef " + guard + "\n"
                             "#define " + guard + "\n\n"
                             "#include <string>\n"
                             "#include <vector>\n"
                             "#include <memory>\n\n";
        
        if (!config.description.empty()) {
            content += "/**\n * " + config.description + "\n";
            if (!config.author.empty()) {
                content += " * @author " + config.author + "\n";
            }
            if (!config.version.empty()) {
                content += " * @version " + config.version + "\n";
            }
            content += " */\n";
        }
        
        switch (config.type) {
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
        return "class " + config.className + " {\n"
               "private:\n"
               "    std::string appName;\n"
               "    bool isRunning;\n\n"
               "public:\n"
               "    " + config.className + "();\n"
               "    ~" + config.className + "();\n\n"
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
        return "class " + config.className + " {\n"
               "private:\n"
               "    // Private members\n\n"
               "public:\n"
               "    " + config.className + "();\n"
               "    ~" + config.className + "();\n\n"
               "    // Public API\n"
               "    void process();\n"
               "    std::string getVersion() const;\n"
               "};\n\n"
               "// Utility functions\n"
               "namespace " + toLower(config.className) + "_utils {\n"
               "    void helperFunction();\n"
               "}\n";
    }
    
    std::string generateTestClass() {
        return "class " + config.className + "Test {\n"
               "private:\n"
               "    int passedTests;\n"
               "    int failedTests;\n\n"
               "    void assert(bool condition, const std::string& testName);\n\n"
               "public:\n"
               "    " + config.className + "Test();\n"
               "    ~" + config.className + "Test();\n\n"
               "    int runAllTests();\n"
               "    void testBasicFunctionality();\n"
               "    void testEdgeCases();\n"
               "    void printResults();\n"
               "};\n";
    }
    
    void generateImplementationFile() {
        std::string filename = config.name + "/src/" + config.name + ".cpp";
        std::string content = "#include \"../include/" + config.name + ".h\"\n"
                             "#include <iostream>\n"
                             "#include <stdexcept>\n\n";
        
        switch (config.type) {
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
        return config.className + "::" + config.className + "()\n"
               "    : appName(\"" + config.className + "\"), isRunning(false) {\n"
               "    // Constructor implementation\n"
               "}\n\n"
               + config.className + "::~" + config.className + "() {\n"
               "    cleanup();\n"
               "}\n\n"
               "int " + config.className + "::run() {\n"
               "    try {\n"
               "        initialize();\n"
               "        isRunning = true;\n"
               "        \n"
               "        std::cout << \"Running " + config.description + "...\" << std::endl;\n"
               "        \n"
               "        // Main application loop\n"
               "        while (isRunning) {\n"
               "            update();\n"
               "            // TODO: Implement main logic: " + config.goal + "\n"
               "            break; // Remove this for continuous running\n"
               "        }\n"
               "        \n"
               "        return 0;\n"
               "    } catch (const std::exception& e) {\n"
               "        std::cerr << \"Error: \" << e.what() << std::endl;\n"
               "        return 1;\n"
               "    }\n"
               "}\n\n"
               "void " + config.className + "::initialize() {\n"
               "    std::cout << \"Initializing \" << appName << \"...\" << std::endl;\n"
               "    // TODO: Add initialization logic\n"
               "}\n\n"
               "void " + config.className + "::update() {\n"
               "    // TODO: Add update logic\n"
               "}\n\n"
               "void " + config.className + "::cleanup() {\n"
               "    std::cout << \"Cleaning up \" << appName << \"...\" << std::endl;\n"
               "    // TODO: Add cleanup logic\n"
               "}\n\n"
               "void " + config.className + "::shutdown() {\n"
               "    isRunning = false;\n"
               "}\n";
    }
    
    std::string generateLibraryImplementation() {
        return config.className + "::" + config.className + "() {\n"
               "    // Constructor implementation\n"
               "}\n\n"
               + config.className + "::~" + config.className + "() {\n"
               "    // Destructor implementation\n"
               "}\n\n"
               "void " + config.className + "::process() {\n"
               "    std::cout << \"Processing with " + config.className + "...\" << std::endl;\n"
               "    // TODO: Implement core functionality: " + config.goal + "\n"
               "}\n\n"
               "std::string " + config.className + "::getVersion() const {\n"
               "    return \"" + config.version + "\";\n"
               "}\n\n"
               "// Utility functions implementation\n"
               "namespace " + toLower(config.className) + "_utils {\n"
               "    void helperFunction() {\n"
               "        // TODO: Implement helper functionality\n"
               "    }\n"
               "}\n";
    }
    
    std::string generateTestImplementation() {
        return config.className + "Test::" + config.className + "Test()\n"
               "    : passedTests(0), failedTests(0) {\n"
               "    // Test constructor\n"
               "}\n\n"
               + config.className + "Test::~" + config.className + "Test() {\n"
               "    // Test destructor\n"
               "}\n\n"
               "int " + config.className + "Test::runAllTests() {\n"
               "    std::cout << \"Running all tests for " + config.className + "...\" << std::endl;\n"
               "    \n"
               "    testBasicFunctionality();\n"
               "    testEdgeCases();\n"
               "    \n"
               "    printResults();\n"
               "    return (failedTests == 0) ? 0 : 1;\n"
               "}\n\n"
               "void " + config.className + "Test::testBasicFunctionality() {\n"
               "    // TODO: Implement basic functionality tests\n"
               "    assert(true, \"Basic functionality test\");\n"
               "}\n\n"
               "void " + config.className + "Test::testEdgeCases() {\n"
               "    // TODO: Implement edge case tests\n"
               "    assert(true, \"Edge cases test\");\n"
               "}\n\n"
               "void " + config.className + "Test::assert(bool condition, const std::string& testName) {\n"
               "    if (condition) {\n"
               "        std::cout << \"[PASS] \" << testName << std::endl;\n"
               "        passedTests++;\n"
               "    } else {\n"
               "        std::cout << \"[FAIL] \" << testName << std::endl;\n"
               "        failedTests++;\n"
               "    }\n"
               "}\n\n"
               "void " + config.className + "Test::printResults() {\n"
               "    std::cout << \"\\nTest Results:\" << std::endl;\n"
               "    std::cout << \"Passed: \" << passedTests << std::endl;\n"
               "    std::cout << \"Failed: \" << failedTests << std::endl;\n"
               "    std::cout << \"Total: \" << (passedTests + failedTests) << std::endl;\n"
               "}\n";
    }
    
    void generateCMakeFile() {
        std::string filename = config.name + "/CMakeLists.txt";
        std::string content = "cmake_minimum_required(VERSION 3.12)\n"
                             "project(" + config.name + " VERSION " + config.version + ")\n\n"
                             "set(CMAKE_CXX_STANDARD 17)\n"
                             "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
                             "# Include directories\n"
                             "include_directories(include)\n\n";
        
        if (config.type == ProjectType::CONSOLE_APP || config.type == ProjectType::GUI_APP) {
            content += "# Executable\n"
                      "add_executable(" + config.name + "\n"
                      "    src/main.cpp\n"
                      "    src/" + config.name + ".cpp\n"
                      ")\n\n";
        } else if (config.type == ProjectType::STATIC_LIBRARY) {
            content += "# Static Library\n"
                      "add_library(" + config.name + " STATIC\n"
                      "    src/" + config.name + ".cpp\n"
                      ")\n\n";
        } else if (config.type == ProjectType::SHARED_LIBRARY) {
            content += "# Shared Library\n"
                      "add_library(" + config.name + " SHARED\n"
                      "    src/" + config.name + ".cpp\n"
                      ")\n\n";
        }
        
        if (config.includeTests) {
            content += "# Tests\n"
                      "enable_testing()\n"
                      "add_executable(" + config.name + "_tests\n"
                      "    tests/test_" + config.name + ".cpp\n"
                      ")\n"
                      "add_test(NAME " + config.name + "_tests COMMAND " + config.name + "_tests)\n\n";
        }
        
        content += "# Compiler flags\n"
                  "target_compile_options(" + config.name + " PRIVATE\n"
                  "    -Wall -Wextra -Wpedantic\n"
                  ")\n\n"
                  "# Installation\n"
                  "install(TARGETS " + config.name + " DESTINATION bin)\n"
                  "install(FILES include/" + config.name + ".h DESTINATION include)\n";
        
        writeFile(filename, content);
    }
    
    void generateMakefile() {
        std::string filename = config.name + "/Makefile";
        std::string content = "# Makefile for " + config.name + "\n\n"
                             "CXX = g++\n"
                             "CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude\n"
                             "SRCDIR = src\n"
                             "OBJDIR = obj\n"
                             "SOURCES = $(wildcard $(SRCDIR)/*.cpp)\n"
                             "OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)\n"
                             "TARGET = " + config.name + "\n\n"
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
        if (config.type == ProjectType::UNIT_TEST) return; // Already handled in main files
        
        std::string filename = config.name + "/tests/test_" + config.name + ".cpp";
        std::string content = "#include \"../include/" + config.name + ".h\"\n"
                             "#include <iostream>\n"
                             "#include <cassert>\n\n"
                             "// Simple test framework - consider using Google Test, Catch2, or similar\n\n"
                             "void test_" + toLower(config.className) + "_creation() {\n"
                             "    " + config.className + " obj;\n"
                             "    std::cout << \"[PASS] " + config.className + " creation test\" << std::endl;\n"
                             "}\n\n"
                             "void test_" + toLower(config.className) + "_functionality() {\n"
                             "    " + config.className + " obj;\n"
                             "    // TODO: Add specific functionality tests\n"
                             "    std::cout << \"[PASS] " + config.className + " functionality test\" << std::endl;\n"
                             "}\n\n"
                             "int main() {\n"
                             "    std::cout << \"Running tests for " + config.className + "...\" << std::endl;\n\n"
                             "    test_" + toLower(config.className) + "_creation();\n"
                             "    test_" + toLower(config.className) + "_functionality();\n\n"
                             "    std::cout << \"All tests passed!\" << std::endl;\n"
                             "    return 0;\n"
                             "}\n";
        
        writeFile(filename, content);
    }
    
    void generateGitIgnore() {
        std::string filename = config.name + "/.gitignore";
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
                             + config.name + "\n"
                             + config.name + "_tests\n";
        
        writeFile(filename, content);
    }
    
    void generateLicense() {
        std::string filename = config.name + "/LICENSE";
        std::string content = "MIT License\n\n"
                             "Copyright (c) 2024 " + config.author + "\n\n"
                             "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
                             "of this software and associated documentation files (the \"Software\"), to deal\n"
                             "in the Software without restriction, including without limitation the rights\n"
                             "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
                             "copies of the Software, and to permit persons to whom the Software is\n"
                             "furnished to do so, subject to the following conditions:\n\n"
                             "The above copyright notice and this permission notice shall be included in all\n"
                             "copies or substantial portions of the Software.\n\n"
                             "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
                             "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
                             "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
                             "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
                             "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
                             "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
                             "SOFTWARE.\n";
        
        writeFile(filename, content);
    }
    
    void generateReadme() {
        std::string filename = config.name + "/README.md";
        std::string content = "# " + config.className + "\n\n"
                             "## Description\n"
                             + config.description + "\n\n"
                             "## Goal\n"
                             + config.goal + "\n\n"
                             "## Project Structure\n"
                             "```\n"
                             + config.name + "/\n"
                             "├── include/\n"
                             "│   └── " + config.name + ".h          # Header file\n";
        
        if (config.type != ProjectType::HEADER_ONLY) {
            content += "├── src/\n";
            if (config.type != ProjectType::STATIC_LIBRARY && 
                config.type != ProjectType::SHARED_LIBRARY) {
                content += "│   ├── main.cpp              # Entry point\n";
            }
            content += "│   └── " + config.name + ".cpp         # Implementation\n";
        }
        
        if (config.includeTests) {
            content += "├── tests/\n"
                      "│   └── test_" + config.name + ".cpp    # Unit tests\n";
        }
        
        if (config.useCMake) {
            content += "├── build/                    # Build directory\n"
                      "├── CMakeLists.txt            # CMake configuration\n";
        } else {
            content += "├── Makefile                  # Build configuration\n";
        }
        
        content += "├── README.md                 # This file\n";
        
        if (config.includeGitIgnore) {
            content += "├── .gitignore                # Git ignore rules\n";
        }
        
        content += "└── LICENSE                   # License file\n"
                  "```\n\n"
                  "## Requirements\n"
                  "- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)\n";
        
        if (config.useCMake) {
            content += "- CMake 3.12 or higher\n";
        } else {
            content += "- Make utility\n";
        }
        
        content += "\n## Building\n\n";
        
        if (config.useCMake) {
            content += "### Using CMake\n"
                      "```bash\n"
                      "mkdir build && cd build\n"
                      "cmake ..\n"
                      "make\n"
                      "```\n\n";
        }
        
        content += "### Using Makefile\n"
                  "```bash\n"
                  "make                    # Build debug version\n"
                  "make release           # Build optimized version\n"
                  "make clean             # Clean build files\n"
                  "```\n\n"
                  "### Manual Compilation\n"
                  "```bash\n";
        
        if (config.type == ProjectType::CONSOLE_APP || config.type == ProjectType::GUI_APP) {
            content += "g++ -std=c++17 -Iinclude src/main.cpp src/" + config.name + ".cpp -o " + config.name + "\n";
        } else if (config.type == ProjectType::STATIC_LIBRARY) {
            content += "g++ -std=c++17 -Iinclude -c src/" + config.name + ".cpp -o " + config.name + ".o\n"
                      "ar rcs lib" + config.name + ".a " + config.name + ".o\n";
        } else if (config.type == ProjectType::SHARED_LIBRARY) {
            content += "g++ -std=c++17 -Iinclude -fPIC -shared src/" + config.name + ".cpp -o lib" + config.name + ".so\n";
        }
        
        content += "```\n\n";
        
        if (config.type == ProjectType::CONSOLE_APP || config.type == ProjectType::GUI_APP) {
            content += "## Running\n"
                      "```bash\n"
                      "./" + config.name + "\n"
                      "```\n\n";
        }
        
        if (config.includeTests) {
            content += "## Testing\n";
            if (config.useCMake) {
                content += "```bash\n"
                          "cd build\n"
                          "make test\n"
                          "```\n\n";
            } else {
                content += "```bash\n"
                          "g++ -std=c++17 -Iinclude tests/test_" + config.name + ".cpp src/" + config.name + ".cpp -o test_" + config.name + "\n"
                          "./test_" + config.name + "\n"
                          "```\n\n";
            }
        }
        
        content += "## Features\n"
                  "- Modern C++17 codebase\n"
                  "- Clean project structure\n"
                  "- Comprehensive build system\n";
        
        if (config.includeTests) {
            content += "- Unit testing framework\n";
        }
        
        content += "- Cross-platform compatibility\n\n"
                  "## Development\n"
                  "### Adding New Features\n"
                  "1. Add declarations to `include/" + config.name + ".h`\n"
                  "2. Implement functionality in `src/" + config.name + ".cpp`\n";
        
        if (config.includeTests) {
            content += "3. Add tests in `tests/test_" + config.name + ".cpp`\n";
        }
        
        content += "\n### Code Style\n"
                  "- Use consistent indentation (4 spaces)\n"
                  "- Follow C++ naming conventions\n"
                  "- Add comments for complex logic\n"
                  "- Keep functions focused and small\n\n"
                  "## Contributing\n"
                  "1. Fork the repository\n"
                  "2. Create a feature branch\n"
                  "3. Make your changes\n"
                  "4. Add tests for new functionality\n"
                  "5. Submit a pull request\n\n"
                  "## License\n"
                  "This project is licensed under the MIT License - see the LICENSE file for details.\n\n"
                  "## Author\n"
                  + config.author + "\n\n"
                  "## Version\n"
                  + config.version + "\n";
        
        writeFile(filename, content);
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
    config.className = capitalize(config.name);
    
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
    
    return config;
}

int main() {
    try {
        ProjectConfig config = getProjectConfig();
        
        TemplateGenerator generator;
        generator.setConfig(config);
        generator.generateProject();
        
        std::cout << "\n=== Generation Complete ===\n";
        std::cout << "Your " << config.description << " project is ready!\n";
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
