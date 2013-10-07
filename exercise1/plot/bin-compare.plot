set terminal png

set xlabel "Elements"
set ylabel "time/comparisons"

plot "data/bin-comp-ins.data" using 1:2 title "Insert" with lines , \
     "data/bin-comp-del.data" using 1:2 title "Delete" with lines , \
     "data/bin-comp-dec.data" using 1:2 title "Decrement" with lines
