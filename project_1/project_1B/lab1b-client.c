// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <zlib.h>

int port_arg, compress_passed, debug_passed, sockfd;
int process_finished, logfd;
char *log_arg, *host_arg;
struct termios original_terminal_mode;

void error_report(char* message);
void client_connect(char *hostname, unsigned int port_num);
void process_args(int argc, char* argv[]);
void set_terminal();
void reset_terminal();
void server_io();
void sockfd_read();
void stdin_read();
void exit_routine();
void dummy_test();
// compressed buffer must be 256 bytes large
int compress_data(char *uncompressed_buffer, char* compressed_buffer, int read_ret);
int decompress_data(char *uncompressed_buffer, char* compressed_buffer, int read_ret);


int main(int argc, char *argv[]) {
    // var declarations

    // process command line args
    process_args(argc, argv);

    // connect the socket
    client_connect(host_arg, port_arg);

    // set terminal to non-canonical, no-echo mode
    set_terminal();

    // establish exit routine
    logfd = -1;
    if (atexit(exit_routine) < 0) {
        error_report("lab1b-client error: failed to register proper exit routine");
    }

    // perform i/o with the server
    server_io();

    // explicitly exit, just in case
    exit(0);
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

void dummy_test() {
    // test some stuff
    char buf[256];
    size_t read_ret = read(sockfd, buf, 255);
    write(1, buf, read_ret);
    exit(0);
}

void exit_routine() {
    // close the socket
    if (close(sockfd) < 0) {
        error_report("lab1b-client error: failed to close socket");
    }

    // close log file if necessary
    if (logfd > 0) {
        if (close(logfd) < 0) {
            error_report("lab1b-client error: failed to close log file descriptor");
        }
        logfd = -1;
    }

    // restore terminal modes
    reset_terminal();

    if (debug_passed) {
        fprintf(stderr, "finishing exit_routine\n");
    }
}

void server_io() {
    // var declarations
    int poll_ret;
    struct pollfd fds[2];
    
    // create 2 pollfd structs --> fds[0] = stdin --> fds[1] = sockfd
    fds[0].fd = 0;
    fds[0].events = POLLIN | POLLHUP;
    fds[0].revents = 0;
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLHUP;
    fds[1].revents = 0;

    // create log file if needed
    if (log_arg) {
        logfd = creat(log_arg, 0666);
        if (logfd < 0) {
            error_report("lab1b-client error: creat() failed to create file");
    }
    }
    
    // interact with server
    process_finished = 0;
    while(1) {
        // poll stdin and sockfd
        poll_ret = poll(fds, 2, -1);
        if (poll_ret < 0) {
            error_report("lab1b-client error: poll() failed");
        }

        else if (poll_ret > 0) {

            // if sockfd is ready to read
            if (fds[1].revents & POLLIN) {
                sockfd_read();
            }

            // if stdin ready to read
            if (fds[0].revents & POLLIN) {
                stdin_read();
            }

            // if stdin hung up
            if (fds[0].revents & (POLLHUP | POLLERR)) {
                error_report("lab1b-client error: error polling from stdin");
            }

            // if sockfd hung up
            if (fds[1].revents & (POLLHUP | POLLERR)) {
                process_finished = 1;
            }

        }

        if (process_finished) {
            exit(0);
        }
    }
}

void stdin_read() {
    // var declarations
    char buf[256], compressed_buffer[256];
    size_t write_ret;
    int read_ret, old_read_ret;

    // read from stdin
    read_ret = read(STDIN_FILENO, buf, sizeof(buf));
    if (read_ret < 0) {
        error_report("lab1b-client error: read() failed");
    }

    // write to stdout
    for (int i=0; i<read_ret; i++) {
        // handle'\r' and '\n'
        if (buf[i] == 0xd || buf[i] == 0xa) {
             write_ret = write(STDOUT_FILENO, "\r\n", 2);
             if (write_ret < 0) {
                 error_report("lab1b-client error: write() failed");
             }
        }

        // handle all other chars
        else {
            write_ret = write(STDOUT_FILENO, &buf[i], 1);
            if (write_ret < 0) {
                 error_report("lab1b-client error: write() failed");
             }
        }
    }

    // compress before sending
    old_read_ret = read_ret;
    if (compress_passed && read_ret > 0) {
        read_ret = compress_data(buf, compressed_buffer, read_ret);
    }
    else {
        bcopy(buf, compressed_buffer, read_ret);
    }
    
    // write to sockfd
    write_ret = write(sockfd, compressed_buffer, read_ret);
    if (write_ret < 0) {
        error_report("lab1b-client error: write() failed");
    }

    // if log passed, log chars written to socket
    if (log_arg && read_ret > 0) {
        // write "SENT "
        write_ret = write(logfd, "SENT ", 5);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }
        
        // write "#"
        char number_to_display[50];
        sprintf(number_to_display, "%d", read_ret);
        write_ret = write(logfd, number_to_display, strlen(number_to_display));
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write " bytes: "
        write_ret = write(logfd, " bytes: ", 8);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write sent contents
        write_ret = write(logfd, compressed_buffer, read_ret);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write newline
        write_ret = write(logfd, "\n", 1);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

    }
}

