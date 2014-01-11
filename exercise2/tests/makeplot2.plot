set terminal png

set xlabel "# of nodes"
set ylabel "measured time / theoretical time"

unset ytics

#
#
# Complexity benchmarks
#
#

set output "bench_insert.png"
plot    "rb_insert_bench.txt" using 1:($2/log($1)) title 'RB tree' with lines, \
        "veb_insert_bench.txt" using 1:($2/log(log($1))) title 'vEB tree' with lines

set output "bench_predecessor.png"
plot    "rb_pred_bench.txt" using 1:(($2/log($1))/1000) title 'RB tree' with lines, \
        "veb_pred_bench.txt" using 1:($2/log(log($1))) title 'vEB tree' with lines

set output "bench_delete.png"
plot    "rb_delete_bench.txt" using 1:($2/log($1)) title 'RB tree' with lines, \
        "veb_delete_bench.txt" using 1:($2/log(log($1))) title 'vEB tree' with lines


#
#
# Bigger complexity benchmark for rb_pred
#
#

set output "bench_predecessor_big_1000.png"
plot    "rb_pred_bench_big.txt" using 1:($2/log($1)) title 'RB tree' with lines


#
#
# Speed benchmarks
#
#

set ylabel "time"
set logscale x

set output "bench_speed_insert.png"
plot    "rb_insert_bench.txt" using 1:2 title 'RB tree' with lines, \
        "veb_insert_bench.txt" using 1:2 title 'vEB tree' with lines

set output "bench_speed_predecessor.png"
plot    "rb_pred_bench.txt" using 1:($2/1000) title 'RB tree' with lines, \
        "veb_pred_bench.txt" using 1:2 title 'vEB tree' with lines

set output "bench_speed_delete.png"
plot    "rb_delete_bench.txt" using 1:2 title 'RB tree' with lines, \
        "veb_delete_bench.txt" using 1:2 title 'vEB tree' with lines
