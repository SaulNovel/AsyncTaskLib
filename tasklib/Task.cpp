#include "Task.h"

namespace {
    std::unordered_map<Task::StateType, std::string> statusToStr = {
        {Task::StateType::running, "running"},
        {Task::StateType::paused, "paused"},
        {Task::StateType::stopped, "stopped"},
        {Task::StateType::completed, "completed"},
    };
}

std::ostream& operator<<(std::ostream& os, Task& task) {
    os << "Task id: '" << task.id() << "' status: '" << statusToStr[task.status()]  << "' progress: " << task.progress() << "%";
    return os;
}

void Task::start() {
    // If there is no thread associated, default constructed std::thread::id is returned
    if (thread_.get_id() !=  std::thread::id()) {
        std::ostringstream msg;
        msg << "Cannot start task, '" << id() << "', it's running or completed";
        throw std::runtime_error(msg.str());
    }
        
    thread_ = std::thread(&Task::callbackFuntion, this);
}

void Task::pause() {
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
}

void Task::resume() {
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
        // Wait till thread changes status to running
        std::unique_lock<std::mutex> lock(mutex_state_);
        condition_state_.wait(lock, [&]() {
            return state_ == StateType::running;
        });
    }
}

void Task::stop() {
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
        // Wait till thread changes status to completed/stopped
        std::unique_lock<std::mutex> lock(mutex_state_);
        condition_state_.wait(lock, [&]() {
            return (state_ == StateType::completed || state_ == StateType::stopped);
        });
    }
}

void Task::joinTask() {
    std::unique_lock<std::mutex> lock(mutex_state_);
    condition_state_.wait(lock, [&]() {
        return (state_ == StateType::completed || state_ == StateType::stopped);
    });
}

void Task::join() {
    if (thread_.joinable()) {
            thread_.join();
    }
}

void Task::checkCommand() {
    switch(command_) {
        case CommandType::pause:
        {
            {
                std::unique_lock<std::mutex> lock(mutex_state_);
                state_ = StateType::paused;
                condition_state_.notify_one();
            }
            {
                // Wait till top thread changes command to pause
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
                    throw StopException();
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
            throw StopException();
        }
    }
}

void Task::callbackFuntion() {
    bool completed;
    try {
        execute();
        completed = true;
    } 
    catch (const StopException& e) {
        completed = false;
    }
    catch (const std::exception& e) {
        std::cout << "exception thrown: " << e.what() << std::endl;
        completed = true;
    }
        
    std::unique_lock<std::mutex> lock(mutex_state_);
    state_ = completed ? StateType::completed : StateType::stopped;

    condition_state_.notify_one();
}