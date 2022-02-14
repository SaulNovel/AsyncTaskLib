#ifndef FIBONACCI
#define FIBONACCI

#include <Task/Task.h>

class Fibonacci : public Task
{

public:
    Fibonacci(const int id, const int num) 
    : Task(id), num_(num), res_(0) 
    {}

    double progress() const override {
        return 0.0;
    }

private:
    int num_;
    std::atomic<int> res_;

private:

    int fibonacci(int x) {
        if (x < 2) { return x; }

        checkCommand();

        return fibonacci(x-1) + fibonacci(x-2);
    }

    void execute() override {
        res_ = fibonacci(num_);
    }

};

#endif
