#ifndef TASK
#define TASK

#include <Task/StopException.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <iostream>
#include <sstream>

#include <string>
#include <unordered_map>

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

    ~Task() {
        if (command_ != CommandType::stop) {
            stop();
        }

        join();
    }

    Task (const Task&) = delete;
    Task& operator= (const Task&) = delete;

    void start() {
        // If there is no thread associated, default constructed std::thread::id is returned
        if (thread_.get_id() !=  std::thread::id()) {
            std::ostringstream msg;
            msg << "Cannot start task, '" << id() << "', it's running or completed";
            throw std::runtime_error(msg.str());
        }
        
        thread_ = std::thread(&Task::callbackFuntion, this);

        std::cout << "Starting  task '" << id() << "'" << std::endl;
    }

    void pause() {
        if (command_ != CommandType::run || state_ == StateType::completed) {
            std::ostringstream msg;
            msg << "Cannot pause task, '" << id() << "', not running";
            throw std::runtime_error(msg.str());
        }

        command_ = CommandType::pause;

        std::unique_lock<std::mutex> lock(mutex_state_);
        condition_state_.wait(lock, [&]() {
            return state_ != StateType::running;
        });

        std::cout << "Pausing  task '" << id() << "'" << std::endl;

    }

    void resume() {
        if (command_ != CommandType::pause) {
            std::ostringstream msg;
            msg << "Cannot resume task, '" << id() << "', not paused";
            throw std::runtime_error(msg.str());
        }
        {
            std::unique_lock<std::mutex> lock(mutex_control_);
            command_ = CommandType::run;
            condition_control_.notify_one();
        }

        {
            // Wait till thread toggles status to running
            std::unique_lock<std::mutex> lock(mutex_state_);
            condition_state_.wait(lock, [&]() {
                return state_ == StateType::running;
            });
        }

        std::cout << "Resuming  task '" << id() << "'" << std::endl;
    }

    void stop() {

        if (command_ != CommandType::run && command_ != CommandType::pause) {
            std::ostringstream msg;
            msg << "Cannot stop task, '" << id() << "', not running";
            throw std::runtime_error(msg.str());
        }

        {
            std::unique_lock<std::mutex> lock(mutex_control_);
            command_ = CommandType::stop;
            condition_control_.notify_one();
        }

        {
            std::unique_lock<std::mutex> lock(mutex_state_);
            condition_state_.wait(lock, [&]() {
                return (state_ == StateType::completed || state_ == StateType::stopped);
            });
        }

        std::cout << "Stopping  task '" << id() << "'" << std::endl;
    }

    void joinTask() {
        std::unique_lock<std::mutex> lock(mutex_state_);
        condition_state_.wait(lock, [&]() {
            return (state_ == StateType::completed || state_ == StateType::stopped);
        });
    }

    const int id() const {
        return id_;
    }

    const StateType status() {
        return state_;
    }

    /* std::thread builtin */
    bool notAThread() const {
        return thread_.get_id() ==  std::thread::id();
    }

    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    /* std::thread builtin */

    virtual double progress() const = 0;

protected:

    void checkCommand() {
        switch(command_) {
            case CommandType::pause:
            {
                {
                    std::unique_lock<std::mutex> lock(mutex_state_);
                    state_ = StateType::paused;
                    condition_state_.notify_one();
                }
                {
                    std::unique_lock<std::mutex> lock(mutex_control_);
                    condition_control_.wait(lock, [&]() {
                        return command_ != CommandType::pause;
                    });

                    if (command_ == CommandType::run) {

                        std::unique_lock<std::mutex> lock(mutex_state_);
                        state_ = StateType::running;
                        condition_state_.notify_one();
                        return;
                    }

                    if (command_ == CommandType::stop) {
                        throw StopException("Stop");
                    }
                }
                break;
            }
            case CommandType::run:
            {
                return;
            }
            case CommandType::stop:
            {
                throw StopException("Stop");
            }
        }
    }

private:

    void callbackFuntion() {

        bool completed;

        try {
            execute();
            completed = true;
        } 
        catch (const StopException& e) {
            std::cout << "Task '" << id() << "' exception thrown: " << e.what() << std::endl;
            completed = false;
        }
        catch (const std::exception& e) {
            std::cout << "exception thrown: " << e.what() << std::endl;
            completed = true;
        }
        
        std::unique_lock<std::mutex> lock(mutex_state_);
        state_ = completed ? StateType::completed : StateType::stopped;

        std::cout << (state_ == StateType::completed ? "Task completed\n" : "Task stopped\n") << std::endl;

        condition_state_.notify_one();
    }

    /* TODO: Override this function to customize a task */
    virtual void execute() = 0;
};

#endif
