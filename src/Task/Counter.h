#ifndef COUNTER
#define COUNTER

#include <Task/Task.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

    // Constructor of the task takes arguments
    // implements a method getResult

    // 2 options:
    // - return values from here  (template function)
    // - do getters/setters -> wait till status() == completed on main thread
    void execute() {

        while(++count_ < threshold_) {

            checkCommand();
        
            sleep(1);
            std::cout << "count: " << count_ << std::endl;
        }
    }

};

#endif
