// NAdsasME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


// global vars
int process_finished = 0;
int debug_passed = 0;
struct termios original_terminal_mode;

// handle SIGPIPE errors from child process
void sigpipe_handler(int sig) {
  process_finished = 1;

  if (debug_passed) {
    fprintf(stderr, "SIGPIPE handler called\n");
  }
}


// restore original terminal mode
void reset_terminal() {
  tcsetattr(0, TCSANOW, &original_terminal_mode);
}


void io_error(int read, struct termios* original_terminal_mode) {
  // read error
  if (read) {
    fprintf(stderr, "lab1a read error: %s\n", strerror(errno));
  }
  else {
    fprintf(stderr, "lab1a write error: %s\n", strerror(errno));
  }
  reset_terminal();
  exit(1);
}


// generic error handling
void error_exit(struct termios* original_terminal_mode) {
  reset_terminal();
  exit(1);
}


// read from stdin, write to stdout
void no_option_passed(struct termios* original_terminal_mode) {
  char buf[100];
  size_t read_ret;
  size_t write_ret;
  while(1) {
    if ( (read_ret = read(0, buf, sizeof(buf))) < 0) {
      io_error(1, original_terminal_mode);
    }
    for (int i=0; i<read_ret; i++) {

      if (buf[i] == 0x4) { // ctrl+D
	      if (write(1, "^D", 2) < 0) {
          io_error(0,original_terminal_mode);
        }
	      reset_terminal();
	      exit(0);
      }
      else if (buf[i] == 0xd || buf[i] == 0xa) {
	      if (write(1, "\r\n", 2) < 0) {
          io_error(0, original_terminal_mode);
        }
      }
      else {
	      if (write(1, &buf[i], 1) < 0) {
          io_error(0, original_terminal_mode);
        }
      }
    }
  }
}


void option_passed(struct termios* original_terminal_mode, const char* prog_name) {
  printf("program name: %s\n", prog_name);
  
  // register SIGPIPE handler
  if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
    fprintf(stderr, "lab1a SIGPIPE handler error: %s\n", strerror(errno));
    error_exit(original_terminal_mode);
  }

  // create pipes
  int parent_to_child[2];
  int child_to_parent[2];
  if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0) {
    fprintf(stderr, "lab1a pipe creation error: %s\n", strerror(errno));
    error_exit(original_terminal_mode);
  }

  int fork_ret = fork();

  // child
  if (fork_ret == 0) {
    // closed unused ends of pipes
    close(child_to_parent[0]);
    close(parent_to_child[1]);

    // redirect stdin to pipe
    close(0);
    dup(parent_to_child[0]);
    close(parent_to_child[0]);

    // redirect stdout/stderr to pipe
    close(1);
    dup(child_to_parent[1]);
    close(2);
    dup(child_to_parent[1]);
    close(child_to_parent[1]);

    //execute program
    if (execlp(prog_name, prog_name, NULL) < 0) {
      fprintf(stderr, "lab1a execlp error: %s\n", strerror(errno));
      error_exit(original_terminal_mode);
    }
  }

  // parent
  else if (fork_ret > 0) {
    // close unused ends of pipes
    close(child_to_parent[1]);
    close(parent_to_child[0]);

    // create 2 pollfd structs
    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLIN + POLLHUP + POLLERR;
    fds[0].revents = 0;
    fds[1].fd = child_to_parent[0];
    fds[1].events = POLLIN + POLLHUP + POLLERR;
    fds[1].revents = 0;

    while(1) {
      int poll_ret;
      
      if ((poll_ret = poll(fds, 2, -1)) < 0) {
        // poll error
	      fprintf(stderr, "lab1a poll error: %s\n", strerror(errno));
	      error_exit(original_terminal_mode);
      }
      else if (poll_ret > 0) {
        // keyboard input ready
	      if (fds[0].revents & POLLIN) {
	        char buf[50];
	        size_t read_ret;
	        size_t write_ret;
	        // read keyboard input
	        if ((read_ret = read(0, buf, sizeof(buf))) > 0) {
	          for (int i=0; i<read_ret; i++) {
	            
              if (debug_passed) {
                  fprintf(stderr, "(k)");
              }

              // handle "\r" and "\n"
	            if (buf[i] == 0xd || buf[i] == 0xa) {
                if (write(1, "\r\n", 2) < 0) {
		              io_error(0, original_terminal_mode);
		            }

                if (parent_to_child[1] > 0) {
                  if (write(parent_to_child[1], "\n", 1) < 0) {
                    io_error(0, original_terminal_mode);
                  }
                }
	            }
	            
              // handle "^D", which is EOF
              else if (buf[i] == 0x4) {
                if (write(1, "^D", 2) < 0) {
                  io_error(0, original_terminal_mode);
                }
                
                // close pipe to the shell
                if (parent_to_child[1] > 0) {
                  close(parent_to_child[1]);
                  parent_to_child[1] = -1;
                }
              }

              // handle "^C", which is interrupt char
              else if (buf[i] == 0x3) {
                if (write(1, "^C", 2) < 0) {
                  io_error(0, original_terminal_mode);
                }

                // send SIGINT to shell process
                if (kill(fork_ret, SIGINT) < 0) {
                  // error
                  fprintf(stderr, "lab1a kill error: %s\n", strerror(errno));
                  reset_terminal();
                  exit(1);
                }
              }
    
              // handle all other chars
	            else {
                if (write(1, &buf[i], 1) < 0) {
		              io_error(0, original_terminal_mode);
		            }

                if (parent_to_child[1] > 0) {
                  if (write(parent_to_child[1], &buf[i], 1) < 0) {
                    io_error(0, original_terminal_mode);
                  }
                }
	            }
	          }
	        }
	        if (read_ret < 0) {
            io_error(1, original_terminal_mode);
	        }
	      }
	
        // shell input ready
        if (fds[1].revents & POLLIN) {
	        char buf2[256];
	        size_t read_ret;
	        if ((read_ret = read(child_to_parent[0], buf2, sizeof(buf2))) > 0) {
	          for (int i=0; i<read_ret; i++) {
	      
                if (debug_passed) {
                  fprintf(stderr, "(s)");
                }

	              // handle "\n"
	              if (buf2[i] == 0xa) {
		              if (write(1, "\r\n", 2) < 0) {
                    io_error(0, original_terminal_mode);
		              }
	              }

                // got "^D" from shell
                else if (buf2[i] == 0x4) {
                  if (write(1, "^D", 2) < 0) {
                    io_error(0, original_terminal_mode);
                  }
                  process_finished = 1;
                }
	      
                // all other chars
	              else {
		              if (write(1, &buf2[i], 1) < 0) {
                    io_error(0, original_terminal_mode);
		              }
	              }
	          }
	        }
	        if (read_ret < 0) {
	          io_error(1, original_terminal_mode);
	        }
	      }
	
        // stdin closed prematurely for some reason
        if (fds[0].revents & (POLLHUP | POLLERR)) {
	        process_finished = 1;

          if (debug_passed) {
            fprintf(stderr, "STDIN_FILENO POLLHUP or POLLERR\n");
          }
	      }
	      
        // shell pipe closed
        if (fds[1].revents & (POLLHUP | POLLERR)) {
          process_finished = 1;
          if (debug_passed) {
            fprintf(stderr, "from child pipe POLLHUP or POLLERR\n");
          }
        }
      }

      if (process_finished) {
          break;
        }
    }
  
    // close the pipe
    if (parent_to_child[1] > 0) {
      close(parent_to_child[1]);
    }
        
    // exit with child return status
    int child_ret = 0;
    if (waitpid(fork_ret, &child_ret, 0) < 0) {
      fprintf(stderr, "lab1a waitpid error: %s\n", strerror(errno));
    }
    fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(child_ret), WEXITSTATUS(child_ret));
    reset_terminal();
    exit(0);
  }
  
  // fork error
  else {
    fprintf(stderr, "lab1a fork error: %s\n", strerror(errno));
    error_exit(original_terminal_mode);
  }
}


