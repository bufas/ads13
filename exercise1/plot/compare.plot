set terminal png

set xlabel "Elements"
set ylabel "time/comparisons"

set output "dist/bin-compare.png"
plot "data/bin-comp-ins.data" using 1:2 title "Insert" with lines , \
     "data/bin-comp-del.data" using 1:2 title "Delete" with lines , \
     "data/bin-comp-dec.data" using 1:2 title "Decrement" with lines

set output "dist/fib-compare.png"
plot "data/fib-comp-ins.data" using 1:2 title "Insert" with lines , \
     "data/fib-comp-del.data" using 1:2 title "Delete" with lines , \
     "data/fib-comp-dec.data" using 1:2 title "Decrement" with lines
