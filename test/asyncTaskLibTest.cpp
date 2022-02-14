#include <gtest/gtest.h>
#include <Task/Scheduler.h>

#include <cstdlib>
#include <vector>
#include <unordered_set>

using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::unique_ptr;

TEST(AsyncTaskLibTest, Start) 
{
    Scheduler scheduler;
    Task& fib = scheduler.addTask<Fibonacci>(8);
    
    // Wait till complete
    while(fib.status() == Task::StateType::running) {
        usleep(10);
    }

    ASSERT_EQ(fib.status(), Task::StateType::completed);
}

TEST(AsyncTaskLibTest, Start_Stop)
{
    Scheduler scheduler;
    Task& fib = scheduler.addTask<Fibonacci>(std::numeric_limits<int>::max());

    usleep(100);

    fib.stop();

    ASSERT_EQ(fib.status(), Task::StateType::stopped);
}

TEST(AsyncTaskLibTest, Start_Pause_Stop)
{
    Scheduler scheduler;
    Task& fib = scheduler.addTask<Fibonacci>(std::numeric_limits<int>::max());

    fib.pause();

    int t = 5;
    while(t-- > 0) {
        ASSERT_EQ(fib.status(), Task::StateType::paused);
        usleep(10);
    }

    fib.stop();

    ASSERT_EQ(fib.status(), Task::StateType::stopped);
}

//TODO
static const std::unordered_map<Task::StateType, std::string> statusToStr = {
    {Task::StateType::running, "running"},
    {Task::StateType::paused, "paused"},
    {Task::StateType::stopped, "stopped"},
    {Task::StateType::completed, "completed"},
};

TEST(AsyncTaskLibTest, Start_Pause_Resume) 
{
    Scheduler scheduler;
    // 5 sec
    Task& count = scheduler.addTask<Counter>(5);

    count.pause();
    int t = 5;
    while(t-- > 0) {
        ASSERT_EQ(count.status(), Task::StateType::paused);
        usleep(10);
    }

    count.resume();
    // Wait till complete
    while(count.status() == Task::StateType::running) {
        usleep(10);
    }

    ASSERT_EQ(count.status(), Task::StateType::completed);
}

TEST(AsyncTaskLibTest, Start_Pause_Resume_Stop) 
{
    Scheduler scheduler;
    Task& fib = scheduler.addTask<Fibonacci>(std::numeric_limits<int>::max());

    fib.pause();

    int t = 5;
    while(t-- > 0) {
        ASSERT_EQ(fib.status(), Task::StateType::paused);
        usleep(10);
    }

    fib.resume();
    ASSERT_EQ(fib.status(), Task::StateType::running);

    fib.stop();
    ASSERT_EQ(fib.status(), Task::StateType::stopped);
}

TEST(AsyncTaskLibTest, Start_Pause_Resume_Pause_Resume) 
{
    Scheduler scheduler;
    // 5 sec
    Task& count = scheduler.addTask<Counter>(5);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);

    // Wait till complete
    while(count.status() == Task::StateType::running) {
        usleep(10);
    }

    ASSERT_EQ(count.status(), Task::StateType::completed);
}

/*

Exceptions:
- Command - command
- Start_Start
- Pause_Pause
- Resume_Resume
- Stop_Stop

- Status_command
- 

TEST(AsyncTaskLibTest, Start_Start) 
{
    Scheduler scheduler;
    Task& fib = scheduler.addTask<Fibonacci>(std::numeric_limits<int>::max());
    int task_id = fib.id();

    int t = 5;
    while(t-- > 0) {
        ASSERT_EQ(fib.status(), Task::StateType::running);
        usleep(100);
    }

    try {
        fib.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot start task, '" + std::to_string(task_id) + "', it's running";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }

}
*/


int main(int ac, char* av[])
{
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
} 
