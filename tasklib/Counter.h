#ifndef COUNTER
#define COUNTER

#include "Task.h"
#include <math.h>

using namespace std::chrono_literals;

class Counter : public Task
{

public:
    Counter(const int id, int threshold = std::numeric_limits<int>::max()) 
    : Task(id), threshold_(threshold), count_(0) 
    {
    }

    double progress() override {
        double val = 100.0 * static_cast<double>(count_) / threshold_;
        return std::ceil(val * 100.0) / 100.0;
    }

private:
    const int threshold_;
    std::atomic<int> count_;

    void execute() {

        while(++count_ < threshold_) {
            checkCommand();
            std::this_thread::sleep_for(10ms);
        }
    }

};

#endif
