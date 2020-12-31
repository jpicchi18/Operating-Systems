# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

# variables
CC = /usr/local/cs/bin/gcc # TO-DO: change later
CFALGS = -Wall -Wextra

# targets
lab2_list: lab2_list.c SortedList.c
	$(CC) $(CFLAGS) -lpthread -lprofiler -o $@ $^

tests: lab2_list # TODO: udpate later
	chmod a+x test_cases.sh
	-./test_cases.sh

profile:
	rm -f profile.out profile.gperf ./raw.gperf
	$(CC) $(CFLAGS) -g -lpthread -lprofiler -o lab2_list_extra lab2_list.c SortedList.c
	LD_PRELOAD=/usr/lib64/libprofiler.so \
	CPUPROFILE=./raw.gperf ./lab2_list_extra --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list_extra ./raw.gperf >>profile.out
	pprof --list=thread_work ./lab2_list_extra ./raw.gperf >>profile.out
	rm -f ./raw.gperf lab2_list_extra

graphs: tests
	gnuplot ./lab2b.gp

clean:
	rm -f lab2b-605124511.tar.gz lab2_list ./raw.gperf

dist: graphs profile
	tar -czf lab2b-605124511.tar.gz SortedList.h SortedList.c lab2_list.c Makefile lab2b_list.csv \
	profile.out lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png README test_cases.sh \
	lab2b.gp
