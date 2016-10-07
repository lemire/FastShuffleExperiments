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
