
# TaskMaster CLI
## TaskMaster is a command-line task manager built with C++.

## Features
- Task Management: Add, list, complete, and delete tasks directly from your terminal.
- Data Persistence: Your task list is automatically saved to a local file (tasks.data), so your data is never lost between sessions.
- Dependency Management: Define dependencies between tasks (e.g., Task B cannot start until Task A is complete). The system ensures you always see your tasks in a valid, logical order.
- Priority System: Assign priorities to tasks to help you focus on what's most important.
- Intelligent Ordering:
    The default list command uses a Topological Sort to display tasks in a valid execution order.
    The list --sort priority command uses a Priority Queue to show you the most urgent tasks first.
- Cycle Detection: The application is smart enough to detect impossible workflows, such as circular dependencies (e.g., A -> B -> A), and will alert you to the error.

## Core Concepts Implemented

This project was built to demonstrate a comprehensive understanding of advanced data structures and algorithms from the ADSA syllabus.

| Data Structure / Algorithm | C++ Implementation | Purpose in TaskMaster |
| :--- | :--- | :--- |
| **Hash Table** | `std::unordered_map` | Provides O(1) average-time complexity for all core task operations (lookup, completion, deletion) by mapping a unique `task_id` to its data. |
| **Directed Acyclic Graph** | Adjacency List (via `Task` dependencies) | Models the relationships between tasks, where each task is a node and a dependency is a directed edge. |
| **Topological Sort** | Kahn's Algorithm (BFS-based) | Computes a valid, linear execution order for tasks based on their dependencies. This is the core of the default `list` command. |
| **Priority Queue (Heap)** | `std::priority_queue` | Enables efficient, greedy retrieval of the highest-priority task. This powers the `list --sort priority` command. |

## Project Structure

The codebase is organized into a modular structure to promote clarity and maintainability.

```
.
├── src/
│   ├── main.cpp              # Handles CLI parsing and user interaction
│   ├── task.h                # Defines the Task data structure
│   ├── task_manager.h        # Header for the core logic class
│   ├── task_manager.cpp      # Implements all data structures and algorithms
│   ├── file_handler.h        # Header for data persistence logic
│   └── file_handler.cpp      # Implements saving/loading tasks to a file
└── README.md
```
