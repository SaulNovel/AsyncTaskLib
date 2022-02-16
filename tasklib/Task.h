#ifndef TASK
#define TASK

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <iostream>
#include <sstream>

#include <string>
#include <unordered_map>

#include "StopException.h"

class Task;
std::ostream& operator<<(std::ostream& os, Task& task);

class Task
{

public:

    /* States are modified by inner thread */
    enum class StateType {
        running,
        paused,
        stopped,
        completed,
    };

    /* Commands are modified by parent thread */
    enum class CommandType {
        run,
        pause,
        stop,
    };

private:
    const int id_;
    std::thread thread_;
    
    /* state transitions */
    std::atomic<StateType> state_;
    std::condition_variable condition_state_;
    std::mutex mutex_state_;

    /* command transitions */
    std::atomic<CommandType> command_;
    std::condition_variable condition_control_;
    std::mutex mutex_control_;

public:

    Task(const int id) 
    : id_(id), 
    thread_(),
    state_(StateType::running),
    command_(CommandType::run)
    {}

    virtual ~Task() {
        if (command_ != CommandType::stop) {
            stop();
        }

        join();
    }

    Task (const Task&) = delete;
    Task& operator= (const Task&) = delete;

    const int id() const { return id_; }

    void start();

    void pause();

    void resume();

    void stop();

    /* top thread waits till status switches to stoppped/completed */
    void joinTask();

    /* std::thread builtin */
    void join();

    const StateType status() { return state_; }

    virtual double progress() = 0;

protected:
    void checkCommand();

private:

    void callbackFuntion();

    /* TODO: Override this function to customize a task */
    virtual void execute() = 0;
};

#endif
