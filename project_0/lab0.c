// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include<getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>


void SIGSEGV_handler(int sig) {
  fprintf(stderr, "SIGSEGV_handler: segmentation fault caught.\n");
  exit(4);
}

void cause_segfault() {
  char* char_pointer = NULL;
  *(char_pointer) = 0;
}


int main(int argc, char *argv[]) {

  int returned_option, segfault_passed, catch_passed, input_passed, output_passed;
  int in_fd, out_fd;
  size_t bytes_read, bytes_written;
  

  
  // PART 1: first process all arguments and store the results in variables
  
  struct option cl_options[] = {
				{"input", 1, NULL, 105 /*"i"*/},
				{"output", 1, NULL, 111 /*"o"*/},
				{"segfault", 0, NULL, 115 /*"s"*/},
				{"catch", 0, NULL, 99 /*"c"*/},
				{0,0,0,0}
  };

  segfault_passed = catch_passed = input_passed = output_passed = 0;
  while ( (returned_option = getopt_long(argc, argv, "", cl_options, NULL)) != -1) {

    switch(returned_option) {

    case 105: // input
      input_passed = 1;

      // input redirection
      in_fd = open(optarg, O_RDONLY);
      if (in_fd >= 0) {
	if (close(0) != 0)
	  fprintf(stderr, "error: --input: stdin: %s\n", strerror(errno));
	if (dup(in_fd) < 0)
	  fprintf(stderr, "error: --input: %s: %s\n", optarg, strerror(errno));
	if (close(in_fd) != 0)
	  fprintf(stderr, "error: --input: %s: %s\n", optarg, strerror(errno));
      }
      else {
	fprintf(stderr, "error: --input: %s: %s\n", optarg, strerror(errno));
	exit(2);
      }
	  
      break;
      
    case 111: // output
      output_passed = 1;

      // output redirection
      if (close(1) != 0)
	fprintf(stderr, "error: --output: stdout: %s\n", strerror(errno));
      if ( (out_fd = creat(optarg, 0666)) < 0) {
	fprintf(stderr, "erfror: --output: %s: %s\n", optarg, strerror(errno));
	exit(3);
      }
      
      break;
      
    case 115: // segfault
      segfault_passed = 1;
      break;
      
    case 99: // catch
      catch_passed = 1;
      break;
      
    default:
      fprintf(stderr, "Usage: lab0 [--input=filename] [--output=filename] \
[--segfault] [--catch]\n");
      exit(1);
      break;
      
    }

  }

  /*
  // look for unrecognized arguments
  if (optind < argc) {
    fprintf(stderr, "error: unrecognized arguments: ");
    while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\nUsage: lab0 [--input=filename] [--output=filename] \
[--segfault] [--catch]\n");
    exit(1);
  }
  */
     

  // register the signal handler
  if (catch_passed) {
    signal(SIGSEGV, SIGSEGV_handler);
    if (errno) {
      fprintf(stderr, "error: unable to register SIGSEGV_handler");
      exit(4);
    }
  }

  // perform segfault, if necessary
  if (segfault_passed)
    cause_segfault();


  // copy stdin to stdout
  size_t read_cnt = 45;
  size_t write_cnt;
  size_t write_ret = 0;
  char buf[50];
  
  while ( (bytes_read = read(0, buf, read_cnt)) > 0) {
    write_cnt = bytes_read;
    bytes_written = 0;
    
    while (bytes_written < bytes_read) {
      write_ret = write(1, buf + bytes_written, write_cnt - bytes_written);

      // check for write error
      if (write_ret < 0) {
	fprintf(stderr, "error: write: error in writing bytes to output");
	exit(3);
      }

      bytes_written += write_ret;
    }
  }

  // check for read error
  if (bytes_read != 0) {
    fprintf(stderr, "error: read: %s", strerror(errno));
    exit(2);
  }

  
  exit(0);
}
