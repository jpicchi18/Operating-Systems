// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <poll.h>
#include <fcntl.h>
#include <ctype.h>

// non-beaglebone testing
#ifdef DUMMY

typedef int *mraa_aio_context;
typedef int *mraa_gpio_context;
typedef int mraa_result_t;
typedef int mraa_gpio_edge_t;
typedef int mraa_gpio_dir_t;

#define MRAA_GPIO_IN 0
#define MRAA_SUCCESS 0
#define MRAA_GPIO_EDGE_RISING 0
#define MRAA_GPIO_IN 0

mraa_gpio_context mraa_gpio_init (int pin) {
    pin++;
    return (int *)malloc(sizeof(int));
}

mraa_aio_context mraa_aio_init (int pin) {
    pin++;
    return (int *)malloc(sizeof(int));
}

int mraa_aio_read(mraa_aio_context dev) {
    *dev = 650;
    return *dev;
}

mraa_result_t mraa_gpio_close(mraa_gpio_context dev) {
    if (dev) {
        free(dev);
    }
    return MRAA_SUCCESS;
}

mraa_result_t mraa_aio_close(mraa_aio_context dev) {
    if (dev) {
        free(dev);
    }
    return MRAA_SUCCESS;
}

mraa_result_t mraa_gpio_isr(mraa_gpio_context dev, mraa_gpio_edge_t edge, \
void(*fptr)(void *), void *args) {
    *dev = 650;
    edge++;
    fptr = args;
    args = fptr;
    return MRAA_SUCCESS;
}

mraa_result_t mraa_gpio_dir(mraa_gpio_context dev, mraa_gpio_dir_t dirbutton) {
    *dev = 650;
    dirbutton++;
    return MRAA_SUCCESS;
}

void mraa_deinit() {
    return;
}

#else

#include <mraa/gpio.h>
#include <mraa/aio.h>

#endif

#define GPIO_115_pin 73
#define AIO_pin 0 // TODO: might be 0 instead
#define p 112
#define s 115
#define l 108
#define C 67
#define F 70
#define newline 10
#define nul 0
#define billion

int period_arg;
int just_started_again = 0;
int command_buf_place = 0;
int generate_temp_reports = 1;
char scale_arg, command_buf[1024];
FILE *log_file_ptr = NULL;
mraa_gpio_context button = NULL;
mraa_aio_context sensor = NULL;

void error(char *msg, int exit_code);
void process_args(int argc, char **argv);
void initialize_devices();
void shutdown_procedure();
float get_temp_reading();
void button_press_handler();
void print_temp_entry();
int check_temp_ready(int *next_sec, int *next_nsec);
void read_stdin();
void process_stdin(char *commands_buf);
void register_log_file();
void print_current_time();

int main(int argc, char **argv) {
    // process command line args
    process_args(argc, argv);
    
    // intialize pins for button and sensor
    initialize_devices();

    // signal handler for when button gets pressed
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, button_press_handler, NULL);

    // check temp and stdin
    int next_sec = 0;
    int next_nsec = 0;
    struct pollfd stdin_struct = {STDIN_FILENO, POLLIN, 0};
    int poll_ret;

    while (1) {
        // print temp recording if ready
        if (generate_temp_reports && check_temp_ready(&next_sec, &next_nsec)) {
            print_temp_entry();
        }
        // check for input to stdin
        poll_ret = poll(&stdin_struct, 1, 0);
        if (poll_ret < 0) {
            error("lab4b: failed to poll from stdin", 1);
        }
        else if (poll_ret > 0) {
            // read from stdin and process input
            read_stdin();
        }
    }

    // close hardware pins
    shutdown_procedure();

    return 0;
}

void register_log_file(const char *filename) {
    log_file_ptr = fopen(filename, "w+");
    if (!log_file_ptr) {
        error("lab4b: failed to create or open log file", 1);
    }
}

