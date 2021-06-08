Sarah Mathes, Jakob Miner, Spencer Ross
CS 460, Assignment 3

Compile with 'make', creates an executable ./main.
Execute with command:
./main -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [file name]

This code implements a multi-threaded program that allows
us to measure the performance (i.e., Throughput, Turnaround time, and Waiting time in
Ready Queue) of the four basic CPU scheduling algorithms, namely, FCFS, SJF, Priority
(PR), and Round-Robin (RR). The program emulates/simulates the processes
whose priority, sequence of CPU burst time(ms) and I/O burst time(ms) will be given in an
input file.
We assume that all scheduling algorithms except RR will be non-preemptive, and all 
scheduling algorithms except PR will ignore process priorities (i.e., all processes have the same
priority in FCFS, SJF and RR). We also assume that there is only one IO device and all IO
requests will be served using that device in a FIFO manner.

input.txt can be used as -input [file name] and is an example of the input that the program expects.
Sample input file:
proc 1 7 10 20 10 50 20 40 10
proc 1 5 50 10 30 20 40
sleep 50
proc 2 3 20 50 20
proc 3 3 20 50 20
sleep 50
proc 2 3 20 50 20
stop
