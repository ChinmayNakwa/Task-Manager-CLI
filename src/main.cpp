#include <iostream>
#include <string>
#include <vector>
#include "task_manager.h"

void printHelp() {
    std::cout << "TaskMaster CLI - A To-Do List Manager" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  taskmaster add \"<description>\" [--priority <1-3>]" << std::endl;
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
    std::string command = argv[1];

    try {
        if (command == "add" && argc >= 3) {
            std::string description = argv[2];
            int priority = 1; // Default priority
            if (argc == 5 && std::string(argv[3]) == "--priority") {
                priority = std::stoi(argv[4]);
            }
            tm.addTask(description, priority);
        } else if (command == "list") {
            if (argc == 4 && std::string(argv[2]) == "--sort" && std::string(argv[3]) == "priority") {
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