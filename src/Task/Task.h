#ifndef TASK
#define TASK

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <atomic>

class Task
{

public:
    using TaskId = std::thread::id;

    enum StateType {
        idle,
        paused,
        running,
        stopped,
        completed,
        aborted // exception
    };

public:

    Task() 
    : thread_(), state_(idle), handler_(), condition_(), mutex_()
    {}

    ~Task() {
        stop();
        join();
    }

    Task (const Task&) = delete;
    Task& operator= (const Task&) = delete;

    const TaskId start() {
        if (status() != idle) {
            return id();
        }

        // TODO: Variadic template argument to callback function via forwarding -> check later, it might simplify things when designing a new derived task
        thread_ = std::thread(&Task::callbackFuntion, this);
        handler_ = thread_.native_handle();

        return id();
    }

    void pause() {
        if (status() == running) {
            updateState(paused);
        }
    }

    void resume() {
        if (status() == paused) {
            updateState(running);
        }
    }

    /**
     * Uses native (OS/compiler-dependent) function
     * - pthread_cancel does not warranty that the thread will stop
     * - derived classes must free resources and set cancellation points to ensure the thread will stop  
    */
    void stop() {
        StateType current = status();
        if (current != paused && current != running) {
            return;
        }

        int res = pthread_cancel(handler_);
        if (res == 0) {
            updateState(stopped);
        }
    }

    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    TaskId id() const {
        return thread_.get_id();
    }

    const StateType status() const {
        return state_;
    }

    /**
     * TODO: Override this function to customize a task
    */
    virtual double progress() = 0;

    friend std::ostream& operator<<(std::ostream& os, const Task* task) {
        os << "Task: '" << task->id() << "' status: '" << statusToStr[task->status()] << "' progress: " << task->progress() << "%";
        return os;
    }

protected:
    void interrupt() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&]() {
            cancelPoint();
            return state_ != paused;
        });
    }

    /**
     * Uses native (OS/compiler-dependent) function
     * - derived class must free resources before setting a cancellation point
     * - An alternative is to have a shared flag that is used by the threads to break out of the loop (if any)
    */
    void cancelPoint() {
        pthread_testcancel();
    }

private:
    std::thread thread_;
    StateType state_;
    pthread_t handler_;
    std::condition_variable condition_;
    std::mutex mutex_;


    void updateState(const StateType state) {
        // TODO: wakes up threads everytime state is updated
        // test later if state_== paused and state != pused -> notify()
        std::unique_lock<std::mutex> lock(mutex_);
        state_ = state;
    
        condition_.notify_one();
    }

    /**
     * Uses native (OS/compiler-dependent) function to set cancel state parameters
    */
    void callbackFuntion() {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
        // asynchronous cancellation is not safe, the best is that the use creates cancelation points
        //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        updateState(running);

        try {
            execute();
        } 
        catch (const std::exception& e) {
            // TODO: log & forward exception to Scheduler
            std::cout << "exception thrown: " << e.what() << std::endl;
            updateState(aborted);
            return;
        }

        updateState(completed);
    }

    /**
     * TODO: Override this function to customize a task
    */
    virtual void execute() = 0;

    static std::unordered_map<StateType, std::string> statusToStr;
};

#endif
