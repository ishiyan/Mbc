#pragma once

#include <limits.h>

namespace mbc::numerics::random {

    class RandomGenerator
    {
    public:
        //! A 32-bit pseudo-random unsigned integer from the interval [UINT_MIN, UINT_MAX].
        virtual unsigned int nextUint() noexcept = 0;

        //! A 32-bit pseudo-random signed integer from the interval [0, INT_MAX).
        virtual int nextInt() noexcept;

        //! A 32-bit pseudo-random signed integer from the interval [0, \p maxValue).
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p maxValue must be greater than or equal to zero.
        virtual int nextInt(int maxValue) noexcept;

        //! A 32-bit pseudo-random signed integer from the interval [\p minValue, \p maxValue).
        //! \param minValue The inclusive lower bound of the random number to be generated.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p maxValue must be greater than or equal to \p minValue.
        int nextInt(int minValue, int maxValue) noexcept;

        //! A double-precision floating point pseudo-random number from the interval [0, 1).
        virtual double nextDouble() noexcept;

        //! A double-precision floating point pseudo-random number from the interval[0, \p maxValue).
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \p maxValue must be greater than or equal to 0.
        virtual double nextDouble(double maxValue) noexcept;

        //! A double-precision floating point pseudo-random number from the interval [\p minValue, \pmaxValue).
        //! \param minValue The inclusive lower bound of the random number to be generated. The range between \p minValue and \p maxValue must be less than or equal to \e DBL_MAX.
        //! \param maxValue The exclusive upper bound of the random number to be generated. The \a maxValue must be greater than or equal to \p minValue. The range between \p minValue and \p maxValue must be less than or equal to \e DBL_MAX.
        virtual double nextDouble(double minValue, double maxValue) noexcept;

        //! A pseudo-random boolean value.
        bool nextBoolean() noexcept;

        //! Fills the elements of a specified array of bytes with pseudo-random numbers.
        //! Each element of the array of bytes is set to a random number from the interval [0, UCHAR_MAX].
        void nextBytes(unsigned char* buffer, int bufferLength) noexcept;

        //! Indicates whether the random number generator can be reset, so that it produces the same random number sequence again.
        virtual bool canReset() const noexcept { return false; }

        //! Resets the random number generator, so that it produces the same random number sequence again.
        virtual void reset() noexcept {}

        RandomGenerator(const RandomGenerator& other) noexcept;
        RandomGenerator(RandomGenerator&& other) noexcept;
        RandomGenerator& operator=(const RandomGenerator& other);
        RandomGenerator& operator=(RandomGenerator&& other) noexcept;
        virtual ~RandomGenerator() noexcept;

    protected:
        inline RandomGenerator() noexcept;

        //! The the value of the \e bitCount_ which triggers the RandomGenerator::nextUint() call.
        static const int maximalBitCount{ 32 };

        //! Used to generate up to 32 random boolean values.
        unsigned int bitBuffer_;

        //! Specifies how many random boolean values still can be generated from bitBuffer.
        int bitCount_;

    private:
        //! Represents the multiplier that computes a double-precision floating point number from the interval [0, 1) when it gets applied to a nonnegative 32-bit signed integer.
        const double intToDoubleMultiplier_{ 1. / (1. + INT_MAX) };

        //! Represents the multiplier that computes a double-precision floating point number from the interval [0, 1) when it gets applied to a 32-bit unsigned integer.
        const double uintToDoubleMultiplier_{ 1. / (1. + UINT_MAX) };
    };

    inline int RandomGenerator::nextInt() noexcept
    {
        const int result{ static_cast<int>(nextUint() >> 1) };
        // Exclude INT_MAX value from the range of return values.
        if (result == INT_MAX)
            return nextInt();
        return result;
    }

    inline int RandomGenerator::nextInt(const int maxValue) noexcept
    {
        // The shift operation and extra int cast before the first multiplication give better performance.
        // See comment in the nextDouble().
        return static_cast<int>(static_cast<int>(nextUint() >> 1) * intToDoubleMultiplier_ * maxValue);
    }

    inline int RandomGenerator::nextInt(const int minValue, const int maxValue) noexcept
    {
        const int range{ maxValue - minValue };
        if (range < 0)
        {
            // The range is greater than INT_MAX, so we have to use slower floating point arithmetic.
            // Also all 32 random bits (unsigned int) have to be used which again is slower (see comment in nextDouble()).
            return minValue + static_cast<int>(nextUint() * uintToDoubleMultiplier_ * (static_cast<double>(maxValue) - minValue));
        }
        // 31 random bits (int) will suffice which allows us to shift and cast to an int before the first multiplication and gain better performance.
        // See comment in nextDouble().
        return minValue + static_cast<int>(static_cast<int>(nextUint() >> 1) * intToDoubleMultiplier_ * range);
    }

