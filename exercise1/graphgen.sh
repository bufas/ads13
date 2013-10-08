#!/bin/bash
mkdir -p graphs
for n in 10 100 1000 10000 100000
do
    for d in 0 25 50 75 100
    do
        ./graphgen -n $(n) -d $(d) -f graphs/graph_n$(n)_d$(d).data
    done
done
