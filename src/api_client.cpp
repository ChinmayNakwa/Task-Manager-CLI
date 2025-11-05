#include "api_client.h"
#include <iostream>
#include <cstdio>     
#include <stdexcept>  
#include <array>      
#include <string>     
#include <vector>     

// Define portable macros for popen/pclose to handle both Windows and POSIX systems.
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

std::vector<std::string> ApiClient::generateCommands(const std::string& goal) {
    
    // 1. Define the paths to the Python executable within the virtual environment
    //    and the helper script itself. These paths are relative to the project root
    //    where the `taskmaster` executable is run.
    #ifdef _WIN32
        // Paths for Windows (uses double backslashes for string literals)
        const std::string python_executable = "llm\\venv\\Scripts\\python.exe";
        const std::string python_script     = "llm\\gemini_helper.py";
    #else
        // Paths for macOS and Linux (uses forward slashes)
        const std::string python_executable = "llm/venv/bin/python";
        const std::string python_script     = "llm/gemini_helper.py";
    #endif

    // 2. Escape any double quotes in the goal string to prevent breaking the shell command.
    std::string escaped_goal = goal;
    size_t pos = 0;
    while ((pos = escaped_goal.find('"', pos)) != std::string::npos) {
        escaped_goal.replace(pos, 1, "\\\"");
        pos += 2; // Move past the newly inserted escape character and the quote
    }

    // 3. Construct the full command to be executed.
    //    It pipes the goal into the Python script's standard input.
    //    Example: echo "Plan a trip" | llm/venv/bin/python llm/gemini_helper.py
    std::string full_command = "echo \"" + escaped_goal + "\" | " + python_executable + " " + python_script;

    std::vector<std::string> commands;
    std::array<char, 256> buffer; // A buffer to read lines of output.

    // 4. Execute the command and open a pipe to read its standard output.
    //    The "r" mode stands for "read".
    FILE* pipe = POPEN(full_command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed! Could not execute the helper script. Check if paths are correct.");
    }

    // 5. Read the output from the pipe line by line until the end.
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        std::string line = buffer.data();
        
        // Clean up the line by removing trailing newline characters (\n or \r\n)
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (!line.empty()) {
            commands.push_back(line);
        }
    }

    // 6. Close the pipe and check the script's exit status.
    //    A non-zero status indicates that the Python script failed.
    int status = PCLOSE(pipe);
    if (status != 0) {
        // The Python script is designed to print detailed errors to stderr,
        // which will be visible in the user's console. This C++ error confirms the failure.
        throw std::runtime_error("Helper script exited with an error. See console for details from the script.");
    }

    return commands;
}