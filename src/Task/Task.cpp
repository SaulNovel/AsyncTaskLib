#include <Task/Task.h>

std::unordered_map<Task::StateType, std::string> Task::statusToStr = {
    {Task::StateType::running, "running"},
    {Task::StateType::paused, "paused"},
    {Task::StateType::stopped, "stopped"},
    {Task::StateType::completed, "completed"},
};