int main(int argc, char *argv[]) {

  // retrieve original terminal mode
  if (tcgetattr(0, &original_terminal_mode) < 0) {
    fprintf(stderr, "lab1a tcgetattr error: %s\n", strerror(errno));
    exit(1);
  }
  
  if (atexit(reset_terminal) < 0) {
    fprintf(stderr, "lab1a atexit error: %s\n", strerror(errno));
    exit(1);
  }

  // set new terminal mode
  struct termios new_terminal_mode = original_terminal_mode;
  new_terminal_mode.c_iflag = ISTRIP;
  new_terminal_mode.c_oflag = 0;
  new_terminal_mode.c_lflag = 0;
  if (tcsetattr(0, TCSANOW, &new_terminal_mode) < 0) {
    fprintf(stderr, "lab1a tcsetattr error: %s\n", strerror(errno));
    exit(1);
  }

  // retrieve arguments
  struct option passed_options[] = {
				    {"shell", 1, NULL, 115 /*"s"*/},
            {"debug", 0, NULL, 100 /*"d"*/},
				    {0,0,0,0}
  };
  int shell_passed = 0;
  int returned_option;
  char* program;
  while ((returned_option = getopt_long(argc, argv, "", passed_options, NULL)) \
	 != -1) {
    
    switch(returned_option) {
      case 115 /*"s"*/:
        shell_passed = 1;
        program = optarg;
        break;

      case 100 /*"d"*/:
        debug_passed = 1;
        break;

      default:
        reset_terminal();
        fprintf(stderr, "Usage: lab1a [--shell=program] [--debug]\n");
        exit(1);
    }
  }

  // no option passed --> normal input/output reading
  if (!shell_passed) {
    no_option_passed(&original_terminal_mode);
  }
  else { // option passed --> fork process
    option_passed(&original_terminal_mode, program);
  }

  //exit successfully
  exit(0);
}
