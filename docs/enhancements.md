# C++ Template Generator Enhancements

This document lists 50 potential enhancements for the C++ Template Generator. These improvements are designed to be implemented without requiring any external libraries or frameworks, focusing on expanding functionality, improving user experience, and increasing maintainability.

---

## I. User Experience & Interactivity

#### 1. Command-Line Argument Parsing
- **Description**: Allow users to specify all project configuration options via command-line flags (e.g., `--name "MyProject" --type console`) to bypass the interactive prompts, making the generator scriptable.
- **Implementation**:
    1. In `main()`, check `argc` and `argv` for command-line arguments.
    2. Implement a loop to parse flags (e.g., `--name`, `--type`, `--no-tests`).
    3. Populate the `ProjectConfig` struct from the parsed arguments.
    4. If any arguments are missing, fall back to interactive prompts or use default values.

#### 2. Configuration File Support
- **Description**: Allow the generator to read a configuration from a file (e.g., `template.cfg` in INI or a simple key-value format) using a flag like `--config path/to/file.cfg`.
- **Implementation**:
    1. Add a command-line flag (e.g., `--config`) to specify a config file path.
    2. Write a simple parser function that reads the file line by line, splitting on an '=' character.
    3. Populate the `ProjectConfig` struct with the values from the file.

#### 3. Non-Interactive Mode
- **Description**: Add a `--non-interactive` or `--silent` flag to use sensible default values for any unspecified options, allowing for quick, non-blocking project generation.
- **Implementation**:
    1. Add a boolean flag to the `ProjectConfig` struct, e.g., `bool nonInteractive`.
    2. In `getProjectConfig()`, if this flag is set, skip the `std::cout` and `std::cin` prompts and use pre-defined defaults.

#### 4. Customizable Output Directory
- **Description**: Allow the user to specify a different output directory for the generated project instead of defaulting to the current working directory.
- **Implementation**:
    1. Add a `std::string outputDir` to `ProjectConfig`.
    2. Add a command-line flag (`--output-dir`) and an interactive prompt to set this value.
    3. Prepend `outputDir` to all file and directory paths created by the generator.

#### 5. Input Validation
- **Description**: Implement stricter validation for user inputs, such as ensuring the project name is a valid directory name (no spaces or special characters) and the version follows semantic versioning.
- **Implementation**:
    1. In `getProjectConfig()`, after reading each input, pass it to a validation function.
    2. The validation function for project name should check for invalid characters.
    3. Use a loop to re-prompt the user if the input is invalid.

#### 6. Colorized Console Output
- **Description**: Use ANSI escape codes to add colors to the interactive prompts, section headers, and success/error messages to improve readability.
- **Implementation**:
    1. Define string constants for ANSI color codes (e.g., `const std::string red = "\033[31m";`).
    2. Wrap the output strings in these color codes.
    3. Add a check for terminal type or a `--no-color` flag to disable this feature on unsupported terminals.

#### 7. Pre-Generation Summary
- **Description**: Before generating the files, display a summary of the selected configuration options and ask the user for final confirmation.
- **Implementation**:
    1. After `getProjectConfig()` returns, print the contents of the `config` struct in a readable format.
    2. Ask the user to confirm (`[Y/n]`). If the user enters 'n', exit without generating.

#### 8. Custom Class Name
- **Description**: Explicitly ask the user for a "Class Name" to be used in the generated code, which can differ from the project name.
- **Implementation**:
    1. In `getProjectConfig()`, add a new prompt for the class name.
    2. The default value could be the capitalized version of the project name.
    3. Use this new value instead of `capitalize(config.name)` when generating classes.

#### 9. Save/Load Last Configuration
- **Description**: Automatically save the user's configuration choices to a file (e.g., `~/.cpp-template-generator.cfg`) and offer to load it on the next run.
- **Implementation**:
    1. After a successful generation, write the `ProjectConfig` to a file in the user's home directory.
    2. At startup, check if this file exists. If so, ask the user if they want to load the previous configuration.

#### 10. Dry Run Mode
- **Description**: Add a `--dry-run` flag that simulates the generation process by printing the files and directories that would be created without actually writing anything to disk.
- **Implementation**:
    1. Add a `bool dryRun` flag to `ProjectConfig`.
    2. In `createDirectory` and `writeFile`, check if `dryRun` is true. If so, print what would be done (e.g., `DRY RUN: Would create directory...`) and return immediately.

