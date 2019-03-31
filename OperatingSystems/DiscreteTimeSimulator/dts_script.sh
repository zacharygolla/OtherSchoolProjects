#!/bin/bash
touch results.txt

echo Running FCFS
for ((i = 1; i < 31; i++)); do
   python3 main.py 1 $i 0.06 0
done

echo Running SRTF
for ((i = 1; i < 31; i++)); do
   python3 main.py 2 $i 0.06 0
done

echo Running HRRN
for ((i = 1; i < 31; i++)); do
   python3 main.py 3 $i 0.06 0
done

echo Running RR with low quantum
for ((i = 1; i < 31; i++)); do
   python3 main.py 4 $i 0.06 0.01
done

echo Running RR with high quantum
for ((i = 1; i < 31; i++)); do
   python3 main.py 4 $i 0.06 0.2
done
echo Simulation Complete
