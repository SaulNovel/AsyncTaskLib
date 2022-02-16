#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>
#include <unordered_set>

#include "Scheduler.h"
#include "Counter.h"
#include "TestTask.h"

using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::unique_ptr;
using namespace std::chrono_literals;

/**
 * Commands: run(start/resume), pause, stop 
 * Status: running, paused, stopped, completed
*/

/* --- RUN TO ALL STATUS --- */

// run -> start()
TEST(AsyncTaskLibTest, Run_till_Completed) 
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);
    task.run_ = false;
    
    task.joinTask();

    ASSERT_EQ(task.status(), Task::StateType::completed);
}

// run -> start() while running
TEST(AsyncTaskLibTest, Run_if_Running) 
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

// run -> resume(), while running
TEST(AsyncTaskLibTest, Run_if_Running2) 
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

// run -> start() while paused, run -> resume() while paused
TEST(AsyncTaskLibTest, Run_if_Paused) 
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

    // Resume
    task.resume();
    ASSERT_EQ(task.status(), Task::StateType::running);
}

TEST(AsyncTaskLibTest, Run_if_Paused2) 
{
    Scheduler scheduler;
    // 5 sec
    TestTask& count = scheduler.addTask<TestTask>(10ns);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);
}

// run -> start() while stopped, run -> resume() while stopped

// Separar test: 
// - start
// - resume
TEST(AsyncTaskLibTest, Run_if_Stopped) 
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

    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

// run -> start() while completed, run -> resume() while completed
TEST(AsyncTaskLibTest, Run_if_Completed)
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

    ASSERT_EQ(task.status(), Task::StateType::completed);
}

/* --- STOP TO ALL STATUS --- */

TEST(AsyncTaskLibTest, Stop_if_Running)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.stop();

    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

TEST(AsyncTaskLibTest, Stop_if_Running2) 
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

TEST(AsyncTaskLibTest, Stop_if_Paused)
{
    Scheduler scheduler;
    TestTask& task = scheduler.addTask<TestTask>(10ns);

    task.pause();
    ASSERT_EQ(task.status(), Task::StateType::paused);

    task.stop();
    ASSERT_EQ(task.status(), Task::StateType::stopped);
}

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


/* --- PAUSE TO ALL STATUS --- */

// pause while running
TEST(AsyncTaskLibTest, Pause_if_Running) 
{
    Scheduler scheduler;
    TestTask& count = scheduler.addTask<TestTask>(10ns);
    
    ASSERT_EQ(count.status(), Task::StateType::running);

    count.pause();
    ASSERT_EQ(count.status(), Task::StateType::paused);

    count.resume();
    ASSERT_EQ(count.status(), Task::StateType::running);

    count.run_ = false;
    count.joinTask();
    ASSERT_EQ(count.status(), Task::StateType::completed);
}

// pause while stopped
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

// TODO: NullTask que nunca se ejecuta
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

// pause while paused
//TODO: it freezes
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

    //std::cout << "paused" << std::endl;

    count.resume();

    //std::cout << "try to resume after paused" << std::endl;

    count.run_ = false;
    count.joinTask();

    ASSERT_EQ(count.status(), Task::StateType::completed);
}

int main(int ac, char* av[])
{
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
} 
