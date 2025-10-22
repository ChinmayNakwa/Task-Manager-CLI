#include "file_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>

FileHandler::FileHandler(const std::string& fname) : filename(fname) {}

/**
 * Loads tasks from the data file.
 * File Format: id,is_completed,priority,"description",dep1,dep2,...
 */
void FileHandler::loadTasks(std::unordered_map<int, Task>& tasks, int& nextId) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If the file doesn't exist, it's a fresh start.
        nextId = 1;
        return;
    }

    std::string line;
    int maxId = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        Task t;

        std::getline(ss, field, ',');
        t.id = std::stoi(field);

        std::getline(ss, field, ',');
        t.isCompleted = std::stoi(field);

        std::getline(ss, field, ',');
        t.priority = std::stoi(field);
        
        // Handle the quoted description
        std::getline(ss, field, '"'); // Consume until the first quote
        std::getline(ss, t.description, '"'); // Read the actual description

        std::getline(ss, field, ','); // Consume the comma after the closing quote

        // Read all remaining fields as dependencies
        while(std::getline(ss, field, ',')) {
            if (!field.empty()) {
                t.dependencies.push_back(std::stoi(field));
            }
        }

        tasks[t.id] = t;
        if (t.id > maxId) {
            maxId = t.id;
        }
    }
    nextId = maxId + 1;
    file.close();
}

/**
 * Saves tasks to the data file, overwriting it.
 */
void FileHandler::saveTasks(const std::unordered_map<int, Task>& tasks) {
    std::ofstream file(filename, std::ios::trunc); // Open in truncate mode to overwrite
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& pair : tasks) {
        const Task& t = pair.second;
        file << t.id << "," << t.isCompleted << "," << t.priority << ",\"" << t.description << "\"";
        
        for(int depId : t.dependencies) {
            file << "," << depId;
        }
        file << "\n";
    }
    file.close();
}