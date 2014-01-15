for i in veb #bin fib veb
do
    echo "############################################# $i"
    for n in 10 50 100 500 1000 5000 10000
    do
        for d in 0 #10 25 50 75 100
        do
            echo "n = $n, d = $d%"
            for a in {1..10}
            do
                ./dijkstra -f /Users/andreas/graphs/n${n}_d${d}.graph -impl ${i}
            done
        done
    done
done
