#ifndef TASK_COUNTER
#define TASK_COUNTER

#include <Task/Task.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class Counter : public Task
{

public:
    Counter(int threshold = std::numeric_limits<int>::max()) 
    : threshold_(threshold), count_(0) 
    {
    }

    double progress() override {
        return 0.0;
    }

private:
    const int threshold_;
    int count_;

    // Constructor of the task takes arguments
    // implements a method getResult

    void execute() {
        sleep(1);

        if (count_++ > threshold_) {
            return;
        }

        std::cout << "count: " << count_ << std::endl;

        interrupt();
        
        execute();
    }

};

#endif
