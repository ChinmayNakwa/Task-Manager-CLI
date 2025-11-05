#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map> 

#include "task_manager.h"
#include "api_client.h"

void processAICommand(const std::string& command, TaskManager& tm, std::map<int, int>& placeholderIdMap) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;

    if (cmd == "add") {
        std::string description;
        std::string priority_flag;
        int priority = 1;

        // Find the start and end of the description in quotes
        size_t first_quote = command.find('"');
        size_t last_quote = command.rfind('"');
        if (first_quote == std::string::npos || last_quote == first_quote) {
            std::cerr << "Warning: Skipping malformed AI 'add' command: " << command << std::endl;
            return;
        }
        description = command.substr(first_quote + 1, last_quote - first_quote - 1);

        // Check for a priority flag after the description
        std::string remainder = command.substr(last_quote + 1);
        std::stringstream remainder_ss(remainder);
        remainder_ss >> priority_flag;
        if (priority_flag == "--priority") {
            remainder_ss >> priority;
        }

        // Add the task and store its real ID, mapping it to the placeholder ID
        int placeholder_id = placeholderIdMap.size() + 1;
        int real_id = tm.addTaskAndGetId(description, priority);
        placeholderIdMap[placeholder_id] = real_id;

    } else if (cmd == "depends") {
        std::string from_str, to_str;
        ss >> from_str >> to_str;

        // Remove placeholder tags like '<ID_...>' to get the number
        from_str.erase(0, from_str.find_first_of("0123456789"));
        from_str.erase(from_str.find_last_not_of("0123456789") + 1);
        to_str.erase(0, to_str.find_first_of("0123456789"));
        to_str.erase(to_str.find_last_not_of("0123456789") + 1);

        int placeholder_from = std::stoi(from_str);
        int placeholder_to = std::stoi(to_str);
        
        if (placeholderIdMap.count(placeholder_from) && placeholderIdMap.count(placeholder_to)) {
            int real_from = placeholderIdMap.at(placeholder_from);
            int real_to = placeholderIdMap.at(placeholder_to);
            tm.addDependency(real_from, real_to);
        } else {
            std::cerr << "Warning: Skipping malformed AI 'depends' command with invalid placeholders: " << command << std::endl;
        }
    }
}


void printHelp() {
    std::cout << "TaskMaster CLI - A To-Do List Manager" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  taskmaster add \"<description>\" [--priority <1-3>]" << std::endl;
    std::cout << "  taskmaster breakdown \"<high_level_goal>\"" << std::endl; // New command added
    std::cout << "  taskmaster list [--sort priority]" << std::endl;
    std::cout << "  taskmaster done <task_id>" << std::endl;
    std::cout << "  taskmaster delete <task_id>" << std::endl;
    std::cout << "  taskmaster depends <task_to_wait_for_id> <task_that_waits_id>" << std::endl;
    std::cout << "  taskmaster help" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    TaskManager tm("tasks.data");
    ApiClient apiClient;
    std::string command = argv[1];

    try {
        if (command == "add" && argc >= 3) {
            std::string description = argv[2];
            int priority = 1; // Default priority
            if (argc == 5 && std::string(argv[3]) == "--priority") {
                priority = std::stoi(argv[4]);
            }
            tm.addTask(description, priority);
        } else if (command == "breakdown" && argc == 3) {
            std::string goal = argv[2];
            std::cout << "Analyzing goal with Gemini... this may take a moment." << std::endl;
            
            std::vector<std::string> commands = apiClient.generateCommands(goal);
            
            if (commands.empty()) {
                std::cout << "Could not generate a command plan." << std::endl;
            } else {
                std::cout << "Generated a plan with " << commands.size() << " commands. Executing..." << std::endl;
                std::map<int, int> placeholderIdMap; // Maps <ID_1> -> real_id
                for (const auto& cmd_str : commands) {
                    processAICommand(cmd_str, tm, placeholderIdMap);
                }
                std::cout << "Plan executed successfully." << std::endl;
            }
        } else if (command == "list") {
            if (argc == 3 && std::string(argv[2]) == "--sort" && std::string(argv[3]) == "priority") {
                tm.listByPriority();
            } else if (argc == 2) {
                tm.listTasks();
            } else {
                throw std::runtime_error("Invalid arguments for 'list'.");
            }
        } else if (command == "done" && argc == 3) {
            tm.completeTask(std::stoi(argv[2]));
        } else if (command == "delete" && argc == 3) {
            tm.deleteTask(std::stoi(argv[2]));
        } else if (command == "depends" && argc == 4) {
            tm.addDependency(std::stoi(argv[2]), std::stoi(argv[3]));
        } else if (command == "help") {
            printHelp();
        } else {
            throw std::runtime_error("Unknown command or incorrect arguments.");
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid number format provided for a task ID." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Provided number is out of range." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        printHelp();
    }

    return 0;
}