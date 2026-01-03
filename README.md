# TickList

TickList is a simple TODO list desktop app written in C++ with a wxWidgets GUI. Tasks can be added, marked complete, prioritized, and are automatically saved to disk. All errors are logged to a file.

---

### Prerequisites

- C++20 or newer compiler
- wxWidgets library (3.3) installed

## Usage

- Add, edit, or remove tasks in the list.  
- Task data is saved automatically in `tasks.tof`.

---

## Error Handling & Logging

- Runtime errors and warnings are written to `errors.log` in the application directory.
- Abrupt exits on certain logic errors are logged for debugging.

---

This application is open source and can be modified as needed.
