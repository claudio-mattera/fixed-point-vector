#include <cstdlib>
#include <cmath>
#include <bitset>

namespace {

constexpr int ipow(int base, int exp, int result = 1) {
    return exp < 1
        ? result
        : ipow(base * base, exp / 2, (exp % 2) ? result * base : result);
}

} // unnamed namespace

template <
    std::size_t MAX_SIZE,
    std::size_t INTEGRAL_BITS,
    std::size_t DECIMAL_BITS,
    std::size_t SIGN_BIT>
class fixed_point_vector
{
public:
    fixed_point_vector()
    : position(0)
    {
    }

    void append(float value)
    {
        if (SIGN_BIT > 0) {
            data[position + INTEGRAL_BITS + DECIMAL_BITS] = value < 0;
        }
        value = std::abs(value);

        if (value > MAX_VALUE) {
            value = MAX_VALUE;
        }

        const float shifted = value * (1 << DECIMAL_BITS);
        const int filled = static_cast<int>(value) * (1 << DECIMAL_BITS);
        const std::bitset<INTEGRAL_BITS> integer_part = std::bitset<INTEGRAL_BITS>(value);
        const std::bitset<DECIMAL_BITS> decimal_part = std::bitset<DECIMAL_BITS>(shifted - filled);

        for (std::size_t i = 0; i < INTEGRAL_BITS; ++i) {
            data[position + i] = integer_part[i];
        }
        for (std::size_t i = 0; i < DECIMAL_BITS; ++i) {
            data[position + i + INTEGRAL_BITS] = decimal_part[i];
        }

        position += CHUNK_BITS;
    }

    std::size_t size()
    {
        return position / CHUNK_BITS;
    }

    void set(const std::size_t n, float value)
    {
        if (SIGN_BIT > 0) {
            data[n * CHUNK_BITS + INTEGRAL_BITS + DECIMAL_BITS] = value < 0;
        }
        value = std::abs(value);

        if (value > MAX_VALUE) {
            value = MAX_VALUE;
        }

        const float shifted = value * (1 << DECIMAL_BITS);
        const int filled = static_cast<int>(value) * (1 << DECIMAL_BITS);
        std::bitset<INTEGRAL_BITS> integer_part = std::bitset<INTEGRAL_BITS>(value);
        std::bitset<DECIMAL_BITS> decimal_part = std::bitset<DECIMAL_BITS>(shifted - filled);

        for (std::size_t i = 0; i < INTEGRAL_BITS; ++i) {
            data[n * CHUNK_BITS + i] = integer_part[i];
        }
        for (std::size_t i = 0; i < DECIMAL_BITS; ++i) {
            data[n * CHUNK_BITS + i + INTEGRAL_BITS] = decimal_part[i];
        }
    }

    float operator[](const std::size_t n) const
    {
        std::bitset<INTEGRAL_BITS> integer_part;
        std::bitset<DECIMAL_BITS> decimal_part;
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
    std::bitset<MAX_SIZE * CHUNK_BITS> data;
    std::size_t position;
};
