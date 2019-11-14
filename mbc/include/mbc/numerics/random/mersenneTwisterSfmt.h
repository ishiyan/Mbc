#pragma once

// Uncomment this to use SSE2.
#define MERSENNE_TWISTER_SFMT_SSE2

#include <memory.h>
#include "randomGenerator.h"

#ifdef MERSENNE_TWISTER_SFMT_SSE2
#include <emmintrin.h>
#endif

namespace mbc::numerics::random {

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator engine.
    //! Based upon an information and a reference code presented on
    //! http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html.
    //! This is a little-endian-only implementation.
    //! For a description of parameters, see:
    //! Makoto Matsumoto and Takuji Nishimura, "Dynamic Creation of Pseudorandom Number Generators",
    //! Monte Carlo and Quasi-Monte Carlo Methods 1998, Springer, 2000, pp 56--69.
    //! http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/ARTICLES/articles.html
    //! For the theory, see:
    //! Mutsuo Saito and Makoto Matsumoto, "SIMD-oriented Fast Mersenne Twister: a 128-bit Pseudorandom Number Generator",
    //! Monte Carlo and Quasi-Monte Carlo Methods 2006, Springer, 2008, pp. 607 -- 622.
    //! http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/ARTICLES/articles.html
    //!
    //! \tparam Mexp    The Mersenne Exponent. The period of the sequence is a multiple of 2^MEXP-1.
    //! \tparam Pos1    The pick up position of the array.
    //! \tparam Sl1     The parameter of shift left as four 32-bit registers.
    //! \tparam Sl2     The parameter of shift left as one 128-bit register. The 128-bit integer is shifted by (SL2 * 8) bits.
    //! \tparam Sr1     The parameter of shift right as four 32-bit registers.
    //! \tparam Sr2     The parameter of shift right as one 128-bit register. The 128-bit integer is shifted by (SR2 * 8) bits.
    //! \tparam Msk1    A bitmask used in the recursion. These parameters are introduced to break symmetry of SIMD.
    //! \tparam Msk2    A bitmask used in the recursion. These parameters are introduced to break symmetry of SIMD.
    //! \tparam Msk3    A bitmask used in the recursion. These parameters are introduced to break symmetry of SIMD.
    //! \tparam Msk4    A bitmask used in the recursion. These parameters are introduced to break symmetry of SIMD.
    //! \tparam Parity1 This definition is a part of a 128-bit period certification vector.
    //! \tparam Parity2 This definition is a part of a 128-bit period certification vector.
    //! \tparam Parity3 This definition is a part of a 128-bit period certification vector.
    //! \tparam Parity4 This definition is a part of a 128-bit period certification vector.
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2,
        unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4,
        unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
        class MersenneTwisterSfmtEngine : public RandomGenerator
    {
    private:
        //! The size of internal state array of 128-bit integers.
        static constexpr int n128{ Mexp / 128 + 1 };

        //! The size of internal state array when regarded as an array of 32-bit integers.
        static constexpr int n32{ n128 * 4 };

        //! The size of internal state array when regarded as an array of 64-bit integers.
        static constexpr int n64{ n128 * 2 };

    public:
        //! The minimum size of an array of 32-bit unsigned integers used for MersenneTwisterSfmtEngine::fillArray32.
        static constexpr int fillArray32MinimalLength{ n32 };

        //! Generates 32-bit unsigned integer pseudo-random numbers in the specified array by one call.
        //! Before the first call to MersenneTwisterSfmtEngine::fillArray32(), the generator must be reset.
        //! The pointer to the array must be \b aligned (namely, must be a multiple of 16) in the SIMD version, since it refers to the address of a 128-bit integer.
        //! Note that \e memalign or \e posix_memalign (or \e _aligned_malloc with \e _aligned_free in Visual C++) is available to get the aligned memory.
        //! \param array       An array where pseudo-random 32-bit unsigned integers are filled. 
        //! \param arrayLength The number of integers to be generated. Must be a multiple of 4, and greater than or equal to MersenneTwisterSfmtEngine::fillArray32MinimalLength.
        void fillArray32(unsigned int *array, int arrayLength);

        //! The minimum size of an array of 64-bit unsigned integers used for MersenneTwisterSfmtEngine::fillArray64.
        static constexpr int fillArray64MinimalLength{ n64 };

        //! Generates 64-bit unsigned integer pseudo-random numbers in the specified array by one call.
        //! Before the first call to MersenneTwisterSfmtEngine::fillArray64, the generator must be reset.
        //! The pointer to the array must be \e aligned (namely, must be a multiple of 16) in the SIMD version, since it refers to the address of a 128-bit integer.
        //! Note that \e memalign or \e posix_memalign (or \e _aligned_malloc with \e _aligned_free in Visual C++) is available to get the aligned memory.
        //! \param array       An array where pseudo-random 64-bit unsigned integers are filled. 
        //! \param arrayLength The number of integers to be generated. Must be a multiple of 2, and greater than or equal to MersenneTwisterSfmtEngine::fillArray34MinimalLength.
        void fillArray64(unsigned long long *array, int arrayLength);

        //! A next 32-bit unsigned integer pseudo-random number from the interval [UINT_MIN, UINT_MAX].
        unsigned int nextUint() noexcept override final;

        //! A next 64-bit unsigned integer pseudo-random number from the interval [UINT_MIN, UINT_MAX].
        unsigned long long nextUint64() noexcept;

        //! A next double-precision floating point pseudo-random number from the interval [0, 1) generated from two 32-bit integers.
        double nextDouble() noexcept override final;