---

## II. Generated Project & Code Quality

#### 11. C++20/23 Standard Support
- **Status**: Planned
- **Description**: Add an option to select a newer C++ standard (C++20 or C++23), which would adjust the compiler flags in the generated `CMakeLists.txt` or `Makefile`.
- **Implementation**:
    1. Add a `std::string cppStandard` field to `ProjectConfig`.
    2. Add a prompt for the user to select the standard ("17", "20", "23").
    3. Use this value in `generateCMakeFile` and `generateMakefile` to set `CMAKE_CXX_STANDARD` or the `-std=` flag.

#### 12. Customizable Namespace
- **Status**: Planned
- **Description**: Allow the user to specify a top-level namespace for the generated C++ code.
- **Implementation**:
    1. Add a `std::string projectNamespace` to `ProjectConfig`.
    2. Add a prompt for the namespace.
    3. In the file generation methods, wrap class and function definitions in `namespace ` + `projectNamespace` + ` { ... }`.

#### 13. Generate `.editorconfig` File
- **Description**: Generate a `.editorconfig` file with common settings (e.g., indent style, line endings) to enforce consistent coding styles across different editors.
- **Implementation**:
    1. Create a new method `generateEditorConfig()`.
    2. Inside, write a string containing standard EditorConfig rules (e.g., `root = true`, `[*]`, `indent_style = space`).
    3. Call this method from `generateProject()`.

#### 14. Generate `CONTRIBUTING.md`
- **Description**: Generate a `CONTRIBUTING.md` file with boilerplate contribution guidelines.
- **Implementation**:
    1. Create a `generateContributingMd()` method.
    2. Write a string with standard contribution text (e.g., how to fork, create a branch, and submit a PR).
    3. Add an option to the user prompts to include this file.

#### 15. Generate `CHANGELOG.md`
- **Description**: Generate a `CHANGELOG.md` file, pre-populated with the initial version.
- **Implementation**:
    1. Create a `generateChangelog()` method.
    2. Write a string with a basic changelog structure, including `## [1.0.0] - YYYY-MM-DD` and an "Initial release" note.
    3. Add an option to include this file.

#### 16. Doxygen Configuration
- **Description**: Add an option to generate a `Doxyfile` configuration file, pre-configured with the project name and source/include paths, to facilitate documentation generation.
- **Implementation**:
    1. Create a `generateDoxyfile()` method.
    2. Write a string containing a basic `Doxyfile` configuration, using variables like `config.name` to fill in project-specific details.
    3. Add a user prompt to enable this feature.

#### 17. `.clang-format` File
- **Description**: Add an option to generate a `.clang-format` file with a default code style (e.g., Google, LLVM, or a custom style).
- **Implementation**:
    1. Create a `generateClangFormat()` method.
    2. Write a string with a sample `.clang-format` configuration in YAML format.
    3. Add a user prompt to include this file.

#### 18. Generate Example Usage for Libraries
- **Description**: For library projects, automatically create an `examples/` directory with a small `main.cpp` that demonstrates how to link against and use the library.
- **Implementation**:
    1. In `generateProject()`, if the type is a library, create an `examples/` directory.
    2. Generate an example `main.cpp` that includes the library's header and calls a public function.
    3. Update the `CMakeLists.txt` or `Makefile` to include a target for building the example.

#### 19. Multiple License Options
- **Description**: Allow the user to choose from a list of common open-source licenses (e.g., MIT, GPLv3, Apache 2.0) instead of hardcoding the MIT license.
- **Implementation**:
    1. Store license text for several licenses in a `std::map<std::string, std::string>`.
    2. Prompt the user to choose a license.
    3. In `generateLicense()`, write the selected license text to the `LICENSE` file.

#### 20. Generate Interface/Abstract Classes
- **Description**: Add a new project "subtype" or option to generate a class as an abstract interface with pure virtual functions (`virtual void myFunction() = 0;`).
- **Implementation**:
    1. Add a boolean flag `bool isInterface` to `ProjectConfig`.
    2. Add a prompt to ask the user if the class should be an interface.
    3. In `generateHeaderFile()`, if this is true, generate methods with the `= 0;` specifier and a virtual destructor.

