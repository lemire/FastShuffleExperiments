
set style line 80 lt rgb "#000000"
set border 3 back linestyle 80
set xtics nomirror
set ytics nomirror



set style line 1 lt rgb "#A00000" lw 3 pt 1 ps 1
set style line 2 lt rgb "#00A000" lw 3 pt 5 ps 1
set style line 3 lt rgb "#5060D0" lw 3 pt 7 ps 1
set style line 4 lt rgb "#FF1493" lw 3 pt 9 ps 1
set style line 5 lt rgb "red" lw 3 pt 11 ps 1
set style line 6 lt rgb "#808000" lw 3 pt 13 ps 1
set style line 7 lt rgb "#00008B" lw 3 pt 15 ps 1
set style line 8 lt rgb "#800080" lw 3 pt 21 ps 1
set style line 9 lt rgb "black" lw 3 pt 63 ps 1
set style line 10 lt rgb "blue" lw 3 pt 28 ps 1
set style line 11 lt rgb "violet" lw 3 pt 44 ps 1



set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81
set term pdfcairo fontscale 0.75
#set term png fontscale 0.6
set xlabel "array size"
set ylabel "ns/element"



##################
# Figure 3 a
##################
set key top left nobox opaque
set format x "10^%T"
set logscale x 10
set xrange [1000:]
set out "shuffle32.pdf"

set yrange [0:45]

plot "results.txt" every 2::0 u 1:2 title "OpenBSD-like" with linespoints  ls 5,\
 ""  every 2::0  u 1:3 title "Java-like" with linespoints  ls 4,\
 ""  every 2::0  u 1:4 title "Our approach" with linespoints  ls 3, \
 ""  every 2::1  u 1:7 title "STL std::shuffle" with linespoints  ls 7


##################
# Figure 4.a
##################
set out "shuffle32precompopenbsd.pdf"


plot "results.txt" every 2::0 u 1:2 title "OpenBSD-like" with linespoints  ls 5,\
 ""  every 2::0  u 1:8 title "OpenBSD-like (buffered)" with linespoints  ls 9,\
  ""  every 2::0  u 1:4 title "Our approach" with linespoints  ls 3, \
  ""  every 2::0  u 1:10 title "Our approach (buffered)" with linespoints  ls 10


##################
# Figure 5.a
##################

set out "floatshuffle32.pdf"



plot "results.txt"   every 2::0  u 1:4 title "Our approach (unbiased)" with linespoints  ls 3,\
""   every 2::0  u 1:5 title "floating-point (biased)" with linespoints  ls 4


##################
# Figure 3.b
##################

set out "shuffle64.pdf"



plot "results.txt" every 2::1 u 1:2 title "OpenBSD-like" with linespoints  ls 5,\
 ""  every 2::1  u 1:3 title "Java-like" with linespoints  ls 4,\
 ""  every 2::1  u 1:4 title "Our approach" with linespoints  ls 3, \
 ""  every 2::1  u 1:7 title "STL std::shuffle" with linespoints  ls 7


##################
# Figure 4.b
##################
set out "shuffle64precompopenbsd.pdf"

plot "results.txt" every 2::1 u 1:2 title "OpenBSD-like" with linespoints  ls 5,\
 ""  every 2::1  u 1:8 title "OpenBSD-like (buffered)" with linespoints  ls 9,\
  ""  every 2::1  u 1:4 title "Our approach" with linespoints  ls 3, \
  ""  every 2::1  u 1:10 title "Our approach (buffered)" with linespoints  ls 10


##################
# Figure 5.b
##################

set out "floatshuffle64.pdf"



plot "results.txt"   every 2::1  u 1:4 title "Our approach (unbiased)" with linespoints  ls 3,\
""   every 2::1  u 1:5 title "floating-point (biased)" with linespoints  ls 4

set autoscale y


set yrange [0:]


set key left center nobox opaque



##################
# Figure 2
# Ratio of the timings of the OpenBSD-like approach and of our approach
##################


set ylabel "OpenBSD-like vs. Our approach ratio"




set out "shuffleratio.pdf"

plot "results.txt" every 2::1 u 1:($2/$4) title "64-bit" with linespoints  ls 5, \
"" every 2::0 u 1:($2/$4) title "32-bit" with linespoints  ls 6
