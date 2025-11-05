
# TaskMaster CLI
## TaskMaster is an AI integrated command-line task manager built with C++.

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

## Usage

Run the application from your project's root directory. The general syntax is `./taskmaster <command> [arguments]`.

### `add`
Creates a new task. The description must be enclosed in double quotes. You can optionally assign a priority from 1 (low) to 3 (high).

**Syntax:**
```bash
taskmaster add "<description>" [--priority <level>]
```
**Example(s):**
```bash
# Add a task with default priority
./taskmaster add "Write the first draft of the report"

# Add a high-priority task
./taskmaster add "Submit final report to the board" --priority 3
```

---
### `breakdown`
Leverages the Gemini AI to automatically break down a high-level goal into a detailed project plan. It generates the necessary `add` and `depends` commands and executes them for you.

**Note:** This command requires the `GEMINI_API_KEY` environment variable to be set.

**Syntax:**
```bash
taskmaster breakdown "<high_level_goal>"
```
**Example:**
```bash
./taskmaster breakdown "Organize a charity fundraising event"
```

---
### `list`
Displays all tasks. The default view uses a **topological sort** to show tasks in a valid order based on their dependencies. The optional `--sort priority` flag uses a **priority queue** to show the most important tasks first.

**Syntax:**
```bash
taskmaster list [--sort priority]
```
**Example(s):**
```bash
# List tasks in their logical execution order
./taskmaster list

# List tasks with the highest priority at the top
./taskmaster list --sort priority
```

---
### `done`
Marks a specific task as complete. The task will be marked with an `[X]`.

**Syntax:**
```bash
taskmaster done <task_id>
```
**Example:**
```bash
./taskmaster done 4
```

---
### `delete`
Permanently removes a task from the list.

**Syntax:**
```bash
taskmaster delete <task_id>
```
**Example:**
```bash
./taskmaster delete 7
```

---
### `depends`
Sets a dependency between two tasks. The second task cannot be considered ready until the first task is complete.

**Syntax:**
```bash
taskmaster depends <task_to_wait_for_id> <task_that_waits_id>
```
**Example:**
```bash
# Makes Task 5 dependent on the completion of Task 2
./taskmaster depends 2 5
```

---
### `help`
Displays the help message with a list of all available commands and their syntax.

**Syntax:**
```bash
taskmaster help
```
**Example:**
```bash
./taskmaster help
```

## Project Structure

The codebase is organized into a modular structure to promote clarity and maintainability.

```
.
├── README.md
├── LICENSE
├── requirements.txt
├── .env
├── llm/
│   └── gemini_helper.py
└── src/
    ├── api_client.cpp
    ├── api_client.h
    ├── file_handler.cpp
    ├── file_handler.h
    ├── main.cpp
    ├── task.h
    ├── task_manager.cpp
    └── task_manager.h

```
