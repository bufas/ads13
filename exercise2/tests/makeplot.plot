set terminal png

set xlabel "Nodes (#)"
set ylabel "Time (ms)"

set output "graph_dens0.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::0 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::0 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::0 title 'vEB' with lines

set output "graph_dens10.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::1 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::1 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::1 title 'vEB' with lines

set output "graph_dens25.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::2 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::2 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::2 title 'vEB' with lines

set output "graph_dens50.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::3 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::3 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::3 title 'vEB' with lines

set output "graph_dens75.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::4 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::4 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::4 title 'vEB' with lines

set output "graph_dens100.png"
plot    "bin_dijkstra_bench_processed.txt" using 1:3 every 6::5 title 'Bin' with lines, \
        "fib_dijkstra_bench_processed.txt" using 1:3 every 6::5 title 'Fib' with lines, \
        "veb_dijkstra_bench_processed.txt" using 1:3 every 6::5 title 'vEB' with lines

