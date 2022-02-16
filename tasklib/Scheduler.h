#ifndef SCHEDULER
#define SCHEDULER

#include <vector>

#include "Task.h"

class Scheduler
{ 

 private:

    std::unordered_map<int, std::unique_ptr<Task>> tasks_;
    int count_;

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

        return taskRef;
    }

    const std::vector<int> getTaskIds() const;

    Task& getTask(const int id);
};

#endif
