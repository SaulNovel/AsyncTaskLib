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

    /* Commands are modified by main thread */
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


    /**
     * Calls to std::thread constructor which associates thread_ with a thread of execution.
     * Runs task
     * 
     * @throw runtime_error if thread_ has no thread associated
    */
    void start();

    /**
     * Switches command to pause
     * Locks main thread till status is switched to paused
     * 
     * @throw runtime_error if thread cannot pause
    */
    void pause();

    /**
     * Switches command to run and notifies
     * Locks main thread till status is switched to running
     * 
     * @throw runtime_error if thread cannot resume
    */
    void resume();

    /**
     * Switches command to stop and notifies
     * Locks main thread till status is switched to stopped/completed
     * 
     * @throw runtime_error if thread cannot stop
    */
    void stop();

    /* Locks main thread till inner thread finishes its execution */
    void joinTask();

    /* std::thread builtin */
    void join();

    const StateType status() { return state_; }

    virtual double progress() = 0;

protected:

    /** 
     * Updates state and notifies to main thread
     * Must be added in execute's body of derived class 
     * 
     * @throw StopException if stop command is detected
    */
    void checkCommand();

private:

    /**
     * Callable function, it is a wrapper of execute()
     * Updates state to running when called
     * Updates state to completed/stopped (if StopException thrown)
     * User must re-throw StopException if captured
    */
    void callbackFuntion();

    /**
     * Derived class must implement execute's function that will be called in the thread context
     * Periodically calls checkCommand (User-defined function)
    */
    virtual void execute() = 0;
};

#endif
