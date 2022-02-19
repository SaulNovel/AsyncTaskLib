#ifndef SCHEDULER
#define SCHEDULER

#include <set>
#include <functional>
#include <vector>

#include "Task.h"

class Scheduler
{ 

 private:

    std::unordered_map<int, std::unique_ptr<Task>> tasks_;
    int count_;

    std::vector<std::reference_wrapper<Task>> tasks_ref_;

public:

    Scheduler()
    : count_(0) 
    {}

    ~Scheduler() {
    }

    template<class T, class I>
    T& addTask(const I& input) {
        count_++;
        auto task = std::make_unique<T>(count_, input);
        T& taskRef = *task;
        task->start();
        tasks_[count_] = std::move(task);
        tasks_ref_.push_back(*tasks_[count_]);

        return taskRef;
    }

    const std::set<int> getTaskIds() const;

    Task& getTask(const int id);

    const std::vector<std::reference_wrapper<Task>> getTasks() const {
        return tasks_ref_;
    }
};

#endif
