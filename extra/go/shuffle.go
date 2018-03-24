package shuffle

import "math"
import (
	"github.com/dgryski/go-pcgr"
)

// return a pseudo-random number in [0,r), using the go-like approach, uses the dgryski' PCG for random bits
func Random_bounded_go(r uint32, pcg *pcgr.Rand) uint32 {
	max := uint32(0xFFFFFFFF) % r
	v := pcg.Next()
	for v <= max {
		v = pcg.Next()
	}
	return v % r
}

// return a pseudo-random number in [0,r), using the java-like approach, uses the dgryski' PCG for random bits
func Random_bounded_java(r uint32, pcg *pcgr.Rand) uint32 {
	v := pcg.Next()
	candidate := v % r
	for v-candidate > math.MaxUint32-r+uint32(1) {
		v = pcg.Next()
		candidate = v % r
	}
	return candidate
}

// return a pseudo-random number in [0,r), avoiding divisions, uses the dgryski' PCG for random bits
func Random_bounded_divisionless(r uint32, pcg *pcgr.Rand) uint32 {
	random32bit := pcg.Next()
	multiresult := uint64(random32bit) * uint64(r)
	leftover := uint32(multiresult)
	if leftover < r {
		threshold := uint32(-r) % r
		for leftover < threshold {
			random32bit = pcg.Next()
			multiresult = uint64(random32bit) * uint64(r)
			leftover = uint32(multiresult)
		}
	}
	return uint32(multiresult >> 32) // [0, r)
}

func shuffle_divisionless(array []int, p *pcgr.Rand) {
	r := len(array)
	for i := r; i > 0; i-- {
		//		idx := Random_bounded_divisionless(uint32(i), p)
		random32bit := p.Next()
		multiresult := uint64(random32bit) * uint64(i)
		leftover := uint32(multiresult)
		if leftover < uint32(i) {
			threshold := uint32(-i) % uint32(i)
			for leftover < threshold {
				random32bit = p.Next()
				multiresult = uint64(random32bit) * uint64(i)
				leftover = uint32(multiresult)
			}
		}
		idx := uint32(multiresult >> 32) // [0, r)
		array[idx], array[i-1] = array[i-1], array[idx]
	}
}

func shuffle_java(array []int, p *pcgr.Rand) {
	r := len(array)
	for i := r; i > 0; i-- {
		//idx := Random_bounded_java(uint32(i), p)
		v := p.Next()
		candidate := v % uint32(i)
		for v-candidate > math.MaxUint32-uint32(i)+uint32(1) {
			v = p.Next()
			candidate = v % uint32(i)
		}
		array[candidate], array[i-1] = array[i-1], array[candidate]
	}
}

func shuffle_go(array []int, p *pcgr.Rand) {
	r := len(array)
	for i := r; i > 0; i-- {
		idx := Random_bounded_go(uint32(i), p)
                // inlining does not help?
		/*	max := uint32(0xFFFFFFFF) % uint32(i)
			v := p.Next()
			for v <= max {
				v = p.Next()
			}
			idx := v % uint32(i)
		*/
		array[idx], array[i-1] = array[i-1], array[idx]
	}
}

func shuffle_pcg(array []int, p *pcgr.Rand) {
	r := len(array)
	for i := r; i > 0; i-- {
		idx := p.Bound(uint32(i))
		array[idx], array[i-1] = array[i-1], array[idx]
	}
}
