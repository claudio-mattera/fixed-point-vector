#include <cstdlib>
#include <climits> // For CHAR_BIT
#include <bitset>

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

template <std::size_t SIZE>
class custom_bitset
{
public:
    class reference
    {
    public:
        reference(custom_bitset & object, std::size_t position)
        : object(object),
          position(position)
        {
        }

        operator bool() const
        {
            return object.get(position);
        }

        reference & operator=(bool const value)
        {
            object.set(position, value);
            return *this;
        }

    private:
        custom_bitset & object;
        std::size_t const position;
    };

    custom_bitset(unsigned char value = 0)
    {
        for (std::size_t position = 0; position < SIZE; ++position) {
            set(position, 1 & (value >> position));
        }
    }

    unsigned long to_ulong() const
    {
        unsigned long result = 0;
        for (std::size_t position = 0; position < SIZE; ++position) {
            result |= get(position) << position;
        }
        return result;
    }

    bool operator[](std::size_t position) const
    {
        return get(position);
    }

    bool test(std::size_t position) const
    {
        return get(position);
    }

    bool get(std::size_t position) const
    {
        const size_t byte_index = position / CHAR_BIT;
        const size_t offset = position % CHAR_BIT;
        const unsigned char byte = data[byte_index];
        return 1 & (byte >> offset);
    }

    reference operator[](std::size_t position)
    {
        return reference(*this, position);
    }

    void set(std::size_t position, bool value)
    {
        const size_t byte_index = position / CHAR_BIT;
        const size_t offset = position % CHAR_BIT;
        const unsigned char byte = data[byte_index];
        const unsigned char value_c = value;
        data[byte_index] ^= (-value_c ^ byte) & (1 << offset);
    }

private:
    unsigned char data[SIZE / CHAR_BIT + SIZE % CHAR_BIT];
};

template <std::size_t SIZE>
using bitset = custom_bitset<SIZE>;

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
        value = abs(value);

        if (value > MAX_VALUE) {
            value = MAX_VALUE;
        }

        const float shifted = value * (1 << DECIMAL_BITS);
        const int filled = static_cast<int>(value) * (1 << DECIMAL_BITS);
        const bitset<INTEGRAL_BITS> integer_part = bitset<INTEGRAL_BITS>(value);
        const bitset<DECIMAL_BITS> decimal_part = bitset<DECIMAL_BITS>(shifted - filled);

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

    float operator[](const std::size_t n) const
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
