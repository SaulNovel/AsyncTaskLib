#ifndef ALLOCATE_MEMORY
#define ALLOCATE_MEMORY

#include <Task/Task.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class AllocateMemory : public Task
{

public:
    AllocateMemory(const int id, const int timer) 
    : Task(id), timer_(timer)
    {
    }

    double progress() const override {
        return 0.0;
    }

private:
    int timer_;

    void execute() {

        int* arr = new int[100];            
            
        while(timer_--) {

            try {
                checkCommand();
            }
            catch (const StopException& e) {
                delete[] arr;
                throw StopException("Stop");
            }

            usleep(10);
        }
    }

};

#endif