void sockfd_read() {
    // var declarations
    char buf[256], decompressed_buf[256];
    size_t write_ret;
    int read_ret, write_amt, old_read_ret;
    
    // read from sockfd
    read_ret = read(sockfd, buf, sizeof(buf));
    if (read_ret < 0) {
        error_report("lab1b-client error: read() failed");
    }

    // if we reached EOF
    else if (read_ret == 0) {
        process_finished = 1;
    }

    // decompress
    old_read_ret = read_ret;
    if (compress_passed) {
        read_ret = decompress_data(buf, decompressed_buf, read_ret);
    }
    else {
        bcopy(buf, decompressed_buf, read_ret);
    }

    // write to stdout
    for (int i=0; i<read_ret; i++) {
        // handle '\r' and '\n'
        if (decompressed_buf[i] == 0xd || decompressed_buf[i] == 0xa) {
            write_ret = write(STDOUT_FILENO, "\r\n", 2);
            if (write_ret < 0) {
                error_report("lab1b-client error: write() failed");
            }
        }

        // all other chars
        else {
            write_ret = write(STDOUT_FILENO, &decompressed_buf[i], 1);
            if (write_ret < 0) {
                error_report("lab1b-client error: write() failed");
            }
        }
    }

    // if log passed, log chars written to socket
    if (log_arg && read_ret > 0) {
        // write "SENT "
        write_ret = write(logfd, "RECEIVED ", 9);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }
        
        // write "#"
        char number_to_display[50];
        sprintf(number_to_display, "%d", old_read_ret);
        write_ret = write(logfd, number_to_display, strlen(number_to_display));
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write " bytes: "
        write_ret = write(logfd, " bytes: ", 8);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write sent contents
        write_ret = write(logfd, buf, old_read_ret);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

        // write newline
        write_ret = write(logfd, "\n", 1);
        if (write_ret < 0) {
            error_report("lab1b-client error: write() to logfd failed");
        }

    }
}

void reset_terminal() {
    if (tcsetattr(0, TCSANOW, &original_terminal_mode) < 0) {
        error_report("lab1b-client error: failed to restore terminal modes");
    }
}

void set_terminal() {
    // get initial terminal mode
    if (tcgetattr(0, &original_terminal_mode) < 0) {
        error_report("lab1b-client error: failed to retrieve terminal mode");
    }

    // make sure normal modes restore at program exit
    if (atexit(reset_terminal) < 0) {
        error_report("lab1b-client error: atexit() failed");
    }

    // set new terminal modes
    struct termios new_terminal_mode = original_terminal_mode;
    new_terminal_mode.c_iflag = ISTRIP;
    new_terminal_mode.c_oflag = 0;
    new_terminal_mode.c_lflag = 0;
    if (tcsetattr(0, TCSANOW, &new_terminal_mode) < 0) {
        error_report("lab1b-client error: failed to set new terminal mode");
    }

}

void error_report(char* message) {
    perror(message);
    exit(1);
}

void client_connect(char *hostname, unsigned int port_num) {
    // var declarations
    struct sockaddr_in server_addr;
    struct hostent *server;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error_report("lab1b-client error: socket() failed");
    }

    // convert hostname to IP address
    server = gethostbyname(hostname);
    if (server == NULL) {
        error_report("lab1b-client error: invalid hostname");
    }

    // set server_addr fields
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // connect to the server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error_report("lab1b-client error: connect() failed");
    }
}

void process_args(int argc, char* argv[]) {
    // define valid args
    struct option passed_options[] = {
		{"port", 1, NULL, 112 /*"p"*/},
        {"log", 1, NULL, 108 /*"l"*/},
        {"compress", 0, NULL, 99 /*"c"*/},
        {"host", 1, NULL, 104 /*"h"*/},
        {"debug", 0, NULL, 100 /*"d"*/},
		{0,0,0,0}
    };
    
    // process args from command line
    port_arg = compress_passed = debug_passed = 0;
    int returned_option;
    int port_passed = 0;
    log_arg = host_arg = NULL;
    char* usage_statement = "Usage: lab1b-client --port=portnum [--log=filename] \
[--compress] [--host=hostname]";
    while ((returned_option = getopt_long(argc, argv, "", passed_options, NULL)) \
	 != -1) {
    
    switch(returned_option) {
        case 112 /*"p" for port*/:
            port_arg = atoi(optarg);
            port_passed = 1;
            break;

        case 108 /*"l" for log*/:
            log_arg = optarg;
            break;

        case 99 /* "c" for compress */:
            compress_passed = 1;
            break;

        case 104 /*"h" for host*/:
            host_arg = optarg;
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

    // use default host if none passed
    if (host_arg == NULL) {
        host_arg = "localhost";
    }
}