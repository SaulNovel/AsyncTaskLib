#include <Task/Scheduler.h>

void test() {

    // TODO: mirar boost para la CLI

    Scheduler scheduler;
    //scheduler.addTask(Scheduler::counter, 1e3);
    scheduler.addTask<Counter>(1e3);
    scheduler.addTask<Fibonacci>(100);

    Scheduler::printRunningThreads();

    // Pause counter
    Task& counter = scheduler.getTask(1);

    try {
        counter.start();
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    //const std::unique_ptr<Task>& counter1 = tasks[0];

    counter.pause();

    int x = 3;
    while(x-- > 0) {
        sleep(1);
        std::cout << counter << std::endl;
    }

    counter.resume();

    sleep(2);

    counter.stop();
    
    Task& fib = scheduler.getTask(2);
    fib.stop();

    Scheduler::printRunningThreads();
}

int main()
{

    test();

    //testException();

    return 0;
}