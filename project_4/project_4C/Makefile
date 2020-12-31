# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

CFLAGS = -Wall -Wextra

default: lab4c_tcp.c lab4c_tls.c
	chmod a+x build.sh
	./build.sh

clean:
	rm -f lab4c_tls lab4c_tcp lab4c-605124511.tar.gz

dist: lab4c_tcp.c lab4c_tls.c Makefile README build.sh
	tar -czf lab4c-605124511.tar.gz $^