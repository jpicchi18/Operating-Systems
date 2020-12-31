# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

#! /usr/bin/gnuplot

# general plot parameters
set terminal png
set datafile separator ","

# FILE 1: lab2_1.png
# total number of operations per second for each synchronization method
set title "lab2b-1: Aggregate throughput for each synchronization mechanism"
set xlabel "threads"
set logscale x 2
set ylabel "Throughput (#operations/second)"
set logscale y 10
set output 'lab2b_1.png'
set key left top

# grep out desired lines
plot \
     "< grep -e 'list-none-m,[1-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/ mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[1-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/ spin-lock' with linespoints lc rgb 'green'


# FILE 2: lab2b_2.png
# wait-for-lock time and average time per operation vs # competing threads
set title "lab2b-2: wait-for-lock time and avg. time per operation for mutex"
set xlabel "threads"
set logscale x 2
set ylabel "Time (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top

# grep out desired lines
plot \
     "< grep -e 'list-none-m,[1-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'wait-for-lock time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[1-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'average time per operation' with linespoints lc rgb 'green'


# FILE 3: lab2b_3.png
set title "lab2b-3: Threads and Iterations that run without failure with 4 sublists"
set xlabel "Threads"
set logscale x 2
set ylabel "Iterations per thread"
set logscale y 10
set output 'lab2b_3.png'
# grep out only successful (sum=0) yield runs
plot \
     "< grep list-id-s lab2b_list.csv" using ($2):($3) \
	title 'spin w/ id yields' with points lc rgb 'green', \
     "< grep list-id-m lab2b_list.csv" using ($2):($3) \
	title 'mutex w/ id yields' with points lc rgb 'blue', \
     "< grep list-id-none lab2b_list.csv" using ($2):($3) \
	title 'unprotected w/ id yields' with points lc rgb 'red', \


# FILE 4: lab2b_4.png
# total number of operations per second for each synchronization method
set title "Aggregate throughput for mutex synchronization with varying sublists"
set xlabel "threads"
set logscale x 2
set ylabel "Throughput (#operations/s)"
set logscale y 10
set output 'lab2b_4.png'
set key left top

# grep out desired lines
plot \
     "< grep -E 'list-none-m,[1-9]?,1000,1,|list-none-m,12,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[1-9]*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=4' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-m,[1-9]*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=8' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[1-9]*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=16' with linespoints lc rgb 'orange', \


# FILE 5: lab2b_5.png
# total number of operations per second for each synchronization method
set title "Aggregate throughput for spinlock synchronization with varying sublists"
set xlabel "threads"
set logscale x 2
set ylabel "Throughput (#operations/s)"
set logscale y 10
set output 'lab2b_5.png'
set key left top

# grep out desired lines
plot \
     "< grep -E 'list-none-s,[1-9]?,1000,1,|list-none-m,12,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[1-9]*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=4' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[1-9]*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=8' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[1-9]*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '--lists=16' with linespoints lc rgb 'orange', \
     