#### 21. Add `res/` directory
- **Description**: For project types like GUI or Console applications, add an option to create a `res/` or `assets/` directory for storing resources like images, fonts, or data files.
- **Implementation**:
    1. Add a boolean `includeResources` to `ProjectConfig`.
    2. Add a prompt to ask the user if they need a resources directory.
    3. If true, call `createDirectory(config.name + "/res");` in `generateProject()`.

#### 22. Switchable Header Guards
- **Description**: Allow the user to choose between traditional `#ifndef`/`#define` header guards and the more modern `#pragma once`.
- **Implementation**:
    1. Add an enum `HeaderGuardType { IFNDEF, PRAGMA_ONCE }` to `ProjectConfig`.
    2. Add a prompt for the user to choose.
    3. In `generateHeaderFile()`, use an if-statement to generate the appropriate header guard.

#### 23. Add CPack Support to CMake
- **Description**: For CMake-based projects, automatically generate basic `CPack` commands in `CMakeLists.txt` to allow for easy creation of source and binary packages.
- **Implementation**:
    1. In `generateCMakeFile()`, append `include(CPack)` and `set(CPACK_GENERATOR "ZIP")` (or other generators) to the content.
    2. Use project variables (`PROJECT_NAME`, `PROJECT_VERSION`) to configure CPack settings.

#### 24. Improved Makefile Targets
- **Description**: Enhance the generated `Makefile` with more useful targets, such as `test` (to build and run tests), `install` (to copy binaries to a system location), and `uninstall`.
- **Implementation**:
    1. In `generateMakefile()`, add a `test:` target that depends on the test executable and then runs it.
    2. Improve the `install:` target to be more robust. Add an `uninstall:` target that removes the installed files.

#### 25. Generate Build/Run Scripts
- **Description**: Generate simple `build.sh` and `run.sh` scripts to abstract away the specific build commands, providing a unified interface for the user.
- **Implementation**:
    1. Create `generateBuildScript()` and `generateRunScript()` methods.
    2. The build script would contain the CMake or Make commands.
    3. The run script would simply execute the compiled binary from the correct path.

---

## III. Generator Internals & Maintainability

#### 26. Refactor `TemplateGenerator` Class
- **Status**: Planned
- **Description**: The `TemplateGenerator` class is doing too much. Refactor it by splitting its responsibilities into smaller, more focused classes (e.g., `ProjectWriter`, `CMakeGenerator`, `MakefileGenerator`, `SourceCodeGenerator`).
- **Implementation**:
    1. Define new classes for each responsibility.
    2. Move related methods from `TemplateGenerator` into these new classes (e.g., `generateCMakeFile` goes to `CMakeGenerator`).
    3. The `TemplateGenerator` class will then act as a facade, coordinating these new objects.

#### 27. Use a File-Based Templating System
- **Description**: Instead of hardcoding file content in C++ string literals, move the templates into separate text files. The generator would read these files and replace placeholders like `{{PROJECT_NAME}}`.
- **Implementation**:
    1. Create a `templates/` directory containing template files (e.g., `CMakeLists.txt.tpl`).
    2. Write a simple `replace` function that finds and replaces all occurrences of placeholders in a string.
    3. In the generation methods, read the corresponding template file into a string, perform the replacements, and then write the result.

#### 28. More Specific Error Handling
- **Description**: Improve error handling to provide more specific and helpful messages. For example, if a directory creation fails, report the reason (e.g., permissions denied, path not found).
- **Implementation**:
    1. The `std::filesystem` functions take an `std::error_code` parameter. Use the versions that take this parameter.
    2. After a call, check the error code. If it indicates an error, print a more informative message to `std::cerr`.

#### 29. Internal Logging System
- **Description**: Implement a simple logging mechanism that can write debug or trace information to a file (`generator.log`). This would help in diagnosing issues with the generator itself.
- **Implementation**:
    1. Create a simple `Logger` class with methods like `log_info()`, `log_error()`.
    2. The logger could be a singleton or passed around as a dependency.
    3. Replace `std::cout` status messages with calls to the logger. Add a `--verbose` flag to also print logs to the console.

