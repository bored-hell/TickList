#include "core.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <climits>
#include <filesystem>

static std::random_device rd;
static std::default_random_engine dre(rd());
static std::uniform_int_distribution<uint64_t> uid(0, ULLONG_MAX);

static size_t GenerateId()
{
    return uid(dre);
}

static std::string_view get_value(std::string_view line, std::string_view key, size_t &pos)
{
    pos = line.find(key, pos);
    if (pos == std::string_view::npos)
        return "";

    pos += key.size();

    if (pos < line.size() && line[pos] == '"')
    {
        ++pos;
        size_t end_pos = line.find('"', pos);
        if (end_pos == std::string_view::npos)
        {
            CORE_LOG_FMT("Warning: missing closing quote for key '{}' in line: {}", key, line);
            return "";
        }
        std::string_view value = line.substr(pos, end_pos - pos);
        pos = end_pos + 1;

        while (pos < line.size() && (line[pos] == ',' || line[pos] == ' '))
            ++pos;

        return value;
    }
    else
    {
        size_t end_pos = line.find(',', pos);
        if (end_pos == std::string_view::npos)
            end_pos = line.size();

        std::string_view value = line.substr(pos, end_pos - pos);
        pos = end_pos;

        while (pos < line.size() && (line[pos] == ',' || line[pos] == ' '))
            ++pos;

        return value;
    }
}

static const std::filesystem::path &GetAppPath()
{
  std::filesystem::path home_dir = getenv("HOME");
  if (home_dir.empty())
    std::abort();

  static std::filesystem::path app_path = home_dir / ".TickList";

  if (!std::filesystem::exists(app_path))
  {
    std::error_code ec;
    if (!std::filesystem::create_directory(app_path, ec))
    {
      std::cerr << "std::filesystem error: " << ec.message() << '\n';
      std::abort();
    }
  }

  return app_path;
}

#ifdef _DEBUG
std::ofstream &InitLogger()
{
  static std::ofstream file(GetAppPath()/"errors.log", std::ios::out | std::ios::app);
  if (!file.is_open())
    std::abort();
  return file;
}
#endif

TaskList::TaskList()
{
    LoadTasks();
}

TaskList::~TaskList()
{
    SaveTasks();
}

std::pair<const size_t, Task> &TaskList::AddTask(std::string_view task, int priority)
{
    std::pair<std::unordered_map<size_t, Task>::iterator, bool> result;
    do
    {
        result = data.insert(std::pair{GenerateId(), Task{task.data(), false, priority}});
    } while (!result.second);

    return *(result.first);
}

Task *TaskList::GetTaskFromId(size_t id)
{
    return const_cast<Task *>(const_cast<const TaskList *>(this)->GetTaskFromId(id));
}

const Task *TaskList::GetTaskFromId(size_t id) const
{
    if (data.count(id) == 0)
    {
        CORE_LOG_FMT("Invalid id: {} to get task from", id);
        std::abort();
    }

    auto iter = data.find(id);
    return &iter->second;
}

void TaskList::RemoveTask(size_t id)
{
    if (data.count(id) == 0)
    {
        CORE_LOG_FMT("Removing task with invalid id: {}", id);
        return;
    }
    data.erase(id);
}

void TaskList::SaveTasks() const
{
    const std::filesystem::path filename = GetAppPath()/"tasks.tof";
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        CORE_LOG("File couldn't be opened");
        return;
    }

    for (auto &task : data)
    {
        file << std::format("task:\"{}\", done:{}, priority:{}\n", task.second.task_name, task.second.complete,
                     task.second.priority);
    }
}

void TaskList::LoadTasks()
{
    const std::filesystem::path filename = GetAppPath()/"tasks.tof";
    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::string line;
    Task task;

    while (std::getline(file, line))
    {
        std::string_view line_view = line;
        size_t pos = 0;

        std::string_view content = get_value(line_view, "task:", pos);
        if (content.empty())
        {
            CORE_LOG_FMT("Warning: Skipping malformed line (task not found): {}", line);
            continue;
        }
        task.task_name = content;

        std::string_view done_str = get_value(line_view, "done:", pos);
        bool done = false;
        if (!done_str.empty())
        {
            done = (done_str[0] == '1');
        }
        task.complete = done;

        std::string_view priority_str = get_value(line_view, "priority:", pos);
        if (priority_str.empty())
        {
            CORE_LOG_FMT("Warning: Skipping malformed line (priority not found): {}", line);
            continue;
        }

        int priority = 0;
        std::from_chars(priority_str.data(), priority_str.data() + priority_str.size(), priority);
        task.priority = priority;

        data.insert(std::pair{GenerateId(), task});
    }
}

std::string_view PriorityToStr(int priority)
{
    constexpr std::string_view priorities[] = {"NONE", "LOW", "MEDIUM", "HIGH", "URGENT!"};
    return priorities[priority];
}
