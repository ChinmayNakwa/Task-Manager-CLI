#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

// Represents a single task in the TaskManager.
// This struct holds all data related to one to-do item.
struct Task {
    int id;
    std::string description;
    
    bool isCompleted;
    int priority; // e.g., 1 (low) to 3 (high)
    
    // A list of IDs of tasks that must be completed *before* this one can start.
    std::vector<int> dependencies; 

    // Default constructor to initialize values.
    Task() : id(0), isCompleted(false), priority(1) {} 
};

#endif 