#### 30. Unit Tests for the Generator
- **Description**: Create a suite of unit tests for the generator itself. These tests would run the generator and then verify that the output files and directories are created correctly.
- **Implementation**:
    1. This requires a testing framework, but a simple, self-contained test runner can be written without external libs.
    2. Create a `tests/` directory for the generator's own tests.
    3. Write test cases that call `generator.generateProject()` with various configs, and then use `std::filesystem` to check the output.

#### 31. Group Helper Functions
- **Description**: The helper functions (`capitalize`, `toUpper`, etc.) are in the global namespace. Group them into a `StringUtils` namespace or a static utility class.
- **Implementation**:
    1. Wrap the function definitions in `namespace StringUtils { ... }`.
    2. Update the call sites to be `StringUtils::capitalize(...)`.

#### 32. Consistent `std::filesystem::path` Usage
- **Description**: Instead of building paths by concatenating strings with `/`, use `std::filesystem::path` objects and the `/` operator for path composition to ensure cross-platform correctness.
- **Implementation**:
    1. In `generateProject()`, create a `fs::path rootPath = config.name;`.
    2. Construct all other paths from this base path: `fs::path srcDir = rootPath / "src";`.
    3. Pass the `fs::path` objects directly to `fs::create_directories` and `std::ofstream`.

#### 33. Git Repository Initialization
- **Description**: Add an option to automatically run `git init` in the new project directory and create an initial commit with all the generated files.
- **Implementation**:
    1. Add a `bool initGit` to `ProjectConfig` and a corresponding user prompt.
    2. If true, after generating all files, use the `system()` call to run `git init` and `git add . && git commit -m "Initial commit"`. This is a borderline external dependency, but `git` is often available.

#### 34. Improved Test Framework Generation
- **Description**: Enhance the built-in test framework template with more assertion types (`assertEqual`, `assertNotEqual`, `assertThrow`) and better test result reporting.
- **Implementation**:
    1. In `generateTestImplementation()`, add implementations for new assertion functions.
    2. The `assert` function could be overloaded or templated to handle different types.
    3. Improve the `printResults()` function to show which tests failed.

#### 35. Add Comments to Generated Build Files
- **Description**: Add more comments to the `CMakeLists.txt` and `Makefile` to explain what each section does. This is especially helpful for users who are new to these build systems.
- **Implementation**:
    1. In `generateCMakeFile()` and `generateMakefile()`, add more `"#"` comment lines to the string content, explaining commands like `add_executable`, `target_link_libraries`, etc.

#### 36. Extensible Architecture for New Project Types
- **Description**: Refactor the design to make it easier to add new project types. This could involve using a factory pattern or a map of function pointers/objects for the generation logic.
- **Implementation**:
    1. Create a base `ProjectGenerator` interface with a `generate()` method.
    2. Create concrete classes for each project type (`ConsoleAppGenerator`, `StaticLibGenerator`, etc.).
    3. Use a map `std::map<ProjectType, std::unique_ptr<ProjectGenerator>>` to hold instances of these generators.
    4. The main `generateProject` function would look up the correct generator and call its `generate` method.

#### 37. Support for `.cppm` Module Interface Units
- **Description**: With C++20 support, add an option to generate a C++ module interface file (`.cppm`) instead of a traditional header file.
- **Implementation**:
    1. Add a boolean `useModules` option to `ProjectConfig`.
    2. If true, `generateHeaderFile` would instead generate a `.cppm` file with `export module ...;` syntax.
    3. The implementation file would use `import ...;`.
    4. The build system generation would need to be updated to handle modules (e.g., specific compiler flags).

#### 38. Generate `.gitattributes` file
- **Description**: Generate a `.gitattributes` file to enforce consistent line endings (`* text=auto`) and other git-related settings.
- **Implementation**:
    1. Create a `generateGitAttributes()` method.
    2. Write a string with common `.gitattributes` settings.
    3. Add a user prompt to enable its creation.

#### 39. Platform-Specific Code Placeholders
- **Description**: In generated code, provide preprocessor blocks (`#ifdef _WIN32 ... #elif __linux__ ... #endif`) as placeholders for platform-specific implementations.
- **Implementation**:
    1. In the string templates for source files, add sections like `// Platform-specific code` followed by the `#ifdef` blocks in relevant places (e.g., for file paths or system calls).

