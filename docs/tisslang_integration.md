# Integrating Quanta Memora with Quanta Tissu and TissLang

## 1. Overview

This document outlines how to integrate the **Quanta Memora** C++ project with **Quanta Tissu**, an AI agentic assistant. The integration is orchestrated using **TissLang**, a high-level declarative language designed to manage agentic workflows.

- **Quanta Memora**: A suite of C++ applications, including Cognitive Behavioral Therapy (CBT) tools and an advanced template generator.
- **Quanta Tissu**: A minimal, transformer-based language model built in Python. It serves as the foundation for an agentic coding assistant.
- **TissLang**: A specialized language used to define complex tasks for the Quanta Tissu agent. It allows for file system operations, shell command execution, and outcome verification through assertions.

The goal of this integration is to use the Quanta Tissu agent, directed by a TissLang script, to compile, run, and test the C++ applications provided by Quanta Memora.

## 2. Prerequisites

Before proceeding, ensure you have the following set up:

1.  **Quanta Memora Project**:
    -   A C++17 compatible compiler (e.g., `g++`, `clang++`).
    -   The Quanta Memora source code.

2.  **Quanta Tissu Project**:
    -   Python 3.7+ and NumPy.
    -   The Quanta Tissu source code, cloned from its [GitHub repository](https://github.com/drtamarojgreen/quanta_tissu).

## 3. Integration Workflow

The integration process involves using a TissLang script to orchestrate the agent's interaction with the Quanta Memora codebase. The typical workflow is as follows:

1.  **Define a Task in TissLang**: Write a `.tiss` script that defines a high-level `TASK`.
2.  **Break Down the Task into Steps**: Use `STEP` blocks to define logical units of work, such as compiling the C++ code and then running it.
3.  **Execute Shell Commands**: Use the `RUN` command within a `STEP` to execute shell commands (e.g., `g++` to compile a C++ file).
4.  **Verify Outcomes**: Use `ASSERT` commands to check the results of the previous commands. For example, verify that a compilation command finished with an exit code of 0.
5.  **Execute the TissLang Script**: The Quanta Tissu agent parses and executes the TissLang script, carrying out the defined actions.

## 4. Example: Automating a CBT App with TissLang

This example demonstrates how to create a TissLang script to compile and run the `thought_record_journal` application from the Quanta Memora suite.

### Step 1: Create the TissLang Script

Create a file named `run_cbt_app.tiss` with the following content:

```tisslang
#TISS! Language=CPP

TASK "Compile and run the Thought Record Journal CBT application"

STEP "Compile the C++ source code" {
    # This command compiles the thought_record_journal.cpp file
    # and creates an executable named 'thought_record_journal'.
    RUN "g++ -std=c++17 -O2 cbt_apps/thought_record_journal.cpp -o thought_record_journal"

    # Verify that the compilation was successful.
    ASSERT LAST_RUN.EXIT_CODE == 0
    ASSERT FILE_EXISTS "thought_record_journal"
}

STEP "Run the compiled application" {
    # Execute the program.
    # Note: As the CBT apps are interactive, we can't easily check STDOUT here.
    # For this example, we will just confirm it runs without immediate error.
    RUN "./thought_record_journal"

    # Verify that the program started successfully.
    ASSERT LAST_RUN.EXIT_CODE == 0
}

STEP "Clean up the executable" {
    # Remove the compiled binary to keep the directory clean.
    RUN "rm thought_record_journal"
    ASSERT LAST_RUN.EXIT_CODE == 0
    ASSERT NOT FILE_EXISTS "thought_record_journal"
}
```

### Step 2: Execute the Script

To run this script, you would invoke the Quanta Tissu agent and point it to the `run_cbt_app.tiss` file. The (hypothetical) command would look something like this:

```bash
python quanta_tissu/agent.py --file run_cbt_app.tiss
```

### Expected Outcome

The Quanta Tissu agent will:
1.  Execute the `g++` command to compile `thought_record_journal.cpp`.
2.  Verify that the compilation succeeded and the `thought_record_journal` executable was created.
3.  Run the `thought_record_journal` application.
4.  Verify that the application ran without error.
5.  Remove the `thought_record_journal` executable.

This example illustrates how TissLang can serve as a powerful bridge, allowing an AI agent to manage and interact with traditional compiled codebases like Quanta Memora.
