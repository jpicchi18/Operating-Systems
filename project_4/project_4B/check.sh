#!/bin/bash

# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu


# counter for number of errors
let e=0

# test bad option
echo "bad option test... "
{ sleep 1; echo "OFF"; } | ./lab4b --dummy=4 &>/dev/null
if [ $? -ne 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test a bad scale arg
echo "testing a bad argument to '--scale'... "
{ sleep 1; echo "OFF"; } | ./lab4b --scale=A &>/dev/null
if [ $? -ne 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# create some successful cases
{ sleep 1; echo "SCALE=C"; sleep 3; echo "SCALE=F"; sleep 3; echo "PERIOD=1"; sleep 3; \
echo "STOP"; sleep 3; echo "START"; echo "LOG message"; sleep 2; echo "OFF"; } \
| ./lab4b --scale=F --period=2 --log=.testlog &>/dev/null

# test good return code on clean exit
echo "testing return code on clean exit... "
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test --log
echo "testing functionality of '--log=logfile'... "
if [ -s .testlog ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test --SCALE=C
echo "testing functionality of stdin command 'SCALE=C'... "
cat .testlog | grep "SCALE=C" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test --SCALE=F
echo "testing functionality of stdin command 'SCALE=F'... "
cat .testlog | grep "SCALE=F" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test --PERIOD=1
echo "testing functionality of stdin command 'PERIOD=num'... "
cat .testlog | grep "PERIOD=1" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test STOP
echo "testing functionality of stdin command 'STOP'... "
cat .testlog | grep "STOP" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test STOP
echo "testing functionality of stdin command 'START'... "
cat .testlog | grep "START" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test LOG
echo "testing functionality of stdin command 'LOG=message'... "
cat .testlog | grep "LOG message" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test OFF
echo "testing receipt of stdin command 'OFF'... "
cat .testlog | grep "OFF" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test OFF again
echo "testing functionality of stdin command 'OFF'... "
cat .testlog | grep -E "^*:*:* SHUTDOWN$" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

# test time and temp log
echo "testing for presence of temperature logs... "
cat .testlog | grep -E "^*:*:* *.*" &>/dev/null
if [ $? -eq 0 ]
then
    echo "PASSED"
else
    echo "FAILED"
    let e=$e+1
fi

if [[ $e -eq 0 ]]
then
    echo "lab4b PASSED all smoke tests"
else
    echo "lab4b FAILED one or more smoke tests"
fi

# clean up
rm -f .testlog

exit 0