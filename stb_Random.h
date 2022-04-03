/*
	STB-Like Random Utils
	Author: Krivenko Slava (NeoWise69/thekrovee)
	
	v 0.01 04.02.2022 - Initial stand-up of interface functions. Added fast pseudo-random number generator support. WIP.
	v 0.02 04.04.2022 - Added complex PRNG, inspired by hash function. Using idea of DOOM's random generator in kernel. WIP.
	v 0.02 04.04.2022 - Added C-Style functions to be compatible with pure C. WIP.
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
	Random generator, that uses hash algorithm in that core.
	Using DOOM's character generator, it generates table
	of random 'words', and then using that to hash every single
	character. This 'word table' creates randomness.
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

#ifdef __cplusplus

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

#else

/* 
	Main interface on that random engine. Supports different random-engine algorithms. 
*/
unsigned long srandom(int level = RANDOM_LEVEL_DEFAULT);
/* 
	Main interface on that random engine. Supports different random-engine algorithms. 
	Limits minimal generated number.
*/
unsigned long srandom_min(unsigned long minval, int level = RANDOM_LEVEL_DEFAULT);
/*
	Main interface on that random engine. Supports different random-engine algorithms.
	Limits maximum generated number.
*/
unsigned long srandom_max(unsigned long maxval, int level = RANDOM_LEVEL_DEFAULT);
/*
	Main interface on that random engine. Supports different random-engine algorithms.
	Limits generated number between these two numbers.
*/
unsigned long srandom_range(unsigned long minval, unsigned long maxval, int level = RANDOM_LEVEL_DEFAULT);

#endif

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

	namespace hashv {

		static unsigned long hash(const char*);
		static void prepare(int);
		static unsigned long complex_u32(void);

	}

}

inline void seed(long val) {
	_details::prng::fast_seed(val);
	_details::hashv::prepare(val);
}

#ifdef __cplusplus

srandom::srandom(int level) {
	switch (level) {
		case RANDOM_LEVEL_FAST: value = _details::random_fast(); break;
		case RANDOM_LEVEL_MEDIUM: value = _details::random_medium(); break;
		case RANDOM_LEVEL_COMPLEX: value = _details::random_complex(); break;
		case RANDOM_LEVEL_HARD: value = _details::random_hard(); break;
		case RANDOM_LEVEL_MADNESS: value = _details::random_max(); break;
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
	this->max(maxval);
	this->min(minval);
	return *this;
}

inline srandom::operator unsigned long() const {
	return (unsigned long)(this->value);
}

#else

inline unsigned long srandom(int level) {
	switch (level) {
	case RANDOM_LEVEL_FAST: return _details::random_fast(); 
	case RANDOM_LEVEL_MEDIUM: return _details::random_medium(); 
	case RANDOM_LEVEL_COMPLEX: return _details::random_complex(); 
	case RANDOM_LEVEL_HARD: return _details::random_hard(); 
	case RANDOM_LEVEL_MADNESS: return _details::random_max(); 
	default:
		return _details::random_fast(); 
	}
}

inline unsigned long srandom_min(unsigned long minval, int level) {
	unsigned long value = 0;
	switch (level) {
		case RANDOM_LEVEL_FAST: value = _details::random_fast(); break; 
		case RANDOM_LEVEL_MEDIUM: value = _details::random_medium(); break; 
		case RANDOM_LEVEL_COMPLEX: value = _details::random_complex(); break; 
		case RANDOM_LEVEL_HARD: value = _details::random_hard(); break; 
		case RANDOM_LEVEL_MADNESS: value = _details::random_max(); break; 
		default:
			value = _details::random_fast(); break; 
	}
	value += value < minval ? minval : 0;
	return value;
}

inline unsigned long srandom_max(unsigned long maxval, int level) {
	unsigned long value = 0;
	switch (level) {
		case RANDOM_LEVEL_FAST: value = _details::random_fast(); break; 
		case RANDOM_LEVEL_MEDIUM: value = _details::random_medium(); break; 
		case RANDOM_LEVEL_COMPLEX: value = _details::random_complex(); break; 
		case RANDOM_LEVEL_HARD: value = _details::random_hard(); break; 
		case RANDOM_LEVEL_MADNESS: value = _details::random_max(); break; 
		default:
			value = _details::random_fast(); break; 
	}
	value = value > maxval ? (value % maxval) : value;
	return value;
}

inline unsigned long srandom_range(unsigned long minval, unsigned long maxval, int level) {
	unsigned long value = 0;
	switch (level) {
		case RANDOM_LEVEL_FAST: value = _details::random_fast(); break; 
		case RANDOM_LEVEL_MEDIUM: value = _details::random_medium(); break; 
		case RANDOM_LEVEL_COMPLEX: value = _details::random_complex(); break; 
		case RANDOM_LEVEL_HARD: value = _details::random_hard(); break; 
		case RANDOM_LEVEL_MADNESS: value = _details::random_max(); break; 
		default:
			value = _details::random_fast(); break; 
	}
	value = value > maxval ? (value % maxval) : value;
	value += (value < minval) ? minval : 0;
	return value;
}

#endif

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

	namespace hashv {

		static unsigned char doom_rndtable[256] = {
			0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66 ,
			74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36 ,
			95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188 ,
			52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224 ,
			149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242 ,
			145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0 ,
			175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235 ,
			25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113 ,
			94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75 ,
			136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196 ,
			135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113 ,
			80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241 ,
			24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224 ,
			145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95 ,
			28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226 ,
			71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36 ,
			17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106 ,
			197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136 ,
			120, 163, 236, 249
		};

		static int doom_rndindex = 0;

		static char doom_gen_char(void);

		typedef struct LGState {
			unsigned long table[256];
			unsigned long temporary;
		} LGState;
		static LGState state;

		static char word_table[256][8] = {0};
		static int counter = 0;

		void gen_word_table(int);

	}

	unsigned long random_complex() {
		return _details::hashv::complex_u32();
	}

	unsigned long random_hard() {
		return 0;
	}

	unsigned long random_max() {
		return 0;
	}

	namespace hashv {

		unsigned long hash(const char* cp) {
			unsigned long result = 0x811c9dc5;
			while (*cp) {
				result ^= (unsigned char)*cp++;
				result *= 0x01000193;
			}
			return result;
		}

		void prepare(int val) {
			gen_word_table(val);

			for (int i = 0; i < 256; i++)
				state.table[i] = hash(word_table[i] + i);
			state.temporary = state.table[32] ^ hash(word_table[32]);
		}

		unsigned long complex_u32(void) {
			
			if (counter > 0) {
				return state.table[counter--];
			}

			prepare((int)(state.temporary));
			return state.table[counter--];

		}

		char doom_gen_char(void) {
			doom_rndindex = (doom_rndindex + 1) & 0xFF;
			return (char)(doom_rndtable[doom_rndindex]);
		}

		void gen_word_table(int val) {
			for (int i = 0; i < 256; i++) {
				for (int ci = 0; ci < 8; ci++)
					word_table[i][ci] = (val + doom_gen_char()) % 128;
				counter++;
			}
		}

	}

}

#endif // !STB_RANDOM_IMPLEMENTATION
#endif // !_STB_RANDOM_