        //! A next double-precision floating point pseudo-random number from the interval [0, 1] generated from two 32-bit integers.
        double nextDoubleInclusiveOne() noexcept;

        //! A next double-precision floating point pseudo-random number from the interval (0, 1) generated from two 32-bit integers.
        double nextDoublePositive() noexcept;

        //! A double-precision floating point pseudo-random number from the interval [0, \p maxValue) generated from two 32-bit integers.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p maxValue must be greater than or equal to 0.
        double nextDouble(double maxValue) noexcept override final;

        //! A double-precision floating point pseudo-random number from the interval [\p  minValue, \p  maxValue) generated from two 32-bit integers.
        //! \param minValue The inclusive lower bound of the random number to be generated. The range between \p  minValue and \p  maxValue must be less than or equal to \e DBL_MAX.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p  maxValue must be greater than or equal to \p  minValue. The range between \p  minValue and \p  maxValue must be less than or equal to \e DBL_MAX.
        double nextDouble(double minValue, double maxValue) noexcept override final;

        //! A next double-precision floating point pseudo-random number from the interval [0, 1) generated from one 64-bit integer.
        double nextDouble64() noexcept;

        //! A double-precision floating point pseudo-random number from the interval [0, \p  maxValue) generated from one 64-bit integer.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p  maxValue must be greater than or equal to 0.
        double nextDouble64(double maxValue) noexcept;

        //! A double-precision floating point pseudo-random number from the interval [\p  minValue, \p  maxValue) generated from one 64-bit integer.
        //! \param minValue The inclusive lower bound of the random number to be generated. The range between \p  minValue and \p  maxValue must be less than or equal to \e DBL_MAX.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p  maxValue must be greater than or equal to \p  minValue. The range between \p  minValue and \p  maxValue must be less than or equal to \e DBL_MAX.
        double nextDouble64(double minValue, double maxValue) noexcept;

        //! Indicates whether the generator can be reset, so that it produces the same pseudo-random number sequence again.
        [[nodiscard]] bool canReset() const noexcept override final { return true; }

        //! Resets the generator, so that it produces the same pseudo-random number sequence again.
        void reset() noexcept override final { init(); }

        MersenneTwisterSfmtEngine(const MersenneTwisterSfmtEngine& other) noexcept;
        MersenneTwisterSfmtEngine(MersenneTwisterSfmtEngine&& other) noexcept;
        MersenneTwisterSfmtEngine& operator=(const MersenneTwisterSfmtEngine& other);
        MersenneTwisterSfmtEngine& operator=(MersenneTwisterSfmtEngine&& other) noexcept;

    protected:
        //! Constructs a new instance of the engine using the default seed.
        MersenneTwisterSfmtEngine() noexcept;

        //! Constructs a new instance of the engine using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmtEngine(int seed) noexcept;

        //! Constructs a new instance of the engine using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmtEngine(const int* seedArray, int seedArrayLength);

        virtual ~MersenneTwisterSfmtEngine();

    private:
        //! The default seed value.
        static constexpr unsigned int defaultSeed{ 19591204U };

        //! 128-bit data structure.
        typedef union
        {
            unsigned int u[4];
            unsigned long long u64[2];
#ifdef MERSENNE_TWISTER_SFMT_SSE2
            __m128i si;
#endif
        } w128;

        //! Stores the state array.
        w128 stateArray_[n128];

        //! An index to the 32-bit word within 128-bit structure;
        int index_{0};

        //! The used seed value.
        unsigned int seedValue_;

        //! The used seed array.
        unsigned int* seedArray_;

        //! The used seed array length.
        int seedArrayLength_;

        //! Certificate the period of 2^{MEXP}.
        void periodCertification() noexcept;

        static unsigned int func1(unsigned int x) noexcept;
        static unsigned int func2(unsigned int x) noexcept;
        void init() noexcept;

#ifdef MERSENNE_TWISTER_SFMT_SSE2
        static __m128i sse2ParamMask() noexcept;
#ifdef _MSC_VER
#if _MSC_VER > 1700
        static __m128i __vectorcall mmRecursion(__m128i a, __m128i b, __m128i c, __m128i d) noexcept;
#else
        static void mmRecursion(__m128i *r, __m128i a, __m128i b, __m128i c, __m128i *d) noexcept;
#endif
#else
        static void mmRecursion(__m128i *r, __m128i a, __m128i b, __m128i c, __m128i d) noexcept;
#endif
#else
        //! Simulates the little endian SIMD 128-bit left shift.
        static void lshift128(w128* target, const w128* source, const int shift) noexcept;

        //! Simulates the little endian SIMD 128-bit right shift.
        static void rshift128(w128* target, const w128* source, const int shift) noexcept;

        static void simulateRecursion(w128* r, const w128 *a, const w128 *b, const w128 *c, const w128 *d) noexcept;
#endif
        //! Fills the internal state array with pseudo-random integers.
        void regenerate() noexcept;

        //! Fills the user-specified array with pseudo-random integers.
        //! \param array       An 128-bit array to be filled.
        //! \param arrayLength The number of 128-bit numbers to be generated.
        void fillArray(w128 *array, int arrayLength) noexcept;

