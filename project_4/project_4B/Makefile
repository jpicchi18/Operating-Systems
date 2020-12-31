# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

CFLAGS = -Wall -Wextra

lab4b: lab4b.c
	chmod a+x build.sh
	./build.sh

check: lab4b
	chmod a+x check.sh
	-./check.sh

clean:
	rm -f lab4b lab4b-605124511.tar.gz

dist: lab4b.c Makefile README check.sh build.sh
	tar -czf lab4b-605124511.tar.gz $^