#### 40. Generate a `docs` directory
- **Description**: Automatically create a `docs/` directory within the generated project, perhaps with a placeholder `index.md` file, to encourage documentation.
- **Implementation**:
    1. In `generateProject`, add a call to `createDirectory(config.name + "/docs");`.
    2. Optionally, write a simple placeholder file inside it.

#### 41. Add a "verbosity" level
- **Description**: Allow the user to select a verbosity level (e.g., `-v`, `-vv`) which controls how much output the generator produces.
- **Implementation**:
    1. Add an `int verbosity` to `ProjectConfig`.
    2. Use a command-line flag to set this level.
    3. Guard `std::cout` statements with checks like `if (config.verbosity > 0)`.

#### 42. Better Camel/Snake Case Conversion
- **Description**: The `camelToSnake` function is simple. Improve it to handle acronyms (e.g., `MyTCPConnection` -> `my_tcp_connection`) and numbers correctly.
- **Implementation**:
    1. Rewrite `camelToSnake` to be more state-aware. It should check for sequences of uppercase letters and treat them as a single block.

#### 43. Support for project-level `include` directory
- **Description**: Some projects prefer a single top-level `include` directory rather than one inside the project folder. Add an option for this.
- **Implementation**:
    1. Add a boolean `topLevelInclude` to `ProjectConfig`.
    2. If true, create `include/` and `src/project_name` instead of `project_name/include` and `project_name/src`. Adjust build system paths accordingly.

#### 44. Pre-commit Hook Example
- **Description**: Generate an example pre-commit hook (e.g., in `.githooks/pre-commit`) that could be used to run a linter or tests before committing.
- **Implementation**:
    1. Create a `generatePrecommitHook()` method.
    2. Write a simple shell script as a string (e.g., one that runs `make test`).
    3. Create the `.githooks` directory and write the file. Add instructions in the README on how to enable it (`git config core.hooksPath .githooks`).

#### 45. Add `.clang-tidy` configuration file
- **Description**: Generate a `.clang-tidy` file with a basic set of checks enabled to encourage static analysis.
- **Implementation**:
    1. Create a `generateClangTidy()` method.
    2. Write a string with a sample `.clang-tidy` configuration in YAML format, specifying a set of checks.
    3. Add a user prompt to enable this feature.

#### 46. Generate a `THIRD_PARTY_NOTICES.md`
- **Description**: Create a placeholder file for third-party library licenses and notices, which is good practice for projects that eventually add dependencies.
- **Implementation**:
    1. Create a `generateThirdPartyNotices()` method.
    2. Write a simple placeholder text explaining the purpose of the file.
    3. Call this from `generateProject()`.

#### 47. Support for different testing frameworks (placeholders)
- **Description**: While not adding the libraries themselves, allow the user to select a test framework (e.g., Google Test, Catch2). The generator would create the appropriate boilerplate/placeholder code for that framework.
- **Implementation**:
    1. Add an enum for `TestFramework`.
    2. In `generateTestFile()`, use a `switch` statement to generate the correct `main()` function and test case syntax for the selected framework.

#### 48. Generate a `CODE_OF_CONDUCT.md`
- **Description**: Add an option to include a `CODE_OF_CONDUCT.md` file, using a standard template like the Contributor Covenant.
- **Implementation**:
    1. Create a `generateCodeOfConduct()` method.
    2. Store the Contributor Covenant text in a string.
    3. Add a user prompt to enable this feature.

#### 49. Static analysis flags for build systems
- **Description**: Add an option to include compiler flags for static analysis (e.g., `-fsanitize=address,undefined`) in the debug build configuration.
- **Implementation**:
    1. In `generateCMakeFile` and `generateMakefile`, add these flags to the debug-specific `CXXFLAGS` or `target_compile_options`.
    2. This could be controlled by a `bool enableSanitizers` in `ProjectConfig`.

#### 50. Option for single file executable
- **Description**: For simple console apps, provide an option to generate a single `main.cpp` file without a separate class or header, for users who want a minimal starting point.
- **Implementation**:
    1. Add a `bool singleFile` option.
    2. If true, the generator would only create `main.cpp` with a simple `int main()` function and skip creating the class header and implementation files.
