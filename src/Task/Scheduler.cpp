#include <Task/Scheduler.h>

const std::vector<int> Scheduler::getTaskIds() const {
    std::vector<int> task_ids;
    task_ids.reserve(tasks_.size());
    for (auto& item : tasks_) {
        task_ids.push_back(item.first);
    }

    return task_ids;
}

Task& Scheduler::getTask(const int id) {
    if (tasks_.count(id)) {
        return *tasks_[id];   
    }
        
    std::ostringstream msg;
    msg << "Task with id '" << id << "' not found";
    throw std::runtime_error(msg.str());
}