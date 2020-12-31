#! /bin/bash

# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

if [[ `uname -a` == *"armv7l"* ]] # does the architecture match the beaglebone?
then
	gcc -Wall -Wextra -lm -lmraa -o lab4c_tcp lab4c_tcp.c
	gcc -Wall -Wextra -lm -lmraa -lssl -lcrypto -o lab4c_tls lab4c_tls.c
else
	# TODO: change to /usr/local/cs/bin/gcc
	gcc -Wall -Wextra -lm -o lab4c_tcp -DDUMMY lab4c_tcp.c
	gcc -Wall -Wextra -lm -lssl -lcrypto -o lab4c_tls -DDUMMY lab4c_tls.c
fi