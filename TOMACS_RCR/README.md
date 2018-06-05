# Reproducibility of Computational Results (RCR) for "Fast Random Integer Generation in an Interval" (by D. Lemire)


## Pre-requisites

- A bash shell.
- A recent C++ compiler (e.g., GNU G++ or clang++) with the conventional Unix-like toolchain (including make).
- Gnuplot version 5.2 (to generate the figures).

## Quick version

Type

```
make plot
```

This should do everything, including generating the final plots in PDF.

The plots (as PDF) are generated in the same directory as this ``README.md`` file.

## Generation of the data

The generation of the figures and data should be executed on a Linux machine configured for
testing. It is  recommended to turn off TurboBoost or the equivalent, to disable
hyperthreading (since our tests are single threaded) and to set the power policy on
performance (or the equivalent). It is ill-advised to run benchmarks on a regular laptop.

The benchmark verifies that the average of X runs is equal to the mean of X runs within one percent.
When that it is not the case, a warning is issued. It is normal to get such warning for tiny arrays (containing
10 elements), but if you get such warnings for more sizeable arrays, then your results are likely flawed.


Type:

```
make benchmark
```

Assuming that you meet the prerequisites, this will build a ``shuffle`` executable and then run it, outputting the result to a file called ``results.txt``. This file contains several columns. You do not need to manually process these columns.

The command will fail if you do not have a recent C++ compiler available.

The benchmark will take several minutes to run. Be patient. Each test is run at least 5 times (sometimes more). Once it is completed, the ``results.txt`` will be found in the same directory as this ``README.md`` file. It should look like this (the exact numbers will vary):

```
# We interleave 32-bit / 64-bit results.
# The array size comes first, then we present columns of timings.
# First column uses an OpenBSD-like approach with two divisions per ranged random number.
# Second column uses a Java-like approach with one or more divisions per ranged random number.
# Third column uses an approach with nearly no division per ranged random number.
# Fourth column uses an approach based on floating-point numbers.
# Fifth column uses precomputed random indexes.
# Next column relates to std::shuffle (the standard library).
# Next column uses an OpenBSD-like approach with two divisions per ranged random number with the numbers written to a buffer.
# Next column uses a Java-like approach with one or more divisions per ranged random number with the numbers written to a buffer.
# Next column uses an approach with nearly no division per ranged random number with the numbers written to a buffer.
# Next column uses an approach based on floating-point numbers with the numbers written to a buffer.
# Time reported in number of ns per array element in a random shuffle.
5   9.31   7.74   6.07   9.10   4.85   20.79        9.62   8.40   6.66   8.97
5   15.85   10.71   6.01   8.32   4.80   17.54        17.38   11.56   6.24   8.88
10   7.27   5.41   3.92   7.20   2.83   19.39        7.49   5.89   4.27   6.54
10   15.19   9.40   3.83   6.13   2.92   16.45        16.39   9.86   4.03   6.35
50   5.75   3.64   2.26   5.76   1.42   18.74        5.81   4.06   2.46   5.02
50   14.42   8.31   2.26   4.72   1.47   15.45        15.50   8.70   2.34   4.84
100   5.59   3.48   2.03   2.61   1.19   18.66        5.62   3.81   2.21   2.66
100   14.37   8.11   2.04   2.49   1.13   15.32        15.37   8.59   2.08   2.37
500   5.47   3.16   1.98   2.57   0.94   18.55        5.95   4.00   2.37   3.11
500   14.41   7.98   1.93   2.41   0.93   15.25        15.95   8.99   2.41   2.89
1000   5.40   3.08   1.88   2.51   0.94   18.66        6.06   3.84   2.40   2.93
1000   14.36   7.96   1.86   2.39   0.92   15.28        16.03   9.43   2.47   2.87
5000   5.33   2.99   1.83   2.41   0.90   18.57        6.15   3.81   2.43   2.91
5000   14.36   7.96   1.79   2.28   0.89   15.24        16.09   9.78   2.51   2.91
10000   5.32   2.98   1.80   2.39   0.89   18.57        6.18   3.81   2.45   2.93
10000   14.40   7.97   1.78   2.26   0.89   15.23        16.08   9.86   2.56   2.94
50000   5.31   3.04   1.86   2.47   1.31   18.57        6.24   3.87   2.49   2.98
50000   14.55   7.98   1.87   2.45   1.02   15.23        16.16   9.91   2.64   3.03
100000   5.32   3.35   2.11   2.82   1.17   18.59        6.41   4.03   2.63   3.15
100000   14.43   7.98   2.02   2.72   1.15   15.23        16.30   9.98   2.77   3.16
500000   5.45   3.93   2.54   3.37   1.95   18.79        6.74   4.36   2.94   3.49
500000   14.31   7.98   2.45   3.32   1.56   15.26        16.64   10.10   3.11   3.52
1000000   5.64   4.17   2.73   3.59   2.00   18.91        6.94   4.56   3.13   3.68
1000000   14.44   8.08   2.63   3.53   1.92   15.33        16.83   10.18   3.29   3.69
5000000   11.02   8.45   6.30   7.52   4.52   27.53        9.04   6.64   5.37   5.87
5000000   21.20   13.98   6.14   7.59   4.44   23.59        19.03   12.35   5.58   5.95
10000000   14.38   11.03   8.40   9.86   5.86   34.71        10.44   8.04   6.64   7.16
10000000   25.31   17.12   7.82   9.55   5.81   29.62        20.35   13.66   6.74   7.17
50000000   17.79   13.67   10.60   12.25   7.89   43.04        12.54   10.19   8.82   9.27
50000000   30.28   20.78   10.04   12.08   7.89   36.90        22.37   15.60   8.85   9.28
100000000   18.44   14.19   11.06   12.71   8.29   44.66        13.00   10.68   9.35   9.70
100000000   31.26   21.46   10.45   12.54   8.29   38.29        22.70   16.04   9.28   9.72
500000000   19.14   14.91   11.83   13.16   8.71   46.42        13.76   11.56   10.53   10.14
500000000   32.26   22.12   10.86   12.99   8.81   39.64        23.11   16.50   9.73   10.16
1000000000   19.42   15.36   12.43   13.26   8.96   46.97        14.32   12.32   11.69   10.29
1000000000   32.48   22.28   10.97   13.10   9.00   39.90        23.30   16.66   9.89   10.32
```

