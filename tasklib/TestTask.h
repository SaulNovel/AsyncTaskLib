#ifndef TEST_TASK
#define TEST_TASK

#include "Task.h"

class TestTask : public Task
{

public:
    TestTask(const int id, std::chrono::nanoseconds sleep_duration) 
    : Task(id), run_(true), sleep_duration_(sleep_duration)
    {
    }

    ~TestTask() {
        join();
    }

    double progress() override {
        StateType state = status();
        if (state == StateType::completed || state == StateType::stopped) {
            return 100.0;
        }

        return 0.0;
    }

    std::atomic<bool> run_;
private:
    std::chrono::nanoseconds sleep_duration_;

    void execute() {

        while(run_) {
            checkCommand();
            std::this_thread::sleep_for(sleep_duration_);
        }
    }

};

#endif
