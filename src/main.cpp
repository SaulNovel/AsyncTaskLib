#include <Task/Scheduler.h>

void test() {

    // TODO: mirar boost para la CLI

    Scheduler scheduler;
    //scheduler.addTask(Scheduler::counter, 1e3);
    scheduler.addTask<Counter>(1e3);
    scheduler.addTask<Fibonacci>(100);


    Scheduler::printRunningThreads();

    const std::vector<int> task_ids = scheduler.getTaskIds();
    for (const int id : task_ids) {
        scheduler.getTask(id);
    }

    // Pause counter
    Task& counter = scheduler.getTask(1);
    //const std::unique_ptr<Task>& counter1 = tasks[0];

    std::cout << counter << std::endl;

    counter.pause();

    int x = 3;
    while(x-- > 0) {
        sleep(1);
        std::cout << counter << std::endl;
    }

    counter.resume();
    std::cout << counter << std::endl;

    sleep(2);

    counter.stop();

    std::cout << counter << std::endl;

    
    Task& fib = scheduler.getTask(2);
    fib.stop();
    std::cout << fib << std::endl;

    Scheduler::printRunningThreads();
}

int main()
{

    test();

    //testException();

    return 0;
}