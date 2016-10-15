#include <iostream>
#include <array>
#include "fixedpointvector.h"

const constexpr std::size_t COUNT = 600;

typedef fixed_point_vector<COUNT, 5, 2, 1> vector;

int main()
{
    std::cout << "sizeof(float) = " << sizeof(float) << '\n';
    std::cout << "sizeof(double) = " << sizeof(double) << '\n';
    std::cout << "COUNT = " << COUNT << '\n';
    std::cout << "sizeof(vector<COUNT>) = " << sizeof(vector) << '\n';
    std::cout << "sizeof(std::array<float, COUNT>) = " << sizeof(std::array<float, COUNT>) << '\n';
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

    v[3] = 11.25;
    std::cout << v << '\n';
}
