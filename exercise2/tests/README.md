Creating plots for dijkstra benchmarks
======================================

Running `python dijkstra_bench_format.py` will produce the following files:

- dens0.data
- dens10.data
- dens25.data
- dens50.data
- dens75.data
- dens100.data

These files are used by makeplot.plot to generate the graphs. Running `gnuplot makeplot.plot` will produce the graphs as png files.

Creating plots for vEB vs. RB benchmarks
========================================

Just run the makeplot2.plot file, using `gnuplot makeplot2.plot`.