package me.lemire.microbenchmarks.algorithms;

import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
public class Shuffle {

    private static void swap(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    private static int nextPositiveInteger(Random rnd) {
        //int r = rnd.nextInt();
        //return r >>> 1;
        return rnd.nextInt() & 0x7fffffff;
    }

    private static void shuffle_java(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, rnd.nextInt(i));
    }

    private static int ranged_random_go(int  range,  Random rnd) {
        int bits;
        int t = Integer.MAX_VALUE % range;
        do {
            bits = nextPositiveInteger(rnd);
        } while(bits <= t);
        return bits % range;
    }

    private static void shuffle_go(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_go(i,rnd));
    }



    private static int ranged_random_pcglike(int  range,  Random rnd) {
        int threshold = (Integer.MAX_VALUE - range + 1) % range;
        for (;;) {
            int u = nextPositiveInteger(rnd);
            if (u >= threshold)
                return u % range;
        }
    }

    private static void shuffle_pcglike(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_pcglike(i,rnd));
    }

    private static int ranged_random_javalike(int  range,  Random rnd) {
        int urkey = nextPositiveInteger(rnd);
        int candidate = urkey % range;
        while(urkey - candidate > Integer.MAX_VALUE - range + 1 ) {
            urkey = nextPositiveInteger(rnd);
            candidate = urkey % range;
        }
        return candidate;
    }

    private static void shuffle_javalike(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_javalike(i,rnd));
    }

    private static int ranged_random_divisionless(int  range,  Random rnd) {
        long multiresult = Integer.toUnsignedLong(rnd.nextInt()) * range;
        long leftover = multiresult & 0xFFFFFFFFL;
        if(leftover < range) {
            final long threshold = 0xFFFFFFFF % range;
            while (leftover <= threshold) {
                multiresult = Integer.toUnsignedLong(rnd.nextInt()) * range;
                leftover =  multiresult & 0xFFFFFFFFL;
            }
        }
        return (int) (multiresult >>> 32); // [0, range)
    }

    private static void shuffle_divisionless(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, ranged_random_divisionless(i,rnd));
    }
    private static void shuffle_divisionless_inline(int arr[], Random rnd) {
        int size = arr.length;
        // Shuffle array
        for (int i = size; i > 1; i--) {
            int z;
            {
                int range = i;
                long multiresult = Integer.toUnsignedLong(rnd.nextInt()) * range;
                long leftover = multiresult & 0xFFFFFFFFL;
                if(leftover < range) {
                    final long threshold = 0xFFFFFFFF % range;
                    while (leftover <= threshold) {
                        multiresult = Integer.toUnsignedLong(rnd.nextInt()) * range;
                        leftover =  multiresult & 0xFFFFFFFFL;
                    }
                }
                z =  (int) (multiresult >>> 32); // [0, range)
            }
            swap(arr, i - 1, z);
        }
    }


    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 1000;
        int[] array = new int[N];
        int[] pristine;
        public BenchmarkState() {
            for (int k = 0; k < N; ++k)
                array[k] = Shuffle.nextPositiveInteger(Shuffle.r);
            pristine = Arrays.copyOf(array, array.length);
            Arrays.sort(pristine);
        }
    }


    @TearDown
    public void check(BenchmarkState s) {
        Arrays.sort(s.array);
        boolean sane = Arrays.equals(s.array,s.pristine);
        if(! sane ) throw new RuntimeException("Bug?");
    }

    private static ThreadLocalRandom r = ThreadLocalRandom.current();

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
        shuffle_divisionless(s.array, r);
    }
    @Benchmark
    public void test_shuffle_divisionless_inline(BenchmarkState s) {
        shuffle_divisionless_inline(s.array, r);
    }



    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(Shuffle.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(5).forks(1).build();
        new Runner(opt).run();
    }

}