## Generation of the figures (PDF files)

To generate the figures, simply type:

```
make plot
```

(If you have run ``make benchmark`` previously, the ``results.txt`` has been generated, and the plots are produced quickly from this
file. Otherwise, the file ``results.txt`` will be generated by running an expensive benchmark.)

Make sure you have gnuplot installed, otherwise this will fail. On macOS, you can install gnuplot with ``brew install gnuplot --with-cairo`` (after installing brew itself from https://brew.sh. Under Linux, installing gnuplot could be as simple as typing ``sudo apt-get install gnuplot``.

All figures are generated by the gnuplot script called ``shuffle.gnuplot`` found in the
subdirectory ``gnuplot``. This script presumes that you have generate the ``results.txt``
file by running the benchmark.

A total of seven PDF files are produced:
*  Figure 2 (Ratio of the timings of the OpenBSD-like approach and of our approach) is given by file ``shuffleratio.pdf``.
*  Figure 3 (Wall-clock time in nanoseconds per element to shuffle arrays of random integers) is made of two subfigures given by file ``shuffle32.pdf`` (32-bit indexes)  and file ``shuffle64.pdf`` (64-bit indexes).
* Figure 4 (Wall-clock time in nanoseconds per element to shuffle arrays of random integers using either regular shuffles or buffered shuffles with a buffer of size 256) is made of two subfigures given by file ``shuffle32precompopenbsd.pdf`` (32-bit indexes)  and file ``shuffle64precompopenbsd.pdf`` (64-bit indexes).
* Figure 5 (Wall-clock time in nanoseconds per element to shuffle arrays of random integers using either ...) contains two subfigures given by file ``floatshuffle32.pdf`` (32-bit indexes) and file ``floatshuffle64.pdf`` (64-bit indexes).

All the PDF files will appear in the same directory as the ``README.md`` file.
