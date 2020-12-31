// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <getopt.h>
#include <errno.h>
#include <sys/wait.h>
#include <poll.h>
#include <signal.h>
#include <zlib.h>
#include <string.h>

int port_arg, compress_passed, process_finished, fork_ret, debug_passed;
int sockfd;
int to_child[2], from_child[2];
char *shell_arg;

void error_report(char* message);
void server_connect(unsigned int port_num);
void process_args(int argc, char* argv[]);
void sigpipe_handler(int sig);
void spawn_child();
void sockfd_read();
void shell_read();
void get_child_status();
void exit_routine();
void dummy_test();
void close_parent_pipes();
// compressed buffer must be 256 bytes large
int compress_data(char *uncompressed_buffer, char* compressed_buffer, int read_ret);
int decompress_data(char *uncompressed_buffer, char* compressed_buffer, int read_ret);


int main(int argc, char *argv[]) {
    // var declarations
    char buffer[256];

    // process command line args
    process_args(argc, argv);

    // get socket fd
    server_connect(port_arg);

    // register exit routine, initializing pipe vals first
    to_child[0] = to_child[1] = from_child[0] = from_child[1] = -1;
    if (atexit(exit_routine) < 0) {
        error_report("lab1b-server error: failed to register proper exit routine");
    }

    // register SIGPIPE handler
    if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
        error_report("lab1b-server error: failed to register sigpipe handler");
    }

    // create child & perform io
    spawn_child();

    // shouldn't have to get here
    exit(0);
}


int decompress_data(char *compressed_buffer, char* decompressed_buffer, int read_ret) {
    // initialize z_stream object
    z_stream from_sender;
    from_sender.zalloc = Z_NULL;
    from_sender.zfree = Z_NULL;
	from_sender.opaque = Z_NULL;

    if (inflateInit(&from_sender) != Z_OK) {
        error_report("lab1b-client error: zlib inflateInit() failed");
    }

    from_sender.avail_in = read_ret;
    from_sender.next_in = (unsigned char *) compressed_buffer;
    from_sender.avail_out = 256;
    from_sender.next_out = (unsigned char *) decompressed_buffer;

    do {
        inflate(&from_sender, Z_SYNC_FLUSH);
    } while (from_sender.avail_in > 0);

    inflateEnd(&from_sender);

    return(256-from_sender.avail_out);
}

int compress_data(char *uncompressed_buffer, char* compressed_buffer, int read_ret) {
    // initialize the stream
    z_stream sender_to_reciever;
    sender_to_reciever.zalloc = Z_NULL;
    sender_to_reciever.zfree = Z_NULL;
	sender_to_reciever.opaque = Z_NULL;

    if (deflateInit(&sender_to_reciever, Z_DEFAULT_COMPRESSION) != Z_OK) {
        error_report("lab1b-server error: zlib deflateInit() failed");
    }

    sender_to_reciever.avail_in = read_ret;
    sender_to_reciever.next_in = (unsigned char *) uncompressed_buffer;
    sender_to_reciever.avail_out = 256;
    sender_to_reciever.next_out = (unsigned char *) compressed_buffer;

    do {
        deflate(&sender_to_reciever, Z_SYNC_FLUSH);
    } while (sender_to_reciever.avail_in > 0);

    deflateEnd(&sender_to_reciever);

    return(256-sender_to_reciever.avail_out);
}

void close_parent_pipes() {
    if (to_child[1] > 0) {
      if (close(to_child[1]) < 0) {
          error_report("lab1b-server parent error: failed to close write pipe to child");
      }
      to_child[1] = -1;
  }

  if (from_child[0] > 0) {
      if (close(from_child[0]) < 0) {
          error_report("lab1b-server parent error: failed to close read pipe from child");
      }
      from_child[0] = -1;
  }
}

void dummy_test() {
    // test some stuff
    char buf[256];
    size_t read_ret = read(0, buf, 255);
    write(sockfd, buf, read_ret);
    exit(0);
}

void exit_routine() {
    // close remaining pipes
    close_parent_pipes();
    
    // close the socket
    if (close(sockfd) < 0) {
        error_report("lab1b-server error: failed to close socket");
    }

    if (debug_passed) {
        fprintf(stderr, "finishing exit_routine\n");
    }
}

