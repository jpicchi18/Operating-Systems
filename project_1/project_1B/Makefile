# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

# variables
CC = /usr/local/cs/bin/gcc # TO-DO: change later
CFALGS = -Wall -Wextra

# targets
default: lab1b-client lab1b-server

lab1b-client: lab1b-client.c
	$(CC) $(CFLAGS) -lz -o $@ $^

lab1b-server: lab1b-server.c
	$(CC) $(CFLAGS) -lz -o $@ $^

clean:
	rm -f lab1b-client lab1b-server lab1b-605124511.tar.gz

dist: lab1b-client.c lab1b-server.c Makefile README
	tar -czf lab1b-605124511.tar.gz $^
