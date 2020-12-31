# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

CC = /usr/local/cs/bin/gcc
CFALGS = -Wall -Wextra

lab3a: lab3a.c ext2_fs.h
	$(CC) $(CFLAGS) -o $@ lab3a.c

dist:
	tar -czf lab3a-605124511.tar.gz lab3a.c ext2_fs.h Makefile README

clean:
	rm -f lab3a lab3a-605124511.tar.gz