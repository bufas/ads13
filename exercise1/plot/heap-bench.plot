set terminal png

set xlabel "Elements (million)"
set ylabel "Time (seconds)"

set output "dist/bench-fib.png"
plot "data/bench-fib.data" using ($3/1000000):($2/1000000) every 3::0 title "Insert" with lines , \
     "data/bench-fib.data" using ($3/1000000):($2/1000000) every 3::1 title "Delete" with lines , \
     "data/bench-fib.data" using ($3/1000000):($2/1000000) every 3::2 title "Decrease key" with lines

set output "dist/bench-bin.png"
plot "data/bench-bin.data" using ($3/1000000):($2/1000000) every 3::0 title "Insert" with lines , \
     "data/bench-bin.data" using ($3/1000000):($2/1000000) every 3::1 title "Delete" with lines , \
     "data/bench-bin.data" using ($3/1000000):($2/1000000) every 3::2 title "Decrease key" with lines
