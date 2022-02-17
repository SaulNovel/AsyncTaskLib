# Requirements

  * g++ 7.5.0 or newer
  * Boost 1.65.1 or newer
    * The `boost/program_options.hpp` header must be available for the cli application
  * cmake 3.8 or newer
  * googletest 1.11.0 (e2239ee6 (HEAD, tag: release-1.11.0))
    * The `gtest/gtest.h` header must be available for the test program

## Installed Packages

For development the following boost packages were installed.

```
libboost-program-options-dev/bionic,now 1.65.1.0ubuntu1 amd64 [installed,automatic]
libboost-program-options1.65-dev/bionic,now 1.65.1+dfsg-0ubuntu5 amd64 [installed,automatic]
libboost-program-options1.65.1/bionic,now 1.65.1+dfsg-0ubuntu5 amd64 [installed,automatic]
```

Project tree

* [AsyncTaskLib-md](./AsyncTaskLib-md)
  * [tasklib](./tasklib)
    * [Task.h](./tasklib/Task.h)
    * [Task.cpp](./tasklib/Task.cpp)
    * [Scheduler.h](./tasklib/Scheduler.h)
    * [Scheduler.cpp](./tasklib/Scheduler.cpp)
    * [StopException.h](./tasklib/StopException.h)
    * [CMakeLists.txt](./tasklib/CMakeLists.txt)
  * [test](./test)
    * [mainTests.cpp](./test/mainTests.cpp)
    * [googletest](./test/googletest)
    * [CMakeLists.txt](./test/CMakeLists.text)
  * [CMakeLists.txt](./CMakeLists.txt)
  * [README.md](./README.md)

# Compilation

From the root folder execute the following sequence:
  * `mkdir build`
  * `cd build`
  * `cmake ..`
  * `make`

Binaries are generated in build/test and build/cli

If compiling the code manually, include the following flags:

```
-std=c++14 -fpermissive -Wno-deprecated-declarations -fexceptions -g  -Wall -Wno-long-long \ 
-Wconversion -Wwrite-strings -Wsign-compare -Dgtest_disable_pthreads=OFF
```

# Sample Execution

Test: the binary is available in binaries/program_test 

```
 ./program_test             runs tests
```

Command line interface: the binary is available in build/program_cli 

```
./program_cli --help        displays program options
```
```
./program_cli --task types  displays available task types
```
```
./program_cli               start program
```

Once the program is running, the following options are accepted:

```
> start  <task_type_id> starts a task of a given type and and prints its ID
> pause  <task_id>      pause the task with the given id and prints a confirmation message
> resume <task_id>      resume task with the given id (if paused) and prints a confirmation message
> stop   <task_id>      stop the task with the given id (if not stopped) and prints a confirmation message
> status                prints the id, the status and an indicator of progress for each task.
> status <task_id>      As above, but for a single task.
> quit                  gracefully shut down
```

Task types available (all task types have predefined parameters)

 | Task type | ID  |
 | --------- |:---:|
 | test      |  0  |
 | counter   |  1  |
 | fibonacci |  2  |
 
 
