/*
	STB-Like Random Utils
	Author: Krivenko Slava (NeoWise69/thekrovee)
	
	v 0.01 04.02.2022 - Initial stand-up of interface functions. Added fast pseudo-random number generator support. WIP.
*/
#ifndef _STB_RANDOM_
#define _STB_RANDOM_

/*
	Fastest (here) random-number-generator,
	Based on 'Mersenne Twister' algorithm.
*/
#define RANDOM_LEVEL_FAST (1 << 1)
/*
	Not so fast random-number-generator,
	but still based on algorithm from fast one version.
	The only differences is this one have a more
	depth iterations (unrolls).
*/
#define RANDOM_LEVEL_MEDIUM (1 << 2)
/*
	NOT IMPLEMENTED.
*/
#define RANDOM_LEVEL_COMPLEX (1 << 3)
/*
	NOT IMPLEMENTED.
*/
#define RANDOM_LEVEL_HARD (1 << 4)
/*
	NOT IMPLEMENTED.
*/
#define RANDOM_LEVEL_MADNESS (1 << 5)
/*
	Default level of randomness is 'fast'
*/
#define RANDOM_LEVEL_DEFAULT RANDOM_LEVEL_FAST

/* Interface for initializing random engine with seed. */
void seed(long val);

/* Main interface on that random engine. Supports different random-engine algorithms. */
struct srandom {
	/* Main interface on that random engine. Supports different random-engine algorithms. */
	srandom(int level = RANDOM_LEVEL_DEFAULT);
	/* Limits minimal generated number. */
	srandom& min(unsigned long val);
	/* Limits maximum generated number. */
	srandom& max(unsigned long val);
	/* Limits generated number between these two numbers. */
	srandom& range(unsigned long minval, unsigned long maxval);

	operator unsigned long() const;

private:
	unsigned long value;
};

#ifdef STB_RANDOM_IMPLEMENTATION

namespace _details {

	static unsigned long random_fast();
	static unsigned long random_medium();
	static unsigned long random_complex();
	static unsigned long random_hard();
	static unsigned long random_max();

	namespace prng {

		static void fast_u32();
		static void fast_u32_plus();
		static void fast_seed(long seed);

	}

}

inline void seed(long val) {
	_details::prng::fast_seed(val);
}

srandom::srandom(int level) {
	switch (level) {
		case RANDOM_LEVEL_FAST:     value = _details::random_fast(); break;
		case RANDOM_LEVEL_MEDIUM:   value = _details::random_medium(); break;
		case RANDOM_LEVEL_COMPLEX:  value = _details::random_complex(); break;
		case RANDOM_LEVEL_HARD:     value = _details::random_hard(); break;
		case RANDOM_LEVEL_MADNESS:  value = _details::random_max(); break;
		default:
			                          value = _details::random_fast(); break;
	}
}

inline srandom& srandom::min(unsigned long val) {
	value += value < val ? val : 0;
	return *this;
}

inline srandom& srandom::max(unsigned long val) {
	value = value > val ? (value % val) : value;
	return *this;
}

inline srandom& srandom::range(unsigned long minval, unsigned long maxval) {
	value = value % maxval;
	value += (value < minval) ? minval : 0;

	return *this;
}

inline srandom::operator unsigned long() const {
	return (unsigned long)(this->value);
}

// +-+-+-+-+-+- //

namespace _details {

	namespace prng {

		static const size_t SIZE = 624;
		static const size_t PERIOD = 397;
		static const size_t DIFF = SIZE - PERIOD;
		static const unsigned int MAGIC = 0x9908b0df;
		static long last_seed = 5189;

		typedef struct MTState {
			unsigned int MT[SIZE];
			unsigned int MT_TEMPERED[SIZE];
			size_t index = SIZE;
		} MTState;
		static MTState state;

	}

	unsigned long random_fast() {
		if (prng::state.MT[0] == 0) return -1;

		if (prng::state.index == prng::SIZE) {
			prng::fast_u32();
			prng::state.index = 0;
		}

		return prng::state.MT_TEMPERED[prng::state.index++];
	}

	unsigned long random_medium() {
		if (prng::state.MT[0] == 0) return -1;

		if (prng::state.index == prng::SIZE) {
			prng::fast_u32_plus();
			prng::state.index = 0;
		}

		return prng::state.MT_TEMPERED[prng::state.index++];
	}

#define M32(x) (0x80000000 & x)
#define L31(x) (0x7FFFFFFF & x)

#define prngUNROLL(expr) \
	y = M32(state.MT[i]) | L31(state.MT[i + 1]); \
	state.MT[i] = state.MT[expr] ^ (y >> 1) ^ (((int(y) << 31) >> 31) & MAGIC); \
	++i;

	namespace prng {

		inline void fast_u32() {
			size_t i = 0;
			unsigned int y;

			while (i < DIFF) {
				prngUNROLL(i + PERIOD)
			}
			while (i < SIZE - 1) {
				prngUNROLL(i - DIFF)
			}
			{
				y = M32(state.MT[SIZE - 1]) | L31(state.MT[0]);
				state.MT[SIZE - 1] = state.MT[PERIOD - 1] ^ (y >> 1) ^ (((int(y) << 31) >> 31) & MAGIC);
			}
			for (size_t i = 0; i < SIZE; ++i) {
				y = state.MT[i];
				y ^= y >> 11;
				y ^= y << 7 & 0x9d2c5680;
				y ^= y << 15 & 0xefc60000;
				y ^= y >> 18;
				state.MT_TEMPERED[i] = y;
			}
			state.index = 0;
		}

		inline void fast_u32_plus() {
			size_t i = 0;
			unsigned int y;

			while (i < DIFF) {
        prngUNROLL(i + PERIOD)
        prngUNROLL(i + PERIOD)
			}
			while (i < SIZE - 1) {
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
				prngUNROLL(i - DIFF)
			}
			{
				y = M32(state.MT[SIZE - 1]) | L31(state.MT[0]);
				state.MT[SIZE - 1] = state.MT[PERIOD - 1] ^ (y >> 1) ^ (((int(y) << 31) >> 31) & MAGIC);
			}
			for (size_t i = 0; i < SIZE; ++i) {
				y = state.MT[i];
				y ^= y >> 11;
				y ^= y << 7 & 0x9d2c5680;
				y ^= y << 15 & 0xefc60000;
				y ^= y >> 18;
				state.MT_TEMPERED[i] = y;
			}
			state.index = 0;
		}

		inline void fast_seed(long seed) {
			if (seed == 5189 || seed == last_seed)
				state.MT[0] = last_seed++;
			else
				state.MT[0] = seed;

			state.index = SIZE;
			for (uint_fast32_t i = 1; i < SIZE; ++i)
				state.MT[i] = 0x6c078965 * (state.MT[i - 1] ^ state.MT[i - 1] >> 30) + i;
		}

	}


	unsigned long random_complex() {
		return 0;
	}

	unsigned long random_hard() {
		return 0;
	}

	unsigned long random_max() {
		return 0;
	}

}

#endif // !STB_RANDOM_IMPLEMENTATION
#endif // !_STB_RANDOM_
