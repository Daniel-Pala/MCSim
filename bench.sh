#!/bin/bash
N=100000000
echo "threads,estimate,error,time" > risultati.csv
for T in $(seq 1 16); do
    for run in 1 2 3 4 5; do
        ./pi $N $T 1 >> risultati.csv
    done
done
