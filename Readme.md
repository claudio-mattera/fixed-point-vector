Fixed Point Vector
====

A C++ library for packed vectors of fixed-point numbers.

In this library, each element is represented using a fixed-point number.
A fixed number of bits are used for the integral part and for the decimal part.
An individual bit can be used for the sign, if necessary.
Moreover, elements are packed inside the vector.
E.g. if each element has size of 10 bits, it cannot fit in 1 byte.
Using 2 bytes would be enough, but it would also waste 6 bits.
Instead, inside the vector some of the bytes contain parts of multiple elements.
So, instead of using 200 bytes to individually store 100 elements, only 125 bytes are necessary.

This library consists of only header files, and it uses templates and `constexpr` expressions to reduce the runtime computational load.
However, there is a trade-off between memory efficiency and time to read/write individual elements.
This approach makes sense for memory-constrained applications that need to store a large amount of data with limited range and precision, and can sacrifice significant computational power.

This library was originally developed to store temperature readings over a long period of time on a machine with only 2 kB memory.
Temperature only ranged from -10 to 30 C and the sensors only had around 0.1 C precision.
Keeping one-minute resolution over one day required storing 1440 readings, which would have required more than 5 kB using single-precision floating-point numbers.
Using this library, instead, and using 5 bits for the integral part, 3 for the decimal part and 1 bit for the sign, allowed to pack each reading in 9 bits, totalling to only 1.6 kB for an entire day with one-minute resolution.

This library is licensed under the MIT licence.


Installation
----

This library consists of a single header file: `lib/include/fixedpointvector.h`.
It is enough to copy it anywhere it can be included in source files.
In alternative, it is possible to import it as a CMake library

Note that this library depends on `bitset` header file, which is part of the standard library.
For those C++ implementations that do not provide it, a re-implementation is also available as a single header `fixedpointvector-bitset.h`.


Usage
----

The library exposes a the template class `fixed_point_vector<MAX_SIZE, INTEGRAL_BITS, DECIMAL_BITS, SIGN_BIT` which implements a subset of the standard `array` class.
`MAX_SIZE` is the maximal number of elements that can be stored in the vector, `INTEGRAL_BITS` and `DECIMAL_BITS` sets the number of bits to use for the integral and decimal parts, and `SIGN_BIT` can be 1 or 0 depending if elements can be negative or not.
Whenever an element is added to the vector, it is truncated to the supported range and precision.

    #include <iostream>

    #include <fixedpointvector.h>

    const constexpr std::size_t COUNT = 10;
    typedef fixed_point_vector<COUNT, 5, 2, 1> vector;

    vector v;
    v.append(0);
    v.append(12.00);
    v.append(12.25);
    v.append(-12.50);
    v.append(12.75);
    v.append(12.94);
    v.append(-2.0004);
    v.append(1223.94);
    v.append(-1223.94);
    v.append(1);

    std::cout << v << '\n';
