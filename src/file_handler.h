#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "task.h"

// Manages the persistence of tasks by saving to and loading from a file.
class FileHandler {
public:
    // Constructor takes the filename to manage.
    explicit FileHandler(const std::string& filename);

    // Reads tasks from the file into the provided map and sets the next available ID.
    void loadTasks(std::unordered_map<int, Task>& tasks, int& nextId);
    
    // Saves all tasks from the map into the file, overwriting its contents.
    void saveTasks(const std::unordered_map<int, Task>& tasks);

private:
    std::string filename;
};

#endif