void process_command(char *command) {
    char period_buf[8], log_buf[5];

    // copy chars to see if command is "period"
    for (int i=0; i<7; i++) {
        period_buf[i] = command[i];
    }
    period_buf[7] = nul;

    // copy chars to see if command is "log"
    for (int i=0; i<4; i++) {
        log_buf[i] = command[i];
    }
    log_buf[4] = nul;
    
    if (strcmp(command, "SCALE=F") == 0) {
        // all subsequent reports should be fahrenheit
        scale_arg = F;
    }
    else if (strcmp(command, "SCALE=C") == 0) {
        // change reports to celcius
        scale_arg = C;
    }
    else if (strcmp(command, "STOP") == 0) {
        // stop generating temp reports
        generate_temp_reports = 0;
        just_started_again = 1;
    }
    else if (strcmp(command, "START") == 0) {
        // start generating reports
        generate_temp_reports = 1;
    }
    else if (strcmp(command, "OFF") == 0) {
        // log to file
        if (log_file_ptr) {
            fprintf(log_file_ptr, "%s\n", command);
        }
        
        // same behavior as button
        button_press_handler();
    }
    else if (strcmp(period_buf, "PERIOD=") == 0) {
        char *new_period_arg = &command[7];
        
        for (unsigned int i=0; i<strlen(new_period_arg); i++) {
            if (!isdigit(new_period_arg[i])) {
                // TODO GENERATE AN ERROR? OR JUST DO NOTHING?
                shutdown_procedure();
                error("lab4b: invalid argument to the stdin command 'PERIOD='", 1);
            }
        }

        period_arg = atoi(new_period_arg);
    }
    else if (strcmp(log_buf, "LOG ") == 0) {
        // do nothing
    }
    else {
        // do nothing, just log it if command is invalid
    }

    if (log_file_ptr) {
        fprintf(log_file_ptr, "%s\n", command);
    }
}

void read_stdin() {
    // var initialization
    char buf[1024];
    ssize_t read_ret;
    
    // read from stdin
    read_ret = read(STDIN_FILENO, buf, sizeof(buf));
    if (read_ret < 0) {
        error("lab4b: failed to read from stdin", 1);
    }

    // process stdin input
    for (int i=0; i<read_ret; i++) {
        if (buf[i] == newline) {
            command_buf[command_buf_place] = nul;
            process_command(command_buf);
            command_buf_place = 0;
        }
        else {
            command_buf[command_buf_place] = buf[i];
            command_buf_place++;
            if (command_buf_place > 1023) {
                shutdown_procedure();
                error("lab4b error: stdin command is too large", 1);
            }
        }
    }
}

int check_temp_ready(int *next_sec, int *next_nsec) {
    if (just_started_again) {
        *next_sec = 0;
        *next_nsec = 0;
        just_started_again = 0;
    }
    
    // get current time
    struct timespec current_time;
    if (clock_gettime(CLOCK_MONOTONIC, &current_time) != 0) {
        error("lab4b: gettime() failed while checking if it's time to print temp", 1);
    }

    if (current_time.tv_sec > *next_sec) {
        *next_sec = current_time.tv_sec + period_arg;
        *next_nsec = current_time.tv_nsec;
        return 1;
    }
    else if (current_time.tv_sec == *next_sec && current_time.tv_nsec >= *next_nsec) {
        *next_sec = current_time.tv_sec + period_arg;
        *next_nsec = current_time.tv_nsec;
        return 1;
    }
    else {
        return 0;
    }
}

void print_temp_entry() {
    print_current_time();

    // print temp
    float temp = get_temp_reading();
    printf("%.1f\n", temp);
    if (log_file_ptr) {
        fprintf(log_file_ptr, "%.1f\n", temp);
    }
}

