#include <gtest/gtest.h>
#include <Task/Scheduler.h>

#include <cstdlib>

using std::vector;
using std::string;

// TODO parametrice tests, build input regarding task type and expected output
TEST(AsyncTaskLibTest, Pause_Resume) 
{
        Scheduler scheduler;

        /* Create and start tasks */
        int num_threads_available = static_cast<int>(std::thread::hardware_concurrency());
        for (int i=0; i<num_threads_available; i++) {
                int task_id = scheduler.addTask(Scheduler::counter, 10);
                scheduler.getTask(task_id)->start();
        }

        usleep(10);

        const std::unordered_map<int, std::unique_ptr<Task>>& tasks = scheduler.getTasks();

        for (auto& item : tasks) {
                const std::unique_ptr<Task>& task = item.second;
                GTEST_ASSERT_EQ(task->status(), Task::running);
        }

        /* Pause tasks */
        for (auto& item : tasks) {
                const std::unique_ptr<Task>& task = item.second;
                task->pause();
        }

        int x = 10;
        while(x-- > 0) {
                for (auto& item : tasks) {
                        const std::unique_ptr<Task>& task = item.second;
                        GTEST_ASSERT_EQ(task->status(), Task::paused);
                        // TODO: check no update in progress
                }
                usleep(100);
        }

        /* Resume tasks */
        for (auto& item : tasks) {
                const std::unique_ptr<Task>& task = item.second;
                task->resume();
                GTEST_ASSERT_EQ(task->status(), Task::running);
        }

        /* Wait till all tasks finished */
        int num_tasks = static_cast<int>(tasks.size());
        int count = 0;
        while(count < num_tasks) {
                for (auto& item : tasks) {
                        const std::unique_ptr<Task>& task = item.second;
                        if (task->status() == Task::completed) {
                                count++;
                                //GTEST_ASSERT_EQ(task->joinable(), false);
                        }
                }
        }
}


TEST(AsyncTaskLibTest, Stop_Long_Tasks) 
{
        Scheduler scheduler;
        const int min = static_cast<int>(Scheduler::counter);
        const int max = static_cast<int>(Scheduler::fibonacci);

        /* Create tasks and launch them */
        int num_threads_available = static_cast<int>(std::thread::hardware_concurrency());
        for (int i=0; i<num_threads_available; i++) {
                int rand_num = rand() % (max-min + 1) + min;
                int task_id = scheduler.addTask(static_cast<Scheduler::TaskType>(rand_num), std::numeric_limits<int>::max());
                scheduler.getTask(task_id)->start();
        }

        const std::unordered_map<int, std::unique_ptr<Task>>& tasks = scheduler.getTasks();

        usleep(100);

        /* Stop tasks */
        for (auto& item : tasks) {
                const std::unique_ptr<Task>& task = item.second;
                GTEST_ASSERT_EQ(task->status(), Task::running);               
                task->stop();
        }

        usleep(100);
        
        for (auto& item : tasks) {
                const std::unique_ptr<Task>& task = item.second;
                GTEST_ASSERT_EQ(task->status(), Task::stopped);
                // TODO: joinable does not work
                //GTEST_ASSERT_EQ(task->joinable(), false);
        }

}


int main(int ac, char* av[])
{
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
} 
