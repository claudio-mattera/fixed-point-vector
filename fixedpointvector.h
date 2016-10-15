#include <cstdlib> // For std::size_t
#include <ostream> // For std::ostream

namespace {

constexpr int ipow(int base, int exp, int result = 1) {
    return exp < 1
        ? result
        : ipow(base * base, exp / 2, (exp % 2) ? result * base : result);
}

constexpr float abs(float value)
{
    return value >= 0 ? value : -value;
}

} // unnamed namespace

#if 1
# include "bitset.h"
#else
# include <bitset>
template <std::size_t SIZE>
using bitset = std::bitset<SIZE>;
#endif

template <
    std::size_t MAX_SIZE,
    std::size_t INTEGRAL_BITS,
    std::size_t DECIMAL_BITS,
    std::size_t SIGN_BIT>
class fixed_point_vector
{
private:
    class reference
    {
    public:
        reference(fixed_point_vector & object, std::size_t position)
        : object(object),
          position(position)
        {
        }

        operator float() const
        {
            return object.get_value(position);
        }

        reference & operator=(float const value)
        {
            object.set_value(position, value);
            return *this;
        }

    private:
        fixed_point_vector & object;
        std::size_t const position;
    };

public:
    fixed_point_vector()
    : position(0)
    {
    }

    void append(float value)
    {
        set_value(position / CHUNK_BITS, value);
        position += CHUNK_BITS;
    }

    std::size_t size() const
    {
        return position / CHUNK_BITS;
    }

    float operator[](const std::size_t n) const
    {
        return get_value(n);
    }

    reference operator[](std::size_t position)
    {
        return reference(*this, position);
    }

private:
    void set_value(const std::size_t n, float value)
    {
        if (SIGN_BIT > 0) {
            data[n * CHUNK_BITS + INTEGRAL_BITS + DECIMAL_BITS] = value < 0;
        }
        value = abs(value);

        if (value > MAX_VALUE) {
            value = MAX_VALUE;
        }

        const float shifted = value * (1 << DECIMAL_BITS);
        const int filled = static_cast<int>(value) * (1 << DECIMAL_BITS);
        bitset<INTEGRAL_BITS> integer_part = bitset<INTEGRAL_BITS>(value);
        bitset<DECIMAL_BITS> decimal_part = bitset<DECIMAL_BITS>(shifted - filled);

        for (std::size_t i = 0; i < INTEGRAL_BITS; ++i) {
            data[n * CHUNK_BITS + i] = integer_part[i];
        }
        for (std::size_t i = 0; i < DECIMAL_BITS; ++i) {
            data[n * CHUNK_BITS + i + INTEGRAL_BITS] = decimal_part[i];
        }
    }

    float get_value(const std::size_t n) const
    {
        bitset<INTEGRAL_BITS> integer_part;
        bitset<DECIMAL_BITS> decimal_part;
        for (std::size_t i = 0; i < INTEGRAL_BITS; ++i) {
            integer_part[i] = data[n * CHUNK_BITS + i];
        }
        for (std::size_t i = 0; i < DECIMAL_BITS; ++i) {
            decimal_part[i] = data[n * CHUNK_BITS + i + INTEGRAL_BITS];
        }
        const float s = SIGN_BIT > 0 && data.test(n * CHUNK_BITS + INTEGRAL_BITS + DECIMAL_BITS) ? -1 : 1;
        const int i = integer_part.to_ulong();
        const int d = decimal_part.to_ulong();
        const float v = 1 << DECIMAL_BITS;
        const float t = d / v;
        return s * (i + t);
    }

private:
    static const constexpr float MAX_VALUE =
        (ipow(2, INTEGRAL_BITS) - 1.0) + (1.0 - 1.0 / ipow(2, DECIMAL_BITS));
    static const constexpr std::size_t CHUNK_BITS =
        INTEGRAL_BITS + DECIMAL_BITS + SIGN_BIT;
    bitset<MAX_SIZE * CHUNK_BITS> data;
    std::size_t position;
};

template <
    std::size_t MAX_SIZE,
    std::size_t INTEGRAL_BITS,
    std::size_t DECIMAL_BITS,
    std::size_t SIGN_BIT>
std::ostream & operator<<(
    std::ostream & stream,
    const fixed_point_vector
        <MAX_SIZE, INTEGRAL_BITS, DECIMAL_BITS, SIGN_BIT> & vector)
{
    if (vector.size() > 0) {
        stream << vector[0];
        for (std::size_t i = 1; i < vector.size(); ++i) {
            stream << ", " << vector[i];
        }
    }
    return stream;
}
