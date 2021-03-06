#ifndef FIBONACCI
#define FIBONACCI

#include "Task.h"

class Fibonacci : public Task
{

public:
    Fibonacci(const int id, const int num) 
    : Task(id), num_(num), res_(0), progress_(0.0)
    {}

    double progress() override {
        return progress_;
    }

    int getResult() {
        StateType state = status();
        if (state == StateType::completed || state == StateType::stopped) {
            return res_;
        }

        throw std::runtime_error("Result for fibonacci of '" + std::to_string(num_) + "' not available");
    }

private:
    const int num_;
    std::atomic<int> res_;
    std::atomic<double> progress_;

private:

    int fibonacci(int x) {
        if (x < 2) { return x; }
        checkCommand();

        return fibonacci(x-1) + fibonacci(x-2);
    }

    void execute() override {
        res_ = fibonacci(num_);
        progress_ = 100.0;
    }

};

#endif
