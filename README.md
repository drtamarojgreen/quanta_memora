# Advanced C++ Template Generator

## Description

A powerful and comprehensive C++ project template generator that creates well-structured, modern C++17 projects with various configurations including console applications, libraries, GUI applications, and unit test frameworks.

## Features

### Enhanced Capabilities

The expanded template generator now includes:

- **Multiple Project Types**: Console apps, static/shared libraries, header-only libraries, GUI applications, and unit test frameworks
- **Modern C++17 Support**: Uses contemporary C++ standards and best practices
- **Flexible Build Systems**: Support for both CMake and traditional Makefiles
- **Comprehensive Project Structure**: Organized directory layout with proper separation of concerns
- **Testing Integration**: Built-in unit testing framework with optional test generation
- **Documentation Generation**: Automatic README.md, LICENSE, and .gitignore file creation
- **Cross-Platform Compatibility**: Works on Windows, Linux, and macOS
- **Interactive Configuration**: User-friendly prompts for project customization

### Project Types Supported

1. **Console Application**: Standard command-line applications with main entry point
2. **Static Library**: Compiled static libraries (.a/.lib files)
3. **Shared Library**: Dynamic libraries (.so/.dll files)
4. **Header-Only Library**: Template-based libraries with inline implementations
5. **GUI Application**: Framework for GUI applications (Qt, GTK, etc.)
6. **Unit Test Framework**: Dedicated testing applications with test runners

### Generated Project Structure

```
project_name/
├── include/
│   └── project_name.h          # Header declarations
├── src/
│   ├── main.cpp                # Entry point (for applications)
│   └── project_name.cpp        # Implementation
├── tests/
│   └── test_project_name.cpp   # Unit tests
├── build/                      # Build directory (CMake)
├── CMakeLists.txt              # CMake configuration
├── Makefile                    # Alternative build system
├── README.md                   # Project documentation
├── LICENSE                     # MIT License
└── .gitignore                  # Git ignore rules
```

## Original vs Enhanced Comparison

### Original Template Generator
- Basic console application template only
- Simple file generation (main.cpp, header, implementation, README)
- Limited customization options
- Basic build instructions
- Minimal project structure

### Enhanced Template Generator
- **6 different project types** with specialized templates
- **Advanced class generation** with proper inheritance and design patterns
- **Comprehensive build system support** (CMake + Makefile)
- **Integrated testing framework** with test runners and assertions
- **Professional documentation** with detailed README, LICENSE, and .gitignore
- **Interactive configuration** with user-friendly prompts
- **Cross-platform compatibility** with proper file system handling
- **Modern C++17 features** and best practices
- **Error handling and validation**
- **Extensible architecture** for future enhancements

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Standard C++ library with filesystem support
- CMake 3.12+ (optional, for CMake-based projects)
- Make utility (for Makefile-based projects)

## Building the Template Generator

### Using G++
```bash
g++ -std=c++17 template_generator.cpp -o template_generator
```

### Using CMake
```bash
mkdir build && cd build
cmake ..
make
```

### Using Visual Studio (Windows)
```bash
cl /std:c++17 template_generator.cpp
```

## Usage

### Interactive Mode
```bash
./template_generator
```

The generator will prompt you for:
- Project name
- Project description
- Project goal/purpose
- Author name
- Version number
- Project type selection
- Build system preference (CMake/Makefile)
- Testing framework inclusion
- Git ignore file generation

### Example Session
```
=== Advanced C++ Template Generator ===

Enter project name (e.g., graph_analyzer, json_parser): my_calculator
Enter project description: A scientific calculator with advanced functions
Enter project goal: Create a user-friendly calculator for mathematical operations
Enter author name: John Doe
Enter version (default: 1.0.0): 1.0.0

Select project type:
1. Console Application
2. Static Library
3. Shared Library
4. Header-Only Library
5. GUI Application
6. Unit Test Framework
Choice (1-6): 1

Use CMake? (y/N): y
Include unit tests? (y/N): y
Include .gitignore? (Y/n): y
```

## Generated Files Overview

### Core Files
- **Header File** (`include/project_name.h`): Class declarations, documentation, and API definitions
- **Implementation File** (`src/project_name.cpp`): Method implementations with error handling
- **Main File** (`src/main.cpp`): Application entry point with proper initialization

### Build System Files
- **CMakeLists.txt**: Modern CMake configuration with C++17 support, testing, and installation
- **Makefile**: Traditional makefile with debug/release targets and dependency management

### Documentation Files
- **README.md**: Comprehensive project documentation with build instructions and usage examples
- **LICENSE**: MIT license with proper copyright attribution
- **.gitignore**: Comprehensive ignore rules for C++ projects

### Testing Files
- **Test Files** (`tests/test_project_name.cpp`): Unit test framework with assertion helpers

## Advanced Features

### Class Generation Patterns

#### Application Classes
- Constructor/destructor with proper resource management
- Main application loop with error handling
- Initialize/update/cleanup lifecycle methods
- Shutdown mechanism for graceful termination

#### Library Classes
- Public API with version information
- Utility namespaces for helper functions
- Proper encapsulation and data hiding
- Exception-safe implementations

#### Test Classes
- Test runner with pass/fail tracking
- Assertion helpers for common test patterns
- Result reporting and statistics
- Extensible test framework structure

### Build System Features

#### CMake Support
- Modern CMake 3.12+ practices
- Automatic dependency detection
- Testing integration with CTest
- Installation and packaging support
- Compiler flag management

#### Makefile Support
- Debug and release configurations
- Automatic dependency generation
- Clean and install targets
- Cross-platform compatibility

## Customization and Extension

The template generator is designed to be easily extensible:

### Adding New Project Types
1. Add new enum value to `ProjectType`
2. Implement generation methods for the new type
3. Add user interface options
4. Update documentation

### Modifying Templates
- Edit the generation methods in `TemplateGenerator` class
- Customize file content in the respective `generate*` methods
- Add new file types by creating new generation methods

### Build System Integration
- Extend CMake templates for specific frameworks
- Add support for additional build systems (Bazel, Ninja, etc.)
- Integrate with package managers (Conan, vcpkg)

## Best Practices Implemented

### Code Organization
- Clear separation of interface and implementation
- Proper header guards and include management
- Namespace usage for utility functions
- Consistent naming conventions

### Error Handling
- Exception-safe code with RAII principles
- Proper error reporting and logging
- Graceful failure handling
- Resource cleanup in destructors

### Documentation
- Comprehensive inline documentation
- Usage examples and build instructions
- API documentation with parameter descriptions
- Contributing guidelines and coding standards

### Testing
- Unit test framework integration
- Test-driven development support
- Continuous integration ready
- Code coverage considerations

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow C++17 best practices
- Add tests for new functionality
- Update documentation for changes
- Ensure cross-platform compatibility

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Version History

### v2.0.0 (Current)
- Complete rewrite with advanced features
- Multiple project type support
- CMake and Makefile generation
- Comprehensive testing framework
- Professional documentation generation
- Interactive configuration system

### v1.0.0 (Original)
- Basic console application template
- Simple file generation
- Minimal project structure

## Author

Enhanced and expanded by the development team to provide a comprehensive C++ project scaffolding solution.

## Acknowledgments

- Original template generator concept
- C++ community best practices
- Modern CMake guidelines
- Cross-platform development standards
