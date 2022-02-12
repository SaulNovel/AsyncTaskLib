#include <Task/Scheduler.h>

void test() {
    Scheduler scheduler;
    scheduler.addTask(Scheduler::counter, 100);
    scheduler.addTask(Scheduler::fibonacci, 100);

    Scheduler::printRunningThreads();

    const std::unordered_map<int, std::unique_ptr<Task>>& tasks = scheduler.getTasks();
    for (auto& item : tasks) {
        const std::unique_ptr<Task>& task = item.second;
        task->start();
    }

    Scheduler::printRunningThreads();

    sleep(3);

    // Pause counter
    const std::unique_ptr<Task>& counter = scheduler.getTask(0);
    //const std::unique_ptr<Task>& counter1 = tasks[0];

    counter->pause();

    int x = 3;
    while(x-- > 0) {
        sleep(1);
        std::cout << counter.get() << std::endl;
    }

    counter->resume();

    const std::unique_ptr<Task> & fib = scheduler.getTask(1);

    fib->stop();

    Scheduler::printRunningThreads();
}

int main()
{

    test();

    //testException();

    return 0;
}