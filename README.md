# Operating Systems
Course projects for the UCLA course "Operating Systems Principles"

All projects were implement in the C programming language and contain a Makefile with the following targets and more: a target to compile the executables of the program, a target to run a bash script with a series of smoke tests, a "clean" target to remove residual files from the directory, and a "dist" target to compress the program files into a distributable tarball.


[warmup project](https://github.com/jpicchi18/operating_systems/tree/main/project_0):
- Implemented a program that takes a variety of command line options and reads from stdin/writes to stdout.
- Depending on the options passed, the program also has the capability of reading input from a file, writing output to a file, generating a segfault, and catching a segfault using a signal handler.


[Telnet Shell with Compressed Network Communication](https://github.com/jpicchi18/operating_systems/tree/main/project_1):
- Designed a program that sets up a parent shell and child shell session that communicate using a pipe.
- The parent shell performs full duplex, character-at-a-time I/O when no option is passed. When the "--shell=program" option is passed, the child shell process performs the "program" and sends the result back to the parent through a pipe.
  - Otherwise, the child sends the input back through the pipe as the output. The program also features a 
