#ifndef SCHEDULER
#define SCHEDULER

#include <Task/Counter.h>
#include <Task/Fibonacci.h>

// TODO
#include <unistd.h>
#include <sys/syscall.h>

class Scheduler
{ 
 
 public:
    enum TaskType {
        counter,
        fibonacci
    };
 
 private:

    std::unordered_map<int, std::unique_ptr<Task>> tasks_;
    int count_;

// TODO: Fordward arguments
    template<class T>
    std::unique_ptr<Task> createTask(const TaskType type, T input) {
        switch(type) {
        case counter:
            return std::make_unique<Counter>(std::forward<T>(input));
        case fibonacci:
            return std::make_unique<Fibonacci>(std::forward<T>(input));
        default:
            std::ostringstream msg;
            msg  << "Task type: '" << type << "' not registered";
            throw std::runtime_error(msg.str());
        }
    }

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

    template<class T>
    void addTask(const TaskType type, T input) {
        std::unique_ptr<Task> task = createTask(static_cast<TaskType>(type), std::forward<T>(input));
        tasks_[count_++] = std::move(task);
    }

    const std::unordered_map<int, std::unique_ptr<Task>>& getTasks() {
        return tasks_;
    }

    const std::unique_ptr<Task>& getTask(const int id) {
        if (tasks_.count(id)) {
            return tasks_[id];   
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
