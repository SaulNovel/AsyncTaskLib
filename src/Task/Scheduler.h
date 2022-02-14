#ifndef SCHEDULER
#define SCHEDULER

#include <Task/Counter.h>
#include <Task/Fibonacci.h>

#include <vector>

// TODO
#include <unistd.h>
#include <sys/syscall.h>

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
        for (auto& item : tasks_) {
            item.second->join();
        }
    }

    template<class T, class I>
    Task& addTask(const I& input) {
        count_++;
        std::unique_ptr<Task> task = std::make_unique<T>(count_, input);
        Task& taskRef = *task;
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

    static void printRunningThreads() {

        std::lock_guard<std::mutex> lock(print_mutex_);
        std::cout << "------------------------------------------" << std::endl;
        std::ostringstream cmd;
        cmd << "ps -T | grep " << syscall(SYS_gettid);
        system(cmd.str().c_str());
        std::cout << "------------------------------------------" << std::endl;
    }
};


std::mutex Scheduler::print_mutex_;

#endif