void spawn_child() {
    // var declaration

    // create pipes
    if (pipe(to_child) < 0 || pipe(from_child) < 0) {
        error_report("lab1b-server error: failed to create pipes");
    }

    // fork a new process
    fork_ret = fork();
    if (fork_ret < 0) {
        error_report("lab1b-server error: failed to fork new process");
    }

    // child
    else if (fork_ret == 0) {
        // close the socket
        if (close(sockfd) < 0)
            error_report("lab1b-server child error: failed to close socket");

        // close unused ends of pipes
        if (close(from_child[0]) < 0 || close(to_child[1]) < 0) {
            error_report("lab1b-server child error: failed to close pipe ends");
        }

        // redirect stdin to pipe
        if (close(STDIN_FILENO) < 0 || dup(to_child[0]) < 0 || close(to_child[0]) < 0) {
            error_report("lab1b-server child error: failed to redirect stdin to pipe");
        }

        // redirect stdout/stderr to pipe
        if (close(STDOUT_FILENO) < 0 || dup(from_child[1]) < 0 || close(STDERR_FILENO) < 0 || \
        dup(from_child[1]) < 0 || close(from_child[1]) < 0) {
            error_report("lab1b-server child error: failed to redirect stdout/stderr to pipe");
        }

        //execute program
        if (execlp(shell_arg, shell_arg, NULL) < 0) {
            error_report("lab1b-server child error: execlp() failed");
        }
    }

    // parent
    else if (fork_ret > 0) {
        // var declaration
        int poll_ret;
        struct pollfd fds[2];

        // close unused pipe ends
        if (close(to_child[0]) < 0 || close(from_child[1]) < 0) {
            error_report("lab1b-server parent error: failed to close pipe ends");
        }

        // create 2 pollfd structs --> fds[0] = sockfd --> fds[1] = from_child[0]
        fds[0].fd = sockfd;
        fds[0].events = POLLIN | POLLHUP;
        fds[0].revents = 0;
        fds[1].fd = from_child[0];
        fds[1].events = POLLIN | POLLHUP;
        fds[1].revents = 0;

        process_finished = 0;
        while(1) {
            // poll sockfd and from_child[0]
            poll_ret = poll(fds, 2, -1);
            if (poll_ret < 0) {
                error_report("lab1b-server parent error: poll() failed");
            }

            else if (poll_ret > 0) {

                // sockfd ready to read
                if (fds[0].revents & POLLIN) {
                    if (debug_passed) {
                        fprintf(stderr, "sockfd reading\n");
                    }
                    sockfd_read();
                }

                // from_shell[0] ready to read
                if (fds[1].revents & POLLIN) {
                    if (debug_passed)
                        fprintf(stderr, "from_shell[0] reading\n");
                    shell_read();
                }

                // socket (ie client) hung up
                if (fds[0].revents & (POLLHUP | POLLERR)) {
                    error_report("lab1b-server parent error: client hung up before server");
                }

                // child hung up
                if (fds[1].revents & (POLLHUP | POLLERR)) {
                    if (debug_passed) {
                        fprintf(stderr, "child hung up\n");
                    }
                    process_finished = 1;
                }

            }

            // see if we're done
            if (process_finished) {
                if (debug_passed) {
                    fprintf(stderr, "process_finished\n");
                }
                
                // close pipes
                close_parent_pipes();

                // get child status
                get_child_status();

                exit(0);
            }
        }
    }
}

void get_child_status() {
    // close pipes, if open
    close_parent_pipes();
    
    // var declarations
    int waitpid_ret;

    // harvest child return status
    if (waitpid(fork_ret, &waitpid_ret, 0) < 0) {
        error_report("lab1b-server parent error: waitpid() failed");
    }

    // report child return status
    fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(waitpid_ret), WEXITSTATUS(waitpid_ret));
}

void shell_read() {
    // var declarations
    char buf[256], new_buf[256];
    size_t write_ret;
    int read_ret, compress_ret;

    read_ret = read(from_child[0], buf, sizeof(buf));
    if (read_ret < 0) {
        error_report("lab1b-server parent error: write() failed");
    }

    // check for EOF
    if (read_ret == 0) {
        process_finished = 1;
    }

    // compress if necessary
    if (compress_passed && read_ret > 0) {
        compress_ret = compress_data(buf, new_buf, read_ret);
    }
    else {
        bcopy(buf, new_buf, read_ret);
        compress_ret = read_ret;
    }

    write_ret = write(sockfd, new_buf, compress_ret);
    if (write_ret < 0) {
        error_report("lab1b-server parent error: write() failed");
    }
}

