// requires nightly rust
#![feature(test)]
#![feature(type_ascription)]
extern crate rand;
extern crate test;
extern crate pcg;


#[cfg(test)]
mod tests {
    use test::Bencher;
    use test::black_box;
    use rand::{Rng, StdRng, SeedableRng};
    use pcg::PcgRng;

    fn divisionlessrand(r: &mut StdRng, range: u32) -> u32 {
        let range64 = range as u64;
        let mut random32bit = r.gen::<u32>() as u64;
        let mut multiresult = random32bit * range64;
        let mut leftover = multiresult as u32;
        if leftover < range {
            let threshold = (!range + 1) % range;
            while leftover < threshold {
                random32bit = r.gen::<u32>() as u64;
                multiresult = random32bit * range64;
                leftover = multiresult as u32;
            }
        }
        return (multiresult >> 32) as u32;
    }

    fn divisionlesspcg(r: &mut PcgRng, range: u32) -> u32 {
        let range64 = range as u64;
        let mut random32bit = r.gen::<u32>() as u64;
        let mut multiresult = random32bit * range64;
        let mut leftover = multiresult as u32;
        if leftover < range {
            let threshold = (!range + 1) % range;
            while leftover < threshold {
                random32bit = r.gen::<u32>() as u64;
                multiresult = random32bit * range64;
                leftover = multiresult as u32;
            }
        }
        return (multiresult >> 32) as u32;
    }

    fn divisionlessshuffle<T>(r: &mut StdRng, values: &mut [T]) {
        let mut i = values.len();
        while i >= 2 {
            i -= 1;
            values.swap(i, divisionlessrand(r, (i + 1) as u32) as usize);
        }
    }

    fn pcgdivisionlessshuffle<T>(r: &mut PcgRng, values: &mut [T]) {
        let mut i = values.len();
        while i >= 2 {
            i -= 1;
            values.swap(i, divisionlesspcg(r, (i + 1) as u32) as usize);
        }
    }

    fn pcgshuffle<T>(r: &mut PcgRng, values: &mut [T]) {
        let mut i = values.len();
        while i >= 2 {
            i -= 1;
            values.swap(i, r.gen_range(0, i + 1) as usize);
        }
    }

    #[bench]
    fn shuffle_bench(b: &mut Bencher) {
        let mut array: [i32; 1000] = [0; 1000];
        for x in 0..1000 {
            array[x] = x as i32;
        }
        let mut rng = StdRng::new().unwrap();
        b.iter(|| {
            rng.shuffle(&mut array);
            black_box(array);
        });
    }

    #[bench]
    fn pcgshuffle_bench(b: &mut Bencher) {
        let mut array: [i32; 1000] = [0; 1000];
        for x in 0..1000 {
            array[x] = x as i32;
        }
        let mut rng = PcgRng::from_seed([100, 200]);
        b.iter(|| {
            pcgshuffle(&mut rng, &mut array);
            black_box(array);
        });
    }

    #[bench]
    fn divisionless_pcgshuffle_bench(b: &mut Bencher) {
        let mut array: [i32; 1000] = [0; 1000];
        for x in 0..1000 {
            array[x] = x as i32;
        }
        let mut rng = PcgRng::from_seed([100, 200]);
        b.iter(|| {
            pcgdivisionlessshuffle(&mut rng, &mut array);
            black_box(array);
        });
    }
    #[bench]
    fn divisionless_shuffle_bench(b: &mut Bencher) {
        let mut array: [i32; 1000] = [0; 1000];
        for x in 0..1000 {
            array[x] = x as i32;
        }
        let mut rng = StdRng::new().unwrap();
        b.iter(|| {
            divisionlessshuffle(&mut rng, &mut array);
            black_box(array);
        });
    }
}
