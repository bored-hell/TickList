# TickList

TickList is a simple TODO list desktop app written in C++ with a wxWidgets GUI. Tasks can be added, marked complete, prioritized, and are automatically saved to disk. All errors are logged to a file.

---

## Supported Platforms

- Currently, **TickList supports Linux only**.

---

## Installation

### Prerequisites

- C++20 or newer compiler  
- wxWidgets library (3.3) installed on Linux

### Build Instructions

1. Clone this repository:
```

git clone https://github.com/bored-hell/TickList.git
cd TickList

```
3. Build:
```

./build.sh

```
4. Run the application:
```

./bin/TickList

```

---

## Usage

- Add, edit, or remove tasks in the list.  
- Task data is saved automatically in `tasks.tof`.

---

## Error Handling & Logging

- Runtime errors and warnings are written to `errors.log` in the application directory.
- Abrupt exits on certain logic errors are logged for debugging.

---

This application is open source and can be modified as needed.
