#include "task_manager.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>

TaskManager::TaskManager(const std::string& filename) : fileHandler(filename) {
    fileHandler.loadTasks(tasks, nextId);
}

// The destructor ensures that the current state is saved when the program exits.
TaskManager::~TaskManager() {
    fileHandler.saveTasks(tasks);
}

void TaskManager::addTask(const std::string& description, int priority) {
    Task t;
    t.id = nextId++;
    t.description = description;
    t.isCompleted = false;
    t.priority = priority;
    tasks[t.id] = t;
    std::cout << "Success: Added task " << t.id << ": \"" << description << "\"" << std::endl;
}

void TaskManager::completeTask(int id) {
    if (tasks.find(id) == tasks.end()) {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
        return;
    }
    tasks.at(id).isCompleted = true;
    std::cout << "Success: Completed task " << id << "." << std::endl;
}

void TaskManager::deleteTask(int id) {
    if (tasks.erase(id) == 0) {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
        return;
    }
    
    // Also remove this task from any other task's dependency list.
    for (auto& pair : tasks) {
        auto& deps = pair.second.dependencies;
        deps.erase(std::remove(deps.begin(), deps.end(), id), deps.end());
    }
    std::cout << "Success: Deleted task " << id << "." << std::endl;
}

void TaskManager::addDependency(int taskToWaitFor, int taskThatWaits) {
    if (tasks.find(taskToWaitFor) == tasks.end() || tasks.find(taskThatWaits) == tasks.end()) {
        std::cerr << "Error: One or both task IDs are invalid." << std::endl;
        return;
    }
    if(taskToWaitFor == taskThatWaits){
        std::cerr << "Error: A task cannot depend on itself." << std::endl;
        return;
    }
    // Task `taskThatWaits` now depends on `taskToWaitFor`.
    tasks.at(taskThatWaits).dependencies.push_back(taskToWaitFor);
    std::cout << "Success: Task " << taskThatWaits << " now depends on task " << taskToWaitFor << "." << std::endl;
}

void TaskManager::listTasks() {
    std::cout << "--- To-Do List (Execution Order) ---" << std::endl;
    if (tasks.empty()) {
        std::cout << "Your task list is empty." << std::endl;
        return;
    }
    
    std::vector<int> sortedIds = topologicalSort();

    if (sortedIds.empty() && !tasks.empty()) {
         std::cout << "\nWarning: A circular dependency was detected! Cannot determine a valid order." << std::endl;
         std::cout << "Listing all tasks without ordering:" << std::endl;
         for(const auto& pair : tasks) printTask(pair.second);
    } else {
        for (int id : sortedIds) {
            printTask(tasks.at(id));
        }
    }
}

/**
 * Uses a std::priority_queue (implemented as a binary heap, a type of tree)
 * to greedily select and display the highest-priority task at each step.
 */
void TaskManager::listByPriority() {
    std::cout << "--- To-Do List (Priority Order) ---" << std::endl;
    if (tasks.empty()) {
        std::cout << "Your task list is empty." << std::endl;
        return;
    }

    // A lambda expression to define the comparison for a max-heap based on priority.
    auto compare = [](const Task& a, const Task& b) {
        return a.priority < b.priority;
    };
    std::priority_queue<Task, std::vector<Task>, decltype(compare)> pq(compare);

    for (const auto& pair : tasks) {
        if(!pair.second.isCompleted){
            pq.push(pair.second);
        }
    }

    if (pq.empty()) {
        std::cout << "All tasks are completed!" << std::endl;
        return;
    }

    while (!pq.empty()) {
        printTask(pq.top());
        pq.pop();
    }
}

/**
 * Implements Kahn's algorithm for topological sorting.
 * This determines a valid linear sequence to execute tasks based on their dependencies.
 */
std::vector<int> TaskManager::topologicalSort() {
    std::unordered_map<int, int> in_degree;
    std::unordered_map<int, std::vector<int>> adj;
    std::vector<int> sortedOrder;

    for (const auto& pair : tasks) {
        in_degree[pair.first] = 0;
    }

    // Build adjacency list and in-degree map from dependencies
    for (const auto& pair : tasks) {
        int task_id = pair.first;
        for (int dependency_id : pair.second.dependencies) {
            // Edge from dependency_id -> task_id
            adj[dependency_id].push_back(task_id);
            in_degree[task_id]++;
        }
    }

    // Initialize queue with all nodes having an in-degree of 0
    std::queue<int> q;
    for (const auto& pair : in_degree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        sortedOrder.push_back(u);

        // For each neighbor, reduce its in-degree
        if (adj.count(u)) {
            for (int v : adj.at(u)) {
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }
    }

    // If the sorted order contains fewer tasks than exist, there must be a cycle.
    if (sortedOrder.size() != tasks.size()) {
        return {}; // Return empty vector to indicate a cycle was detected.
    }
    
    return sortedOrder;
}

void TaskManager::printTask(const Task& task) {
    std::cout << "[" << (task.isCompleted ? "X" : " ") << "] "
              << "ID: " << task.id 
              << " | P: " << task.priority
              << " | " << task.description;
    if (!task.dependencies.empty()) {
        std::cout << " (depends on: ";
        for (size_t i = 0; i < task.dependencies.size(); ++i) {
            std::cout << task.dependencies[i] << (i == task.dependencies.size() - 1 ? "" : ", ");
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}