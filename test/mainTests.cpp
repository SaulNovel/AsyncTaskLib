#include <cstdlib>
#include <vector>
#include <unordered_set>

#include "gtest/gtest.h"

#include "Scheduler.h"
#include "Counter.h"
#include "TestTask.h"

using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::unique_ptr;
using namespace std::chrono_literals;

/**
 * Commands: 
 * - start
 * - resume
 * - pause
 * - stop 
 * 
 * Status: 
 * - running
 * - paused
 * - stopped
 * - completed
*/

/* --- START COMMAND TO ALL STATUS --- */

/**
 * Test: task completes its execution
 * - Step 1: start task
 * - Step 2: lock main thread till task is completed
 * Expected: task.status() == completed
*/
TEST(AsyncTaskLibTest, Start_Completed) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);
    task.run_ = false;
    
    task.joinTask();

    ASSERT_EQ(task.status(), Task::StateType::completed);
}

/**
 * Test: start command triggered while task running
 * - Step 1: start task (x2)
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Start_if_Running) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    try {
        task.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot start task, '" + std::to_string(task.id()) + "', it's running or completed";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/**
 * Test: start command triggered while task paused
 * - Step 1: start task
 * - Step 2: pause task
 * - Step 3: start task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Start_if_Paused) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.pause();
    ASSERT_EQ(task.status(), Task::StateType::paused);

    try {
        task.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot start task, '" + std::to_string(task.id()) + "', it's running or completed";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/**
 * Test: start command trigger while task stopped
 * - Step 1: start task
 * - Step 2: stop task
 * - Step 3: start task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Start_if_Stopped) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);

    try {
        task.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot start task, '" + std::to_string(task.id()) + "', it's running or completed";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/**
 * Test: start command trigger while task completed
 * - Step 1: start task
 * - Step 2: lock main thread till task is completed
 * - Step 3: start task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Start_if_Completed)
{
    Scheduler scheduler;

    TestTask& task = scheduler.addTask<TestTask>(10ns);
    task.run_ = false;
    
    task.joinTask();

    ASSERT_EQ(task.status(), Task::StateType::completed);

    try {
        task.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot start task, '" + std::to_string(task.id()) + "', it's running or completed";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/* --- RESUME COMMAND TO ALL STATUS --- */

/**
 * Test: resume command triggered while task running
 * - Step 1: start task
 * - Step 2: resume task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Resume_if_Running) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    try {
        task.resume();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot resume task, '" + std::to_string(task.id()) + "', not paused";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/**
 * Test: pause and resume workflow
 * - Step 1: start task
 * - Step 2: pause task
 * - Step 3: resume task
 * Expected: task.status() == paused then task.status() == running
*/
TEST(AsyncTaskLibTest, Resume_if_Paused) 
{
    Scheduler scheduler;
    TestTask& count = scheduler.addTask<TestTask>(10ns);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);
}

/**
 * Test: resume command trigger while task stopped
 * - Step 1: start task
 * - Step 2: stop task
 * - Step 3: resume task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Resume_if_Stopped) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);

    try {
        task.resume();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot resume task, '" + std::to_string(task.id()) + "', not paused";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/**
 * Test: resume command trigger while task completed
 * - Step 1: start task
 * - Step 2: lock main thread till task is completed
 * - Step 3: resume task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Resume_if_Completed)
{
    Scheduler scheduler;

    TestTask& task = scheduler.addTask<TestTask>(10ns);
    task.run_ = false;
    
    task.joinTask();

    ASSERT_EQ(task.status(), Task::StateType::completed);

    try {
        task.resume();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot resume task, '" + std::to_string(task.id()) + "', not paused";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/* --- STOP COMMAND TO ALL STATUS --- */

/**
 * Test: stop command trigger while task running
 * - Step 1: start task
 * - Step 2: stop task
 * Expected: task.status() == stopped
*/
TEST(AsyncTaskLibTest, Stop_if_Running)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();

    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

/**
 * Test: pause resume and stop workflow
 * - Step 1: start task
 * - Step 2: pause task
 * - Step 2: resume task
 * - Step 3: stop task
 * Expected: task.status() == stopped
*/
TEST(AsyncTaskLibTest, Stop_if_Resumed) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.pause();
    ASSERT_EQ(task.status(), Task::StateType::paused);

    task.resume();
    ASSERT_EQ(task.status(), Task::StateType::running);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

/**
 * Test: Stop command trigger while paused
 * - Step 1: start task
 * - Step 2: pause task
 * - Step 2: stop task
 * Expected: task.status() == stopped
*/
TEST(AsyncTaskLibTest, Stop_if_Paused)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.pause();
    ASSERT_EQ(task.status(), Task::StateType::paused);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

/**
 * Test: Stop command trigger while completed
 * - Step 1: start task
 * - Step 2: lock main thread till task is completed
 * - Step 3: stop task
 * Expected: stop does not alter compledted status
*/
TEST(AsyncTaskLibTest, Stop_if_Completed)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.run_ = false;
    task.joinTask();
    ASSERT_EQ(task.status(), Task::StateType::completed);

    task.stop();

    ASSERT_EQ(task.status(), Task::StateType::completed);
}

/**
 * Test: Stop command trigger while stopped
 * - Step 1: start task
 * - Step 2: stop task (x2)
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Stop_if_Stopped)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();
    try {
        task.stop();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot stop task, '" + std::to_string(task.id()) + "', not running";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

/* --- PAUSE COMMAND TO ALL STATUS --- */

/**
 * Test: Pause command while task running 
 * - Step 1: start task
 * - Step 2: pause task
 * Expected: task.status() == paused
*/
TEST(AsyncTaskLibTest, Pause_if_Running) 
{
    Scheduler scheduler;
    TestTask& count = scheduler.addTask<TestTask>(10ns);
    
    ASSERT_EQ(count.status(), Task::StateType::running);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);
}

/**
 * Test: Pause command while task stopped 
 * - Step 1: run task
 * - Step 2: stop task
 * - Step 3: pause task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Pause_if_Stopped) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);

    try {
        task.pause();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot pause task, '" + std::to_string(task.id()) + "', not running";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
    ASSERT_EQ(task.status(), Task::StateType::stopped);

}

/**
 * Test: Pause command while task completed 
 * - Step 1: run task
 * - Step 2: lock main thread till task is completed
 * - Step 3: pause task
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Pause_if_completed)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.run_ = false;
    task.joinTask();
    ASSERT_EQ(task.status(), Task::StateType::completed);

    try {
        task.pause();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot pause task, '" + std::to_string(task.id()) + "', not running";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }

    ASSERT_EQ(task.status(), Task::StateType::completed);
}

/**
 * Test: Pause command while task paused 
 * - Step 1: run task
 * - Step 2: pause task (x2)
 * Expected: runtime_error
*/
TEST(AsyncTaskLibTest, Pause_if_Paused) 
{
    Scheduler scheduler;
    TestTask& count = scheduler.addTask<TestTask>(10ns);
    
    count.pause();

    ASSERT_EQ(count.status(), Task::StateType::paused);

    try {
        count.pause();
        FAIL() << "Expected std::runtime_error";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        const std::string exp_e = "Cannot pause task, '" + std::to_string(count.id()) + "', not running";
        ASSERT_EQ(std::string(e.what()), exp_e);
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

int main(int ac, char* av[])
{
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
} 
