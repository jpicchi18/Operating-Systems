#! /bin/bash

# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

if [[ `uname -a` == *"armv7l"* ]] # does the architecture match the beaglebone?
then
	gcc -Wall -Wextra -lm -lmraa -o lab4b lab4b.c
else
	/usr/local/cs/bin/gcc -Wall -Wextra -lm -o lab4b -DDUMMY lab4b.c
fi