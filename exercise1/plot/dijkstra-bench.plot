set terminal png

set xlabel "Density"
set ylabel "Time"

set output "dist/dijkstra-bench-1024.png"
plot "data/dijkstra-bench-1024.data" using 3:1 every 2::0 title "Fib heap" with lines , \
     "data/dijkstra-bench-1024.data" using 3:1 every 2::1 title "Bin heap" with lines


set output "dist/dijkstra-bench-2048.png"
plot "data/dijkstra-bench-2048.data" using 3:1 every 2::0 title "Fib heap" with lines , \
     "data/dijkstra-bench-2048.data" using 3:1 every 2::1 title "Bin heap" with lines


set output "dist/dijkstra-bench-4096.png"
plot "data/dijkstra-bench-4096.data" using 3:1 every 2::0 title "Fib heap" with lines , \
     "data/dijkstra-bench-4096.data" using 3:1 every 2::1 title "Bin heap" with lines


set output "dist/dijkstra-bench-8192.png"
plot "data/dijkstra-bench-8192.data" using 3:1 every 2::0 title "Fib heap" with lines , \
     "data/dijkstra-bench-8192.data" using 3:1 every 2::1 title "Bin heap" with lines


set output "dist/dijkstra-bench-16384.png"
plot "data/dijkstra-bench-16384.data" using 3:1 every 2::0 title "Fib heap" with lines , \
     "data/dijkstra-bench-16384.data" using 3:1 every 2::1 title "Bin heap" with lines
