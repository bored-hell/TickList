#pragma once
#ifndef CORE_HPP_
#define CORE_HPP_

#include <string>
#include <string_view>
#include <functional>
#include <fstream>
#include <unordered_map>
#include <format>

#ifndef _DEBUG
std::ofstream &InitLogger();

#define CORE_LOG(msg) \
  do {\
    std::ofstream &logfile = InitLogger(); \
    logfile << __LINE__ << ":" << __FILE__ << ":" << __func__ << ": " << msg << '\n'; \
  } while (0)

#define CORE_LOG_FMT(msg, ...) \
  do { \
    std::ofstream &logfile = InitLogger(); \
    logfile << __LINE__ << ":" << __FILE__ << ":" << __func__ << ": " << std::format(msg, __VA_ARGS__) << '\n'; \
  } while (0)
#else
#define CORE_LOG(msg) (void)0
#define CORE_LOG_FMT(msg, ...) (void)0
#endif

struct Task
{
    std::string task_name;
    bool complete;
    int priority;
};

class TaskList
{
  public:
    TaskList();
    ~TaskList();
    const std::unordered_map<size_t, Task> &GetData() const
    {
        return data;
    }
    std::pair<const size_t, Task> &AddTask(std::string_view task, int priority);
    Task *GetTaskFromId(size_t id);
    const Task *GetTaskFromId(size_t id) const;
    void RemoveTask(size_t id);
    void SaveTasks() const;
    void LoadTasks();

  private:
    std::unordered_map<size_t, Task> data;
};

std::string_view PriorityToStr(int priority);

#endif //! CORE_HPP_
