# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

# variables
CC = /usr/local/cs/bin/gcc
CFALGS = -Wall -Wextra

# targets
default: lab1a

lab1a: lab1a.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f lab1a lab1a-605124511.tar.gz

dist: lab1a.c Makefile README
	tar -czf lab1a-605124511.tar.gz $^
