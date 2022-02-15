#include <iostream>
#include <boost/program_options.hpp>
#include <functional>

#include <Task/Scheduler.h>
#include <Task/Counter.h>
#include <Task/Fibonacci.h>

namespace po = boost::program_options;


enum class TaskType
{
    counter,
    fibonacci
};

static Scheduler scheduler;

namespace CliCommands {

void start(const int task_type_id) {
    const TaskType task_type = static_cast<TaskType>(task_type_id);
    
    int task_id = -1;
    switch(task_type) {
    case TaskType::counter:
        task_id = scheduler.addTask<Counter>(100).id();
        break;
    case TaskType::fibonacci:
        task_id = scheduler.addTask<Fibonacci>(100).id();
        return;
    }

    if (task_id == -1) {
        std::cout << "Error while creating task" << std::endl;
    }

    std::cout << "Task with id '" << task_id << "' started" << std::endl;
}

void pause(const int task_id) {
    auto& task = scheduler.getTask(task_id);
    task.pause();
    std::cout << task << std::endl;
}

void resume(const int task_id) {
    auto& task = scheduler.getTask(task_id);
    task.resume();
    std::cout << task << std::endl;
}

void stop(const int task_id) {
    auto& task = scheduler.getTask(task_id);
    task.stop();
    std::cout << task << std::endl;
}

void statusTask(const int task_id) {
    auto& task = scheduler.getTask(task_id);
    std::cout << task << std::endl;
}

void status(int i = 0) {
    for(int task_id : scheduler.getTaskIds()) {
        auto& task = scheduler.getTask(task_id);
        std::cout << task << std::endl;
    }
}

}

namespace Util
{
    int strToInteger(const std::string& str) {
        try {
            int val = std::stoi(str);
            return val;
        }
        catch (const std::exception& e) {
            std::cout << "Cannot convert second argument to integer: " << e.what() << std::endl;
            return -1;
        }
    }
}

int main(int argc, char* argv[]) 
{

    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "prints help message and instructions")
    ("start <task_type_id>", "starts a task of a given type and and prints its ID")
    ("pause <task_id>", "pause the task with the given id and print a confirmation message")
    ("resume <task_id>", "resume task with the given id (if paused) and print a confirmation message")
    ("stop <task_id>", "stop the task with the given id (if not stopped) and print a confirmation message")
    ("status", "prints the id, the status, progress and task type ID for each task")
    ("status <task_id>", "prints the id, the status, progress and task type ID for the task with the given id")
    ("quit", "gracefully shut down")
    ;

    // Parse the command line
    po::variables_map vm;
    // Parse and store arguments
    po::store(po::parse_command_line(argc, argv, desc), vm);
    // Must be called after parsing and storing
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    using CommandTable = std::unordered_map<std::string, std::function<void(int)>>;

    CommandTable commands;
    commands["start"] = CliCommands::start;
    commands["pause"] = CliCommands::pause;
    commands["resume"] = CliCommands::resume;
    commands["stop"] = CliCommands::stop;
    commands["statusTask"] = CliCommands::statusTask;
    commands["status"] = CliCommands::status;
    
    while (true) {

        std::string input;
        std::cout << ">";
        getline (std::cin, input);
  
        if (input == "quit") {
            return 0;
        }
        
        std::vector<std::string> command;
        
        std::istringstream iss(input);
        for(std::string str; iss >> str;) { 
            command.push_back(str);
        }
        
        if (command.empty() || command.size() > 2) {
            std::cout << "Please introduce max 2 words separated by an space" << std::endl;
            continue;
        }
        
        if (commands.count(command[0]) == 0) {
            std::cout << "Option not supported" << std::endl;
            continue;
        }
        
        command[0] = (command[0] == "status" && command.size() > 1) ? "statusTask" : command[0];
        
        if (commands.count(command[0])) {        
            auto& func = commands[command[0]];
            int param = 0;
            if (command.size() > 1) {
                int res = Util::strToInteger(command[1]);
                if (res == -1) {
                    continue;
                }
                param = res;
            }
            
            func(param);
        }
    }

}