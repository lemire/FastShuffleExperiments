package shuffle

import "testing"
import (
	"github.com/dgryski/go-pcgr"
)

func makeRandomArray(r int, pcg *pcgr.Rand) []int {
	array := make([]int, r, r)
	for i := 0; i < r; i++ {
		array[i] = int(pcg.Next() & 0x7FFFFFFF)
	}
	return array
}

func benchmark_pcg(b *testing.B, r int) {
	b.StopTimer()
	p := pcgr.Rand{0x0ddc0ffeebadf00d, 0xcafebabe}
	array := makeRandomArray(r, &p)
	b.StartTimer()
	for j := 0; j < b.N; j++ {
		shuffle_pcg(array, &p)
	}
}

func BenchmarkPcg1000(b *testing.B) {
	benchmark_pcg(b, 1000)
}

func benchmark_divisionless(b *testing.B, r int) {
	b.StopTimer()
	p := pcgr.Rand{0x0ddc0ffeebadf00d, 0xcafebabe}
	array := makeRandomArray(r, &p)
	b.StartTimer()
	for j := 0; j < b.N; j++ {
		shuffle_divisionless(array, &p)
	}
}

func BenchmarkDivisionless1000(b *testing.B) {
	benchmark_divisionless(b, 1000)
}

func benchmark_go(b *testing.B, r int) {
	b.StopTimer()
	p := pcgr.Rand{0x0ddc0ffeebadf00d, 0xcafebabe}
	array := makeRandomArray(r, &p)
	b.StartTimer()
	for j := 0; j < b.N; j++ {
		shuffle_go(array, &p)
	}
}

func BenchmarkGo1000(b *testing.B) {
	benchmark_go(b, 1000)
}

func benchmark_java(b *testing.B, r int) {
	b.StopTimer()
	p := pcgr.Rand{0x0ddc0ffeebadf00d, 0xcafebabe}
	array := makeRandomArray(r, &p)
	b.StartTimer()
	for j := 0; j < b.N; j++ {
		shuffle_java(array, &p)
	}
}

func BenchmarkJava1000(b *testing.B) {
	benchmark_java(b, 1000)
}
