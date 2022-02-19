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
ldd program_test 
        linux-vdso.so.1 (0x00007fffdad60000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fa8de290000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fa8ddef0000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fa8ddcd0000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fa8ddab0000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa8dd6b0000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fa8dea00000)

 ./program_test             runs tests
```

Command line interface: the binary is available in binaries/program_cli 

```
ldd program_cli 
        linux-vdso.so.1 (0x00007fffc0014000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f0f0c5e0000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f0f0c240000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f0f0c020000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f0f0be00000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0f0ba00000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f0f0cc00000)

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
 
 