    inline double RandomGenerator::nextDouble() noexcept
    {
        // Here a ~2x speed improvement is gained by computing a value that can be cast to an int 
        // before casting to a double to perform the multiplication.
        // Casting a double from an int is a lot faster than from an unsigned int and the extra shift operation 
        // and cast to an int are very fast (the allocated bits remain the same), so overall there's 
        // a significant performance improvement.
        return static_cast<int>(nextUint() >> 1) * intToDoubleMultiplier_;
    }

    inline double RandomGenerator::nextDouble(const double maxValue) noexcept
    {
        // The shift operation and extra int cast before the first multiplication give better performance.
        // See comment in nextDouble().
        return static_cast<int>(nextUint() >> 1) * intToDoubleMultiplier_ * maxValue;
    }

    inline double RandomGenerator::nextDouble(const double minValue, const double maxValue) noexcept
    {
        const double range{ maxValue - minValue };
        // The shift operation and extra int cast before the first multiplication give better performance.
        // See comment in nextDouble().
        return minValue + static_cast<int>(nextUint() >> 1) * intToDoubleMultiplier_ * range;
    }

    inline bool RandomGenerator::nextBoolean() noexcept
    {
        // Buffers 32 random bits for future calls, so the random number generator
        // is only invoked once in every 32 calls.
        if (bitCount_ >= maximalBitCount)
        {
            // Generate 32 more bits (1 uint) and store it for future calls.
            bitBuffer_ = nextUint();
            // Reset the bitCount and use rightmost bit of buffer to generate random bool.
            bitCount_ = 1;
            return (bitBuffer_ & 0x1) == 1;
        }
        // Increase the bitCount and use rightmost bit of shifted buffer to generate random bool.
        ++bitCount_;
        return ((bitBuffer_ >>= 1) & 0x1) == 1;
    }

    inline void RandomGenerator::nextBytes(unsigned char* buffer, const int bufferLength) noexcept
    {
        // Fill the buffer with 4 bytes (1 uint) at a time.
        int i{ 0 };
        unsigned int u;
        while (i < bufferLength - 3)
        {
            u = nextUint();
            buffer[i] = static_cast<unsigned char>(u);
            buffer[++i] = static_cast<unsigned char>(u >> 8);
            buffer[++i] = static_cast<unsigned char>(u >> 16);
            buffer[++i] = static_cast<unsigned char>(u >> 24);
            ++i;
        }
        // Fill up any remaining bytes in the buffer.
        if (i < bufferLength)
        {
            u = nextUint();
            buffer[i] = static_cast<unsigned char>(u);
            if (++i < bufferLength)
            {
                buffer[i] = static_cast<unsigned char>(u >> 8);
                if (++i < bufferLength)
                {
                    buffer[i] = static_cast<unsigned char>(u >> 16);
                    if (++i < bufferLength)
                        buffer[i] = static_cast<unsigned char>(u >> 24);
                }
            }
        }
    }

    inline RandomGenerator::~RandomGenerator() noexcept
    {}

    inline RandomGenerator::RandomGenerator() noexcept :
        bitBuffer_{ 0 },
        bitCount_{ maximalBitCount }
    {}

    inline RandomGenerator::RandomGenerator(const RandomGenerator& other) noexcept :
        bitBuffer_{ other.bitBuffer_ },
        bitCount_{ other.bitCount_ }
    {}

    inline RandomGenerator::RandomGenerator(RandomGenerator&& other) noexcept :
        bitBuffer_{ other.bitBuffer_ },
        bitCount_{ other.bitCount_ }
    {}

    inline RandomGenerator& RandomGenerator::operator=(const RandomGenerator& other)
    {
        if (this != &other)
        {
            bitBuffer_ = other.bitBuffer_;
            bitCount_ = other.bitCount_;
        }
        return *this;
    }

    inline RandomGenerator& RandomGenerator::operator=(RandomGenerator&& other) noexcept
    {
        if (this != &other)
        {
            // Move assignment operator.
            // 1. Release the current object’s resources.
            // 2. Pilfer other’s resource.
            bitBuffer_ = other.bitBuffer_;
            bitCount_ = other.bitCount_;
            // 3. Reset other resources.
        }
        return *this;
    }
}
