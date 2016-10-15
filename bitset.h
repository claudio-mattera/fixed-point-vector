#include <cstdlib> // For std::size_t
#include <climits> // For CHAR_BIT
#include <stdexcept> // For std::out_of_range

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
            object.set_value(position, value);
            return *this;
        }

        bool operator~() const
        {
            return ~object.get(position);
        }

        void flip()
        {
            object.set_value(position, ~object.get(position));
        }

    private:
        bitset & object;
        std::size_t const position;
    };

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
            result |= get(position) << position;
        }
        return result;
    }

    constexpr bool operator[](std::size_t position) const
    {
        return get(position);
    }

    bool test(std::size_t position) const
    {
        ensure_in_range(position);
        return get(position);
    }

    constexpr bool get(std::size_t position) const
    {
        return 1 & (data[position / CHAR_BIT] >> (position % CHAR_BIT));
    }

    bool all() const
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (!get(i)) {
                return false;
            }
        }
        return true;
    }

    bool any() const
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (get(i)) {
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
            if (get(i)) {
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
            (*this)[i] = this->get(i) & that.get(i);
        }
        return *this;
    }

    bitset<SIZE> & operator|=(const bitset<SIZE> & that)
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            (*this)[i] = this->get(i) | that.get(i);
        }
        return *this;
    }

    bitset<SIZE> & operator^=(const bitset<SIZE> & that)
    {
        for (std::size_t i = 0; i < SIZE; ++i) {
            (*this)[i] = this->get(i) ^ that.get(i);
        }
        return *this;
    }

    bitset<SIZE> operator~() const
    {
        bitset<SIZE> result;
        for (std::size_t i = 0; i < SIZE; ++i) {
            result[i] = ~this->get(i);
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
            set_value(i, ~get(i));
        }
        return *this;
    }

    bitset<SIZE> & flip(std::size_t position)
    {
        ensure_in_range(position);
        set_value(position, ~get(position));
        return *this;
    }

    void set_value(std::size_t position, bool value)
    {
        const size_t byte_index = position / CHAR_BIT;
        const size_t offset = position % CHAR_BIT;
        const unsigned char byte = data[byte_index];
        const unsigned char value_c = value;
        data[byte_index] ^= (-value_c ^ byte) & (1 << offset);
    }

private:
    void ensure_in_range(std::size_t position) const
    {
        if (position < 0 || position > SIZE) {
            throw new std::out_of_range("Out of range");
        }
    }

private:
    unsigned char data[SIZE / CHAR_BIT + SIZE % CHAR_BIT];
};
