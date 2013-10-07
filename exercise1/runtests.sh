#!/bin/bash

echo "========= Starting tests ========="
echo "The output is fib bin fib bin fib bin ..."

for n in $(seq 1024 1024 16384)
do
    for d in $(seq 0 10 100)
    do
        echo "-n $n -d $d"
        for i in 1 2 3
        do
            s=$(($n*$d+$i))
            ./dijkstra-fib -n $n -d $d% -s $s
            ./dijkstra-bin -n $n -d $d% -s $s
        done
    done
done

echo "========= Tests completed ========="