void sockfd_read() {
    // var declarations
    char buf[256], decompressed_buf[256];
    size_t read_ret, write_ret;

    read_ret = read(sockfd, buf, sizeof(buf));
    if (read_ret < 0) {
        error_report("lab1b-server parent error: failed to read from socket");
    }

    // decompress data
    if (compress_passed && read_ret > 0) {
        read_ret = decompress_data(buf, decompressed_buf, read_ret);
    }
    else {
        bcopy(buf, decompressed_buf, read_ret);
    }

    for (int i=0; i<read_ret; i++) {
        // handle ^C
        if (decompressed_buf[i] == 0x3) {
            if (kill(fork_ret, SIGINT) < 0) {
                error_report("lab1b-server parent error: failed to send SIGINT to shell");
            }
        }

        // handle EOF ^D
        else if (decompressed_buf[i] == 0x4) {
            // close write side of pipe to shell
            if (to_child[1] > 0) {
                if (close(to_child[1]) < 0) {
                    error_report("lab1b-server parent error: failed to close write pipe to child");
                }

                to_child[1] = -1;
            }
        }

        // handle '/r' and '/n'
        else if (decompressed_buf[i] == 0xd || decompressed_buf[i] == 0xa) {
            if (to_child[1] > 0) {
                write_ret = write(to_child[1], "\n", 1);
                if (write_ret < 0) {
                    error_report("lab1b-server parent error: write() failed");
                }
            }
        }

        // all other chars
        else {
            if (to_child[1] > 0) {
                write_ret = write(to_child[1], &decompressed_buf[i], 1);
                if (write_ret < 0) {
                    error_report("lab1b-server parent error: write() failed");
                }
            }
        }
    }
}

void sigpipe_handler(int sig) {
    if (debug_passed) {
        fprintf(stderr, "called sigpipe_handler\n");
    }
    
    // close pipes, if open
    close_parent_pipes();

    // get child exit status
    get_child_status();

    exit(0);
}

void error_report(char* message) {
    perror(message);
    exit(1);
}

void server_connect(unsigned int port_num) {
    // var declarations
    int old_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    // create socket
    old_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (old_sockfd < 0) {
        error_report("lab1b-server error: socket() sys call failed");
    }

    // set server_addr fields
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_arg);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero)); // padding

    // bind socket to address and port number
    if (bind(old_sockfd, (struct sockaddr *) &server_addr, \
    sizeof(server_addr)) < 0) {
        error_report("lab1b-server error: bind() failed");
    }

    // listen on the socket for connections
    if (listen(old_sockfd, 1) < 0) {
        error_report("lab1b-server error: listen() failed");
    }

    // block until the client connects
    sin_size = sizeof(client_addr);
    sockfd = accept(old_sockfd, (struct sockaddr *) &client_addr, &sin_size);
    if (sockfd < 0) {
        error_report("lab1b error: accept() failed");
    }
    
    // no longer need the old socket fd
    if (close(old_sockfd) < 0) {
        error_report("lab1b-server error: failed to close old sockfd");
    }
}

void process_args(int argc, char* argv[]) {
    // define valid args
    struct option passed_options[] = {
		{"port", 1, NULL, 112 /*"p"*/},
        {"shell", 1, NULL, 115 /*"s"*/},
        {"compress", 0, NULL, 99 /*"c"*/},
        {"host", 1, NULL, 104 /*"h"*/},
        {"debug", 0, NULL, 100 /*"d"*/},
		{0,0,0,0}
    };
    
    // process args from command line
    port_arg = compress_passed = debug_passed = 0;
    int returned_option;
    int port_passed = 0;
    shell_arg = NULL;
    char* usage_statement = "Usage: lab1b-server --port=portnum [--shell=program] \
[--compress]";
    while ((returned_option = getopt_long(argc, argv, "", passed_options, NULL)) \
	 != -1) {
    
    switch(returned_option) {
        case 112 /*"p" for port*/:
            port_arg = atoi(optarg);
            port_passed = 1;
            break;

        case 115 /*"s" for shell*/:
            shell_arg = optarg;
            break;

        case 99 /* "c" for compress */:
            compress_passed = 1;
            break;

        case 100 /*"d"*/:
            debug_passed = 1;
            break;
            
        default:
            fprintf(stderr, "%s\n", usage_statement);
            exit(1);
    }   
    }

    // check for required arguments
    if (port_passed == 0) {
        fprintf(stderr, "%s\n", usage_statement);
        exit(1);
    }

    // use default shell and host if none passed
    if (shell_arg == NULL) {
        shell_arg = "/bin/bash";
    }
}