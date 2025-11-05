#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "task.h"
#include "file_handler.h"

// The main class that orchestrates all task operations.
class TaskManager {
public:
    explicit TaskManager(const std::string& filename);
    ~TaskManager(); // Destructor saves tasks on exit.

    void addTask(const std::string& description, int priority);
    int addTaskAndGetId(const std::string& description, int priority);

    void completeTask(int id);
    void deleteTask(int id);
    void addDependency(int taskToWaitFor, int taskThatWaits);
    
    void listTasks();
    void listByPriority();

private:
    // A hash map provides O(1) average time complexity for task lookups by ID.
    std::unordered_map<int, Task> tasks;
    int nextId;
    FileHandler fileHandler;

    // Helper function to perform topological sort on the task dependency graph.
    std::vector<int> topologicalSort();

    // Helper to print a single task in a formatted way.
    void printTask(const Task& task);
};

#endif