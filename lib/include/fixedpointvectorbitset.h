#include <cstdlib> // For std::size_t
#include <climits> // For CHAR_BIT
#include <stdexcept> // For std::out_of_range

template <std::size_t SIZE>
class bitset
{
private:
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
            return object.get_value(position);
        }

        reference & operator=(bool const value)
        {
            object.set_value(position, value);
            return *this;
        }

        bool operator~() const
        {
            return ~object.get_value(position);
        }

        void flip()
        {
            object.set_value(position, ~object.get_value(position));
        }

    private:
        bitset & object;
        std::size_t const position;
    };

public:
    constexpr bitset()
    : data{0}
    {
    }

    bitset(unsigned long long value)
    {
        for (std::size_t position = 0; position < SIZE; ++position) {
            (*this)[position] = 1 & (value >> position);
        }
    }

    bool operator==(const bitset<SIZE> & that) const
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (this->data[i] != that.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const bitset<SIZE> & that) const
    {
        return ! (*this == that);
    }

    unsigned long to_ulong() const
    {
        unsigned long result = 0;
        for (std::size_t position = 0; position < SIZE; ++position) {
            result |= get_value(position) << position;
        }
        return result;
    }

    constexpr bool operator[](std::size_t position) const
    {
        return get_value(position);
    }

    bool test(std::size_t position) const
    {
        ensure_in_range(position);
        return get_value(position);
    }

    bool all() const
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (!get_value(i)) {
                return false;
            }
        }
        return true;
    }

    bool any() const
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (get_value(i)) {
                return false;
            }
        }
        return true;
    }

    bool none() const
    {
        return !any();
    }

    std::size_t count() const
    {
        std::size_t c = 0;
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (get_value(i)) {
                c += 1;
            }
        }
        return c;
    }

    constexpr std::size_t size() const
    {
        return SIZE;
    }

    bitset<SIZE> & operator&=(const bitset<SIZE> & that)
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            (*this)[i] = this->get_value(i) & that.get_value(i);
        }
        return *this;
    }

    bitset<SIZE> & operator|=(const bitset<SIZE> & that)
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            (*this)[i] = this->get_value(i) | that.get_value(i);
        }
        return *this;
    }

    bitset<SIZE> & operator^=(const bitset<SIZE> & that)
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            (*this)[i] = this->get_value(i) ^ that.get_value(i);
        }
        return *this;
    }

    bitset<SIZE> operator~() const
    {
        bitset<SIZE> result;
        for (std::size_t i = 0; i < SIZE; ++i) {
            result[i] = ~this->get_value(i);
        }
        return result;
    }

    reference operator[](std::size_t position)
    {
        return reference(*this, position);
    }

    bitset<SIZE> & set()
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            set_value(i, true);
        }
        return *this;
    }

    bitset<SIZE> & set(std::size_t position, bool value = true)
    {
        ensure_in_range(position);
        set_value(position, value);
        return *this;
    }

    bitset<SIZE> & reset()
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            set_value(i, false);
        }
        return *this;
    }

    bitset<SIZE> & reset(std::size_t position)
    {
        ensure_in_range(position);
        set_value(position, false);
        return *this;
    }

    bitset<SIZE> & flip()
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            set_value(i, ~get_value(i));
        }
        return *this;
    }

    bitset<SIZE> & flip(std::size_t position)
    {
        ensure_in_range(position);
        set_value(position, ~get_value(position));
        return *this;
    }

private:
    constexpr bool get_value(std::size_t position) const
    {
        return 1 & (data[position / CHAR_BIT] >> (position % CHAR_BIT));
    }

    void set_value(std::size_t position, bool value)
    {
        const std::size_t byte_index = position / CHAR_BIT;
        const std::size_t offset = position % CHAR_BIT;
        const unsigned char byte = data[byte_index];
        const unsigned char value_c = value;
        data[byte_index] ^= (-value_c ^ byte) & (1 << offset);
    }

    void ensure_in_range(std::size_t position) const
    {
        if (position < 0 || position > SIZE) {
            throw new std::out_of_range("Out of range");
        }
    }

private:
    unsigned char data[SIZE / CHAR_BIT + SIZE % CHAR_BIT];
};
