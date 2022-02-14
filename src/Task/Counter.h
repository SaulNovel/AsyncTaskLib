#ifndef COUNTER
#define COUNTER

#include <Task/Task.h>

using namespace std::chrono_literals;

class Counter : public Task
{

public:
    Counter(const int id, int threshold = std::numeric_limits<int>::max()) 
    : Task(id), threshold_(threshold), count_(0) 
    {
    }

    double progress() const override {
        return 0.0;
    }

private:
    const int threshold_;
    int count_;

    void execute() {

        while(++count_ < threshold_) {

            checkCommand();
        
            std::this_thread::sleep_for(100ms);
        }
    }

};

#endif
