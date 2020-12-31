# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

#! /bin/bash


# TEST 1: successful I/O test with --input and --output
echo "test 1" >.test_input.txt
./lab0 --input=.test_input.txt --output=.test_output.txt
if [[ $? -ne 0 ]]; then
    echo -e "TEST 1 failed: bad return status\n" >>.test_status.txt
fi

if [[ ! -a .test_output.txt ]]; then
    echo "early termination: TEST 1 failed: failed to create output file"
    exit
fi

var1=$(diff .test_input.txt .test_output.txt)
if [[ -n "${var1}"  ]]; then
    echo -e "TEST 1 failed: incorrect data transfer\n" >>.test_status.txt
fi


# TEST 2: successful I/O test with stdin and stdout
echo "test 2" | ./lab0 1>.test_output.txt
if [[ $? -ne 0 ]]; then
    echo -e "TEST 2 failed: bad return code\n" >>.test_status.txt
fi

if [[ ! -a .test_output.txt || "$(cat .test_output.txt)" != "test 2" ]]; then
    echo -e "TEST 2 failed: failed to copy stdin to stdout\n" >> .test_status.txt
fi


# TEST 3: unrecognized arguments
./lab0 --input=.test_input.txt --invalid 2>.test_stderr 1>/dev/null
if [[ $? -ne 1 ]]; then
    echo -e "TEST 3 failed: bad return code\n" >>.test_status.txt
fi

if [[ ! -a .test_stderr ]]; then
    echo -e "TEST 3 failed: no usage statement\n" >>.test_status.txt
fi

rm -f .test_stderr


# TEST 4: invalid input file
./lab0 --input=doesnotexist --output=.test_output.txt 2>.test_stderr
if [[ $? -ne 2 ]]; then
    echo -e "TEST 4 failed: bad return code\n" >>.test_status.txt
fi

if [[ ! -a .test_stderr ]]; then
    echo -e "TEST 4 failed: no error report\n" >>.test_status.txt
fi

rm -f .test_stderr


# TEST 5: invalid output file
echo "test 5 input" >.test_input.txt
echo "test 5 output" >.test_output.txt
chmod 000 .test_output.txt
./lab0 --input=.test_input.txt --output=.test_output.txt 2>.test_stderr

if [[ $? -ne 3 ]]; then
    echo -e "TEST 5 failed: bad return code\n" >>.test_status.txt
fi

if [[ ! -a .test_stderr ]]; then
    echo -e "TEST 5 failed: no error report\n" >>.test_status.txt
fi

rm -f .test_output.txt .test_stderr


# TEST 6: taking a segfault
echo "test 6 input" >.test_input.txt
echo "test 6 output" >.test_output.txt
./lab0 --input=.test_input.txt --output=.test_output.txt --segfault 2>.test_stderr

if [[ ! -a .test_stderr || $? -ne 139 ]]; then
    echo -e "TEST 6 failed: --segfault failed\n" >>.test_status.txt
fi

if [[ "$(cat .test_output.txt)" == "$(cat .test_input.txt)" ]]; then
    echo -e "TEST 6 failed: copied stdin to stdout, despite --segfault being passed\n" \
	 >>.test_status.txt
fi

rm -f .test_stderr


# TEST 7: catching a segfault
echo "test 7 input" >.test_input.txt
echo "test 7 output" >.test_output.txt
./lab0 --input=.test_input.txt --output=.test_output.txt --segfault --catch \
       2>.test_stderr

if [[ $? -ne 4 ]]; then
    echo -e "TEST 7 failed: bad return code\n" >>.test_status.txt
fi

if [[ ! -a .test_stderr || "$(cat .test_stderr)" == "Segmentation fault" ]]; then
    echo -e "TEST 7 failed: --catch failed to catch seg fault" >>.test_status.txt
fi
rm -f .test_stderr


# report test results
if [[ ! -a .test_status.txt ]]; then
    echo "check: all tests passed!"
else
    cat .test_status.txt
fi

rm -f .test_input.txt .test_output.txt .test_status.txt
