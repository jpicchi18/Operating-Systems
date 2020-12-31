// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <getopt.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sched.h>
#include <string.h>


int threads_arg, iterations_arg, opt_yield, opt_sync;
long long counter = 0;
char *test_name;
char sync_arg;
void error(char* message, int ret_code);
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
long long_lock = 0;


void process_args(int argc, char* argv[]);
void *thread_work(void *arg);
void add(long long *pointer, long long value);
static inline unsigned long nanosec_from_timespec(struct timespec *timespec_ptr);
void add_mutex(long long *pointer, long long value);
void add_spinlock(long long *pointer, long long value);
void add_comp_swap(long long *pointer, long long value);
void get_test_name();


int main(int argc, char *argv[]) {
    // var initialization
    struct timespec begin, end;
    unsigned long time_diff, nanosec_per_operation, operations_performed;
    
    // process command line arguments
    process_args(argc, argv);

    // create pthreads
    pthread_t threads_array[threads_arg];

    // choose the right add function
    void (*add_ptr)(long long *, long long) = &add;
    if (sync_arg == 109 /*"m"*/) {
        add_ptr = &add_mutex;
    }
    else if (sync_arg == 115 /*"s"*/) {
        add_ptr = &add_spinlock;
    }
    else if (sync_arg == 99 /*"c"*/) {
        add_ptr = &add_comp_swap;
    }

    // get start time
    if (clock_gettime(CLOCK_MONOTONIC, &begin) < 0) {
        error("lab2_add: clock_gettime failed", 1);
    }

    // do thread work
    for (int i=0; i<threads_arg; i++) {
        if (pthread_create(&threads_array[i], NULL, thread_work, add_ptr) != 0) {
            error("lab2_add error: pthread_create failed", 1);
        }
    }

    // wait for threads to finish
    for (int i=0; i<threads_arg; i++) {
        if (pthread_join(threads_array[i], NULL) != 0) {
            error("lab2_add error: pthread_join() failed", 1);
        }
    }

    // get end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) < 0) {
        error("lab2_add error: clock_gettime() failed", 1);
    }

    // calculate time diff
    time_diff = nanosec_from_timespec(&end) - nanosec_from_timespec(&begin);

    // print desired vals
    operations_performed = threads_arg * iterations_arg * 2;
    nanosec_per_operation = time_diff/operations_performed;
    get_test_name();
    printf("%s,%d,%d,%lu,%lu,%lu,%lld\n", test_name, threads_arg, iterations_arg, \
            operations_performed, time_diff, nanosec_per_operation, counter);
    
    return 0;
}


void get_test_name() {
    if (opt_yield) {
        test_name = "add-yield-none";        
        if (sync_arg == 109 /*"m"*/) {
            test_name = "add-yield-m"; 
        }
        else if (sync_arg == 115 /*"s"*/) {
            test_name = "add-yield-s";
        }
        else if (sync_arg == 99 /*"c"*/) {
            test_name = "add-yield-c";
        }
    }
    else {
        test_name = "add-none";        
        if (sync_arg == 109 /*"m"*/) {
            test_name = "add-m"; 
        }
        else if (sync_arg == 115 /*"s"*/) {
            test_name = "add-s";
        }
        else if (sync_arg == 99 /*"c"*/) {
            test_name = "add-c";
        }
    }
}

void add_comp_swap(long long *pointer, long long value) {
    long long prev_val, new_val;
    
    do {
        prev_val = *pointer;
        if (opt_yield) {
            sched_yield();
        }
        new_val = prev_val+value;
    } while (prev_val != __sync_val_compare_and_swap(pointer, prev_val, new_val));
}

void add_spinlock(long long *pointer, long long value) {
    while (__sync_lock_test_and_set(&long_lock, 1));
    long long sum = *pointer + value;
    if (opt_yield) {
        sched_yield();
    }
    *pointer = sum;
    __sync_lock_release(&long_lock);
}

void process_args(int argc, char* argv[]) {
    // define valid args
    struct option passed_options[] = {
		{"threads", 1, NULL, 116 /*"t"*/},
        {"iterations", 1, NULL, 105 /*"i"*/},
        {"yield", 0, NULL, 121 /*"y"*/},
        {"sync", 1, NULL, 115 /*"s"*/},
		{0,0,0,0}
    };
    
    // process args from command line
    threads_arg = iterations_arg = 1;
    opt_yield = 0;
    opt_sync = 0;
    test_name = "add-none";
    int returned_option;
    char* usage_statement = "Usage: lab2a_add [--threads=th_num] [--iterations-it_num] \
[--yield] [--sync=(m,s,c)]";
    while ((returned_option = getopt_long(argc, argv, "", passed_options, NULL)) \
	 != -1) {
    
    switch(returned_option) {
        case 116 /*"t" for "threads"*/:
            threads_arg = atoi(optarg);
            break;

        case 105 /*"i" for "iterations"*/:
            iterations_arg = atoi(optarg);
            break;

        case 121 /*"y" for "yield"*/:
            opt_yield = 1;
            break;

        case 115 /*"s" for "sync"*/:
            if (strlen(optarg) != 1) {
                fprintf(stderr, "%s\n", usage_statement);
                exit(1);
            }
            opt_sync = 1;
            sync_arg = optarg[0];
            break;

        default:
            fprintf(stderr, "%s\n", usage_statement);
            exit(1);
    }   
    }

    // check for proper sync_arg
    if (opt_sync) {
        if (sync_arg != 109 /*"m"*/ && sync_arg != 115 /*"s"*/ && sync_arg != 99 /*"c"*/) {
            fprintf(stderr, "%s\n", usage_statement);
            exit(1);
        }
    }
    else {
        sync_arg = 97 /*"a"*/;
    }
}

void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if (opt_yield) {
        sched_yield();
    }
    *pointer = sum;
}

void *thread_work(void *arg) {
    // cast arg to a function pointer
    void (*add_ptr)(long long *, long long) = (void (*)(long long *, long long)) arg;

    // add to counter
    for (int i=0; i<iterations_arg; i++) { (*add_ptr)(&counter, 1); }

    // subtract from counter
    for (int i=0; i<iterations_arg; i++) { (*add_ptr)(&counter, -1); }

    return NULL;
}

void error(char* message, int ret_code) {
    perror(message);
    exit(ret_code);
}

static inline unsigned long nanosec_from_timespec(struct timespec *timespec_ptr) {
    unsigned long ret_val = timespec_ptr->tv_sec * 1000000000;
    ret_val += timespec_ptr->tv_nsec;
    return ret_val;
}

void add_mutex(long long *pointer, long long value) {
    pthread_mutex_lock(&mutex_lock);
    long long sum = *pointer + value;
    if (opt_yield) {
        sched_yield();
    }
    *pointer = sum;
    pthread_mutex_unlock(&mutex_lock);
}