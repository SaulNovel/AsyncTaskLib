#include <Counter.h>
#include <Fibonacci.h>

// TODO
#include <unistd.h>
#include <sys/syscall.h>

// TODO
class Scheduler
{   
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

    void addTask(int type) {
        std::unique_ptr<Task> task;
        if (type == 1) {
            task = std::make_unique<Counter>(30);
        }
        else {
            task = std::make_unique<Fibonacci>(100);
        }
        tasks_[count_++] = std::move(task);
    }

    const std::unordered_map<int, std::unique_ptr<Task>>& getTasks() {
        return tasks_;
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

void test() {
    Scheduler scheduler;
    scheduler.addTask(1);
    scheduler.addTask(2);

    Scheduler::printRunningThreads();

    const std::unordered_map<int, std::unique_ptr<Task>>& tasks = scheduler.getTasks();
    for (auto& item : tasks) {
        const std::unique_ptr<Task>& task = item.second;
        task->start();
    }

    Scheduler::printRunningThreads();

    sleep(3);

    // Pause counter
    const std::unique_ptr<Task>& counter = tasks[0];
    counter->pause();

    int x = 3;
    while(x-- > 0) {
        sleep(1);
        std::cout << counter.get() << std::endl;
    }

    counter->resume();

    const std::unique_ptr<Task>& fib = tasks[1];

    fib->stop();

    Scheduler::printRunningThreads();

}

int main()
{

    test();

    //testException();

    return 0;
}