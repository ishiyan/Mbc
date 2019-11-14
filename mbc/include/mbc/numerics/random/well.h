#pragma once

#include <climits>
#include <cfloat>
#include <memory>
#include "randomGenerator.h"

namespace mbc::numerics::random
{

    /// Well Equidistributed Long-period Linear
    /// The WELL class of pseudo-random number generators engine as described in a paper by François Panneton, Pierre L'Ecuyer and Makoto Matsumoto.
    /// http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng.pdf.
    /// Improved Long-Period Generators Based on Linear Recurrences Modulo 2, ACM Transactions on Mathematical Software, 32, 1 (2006).
    /// The errata for the paper are in
    /// http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng-errata.txt.
    /// See also
    /// http://www.iro.umontreal.ca/~panneton/WELLRNG.html.

    /// \param K The number of bits in the pool (not necessarily a multiple of 32).
    /// \param M1 The first parameter of the algorithm.
    /// \param M2 The second parameter of the algorithm.
    /// \param M3 The third parameter of the algorithm.
    /// \param W The bits block size (always 32 in the original paper).
    /// \param P The number of unused bits in the last block.
    template <int K, int M1, int M2, int M3, int W = 32, int P = 1, int R> class WellEngine : public RandomGenerator
    {
    private:
        // The bits pool contains K bits, K = R W - P where R is the number
        // of W bits blocks, W is the block size (always 32 in the original paper)
        // and P is the number of unused bits in the last block.
        static_assert(R == ((K + W - P) / W), "Inconsistent value of R.");

        void init()
        {
            const unsigned int slen{ static_cast<unsigned int>(seedArrayLength_) };
            const unsigned int vlen{ static_cast<unsigned int>(R) };
            const unsigned int minlen{ slen < vlen ? slen : vlen };
            std::memcpy(v_, seedArray_, static_cast<size_t>(minlen));
            if (slen < vlen)
            {
                for (unsigned int i = slen; i != vlen; ++i)
                {
                    const unsigned long l{ v_[i - slen] };
                    v_[i] = static_cast<unsigned int>((1812433253ul * (l ^ (l >> 30)) + i) & 0xfffffffful);
                }
            }
            index_ = 0;
            // Reset helper variables used for generation of random boolean values.
            bitBuffer_ = 0;
            bitCount_ = 0;
        }

    protected:
        /// Current index in the bytes pool.
        int index_;

        /// The bytes pool.
        unsigned int v_[R];

        /// Index indirection table giving for each index its predecessor taking table size into account.
        int iRm1_[R];

        /// Index indirection table giving for each index its second predecessor taking table size into account.
        int iRm2_[R];

        /// Index indirection table giving for each index the value index + m1 taking table size into account.
        int i1_[R];

        /// Index indirection table giving for each index the value index + m2 taking table size into account.
        int i2_[R];

        /// Index indirection table giving for each index the value index + m3 taking table size into account.
        int i3_[R];

        unsigned int *seedArray_;
        int seedArrayLength_;

        /// Constructs a new instance of the \e WellEngine, using an integer array seed.
        /// \param seedArray The initial seed (32 bits integers array), if null the seed of the generator will be related to the current time.
        /// \param seedArrayLength The length of an initial seed array.
        WellEngine(const int* seedArray = nullptr, const int seedArrayLength = 0)
        {
            // Precompute indirection index tables. These tables are used for optimizing access
            // they allow saving computations like "(j + R - 2) % R" with costly modulo operations.
            for (int j = 0; j != R; ++j)
            {
                iRm1_[j] = (j + R - 1) % R;
                iRm2_[j] = (j + R - 2) % R;
                i1_[j] = (j + M1) % R;
                i2_[j] = (j + M2) % R;
                i3_[j] = (j + M3) % R;
            }

            if (nullptr == seedArray)
            {
                seedArrayLength_ = 1;
                seedArray_ = new unsigned int[1];// {(uint)Environment.TickCount};
            }
            else
            {
                seedArrayLength_ = seedArrayLength;
                seedArray_ = new unsigned int[seedArrayLength];
                std::memcpy(seedArray_, seedArray, seedArrayLength);
            }
            init();
        }

    public:
        /// Indicates whether the WellEngine can be reset, so that it produces the same pseudo-random number sequence again.
        [[nodiscard]] bool canReset() const override { return true; }

        /// Resets the WellEngine, so that it produces the same pseudo-random number sequence again.
        void reset() override { init(); }

    };

    class Well44497A_UniformRandom : public WellEngine < 44497, 23, 481, 229, 32, 15, 1391 >
    {
    public:
        /// A next random 32-bit unsigned integer ∊[UINT_MIN, UINT_MAX].
        /// <returns>A next random 32-bit unsigned integer.</returns>
        unsigned int nextUint() override
        {
            const int indexRm1{ iRm1_[index_] };
            const int indexRm2{ iRm2_[index_] };
            const unsigned int v0{ v_[index_] };
            const unsigned int vM1{ v_[i1_[index_]] };
            const unsigned int vM2{ v_[i2_[index_]] };
            const unsigned int vM3{ v_[i3_[index_]] };

            // The values below include the errata of the original article.
            const unsigned int z0{ ((0xffff8000u & v_[indexRm1]) ^ (0x00007fffu & v_[indexRm2])) };
            const unsigned int z1{ (v0 ^ (v0 << 24)) ^ (vM1 ^ (vM1 >> 30)) };
            const unsigned int z2{ (vM2 ^ (vM2 << 10)) ^ (vM3 << 26) };
            const unsigned int z3{ z1 ^ z2 };
            const unsigned int z2Prime{ ((z2 << 9) ^ (z2 >> 23)) & 0xfbffffffu };
            const unsigned int z2Second{ ((z2 & 0x00020000u) != 0L) ? (z2Prime ^ 0xb729fcecu) : z2Prime };
            const unsigned int z4{ z0 ^ (z1 ^ (z1 >> 20)) ^ z2Second ^ z3 };

            v_[index_] = z3;
            v_[indexRm1] = z4;
            v_[indexRm2] = v_[indexRm2] & 0xffff8000u;
            index_ = indexRm1;
            return z4;
        }
    };

    class Well44497B_UniformRandom : public WellEngine < 44497, 23, 481, 229, 32, 15, 1391 >
    {

    };

    class Well19937C_UniformRandom : public WellEngine < 19937, 70, 179, 449, 32, 31, 624 >
    {

    };

    class Well19937A_UniformRandom : public WellEngine < 19937, 70, 179, 449, 32, 31, 624 >
    {

    };

    class Well1024A_UniformRandom : public WellEngine < 1024, 3, 24, 10, 32, 1, 32 >
    {

    };

    class Well512A_UniformRandom : public WellEngine < 512, 13, 9, 5, 32, 0, 16 >
    {

    };
}
