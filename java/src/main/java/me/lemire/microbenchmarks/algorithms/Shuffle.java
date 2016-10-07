package me.lemire.microbenchmarks.algorithms;

import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.BenchmarkMode;
import org.openjdk.jmh.annotations.Mode;
import org.openjdk.jmh.annotations.OutputTimeUnit;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

@State(Scope.Thread)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
public class Shuffle {

    private static void swap(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    private static int nextPositiveInteger(ThreadLocalRandom rnd) {
        int r = rnd.nextInt();
        return r >>> 1;
    }

    public static void shuffle_java(int arr[], ThreadLocalRandom rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, rnd.nextInt(i));
    }

    static int ranged_random_go(int  range,  ThreadLocalRandom rnd) {
      int bits;
      int t = Integer.MAX_VALUE % range;
      do {
          bits = nextPositiveInteger(rnd);
      } while(bits <= t);
      return bits % range;
    }

    public static void shuffle_go(int arr[], ThreadLocalRandom rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_go(i,rnd));
    }



    static int ranged_random_pcglike(int  range,  ThreadLocalRandom rnd) {
      int threshold = (Integer.MAX_VALUE - range + 1) % range;
      for (;;) {
        int u = nextPositiveInteger(rnd);
        if (u >= threshold)
            return u % range;
      }
    }
    public static void shuffle_pcglike(int arr[], ThreadLocalRandom rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_pcglike(i,rnd));
    }

    static int ranged_random_javalike(int  range,  ThreadLocalRandom rnd) {
        int urkey = nextPositiveInteger(rnd);
        int candidate = urkey % range;
        while(urkey - candidate > Integer.MAX_VALUE - range + 1 ) {
           urkey = nextPositiveInteger(rnd);
           candidate = urkey % range;
        }
        return candidate;
    }

    public static void shuffle_javalike(int arr[], ThreadLocalRandom rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_javalike(i,rnd));
    }

    static int ranged_random_divisionless(int  range,  ThreadLocalRandom rnd) {
        long random32bit, multiresult;
        long leftover;
        if((range & (range - 1)) == 0)
            return rnd.nextInt() & (range - 1);
        final long mask = 0xFFFFFFFFL;
        random32bit = rnd.nextInt()  & mask;
        multiresult = random32bit * range;
        leftover = multiresult & mask;
        if(leftover < range) {
          final long threshold = 0xFFFFFFFF % range;
          while (leftover <= threshold) {
              random32bit = rnd.nextInt() & mask;
              multiresult = random32bit * range;
              leftover =  multiresult & mask;
          }
        }
        return (int) (multiresult >>> 32); // [0, range)
    }

    public static void shuffle_divisionless(int arr[], ThreadLocalRandom rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_divisionless(i,rnd));
    }

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 10000;
        int[] array = new int[N];

        public BenchmarkState() {
            for (int k = 0; k < N; ++k)
                array[k] = k;
        }

    }

    static ThreadLocalRandom r = ThreadLocalRandom.current();


    @Benchmark
    public void test_shuffle_java(BenchmarkState s) {
        shuffle_java(s.array, r);
    }

    @Benchmark
    public void test_shuffle_go(BenchmarkState s) {
        shuffle_pcglike(s.array, r);
    }

    @Benchmark
    public void test_shuffle_pcglike(BenchmarkState s) {
        shuffle_pcglike(s.array, r);
    }

    @Benchmark
    public void test_shuffle_javalike(BenchmarkState s) {
        shuffle_javalike(s.array, r);
    }

    @Benchmark
    public void test_shuffle_divisionless(BenchmarkState s) {
        shuffle_shuffle_divisionless(s.array, r);
    }


    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(Shuffle.class.getSimpleName()).warmupIterations(2)
        .measurementIterations(3).forks(1).build();

        new Runner(opt).run();
    }

}
