# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

CC = /usr/local/cs/bin/gcc
CFALGS = -Wall -Wextra


default: lab0

lab0: lab0.c
	$(CC) $(CFLAGS) -o $@ $^

check: lab0
	./check.sh 2>/dev/null

clean:
	rm -f lab0 lab0-605124511.tar.gz

dist: lab0.c backtrace.png breakpoint.png README check.sh Makefile
	tar -czf lab0-605124511.tar.gz $^
