#include <Task/Task.h>

std::unordered_map<Task::StateType, std::string> Task::statusToStr = {
    {Task::idle, "idle"},
    {Task::running, "running"},
    {Task::paused, "paused"},
    {Task::stopped, "stopped"},
    {Task::completed, "completed"},
    {Task::aborted, "aborted"}
};