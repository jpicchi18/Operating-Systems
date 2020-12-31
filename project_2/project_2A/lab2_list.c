// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include "SortedList.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>


int threads_arg, iterations_arg, opt_yield, opt_sync;
char *test_name, *yield_arg, random_string[11], sync_arg;
unsigned long num_elements;
void error(char* message, int ret_code);
SortedList_t *head_ptr;
SortedListElement_t *list_elements_ptr;
long long_lock = 0;


void process_args(int argc, char **argv);
void *thread_work(void *arg);
static inline unsigned long nanosec_from_timespec(struct timespec *timespec_ptr);
void print_test_name();
void corrupted_list_error(char *msg);
void SIGSEGV_handler();
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char* argv[]) {
    // var initiailziation
    struct timespec begin, end;
    
    // process command line args
    process_args(argc, argv);

    // initialize an empty list
    SortedList_t head;
    head.next = head.prev = &head;
    head.key = NULL;

    // create and initialize (threads x iterations) number of list elements
    // using random keys (10-element arrays of lowercase chars)
    SortedListElement_t list_elements[num_elements];
    char keys[num_elements][11];

    for (int i=0; i<num_elements; i++) {
        // generate random string
        keys[i][10] = '\0';
        for (int k=0; k<10; k++) {
            keys[i][k] = rand()%26 + 97;
        }
        list_elements[i].key = &keys[i][0];
    }

    // test the "SortedList.c" implementation
    // SortedList_insert(&head, &list_elements[0]);
    // assert(SortedList_length(&head) == 1);
    // assert(SortedList_lookup(&head, list_elements[0].key) == &list_elements[0]);
    // assert(!SortedList_lookup(&head, list_elements[1].key));
    // assert(!SortedList_delete(&list_elements[0]));
    // assert(SortedList_length(&head) == 0);
    // for (int i=0; i<num_elements; i++) {
    //     SortedList_insert(&head, &list_elements[i]);
    // }
    // SortedListElement_t *current = head.next;
    // for (int i=0; i<num_elements; i++) {
    //     assert(SortedList_lookup(&head, list_elements[i].key) == &list_elements[i]);
    //     printf("%s\n", current->key);
    //     current = current->next;
    // }
    // assert(SortedList_length(&head) == 5);
    // for (int i=0; i<num_elements; i++) {
    //     assert(!SortedList_delete(&list_elements[i]));
    // }
    // assert(SortedList_length(&head) == 0);

    // register segfault handler
    if (signal(SIGSEGV, SIGSEGV_handler) < 0) {
        error("lab2_list error: signal() sys call failed", 1);
    }

    // prepare to start threads
    pthread_t threads_array[threads_arg];
    head_ptr = &head;
    list_elements_ptr = list_elements;

    // get start time
    if (clock_gettime(CLOCK_MONOTONIC, &begin) < 0) {
        error("lab2_add: clock_gettime failed", 1);
    }

    // start the threads
    for (int i=0; i<threads_arg; i++) {
        if (pthread_create(&threads_array[i], NULL, thread_work, (void *)(list_elements + iterations_arg*i)) != 0) {
            error("lab2_list error: pthread_create() failed", 1);
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

    // TODO: check list length to confirm it's 0
    if (SortedList_length(head_ptr) != 0) {
        corrupted_list_error("list corrupted: length was nonzero at end of test");
    }

    // calculate run time
    unsigned long time_diff = nanosec_from_timespec(&end) - nanosec_from_timespec(&begin);
    unsigned long operations_performed = threads_arg * iterations_arg * 3;

    // print csv stuff
    print_test_name();
    printf("%d,%d,1,%lu,%lu,%lu\n", threads_arg, iterations_arg, operations_performed, time_diff, time_diff/operations_performed);

    return 0;
}


void SIGSEGV_handler() {
    fprintf(stderr, "list corrupted: segmentation fault\n");
    exit(2);
}

void corrupted_list_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2);
}

