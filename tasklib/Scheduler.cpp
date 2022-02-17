#include "Scheduler.h"

const std::set<int> Scheduler::getTaskIds() const {
    std::set<int> task_ids;
    for (auto& item : tasks_) {
        task_ids.insert(item.first);
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