void print_current_time() {
    struct timespec current_time;
    struct tm *tm;
    if (clock_gettime(CLOCK_REALTIME, &current_time) != 0) {
        error("lab4b: gettime() failed", 1);
    }
    tm = localtime(&(current_time.tv_sec));
    if (!tm) {
        error("lab4b: localtime() system call failed", 1);
    }

    printf("%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);

    if (log_file_ptr) {
        fprintf(log_file_ptr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
    }
}

void button_press_handler() {
    print_current_time();

    // print shutdown message
    printf("SHUTDOWN\n");
    if (log_file_ptr) {
        fprintf(log_file_ptr ,"SHUTDOWN\n");
    }

    shutdown_procedure();
    exit(0);
}

float get_temp_reading() {
    // define calib vals
    const int B = 4275;
    const float R0 = 100000.0;
    
    int reading = mraa_aio_read(sensor);

    float R = 1023.0/((float) reading) - 1.0;
    R = R0 * R;

    // return val depends on unit settings
    float celcius = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;
    if (scale_arg == C) {
        return celcius;
    }
    else if (scale_arg == F) {
        return (celcius * 9)/5 + 32;
    }
    else {
        error("lab4b: argument to '--scale' must be either 'C' or 'F'", 1);
        return -1;
    }
}

void shutdown_procedure() {
    if (button) {
        if (mraa_gpio_close(button) != MRAA_SUCCESS) {
            error("lab4b error: failed to close sensor button", 1);
        }
    }
    if (sensor) {
        if (mraa_aio_close(sensor) != MRAA_SUCCESS) {
            error("lab4b error: failed to close sensor and/or button", 1);
        }
    }
    if (log_file_ptr) {
        if (fclose(log_file_ptr) != 0) {
            error("lab4b: failed to close log file", 1);
        }
    }
}

void initialize_devices() {
    // init with pin numbers on the board
    button = mraa_gpio_init(GPIO_115_pin);
    sensor = mraa_aio_init(AIO_pin);

    // check for init error
    if (!button || !sensor) {
        fprintf(stderr, "lab4b: failed to initialize button and/or sensor\n");
        mraa_deinit();
        exit(1);
    }

    // configure button to be input
    if (mraa_gpio_dir(button, MRAA_GPIO_IN) != MRAA_SUCCESS) {
        error("lab4b: failed to configure button as input", 1);
    }
}

void error(char *msg, int exit_code) {
    perror(msg);
    exit(exit_code);
}

void process_args(int argc, char* argv[]) {
    // define valid args
    struct option passed_options[] = {
		{"period", 1, NULL, p},
        {"scale", 1, NULL, s},
        {"log", 1, NULL, l},
		{0,0,0,0}
    };
    
    // process args from command line
    int returned_option;
    period_arg = 1;
    scale_arg = F;
    log_file_ptr = NULL;
    char* usage_statement = "Usage: lab4b [--period=N] [--scale=(C|F)] \
[--log=path_to_file]\n";
    while ((returned_option = getopt_long(argc, argv, "", passed_options, NULL)) \
	 != -1) {
    
        switch(returned_option) {
            case p:
                for (unsigned int i=0; i<strlen(optarg); i++) {
                    if (!isdigit(optarg[i])) {
                        error("lab4b: argument to '--period' must consist of all digits", 1);
                    }
                }
                period_arg = atoi(optarg);
                break;

            case s:
                if (strlen(optarg) != 1) {
                    fprintf(stderr, "lab4b: argument to '--scale' must be 'C' or 'F'\n");
                    fprintf(stderr, "%s\n", usage_statement);
                    exit(1);
                }
                scale_arg = optarg[0];
                break;

            case l:
                register_log_file(optarg);
                break;

            default:
                fprintf(stderr, "%s\n", usage_statement);
                exit(1);
        }   
    }

    // check for valid scale_arg
    if (scale_arg != F && scale_arg != C) {
        fprintf(stderr, "lab4b: argument to '--period' must be 'C' or 'F'\n");
        fprintf(stderr, "%s\n", usage_statement);
        exit(1);
    }
}