void print_test_name() {
    printf("list-");

    if (opt_yield == 1) {
        printf("i-");
    }
    else if (opt_yield == 2) {
        printf("d-");
    }
    else if (opt_yield == 3) {
        printf("id-");
    }
    else if (opt_yield == 4) {
        printf("l-");
    }
    else if (opt_yield == 5) {
        printf("il-");
    }
    else if (opt_yield == 6) {
        printf("dl-");
    }
    else if (opt_yield == 7) {
        printf("idl-");
    }
    else {
        printf("none-");
    }

    // print sync options
    if (sync_arg == 109 /*"m"*/) {
        printf("m,");
    }
    else if (sync_arg == 115 /*"s"*/) {
        printf("s,");
    }
    else {
        printf("none,");
    }
}

static inline unsigned long nanosec_from_timespec(struct timespec *timespec_ptr) {
    unsigned long ret_val = timespec_ptr->tv_sec * 1000000000;
    ret_val += timespec_ptr->tv_nsec;
    return ret_val;
}

void process_args(int argc, char* argv[]) {
    // define valid args
    struct option passed_options[] = {
		{"threads", 1, NULL, 116 /*"t"*/},
        {"iterations", 1, NULL, 105 /*"i"*/},
        {"yield", 1, NULL, 121 /*"y"*/},
        {"sync", 1, NULL, 115 /*"s"*/},
		{0,0,0,0}
    };
    
    // process args from command line
    threads_arg = iterations_arg = 1;
    opt_sync = opt_yield = 0;
    test_name = "add-none";
    int returned_option;
    yield_arg = NULL;
    char* usage_statement = "Usage: lab2a_list [--threads=th_num] [--iterations-it_num] \
[--yield=[idl]]\n";
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
            yield_arg = optarg;
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

    // parse yield options
    if (yield_arg) {
        size_t yield_arg_length = strlen(yield_arg);

        for (int i=0; i<yield_arg_length; i++) {
            char c = yield_arg[i];
            switch(c) {
                case 105 /*"i"*/:
                    opt_yield = opt_yield | INSERT_YIELD;
                    break;
                case 100 /*"d"*/:
                    opt_yield = opt_yield | DELETE_YIELD;
                    break;
                case 108 /*"l"*/:
                    opt_yield = opt_yield | LOOKUP_YIELD;
                    break;
                default:
                    fprintf(stderr, "%s\n", usage_statement);
                    exit(1);
            }
        }
    }

    // check for proper sync_arg
    if (opt_sync) {
        if (sync_arg != 109 /*"m"*/ && sync_arg != 115 /*"s"*/) {
            fprintf(stderr, "%s\n", usage_statement);
            exit(1);
        }
    }
    else {
        sync_arg = 97 /*"a"*/;
    }

    num_elements = threads_arg*iterations_arg;
}

void error(char* message, int ret_code) {
    perror(message);
    exit(ret_code);
}

void *thread_work(void *arg) {
    SortedListElement_t* elements_for_thread = (SortedListElement_t *)arg;

    // use locks if sync called
    if (sync_arg == 109 /*"m"*/) {
        pthread_mutex_lock(&mutex_lock);
    }
    else if (sync_arg == 115 /*"s"*/) {
        while (__sync_lock_test_and_set(&long_lock, 1));
    }

    // insert all elements into list
    for (int i=0; i<iterations_arg; i++) {
        SortedList_insert(head_ptr, &elements_for_thread[i]);
    }

    // get list length
    int list_length = SortedList_length(head_ptr);
    if (list_length < iterations_arg) {
        corrupted_list_error("corrupted list: bad return value from SortedList_length()");
    }

    // look up and delete the inserted keys
    for (int i=0; i<iterations_arg; i++) {
        SortedListElement_t *element = SortedList_lookup(head_ptr, elements_for_thread[i].key);
        if (!element) {
            corrupted_list_error("list corrupted: couldn't retrieve element with known key");
        }
        if (SortedList_delete(element) != 0) {
            corrupted_list_error("list corrupted: corrupted prev/next pointers");
        }
    }

    // reset locks if sync called
    if (sync_arg == 109 /*"m"*/) {
        pthread_mutex_unlock(&mutex_lock);
    }
    else if (sync_arg == 115 /*"s"*/) {
        __sync_lock_release(&long_lock);
    }

    return NULL;
}