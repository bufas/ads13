set terminal png

set xlabel "Density"
set ylabel "Elements"
set zlabel "Time"

set xrange [0:100]
set yrange [1000:17000]
set zrange [5000:4000000]

set dgrid3d 50,10
set hidden3d

set output "dist/dijkstra-bench-3d.png"
splot "data/dijkstra-bench.data" using 3:2:1 every 2::0 title "Fib heap" with lines , \
      "data/dijkstra-bench.data" using 3:2:1 every 2::1 title "Bin heap" with lines
