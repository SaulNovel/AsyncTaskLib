#include <iostream>
#include <boost/program_options.hpp>
#include <functional>
#include <algorithm>

#include "Scheduler.h"
#include "TestTask.h"
#include "Counter.h"
#include "Fibonacci.h"

#define INVALID_TASK_ID -1

namespace po = boost::program_options;
using namespace std::chrono_literals;

enum class TaskType
{
    test,
    counter,
    fibonacci
};

static Scheduler scheduler;
static std::unordered_map<int, int> task_id_to_type;

const static std::unordered_map<int, std::string> task_type_to_str = {
    {0, "test"},
    {1, "counter"},
    {2, "fibonacci"}
};

class TaskFactory {

public:

    Task& createTask(const TaskType task_type) {

        // Values are predefined
        switch(task_type) {
        case TaskType::test:
            return scheduler.addTask<TestTask>(10ms);
        case TaskType::counter:
            return scheduler.addTask<Counter>(1e5);
        case TaskType::fibonacci:
            return scheduler.addTask<Fibonacci>(20);
        default:
            throw std::invalid_argument("Task type id: '" + std::to_string(static_cast<int>(task_type)) + "' not supported");
        }
    }
};

namespace CliCommands {

void start(const int task_type_id) {

    if (task_type_id == INVALID_TASK_ID) {
        throw std::invalid_argument("parameter <task_type_id> not found");
    }

    TaskFactory task_factory;    
    const TaskType task_type = static_cast<TaskType>(task_type_id);
    auto& task = task_factory.createTask(task_type);

    task_id_to_type[task.id()] = task_type_id; 
    std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_type_id) << std::endl;
}

void pause(const int task_id) {

    if (task_id == INVALID_TASK_ID) {
        throw std::invalid_argument("parameter <task_id> not found");
    }

    auto& task = scheduler.getTask(task_id);
    task.pause();

    std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
}

void resume(const int task_id) {

    if (task_id == INVALID_TASK_ID) {
        throw std::invalid_argument("parameter <task_id> not found");
    }

    auto& task = scheduler.getTask(task_id);
    task.resume();

    std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
}

void stop(const int task_id) {

    if (task_id == INVALID_TASK_ID) {
        throw std::invalid_argument("parameter <task_id> not found");
    }

    auto& task = scheduler.getTask(task_id);
    task.stop();

    std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
}

void status(const int task_id) {
    if (task_id == INVALID_TASK_ID) {
        /*
        for(int task_id : scheduler.getTaskIds()) {
            auto& task = scheduler.getTask(task_id);

            std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
        }
        return;
        */

        for(auto task_ref_wrapper : scheduler.getTasks()) {
            auto& task = task_ref_wrapper.get();
            std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
        }
        return;
    }

    auto& task = scheduler.getTask(task_id);

    std::cout << " -> " << task << " task_type: " << task_type_to_str.at(task_id_to_type.at(task.id())) << std::endl;
}

}

std::string getHelpMessage() {
    std::ostringstream message;
    message << "Allowed actions: " << std::endl
            << "  start <task_type_id>  starts a task of a given type and and prints its ID." << std::endl
            << "  pause <task_id>       pause the task with the given id and print a confirmation message." << std::endl
            << "  resume <task_id>      resume task with the given id (if paused) and print a confirmation message." << std::endl
            << "  stop <task_id>        stop the task with the given id (if not stopped) and print a confirmation message." << std::endl
            << "  status                prints the id, the status, progress and task type ID for each task." << std::endl
            << "  status <task_id>      prints the id, the status, progress and task type ID for the task with the given id." << std::endl
            << "  quit                  gracefully shut down." << std::endl;

            return message.str();
}

std::string getTaskTypesMessage() {
    std::ostringstream message;
    message << "Task type  ID  Description" << std::endl
            << "test       0   starts a dummy task that runs in an infinite loop." << std::endl
            << "counter    1   starts a counter, the task stops when the count reaches a threshold." << std::endl
            << "fibonacci  2   calculates fibonacci sequence." << std::endl
            << std::endl << "Task parameters are predefined." << std::endl;

            return message.str();
}

int main(int argc, char* argv[]) 
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "prints help message and instructions")
    ("task types", "prints supported task typed");

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << getHelpMessage() << std::endl;
            return 0;  
        }

        if (vm.count("task types")) {
            std::cout << getTaskTypesMessage() << std::endl;
            return 0;  
        }
    }
    catch (po::error &e)
    {
        std::cerr << e.what() << std::endl << std::endl;
        std::cout << desc << std::endl << getHelpMessage() << std::endl;
        return 0;
    }

    using CommandTable = std::unordered_map<std::string, std::function<void(int)>>;
    CommandTable commands;
    commands["start"] = CliCommands::start;
    commands["pause"] = CliCommands::pause;
    commands["resume"] = CliCommands::resume;
    commands["stop"] = CliCommands::stop;
    commands["status"] = CliCommands::status;
    
    while (true) {

        try {

            std::string input;
            std::cout << "> ";
            getline(std::cin, input);
              
            std::istringstream iss(input);
            std::vector<std::string> tokens;
            std::string token;
            while (std::getline(iss, token, ' ')) {
                tokens.push_back(token);
            }

            auto it = std::find(tokens.begin(), tokens.end(), "quit");
            if (it != tokens.end()) {
                return 0;
            }

            if (tokens.empty()) { 
                continue; 
            }

            std::string command = tokens[0];
            if (commands.count(command) == 0) {
                std::cout << "Option '" << command << "' not supported" << std::endl;
                continue;
            }

            int cmd_argument = INVALID_TASK_ID;
            if (tokens.size() > 1) {
                try{
                    cmd_argument = std::stoi(tokens[1]);
                }
                catch(const std::exception& e) {
                    std::cout << "Argument '" << tokens[1] << "' not an exisiting <task_id>/<task_type_id>" << std::endl;
                    continue;
                }              
            }

            if (tokens.size() > 2) {
                auto it = std::find_if(tokens.begin()+2, tokens.end(), [](const std::string& token){
                    return token != "";
                });
                if (it != tokens.end()) {
                    std::cout << "Please introduce a valid command followed by and existing <task_id>/<task_type_id>" << std::endl;
                    continue;                    
                }
            }
            
            auto& func = commands[command];
            func(cmd_argument);

        }   
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;

}