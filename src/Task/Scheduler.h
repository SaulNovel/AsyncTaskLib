#ifndef SCHEDULER
#define SCHEDULER

#include <Task/Task.h>

#include <vector>

class Scheduler
{ 

 private:

    std::unordered_map<int, std::unique_ptr<Task>> tasks_;
    int count_;

public:

    static std::mutex print_mutex_;

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

    const std::vector<int> getTaskIds() const {
        std::vector<int> task_ids;
        task_ids.reserve(tasks_.size());
        for (auto& item : tasks_) {
            task_ids.push_back(item.first);
        }

        return task_ids;
    }

    Task& getTask(const int id) {
        if (tasks_.count(id)) {
            return *tasks_[id];   
        }
        
        std::ostringstream msg;
        msg << "Task with id '" << id << "' not found";
        throw std::runtime_error(msg.str());
    }

    void printStatus() {
        std::lock_guard<std::mutex> lock(print_mutex_);
        for (auto& item : tasks_) {
            Task& task = *item.second;
            std::cout << task << std::endl;
        }
    }
};


std::mutex Scheduler::print_mutex_;

#endif
