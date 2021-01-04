# Operating Systems
Course projects for the UCLA course "Operating Systems Principles"

All projects were implement in the C programming language and contain a Makefile with the following targets and more: a target to compile the executables of the program, a target to run a bash script with a series of smoke tests, a "clean" target to remove residual files from the directory, and a "dist" target to compress the program files into a distributable tarball.
The full details of each project can be seen in their respective "README" files.


[Project 0: Warmup Project](https://github.com/jpicchi18/operating_systems/tree/main/project_0):
- Implemented a program that takes a variety of command line options and reads from stdin/writes to stdout.
- Depending on the options passed, the program also has the capability of reading input from a file, writing output to a file, generating a segfault, and catching a segfault using a signal handler.


[Project 1A: Telnet Shell with Pipe](https://github.com/jpicchi18/operating_systems/tree/main/project_1/project_1A):
- a program that sets up a parent shell and child shell session that communicate using a pipe.
- The parent shell performs full duplex, character-at-a-time I/O when no option is passed. When the "--shell=program" option is passed, the child shell process performs the "program" and sends the result back to the parent through a pipe.
  - Otherwise, the child sends the input back through the pipe as the output.
- the program also features a "--debug" option that provides additional output to discern the behaviors of the parent and child shells.
- this project uses the following system calls, among others: ```getopt_long, atexit, write, read, signal, poll, waitpid, fork, pipe```

[Project 1B: Telnet Shell with Socket and Compressed Network Communication](https://github.com/jpicchi18/operating_systems/tree/main/project_1/project_1B):
- similar to project 1A in functionality, but the parent and child shell communicate through a socket over the network, as opposed to a pipe.
- the parent and child shell send data back and forth until one of server terminating events occur
- the program has several options:
  - "--compress" --> compresses data before sending it to the server, and decompresses data after recieving it from the server
    - uses "zlib" to compress data
  - "--portnum=num" --> a required flag that spcifies the port number through which the client should connect to the socket
  - "--log=filename" --> specifies a file to which a log will be written of all data transfered to and from the server over the network.
  - "--host=hostname" --> allows the user to specify a particular hostname if the server program is running on a different machine.
  - "--debug" --> provides additional debugging dialog for program development

[Project 2A: Races and Synchronization](https://github.com/jpicchi18/operating_systems/tree/main/project_2/project_2A):
- analyzing conflicting read-modify-write operations on a single variable and complex data structures (an ordered linked list)
- this project consists of 2 parts:
  1. updates to a shared variable
      - wrote a multithreaded application (using pthreads) that performs parallel updates to a shared variable.
      - addressed the race condition with different synchronization methods (e.g. spin locks, mutex locks, compare-and-swap, test-and-set)
      - executed performance instrumentation and measurement
  2. updates to a shared complex data structure
      - implemented a linked list data structure
      - wrote a multi-threaded application, using pthread that performs, parallel updates to a sorted doubly linked list data structure
      - addressed the race condition with different synchronization methods (e.g. spin locks, mutex locks, compare-and-swap, test-and-set)
      - executed performance instrumentation and measurement
- modeled performance results using Gnuplot scripts to generate graphs
      
[Project 2B: Lock Granularity and Performance](https://github.com/jpicchi18/operating_systems/tree/main/project_2/project_2B):
- executed performance instrumentation and measurement to confirm that mutex and spin-lock are bottlenecks, preventing parallel access to the linked list
- Implemented a new option to divide a list into sublists and support synchronization on sublists, thus allowing parallel access to the (original) list
- took performance measurements to confirm that the problem has been solved.
- used ```gperftools``` and ```valgrind``` measure performance (i.e. perform execution profiling) and detect memory leaks caused by parallel thread access to the linked list

[Project 3A: File System Interpretation](https://github.com/jpicchi18/operating_systems/tree/main/project_3/project_3A):
- designed and implemented a program to read the on-disk representation of a file system, analyze it, and summarize its contents
- explored the on-disk data format of the EXT2 file system
  - mounted a provided image file on a Linux system and explored it with familiar file navigation commands and ```debugfs(8)```
- Wrote a program to analyze the file system in an image file and output a summary to standard out
  - the summary describes the super block, groups, free-lists, inodes, indirect blocks, and directories).
  - The summary is formatted as lines of comma separated values (i.e. csv files)
 
[Project 3B: File System Consistency Analysis](https://github.com/jpicchi18/operating_systems/tree/main/project_3/project_3B):
- Write a program to analyze a file system summary (a .csv file produced by Project 3A) and report on all discovered inconsistencies
  - Detected inconsistencies are reported to standard out
- Detected errors include the following:
  - block consistency audits (of invalid blocks, unreferenced blocks, reserved blocks, indirect blocks, allocated blocks, etc.)
  - inode allocation audits
  - directory consistency audits
  
[Project 4A: Beaglebone Set-Up](https://github.com/jpicchi18/operating_systems/tree/main/project_4/project_4A):
- set up a beaglebone development environment, complete with the following details:
  - the ability to log into the beaglebone via USB and Wifi
  - the ability to transfer files to/from the beaglebone
  - the ability to develop and run programs on the beaglebone
  - the ability to install new software packages on th beaglebone

[Project 4B: Sensors Input](https://github.com/jpicchi18/operating_systems/tree/main/project_4/project_4B):
- set up beaglebone input sensors and wrote a program that performed the following:
  - uses the AIO functions of the MRAA library to get readings from a temperature sensor.
  - samples a temperature sensor at a configurable rate (configurable with a "--period=#" option)
  - converts the sensor value into a temperature (units of temperature are specified with a command line option)
  - creates a report for each sample that includes:
    - time of the sample (e.g. 17:25:58) in the local timezone
    - a single blank/space
    - a decimal temperature in degrees and tenths (e.g. 98.6)
    - a newline character (\n)
  - writes that report to the stdout (fd 1).
  - appends that report to a logfile
  - exits if a button attached to the beaglebone is pushed

[Project 4C: Internet of Things Security](https://github.com/jpicchi18/operating_systems/tree/main/project_4/project_4C):
- created an embedded application that extends the functionality of project 4B
- the application accepts the same commands and generates the same reports as project 3B, but now the input and output are from/to a network connection to a server.
- the beaglebone program features Authenticated TLS Session Encryption with the online server using the OpenSSL TLS library
