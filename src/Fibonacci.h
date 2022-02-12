#ifndef TASK_FIBONACCI
#define TASK_FIBONACCI

#include <Task.h>

class Fibonacci : public Task
{

public:
    Fibonacci(const int num) 
    : num_(num), res_(0) 
    {}

    double progress() override {
        return 0.0;
    }

private:
    int num_;
    int res_;

private:

    int fibonacci(int x) {
        if (x < 2) { return x; }

        // TODO: interrupt does not work here
        interrupt();

        cancelPoint();

        return fibonacci(x-1) + fibonacci(x-2);
    }

    void execute() override {
        res_ = fibonacci(num_);
    }

};

#endif
