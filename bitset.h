#include <cstdlib> // For std::size_t
#include <climits> // For CHAR_BIT

template <std::size_t SIZE>
class bitset
{
public:
    class reference
    {
    public:
        reference(bitset & object, std::size_t position)
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
        bitset & object;
        std::size_t const position;
    };

    bitset(unsigned char value = 0)
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
