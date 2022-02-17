#ifndef COUNTER
#define COUNTER

#include <math.h>

#include "Task.h"

using namespace std::chrono_literals;

class Counter : public Task
{

public:
    Counter(const int id, int threshold = std::numeric_limits<int>::max()) 
    : Task(id), threshold_(threshold), count_(0), progress_(0.0)
    {
    }

    double progress() override {
        return progress_;
    }

private:
    const int threshold_;
    std::atomic<int> count_;
    std::atomic<double> progress_;

    void execute() {

        while(++count_ < threshold_) {
            checkCommand();
            std::this_thread::sleep_for(10ms);
            progress_ = std::ceil(100.0 * static_cast<double>(count_) / threshold_ * 100.0) / 100.0;

        }
    }

};

#endif
