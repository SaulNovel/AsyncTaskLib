#include <Task/Task.h>

namespace {
    std::unordered_map<Task::StateType, std::string> statusToStr = {
        {Task::StateType::running, "running"},
        {Task::StateType::paused, "paused"},
        {Task::StateType::stopped, "stopped"},
        {Task::StateType::completed, "completed"},
    };
}

std::ostream& operator<<(std::ostream& os, Task& task) {
    os << "Task: '" << task.id() << "' status: '" << statusToStr[task.status()]  << "' progress: " << task.progress() << "%";
    return os;
}