        //! There are two common ways to create a double floating point
        //! using MersenneTwisterSfmtEngine::nextUint and dividing by 0xFFFFFFFF + 1, or else generating
        //! two double words and shifting the first by 26 bits and adding the second.
        //! \par
        //! In a newer measurement of the randomness of MT19937 published in the
        //! journal "Monte Carlo Methods and Applications, Vol. 12, No. 5-6, pp. 385 - 393 (2006)"
        //! entitled "A Repetition Test for Pseudo-Random Number Generators",
        //! it was found that the 32-bit version of generating a double fails at the 95% 
        //! confidence level when measuring for expected repetitions of a particular 
        //! number in a sequence of numbers generated by the algorithm.
        //! \par
        //! Due to this, the 53-bit method is implemented here and the 32-bit method
        //! of generating a double is not. If, for some reason,
        //! the 32-bit method is needed, it can be generated by the following:
        //! \code{.cpp}
        //! static_cast<double>(nextUint()) / (static_cast<unsigned long long>(UINT_MAX) + 1);
        //! \endcode
        //! \param translate Translation factor.
        //! \param scale     Scaling factor.
        double next53Bit(double translate, double scale = 9007199254740991.0) noexcept;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 216091.
    class MersenneTwisterSfmt216091 final : public MersenneTwisterSfmtEngine < 216091, 627, 11, 3, 10, 1,
        0xbff7bff7U, 0xbfffffffU, 0xbffffa7fU, 0xffddfbfbU,
        0xf8000001U, 0x89e80709U, 0x3bd2b64bU, 0x0c64b1e4U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt216091() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt216091(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt216091(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt216091(const MersenneTwisterSfmt216091& other) noexcept = default;
        MersenneTwisterSfmt216091(MersenneTwisterSfmt216091&& other) noexcept = default;
        MersenneTwisterSfmt216091& operator=(const MersenneTwisterSfmt216091& other) = default;
        MersenneTwisterSfmt216091& operator=(MersenneTwisterSfmt216091&& other) noexcept = default;
        ~MersenneTwisterSfmt216091() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 132049.
    class MersenneTwisterSfmt132049 final : public MersenneTwisterSfmtEngine < 132049, 110, 19, 1, 21, 1,
        0xffffbb5fU, 0xfb6ebf95U, 0xfffefffaU, 0xcff77fffU,
        0x00000001U, 0x00000000U, 0xcb520000U, 0xc7e91c7dU >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt132049() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt132049(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt132049(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt132049(const MersenneTwisterSfmt132049& other) noexcept = default;
        MersenneTwisterSfmt132049(MersenneTwisterSfmt132049&& other) noexcept = default;
        MersenneTwisterSfmt132049& operator=(const MersenneTwisterSfmt132049& other) = default;
        MersenneTwisterSfmt132049& operator=(MersenneTwisterSfmt132049&& other) noexcept = default;
        ~MersenneTwisterSfmt132049() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 86243.
    class MersenneTwisterSfmt86243 final : public MersenneTwisterSfmtEngine < 86243, 366, 6, 7, 19, 1,
        0xfdbffbffU, 0xbff7ff3fU, 0xfd77efffU, 0xbf9ff3ffU,
        0x00000001U, 0x00000000U, 0x00000000U, 0xe9528d85U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt86243() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt86243(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt86243(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt86243(const MersenneTwisterSfmt86243& other) noexcept = default;
        MersenneTwisterSfmt86243(MersenneTwisterSfmt86243&& other) noexcept = default;
        MersenneTwisterSfmt86243& operator=(const MersenneTwisterSfmt86243& other) = default;
        MersenneTwisterSfmt86243& operator=(MersenneTwisterSfmt86243&& other) noexcept = default;
        ~MersenneTwisterSfmt86243() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 44497.
    class MersenneTwisterSfmt44497 final : public MersenneTwisterSfmtEngine < 44497, 330, 5, 3, 9, 3,
        0xeffffffbU, 0xdfbebfffU, 0xbfbf7befU, 0x9ffd7bffU,
        0x00000001U, 0x00000000U, 0xa3ac4000U, 0xecc1327aU >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt44497() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt44497(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt44497(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt44497(const MersenneTwisterSfmt44497& other) noexcept = default;
        MersenneTwisterSfmt44497(MersenneTwisterSfmt44497&& other) noexcept = default;
        MersenneTwisterSfmt44497& operator=(const MersenneTwisterSfmt44497& other) = default;
        MersenneTwisterSfmt44497& operator=(MersenneTwisterSfmt44497&& other) noexcept = default;
        ~MersenneTwisterSfmt44497() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 19937.
    class MersenneTwisterSfmt19937 final : public MersenneTwisterSfmtEngine < 19937, 122, 18, 1, 11, 1,
        0xdfffffefU, 0xddfecb7fU, 0xbffaffffU, 0xbffffff6U,
        0x00000001U, 0x00000000U, 0x00000000U, 0x13c9e684U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt19937() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt19937(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt19937(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt19937(const MersenneTwisterSfmt19937& other) noexcept = default;
        MersenneTwisterSfmt19937(MersenneTwisterSfmt19937&& other) noexcept = default;
        MersenneTwisterSfmt19937& operator=(const MersenneTwisterSfmt19937& other) = default;
        MersenneTwisterSfmt19937& operator=(MersenneTwisterSfmt19937&& other) noexcept = default;
        ~MersenneTwisterSfmt19937() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 11213.
    class MersenneTwisterSfmt11213 final : public MersenneTwisterSfmtEngine < 11213, 68, 14, 3, 7, 3,
        0xeffff7fbU, 0xffffffefU, 0xdfdfbfffU, 0x7fffdbfdU,
        0x00000001U, 0x00000000U, 0xe8148000U, 0xd0c7afa3U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt11213() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt11213(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt11213(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt11213(const MersenneTwisterSfmt11213& other) noexcept = default;
        MersenneTwisterSfmt11213(MersenneTwisterSfmt11213&& other) noexcept = default;
        MersenneTwisterSfmt11213& operator=(const MersenneTwisterSfmt11213& other) = default;
        MersenneTwisterSfmt11213& operator=(MersenneTwisterSfmt11213&& other) noexcept = default;
        ~MersenneTwisterSfmt11213() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 4253.
    class MersenneTwisterSfmt4253 final : public MersenneTwisterSfmtEngine < 4253, 17, 20, 1, 7, 1,
        0x9f7bffffU, 0x9fffff5fU, 0x3efffffbU, 0xfffff7bbU,
        0xa8000001U, 0xaf5390a3U, 0xb740b3f8U, 0x6c11486dU >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt4253() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt4253(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt4253(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt4253(const MersenneTwisterSfmt4253& other) noexcept = default;
        MersenneTwisterSfmt4253(MersenneTwisterSfmt4253&& other) noexcept = default;
        MersenneTwisterSfmt4253& operator=(const MersenneTwisterSfmt4253& other) = default;
        MersenneTwisterSfmt4253& operator=(MersenneTwisterSfmt4253&& other) noexcept = default;
        ~MersenneTwisterSfmt4253() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 2281.
    class MersenneTwisterSfmt2281 final : public MersenneTwisterSfmtEngine < 2281, 12, 19, 1, 5, 1,
        0xbff7ffbfU, 0xfdfffffeU, 0xf7ffef7fU, 0xf2f7cbbfU,
        0x00000001U, 0x00000000U, 0x00000000U, 0x41dfa600U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt2281() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt2281(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt2281(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt2281(const MersenneTwisterSfmt2281& other) noexcept = default;
        MersenneTwisterSfmt2281(MersenneTwisterSfmt2281&& other) noexcept = default;
        MersenneTwisterSfmt2281& operator=(const MersenneTwisterSfmt2281& other) = default;
        MersenneTwisterSfmt2281& operator=(MersenneTwisterSfmt2281&& other) noexcept = default;
        ~MersenneTwisterSfmt2281() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 1279.
    class MersenneTwisterSfmt1279 final : public MersenneTwisterSfmtEngine < 1279, 7, 14, 3, 5, 1,
        0xf7fefffdU, 0x7fefcfffU, 0xaff3ef3fU, 0xb5ffff7fU,
        0x00000001U, 0x00000000U, 0x00000000U, 0x20000000U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt1279() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt1279(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt1279(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt1279(const MersenneTwisterSfmt1279& other) noexcept = default;
        MersenneTwisterSfmt1279(MersenneTwisterSfmt1279&& other) noexcept = default;
        MersenneTwisterSfmt1279& operator=(const MersenneTwisterSfmt1279& other) = default;
        MersenneTwisterSfmt1279& operator=(MersenneTwisterSfmt1279&& other) noexcept = default;
        ~MersenneTwisterSfmt1279() = default;
    };

    //! The SIMD-oriented Fast Mersenne Twister (SFMT) uniform pseudo-random number generator with the Mersenne exponent equal to 607.
    class MersenneTwisterSfmt607 final : public MersenneTwisterSfmtEngine < 607, 2, 15, 3, 13, 3,
        0xfdff37ffU, 0xef7f3f7dU, 0xff777b7dU, 0x7ff7fb2fU,
        0x00000001U, 0x00000000U, 0x00000000U, 0x5986f054U >
    {
    public:
        //! Constructs a new instance of the generator using a default seed.
        MersenneTwisterSfmt607() noexcept = default;

        //! Constructs a new instance of the generator using an integer seed.
        //! \param seed The initial 32-bit integer seed.
        explicit MersenneTwisterSfmt607(const int seed) noexcept : MersenneTwisterSfmtEngine(seed) {}

        //! Constructs a new instance of the generator using an integer array seed.
        //! \param seedArray       The initial seed array of 32-bit integers.
        //! \param seedArrayLength The length of the seed array, must be positive.
        MersenneTwisterSfmt607(const int* seedArray, const int seedArrayLength) : MersenneTwisterSfmtEngine(seedArray, seedArrayLength) {}

        MersenneTwisterSfmt607(const MersenneTwisterSfmt607& other) noexcept = default;
        MersenneTwisterSfmt607(MersenneTwisterSfmt607&& other) noexcept = default;
        MersenneTwisterSfmt607& operator=(const MersenneTwisterSfmt607& other) = default;
        MersenneTwisterSfmt607& operator=(MersenneTwisterSfmt607&& other) noexcept = default;
        ~MersenneTwisterSfmt607() = default;
    };

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    fillArray32(unsigned int *array, const int arrayLength)
    {
        if (index_ != n32)
            throw std::logic_error("The internal state is not properly initialized, index_ != N32.");
        if (arrayLength % 4 != 0)
            throw std::invalid_argument("The array length must be multiple of 4.");
        if (arrayLength < fillArray32MinimalLength)
            throw std::invalid_argument("The array length must be greater than or equal to fillArray32MinimalLength.");
        fillArray(reinterpret_cast<w128*>(array), arrayLength / 4);
        index_ = n32;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    fillArray64(unsigned long long *array, const int arrayLength)
    {
        if (index_ != n32)
            throw std::logic_error("The internal state is not properly initialized, index_ != N32. Please reset generator.");
        if (arrayLength % 2 != 0)
            throw std::invalid_argument("The array length must be multiple of 2.");
        if (arrayLength < fillArray64MinimalLength)
            throw std::invalid_argument("The array length must be greater than or equal to fillArray64MinimalLength.");
        fillArray(reinterpret_cast<w128*>(array), arrayLength / 2);
        index_ = n32;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    unsigned int MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextUint() noexcept
    {
        if (index_ >= n32)
        {
            regenerate();
            index_ = 0;
        }
        const unsigned int *p32{ &stateArray_[0].u[0] };
        return p32[index_++];
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    unsigned long long MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextUint64() noexcept
    {
        if (index_ % 2 != 0)
            ++index_;// throw std::logic_error("The internal state is not properly initialized, index_%2 != 0. Please reset generator.");
        if (index_ >= n32)
        {
            regenerate();
            index_ = 0;
        }
        const unsigned long long *p64{ &stateArray_[0].u64[0] };
        const unsigned long long r{ p64[index_ / 2] };
        index_ += 2;
        return r;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble() noexcept
    {
        /*
        // Get 27 pseudo-random bits.
        unsigned long long a{ static_cast<unsigned long long>(nextUint()) >> 5 };
        // Get 26 pseudo-random bits.
        unsigned long long b{ static_cast<unsigned long long>(nextUint()) >> 6 };

        // Shift the 27 pseudo-random bits (a) over by 26 bits (* 67108864.0) and
        // add another pseudo-random 26 bits (+ b).
        // 9007199254740991.0 is the maximum double value which the 53 significand can hold when the exponent is 0.
        return (a * 67108864.0 + b) / (9007199254740991.0 + 1);
        */
        const unsigned int first{ nextUint() }, second{ nextUint() };
        return (first | (static_cast<unsigned long long>(second) << 32)) / 18446744073709551616.0;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDoubleInclusiveOne() noexcept
    {
        return next53Bit(0);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDoublePositive() noexcept
    {
        return next53Bit(0.5);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble(const double maxValue) noexcept
    {
        return nextDouble() * maxValue;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble(const double minValue, const double maxValue) noexcept
    {
        return minValue + nextDouble() * (maxValue - minValue);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble64() noexcept
    {
        return nextUint64() * (1.0 / 18446744073709551616.0);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble64(const double maxValue) noexcept
    {
        return nextDouble64() * maxValue;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    nextDouble64(const double minValue, const double maxValue) noexcept
    {
        return minValue + nextDouble64() * (maxValue - minValue);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    MersenneTwisterSfmtEngine(const MersenneTwisterSfmtEngine& other) noexcept :
        RandomGenerator(other), index_{ other.index_ }, seedValue_{ other.seedValue_ },
        seedArray_{ nullptr }, seedArrayLength_{ 0 }
    {
        if (nullptr != other.seedArray_ && 0 < other.seedArrayLength_)
        {
            seedArrayLength_ = other.seedArrayLength_;
            seedArray_ = new unsigned int[seedArrayLength_];
            memcpy(seedArray_, other.seedArray_, sizeof(unsigned int) * seedArrayLength_);
        }
        memcpy(stateArray_, other.stateArray_, sizeof(stateArray_));
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    MersenneTwisterSfmtEngine(MersenneTwisterSfmtEngine&& other) noexcept :
        RandomGenerator(std::move(other)), index_{ other.index_ }, seedValue_{ other.seedValue_ },
        seedArray_{ other.seedArray_ }, seedArrayLength_{ other.seedArrayLength_ }
    {
        memcpy(stateArray_, other.stateArray_, sizeof(stateArray_));
        other.seedArray_ = nullptr;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    MersenneTwisterSfmtEngine() noexcept :
        RandomGenerator(), seedValue_{ defaultSeed }, seedArray_{ nullptr }, seedArrayLength_{ 0 }
    {
        init();
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    MersenneTwisterSfmtEngine(const int seed)  noexcept:
        RandomGenerator(), seedValue_{ static_cast<unsigned int>(seed) }, seedArray_{ nullptr }, seedArrayLength_{ 0 }
    {
        init();
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    MersenneTwisterSfmtEngine(const int* seedArray, const int seedArrayLength) :
        RandomGenerator(), seedValue_{ defaultSeed }, seedArray_{ nullptr }, seedArrayLength_{ 0 }
    {
        if (nullptr != seedArray && 0 < seedArrayLength)
        {
            seedArrayLength_ = seedArrayLength;
            seedArray_ = new unsigned int[seedArrayLength];
            memcpy(seedArray_, seedArray, sizeof(unsigned int) * seedArrayLength);
        }
        init();
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>&
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    operator=(const MersenneTwisterSfmtEngine& other)
    {
        if (this != &other)
        {
            if (nullptr != seedArray_)
                delete[] seedArray_;
            if (nullptr != other.seedArray_ && 0 < other.seedArrayLength_)
            {
                seedArrayLength_ = other.seedArrayLength_;
                seedArray_ = new unsigned int[seedArrayLength_];
                memcpy(seedArray_, other.seedArray_, sizeof(unsigned int) * seedArrayLength_);
            }
            else
            {
                seedArrayLength_ = 0;
                seedArray_ = nullptr;
            }
            index_ = other.index_;
            memcpy(stateArray_, other.stateArray_, sizeof(stateArray_));
            bitBuffer_ = other.bitBuffer_;
            bitCount_ = other.bitCount_;
        }
        return *this;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>&
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    operator=(MersenneTwisterSfmtEngine&& other) noexcept
    {
        if (this != &other)
        {
            if (nullptr != seedArray_)
                delete[] seedArray_;
            seedArrayLength_ = other.seedArrayLength_;
            seedArray_ = other.seedArray_;
            other.seedArray_ = nullptr;
            index_ = other.index_;
            memcpy(stateArray_, other.stateArray_, sizeof(stateArray_));
            bitBuffer_ = other.bitBuffer_;
            bitCount_ = other.bitCount_;
        }
        return *this;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    ~MersenneTwisterSfmtEngine()
    {
        delete[] seedArray_;
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    periodCertification() noexcept
    {
        const unsigned int parity[4]{ Parity1, Parity2, Parity3, Parity4 };
        unsigned int *p32{ &stateArray_[0].u[0] };
        int inner{ 0 };
        for (int i = 0; i < 4; ++i)
            inner ^= static_cast<int>(p32[i] & parity[i]);
        for (int i = 16; i > 0; i >>= 1)
            inner ^= inner >> i;
        inner &= 1;
        // Check OK.
        if (inner == 1)
            return;
        // Check NG, and modification.
        for (int i = 0; i < 4; ++i)
        {
            unsigned int work{ 1 };
            const unsigned int par{ parity[i] };
            for (int j = 0; j < 32; ++j)
            {
                if ((work & par) != 0)
                {
                    p32[i] ^= work;
                    return;
                }
                work <<= 1;
            }
        }
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    unsigned int MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    func1(const unsigned int x) noexcept
    {
        return (x ^ (x >> 27)) * static_cast<unsigned int>(1664525UL);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    unsigned int MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    func2(const unsigned int x) noexcept
    {
        return (x ^ (x >> 27)) * static_cast<unsigned int>(1566083941UL);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    init() noexcept
    {
        unsigned int *p32{ &stateArray_[0].u[0] };
        if (nullptr == seedArray_)
        {
            unsigned int u{ seedValue_ };
            p32[0] = u;
            for (int i = 1; i < n32; ++i)
            {
                u = static_cast<unsigned int>(1812433253UL * (u ^ (u >> 30))) + static_cast<unsigned int>(i);
                p32[i] = u;
            }
        }
        else
        {
            const int lag{ n32 >= 623 ? 11 : (n32 >= 68 ? 7 : (n32 >= 39 ? 5 : 3)) };
            const int mid{ (n32 - lag) / 2 };
            memset(stateArray_, 0x8b, sizeof(stateArray_));
            const int length{ seedArrayLength_ };
            unsigned int r{ func1(p32[0] ^ p32[mid] ^ p32[n32 - 1]) };
            p32[mid] += r;
            r += static_cast<unsigned int>(length);
            p32[mid + lag] += r;
            p32[0] = r;
            const int count{ (length + 1 > n32) ? length : (n32 - 1) };
            int i{ 1 }, j{ 0 };
            for (; j < count && j < length; ++j)
            {
                const int k{ (i + mid) % n32 };
                r = func1(p32[i] ^ p32[k] ^ p32[(i + n32 - 1) % n32]);
                p32[k] += r;
                r += seedArray_[j] + static_cast<unsigned int>(i);
                p32[(i + mid + lag) % n32] += r;
                p32[i] = r;
                // i = (++i) % n32;
                ++i;
                i = i % n32;
            }
            for (; j < count; ++j)
            {
                const int k{ (i + mid) % n32 };
                r = func1(p32[i] ^ p32[k] ^ p32[(i + n32 - 1) % n32]);
                p32[k] += r;
                r += i;
                p32[(i + mid + lag) % n32] += r;
                p32[i] = r;
                // i = (++i) % n32;
                ++i;
                i = i % n32;
            }
            for (j = 0; j < n32; ++j)
            {
                const int k{ (i + mid) % n32 };
                r = func2(p32[i] + p32[k] + p32[(i + n32 - 1) % n32]);
                p32[k] ^= r;
                r -= i;
                p32[(i + mid + lag) % n32] ^= r;
                p32[i] = r;
                // i = (++i) % n32;
                ++i;
                i = i % n32;
            }
        }
        index_ = n32;
        periodCertification();

        // Reset helper variables used for generation of random bool values.
        bitBuffer_ = 0;
        bitCount_ = maximalBitCount;
    }

#ifdef MERSENNE_TWISTER_SFMT_SSE2
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    __m128i MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    sse2ParamMask() noexcept
    {
        static const w128 paramMask{ { Msk1, Msk2, Msk3, Msk4 } };
        return paramMask.si;
    }
#ifdef _MSC_VER
#if _MSC_VER > 1700
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    __m128i __vectorcall MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    mmRecursion(const __m128i a, const __m128i b, const __m128i c, const __m128i d) noexcept
    {
        __m128i y = _mm_srli_epi32(b, Sr1);
        __m128i z = _mm_srli_si128(c, Sr2);
        const __m128i v = _mm_slli_epi32(d, Sl1);
        z = _mm_xor_si128(z, a);
        z = _mm_xor_si128(z, v);
        const __m128i x = _mm_slli_si128(a, Sl2);
        y = _mm_and_si128(y, sse2ParamMask());
        z = _mm_xor_si128(z, x);
        return _mm_xor_si128(z, y);
    }
#else
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    mmRecursion(__m128i *r, const __m128i a, const __m128i b, const __m128i c, __m128i *d) noexcept
    {
        __m128i y = _mm_srli_epi32(b, Sr1);
        __m128i z = _mm_srli_si128(c, Sr2);
        const __m128i v = _mm_slli_epi32(*d, Sl1);
        z = _mm_xor_si128(z, a);
        z = _mm_xor_si128(z, v);
        const __m128i x = _mm_slli_si128(a, Sl2);
        y = _mm_and_si128(y, sse2ParamMask());
        z = _mm_xor_si128(z, x);
        z = _mm_xor_si128(z, y);
        *r = z;
    }
#endif
#else
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    mmRecursion(__m128i *r, __m128i a, __m128i b, __m128i c, __m128i d) noexcept
    {
        __m128i y = _mm_srli_epi32(b, Sr1);
        __m128i z = _mm_srli_si128(c, Sr2);
        const __m128i v = _mm_slli_epi32(d, Sl1);
        z = _mm_xor_si128(z, a);
        z = _mm_xor_si128(z, v);
        const __m128i x = _mm_slli_si128(a, Sl2);
        y = _mm_and_si128(y, sse2ParamMask());
        z = _mm_xor_si128(z, x);
        z = _mm_xor_si128(z, y);
        *r = z;
    }
#endif
#else
    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    lshift128(w128* target, const w128* source, const int shift) noexcept
    {
        const unsigned long long th{ (static_cast<unsigned long long>(source->u[3]) << 32)
            | static_cast<unsigned long long>(source->u[2]) };
        const unsigned long long tl{ (static_cast<unsigned long long>(source->u[1]) << 32)
            | static_cast<unsigned long long>(source->u[0]) };
        const int s{ shift * 8 };
        unsigned long long oh{ th << s };
        const unsigned long long ol{ tl << s };
        oh |= tl >> (64 - s);
        target->u[1] = static_cast<unsigned int>(ol >> 32);
        target->u[0] = static_cast<unsigned int>(ol);
        target->u[3] = static_cast<unsigned int>(oh >> 32);
        target->u[2] = static_cast<unsigned int>(oh);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    rshift128(w128* target, const w128* source, const int shift) noexcept
    {
        const unsigned long long th{ (static_cast<unsigned long long>(source->u[3]) << 32)
            | static_cast<unsigned long long>(source->u[2]) };
        const unsigned long long tl{ (static_cast<unsigned long long>(source->u[1]) << 32)
            | static_cast<unsigned long long>(source->u[0]) };
        const int s{ shift * 8 };
        const unsigned long long oh{ th >> s };
        unsigned long long ol{ tl >> s };
        ol |= th << (64 - s);
        target->u[1] = static_cast<unsigned int>(ol >> 32);
        target->u[0] = static_cast<unsigned int>(ol);
        target->u[3] = static_cast<unsigned int>(oh >> 32);
        target->u[2] = static_cast<unsigned int>(oh);
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    simulateRecursion(w128* r, const w128 *a, const w128 *b, const w128 *c, const w128 *d) noexcept
    {
        w128 x, y;
        lshift128(&x, a, Sl2);
        rshift128(&y, c, Sr2);
        unsigned int *ru{ r->u };
        const unsigned int *au{ a->u }, *bu{ b->u }, *du{ d->u }, *xu{ x.u }, *yu{ y.u };
        ru[0] = au[0] ^ xu[0] ^ ((bu[0] >> Sr1) & Msk1) ^ yu[0] ^ (du[0] << Sl1);
        ru[1] = au[1] ^ xu[1] ^ ((bu[1] >> Sr1) & Msk2) ^ yu[1] ^ (du[1] << Sl1);
        ru[2] = au[2] ^ xu[2] ^ ((bu[2] >> Sr1) & Msk3) ^ yu[2] ^ (du[2] << Sl1);
        ru[3] = au[3] ^ xu[3] ^ ((bu[3] >> Sr1) & Msk4) ^ yu[3] ^ (du[3] << Sl1);
        //r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SFMT_SR1) & SFMT_MSK1) ^ y.u[0] ^ (d->u[0] << SFMT_SL1);
        //r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SFMT_SR1) & SFMT_MSK2) ^ y.u[1] ^ (d->u[1] << SFMT_SL1);
        //r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> SFMT_SR1) & SFMT_MSK3) ^ y.u[2] ^ (d->u[2] << SFMT_SL1);
        //r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> SFMT_SR1) & SFMT_MSK4) ^ y.u[3] ^ (d->u[3] << SFMT_SL1);
    }
#endif

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    regenerate() noexcept
    {
        w128 *p128{ stateArray_ };
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        __m128i r1 = p128[n128 - 2].si, r2 = p128[n128 - 1].si;
#else // SSE2 emulation.
        w128 *r1{ p128 + n128 - 2 }, *r2{ p128 + n128 - 1 };
#endif
        int i{ 0 };
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; i < n128 - Pos1; ++i)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            p128[i].si = mmRecursion(p128[i].si, p128[i + Pos1].si, r1, r2);
#else
            mmRecursion(&p128[i].si, p128[i].si, p128[i + Pos1].si, r1, &r2);
#endif
#else
            mmRecursion(&p128[i].si, p128[i].si, p128[i + Pos1].si, r1, r2);
#endif
            r1 = r2;
            r2 = p128[i].si;
        }
#else // SSE2 emulation.
        for (; i < n128 - Pos1; ++i, ++p128)
        {
            simulateRecursion(p128, p128, p128 + Pos1, r1, r2);
            r1 = r2;
            r2 = p128;
        }
#endif
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; i < n128; ++i)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            p128[i].si = mmRecursion(p128[i].si, p128[i + Pos1 - n128].si, r1, r2);
#else
            mmRecursion(&p128[i].si, p128[i].si, p128[i + Pos1 - n128].si, r1, &r2);
#endif
#else
            mmRecursion(&p128[i].si, p128[i].si, p128[i + Pos1 - n128].si, r1, r2);
#endif
            r1 = r2;
            r2 = p128[i].si;
        }
#else // SSE2 emulation.
        for (; i < n128; ++i, ++p128)
        {
            simulateRecursion(p128, p128, p128 + Pos1 - n128, r1, r2);
            r1 = r2;
            r2 = p128;
        }
#endif
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    void MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    fillArray(w128 *array, const int arrayLength) noexcept
    {
        w128 *p128{ stateArray_ };
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        __m128i r1 = p128[n128 - 2].si, r2 = p128[n128 - 1].si;
#else // SSE2 emulation.
        w128 *r1{ p128 + n128 - 2 }, *r2{ p128 + n128 - 1 }, *pa{ array };
#endif
        int i{ 0 };
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; i < n128 - Pos1; ++i)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            array[i].si = mmRecursion(p128[i].si, p128[i + Pos1].si, r1, r2);
#else
            mmRecursion(&array[i].si, p128[i].si, p128[i + Pos1].si, r1, &r2);
#endif
#else
            mmRecursion(&array[i].si, p128[i].si, p128[i + Pos1].si, r1, r2);
#endif
            r1 = r2;
            r2 = array[i].si;
        }
#else // SSE2 emulation.
        for (; i < n128 - Pos1; ++i, ++pa, ++p128)
        {
            simulateRecursion(pa, p128, p128 + Pos1, r1, r2);
            r1 = r2;
            r2 = pa;
        }
#endif
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; i < n128; ++i)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            array[i].si = mmRecursion(p128[i].si, array[i + Pos1 - n128].si, r1, r2);
#else
            mmRecursion(&array[i].si, p128[i].si, array[i + Pos1 - n128].si, r1, &r2);
#endif
#else
            mmRecursion(&array[i].si, p128[i].si, array[i + Pos1 - n128].si, r1, r2);
#endif
            r1 = r2;
            r2 = array[i].si;
        }
#else // SSE2 emulation.
        for (; i < n128; ++i, ++pa, ++p128)
        {
            simulateRecursion(pa, p128, pa + Pos1 - n128, r1, r2);
            r1 = r2;
            r2 = pa;
        }
#endif
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; i < arrayLength - n128; ++i)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            array[i].si = mmRecursion(array[i - n128].si, array[i + Pos1 - n128].si, r1, r2);
#else
            mmRecursion(&array[i].si, array[i - n128].si, array[i + Pos1 - n128].si, r1, &r2);
#endif
#else
            mmRecursion(&array[i].si, array[i - n128].si, array[i + Pos1 - n128].si, r1, r2);
#endif
            r1 = r2;
            r2 = array[i].si;
        }
#else // SSE2 emulation.
        for (; i < arrayLength - n128; ++i, ++pa)
        {
            simulateRecursion(pa, pa - n128, pa + Pos1 - n128, r1, r2);
            r1 = r2;
            r2 = pa;
        }
#endif
        int j{ 0 };
#ifdef MERSENNE_TWISTER_SFMT_SSE2
        for (; j < 2 * n128 - arrayLength; ++j)
            p128[j] = array[j + arrayLength - n128];
        for (; i < arrayLength; ++i, ++j)
        {
#ifdef _MSC_VER
#if _MSC_VER > 1700
            array[i].si = mmRecursion(array[i - n128].si, array[i + Pos1 - n128].si, r1, r2);
#else
            mmRecursion(&array[i].si, array[i - n128].si, array[i + Pos1 - n128].si, r1, &r2);
#endif
#else
            mmRecursion(&array[i].si, array[i - n128].si, array[i + Pos1 - n128].si, r1, r2);
#endif
            r1 = r2;
            r2 = array[i].si;
            p128[j] = array[i];
        }
#else // SSE2 emulation.
        p128 = stateArray_;
        array += arrayLength - n128;
        for (; j < 2 * n128 - arrayLength; ++j, ++p128, ++array)
            *p128 = *array;
        for (; i < arrayLength; ++i, ++j, ++pa, ++p128)
        {
            simulateRecursion(pa, pa - n128, pa + Pos1 - n128, r1, r2);
            r1 = r2;
            r2 = pa;
            *p128 = *pa;
        }
#endif
    }

    template <int Mexp, int Pos1, int Sl1, int Sl2, int Sr1, int Sr2, unsigned int Msk1, unsigned int Msk2, unsigned int Msk3, unsigned int Msk4, unsigned int Parity1, unsigned int Parity2, unsigned int Parity3, unsigned int Parity4>
    double MersenneTwisterSfmtEngine<Mexp, Pos1, Sl1, Sl2, Sr1, Sr2, Msk1, Msk2, Msk3, Msk4, Parity1, Parity2, Parity3, Parity4>::
    next53Bit(const double translate, const double scale) noexcept
    {
        // Get 27 pseudo-random bits.
        const unsigned long long a{ static_cast<unsigned long long>(nextUint()) >> 5 };
        // Get 26 pseudo-random bits.
        const unsigned long long b{ static_cast<unsigned long long>(nextUint()) >> 6 };

        // Shift the 27 pseudo-random bits (a) over by 26 bits (* 67108864.0) and
        // add another pseudo-random 26 bits (+ b).
        return ((a * 67108864.0 + b) + translate) / scale;
    }
}
