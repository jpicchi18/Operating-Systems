# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

# variables
CC = gcc # TO-DO: change later
CFALGS = -Wall -Wextra

# targets
build: lab2_add lab2_list #TODO: fix later

lab2_add: lab2_add.c
	$(CC) $(CFLAGS) -lpthread -o $@ $^

lab2_list: lab2_list.c SortedList.c
	$(CC) $(CFLAGS) -lpthread -o $@ $^

tests: build # TODO: udpate later
	chmod a+x test_cases.sh
	-./test_cases.sh

graphs: tests
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp

clean:
	rm -f lab2a-605124511.tar.gz lab2_add lab2_list

dist: graphs
	tar -czf lab2a-605124511.tar.gz lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile lab2_add.csv lab2_list.csv \
	README lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png \
	lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png lab2_add.gp lab2_list.gp test_cases.sh
