#include <boost/test/unit_test.hpp>

#include <fixedpointvector.h>

BOOST_AUTO_TEST_SUITE(FixedPointVector)

BOOST_AUTO_TEST_CASE(ten_vector_size)
{
    typedef fixed_point_vector<10, 5, 2, 1> ten_vector;

    BOOST_CHECK_EQUAL(sizeof(ten_vector), 24);
}

BOOST_AUTO_TEST_CASE(one_hundred_vector_size)
{
    typedef fixed_point_vector<100, 5, 2, 1> one_hundred_vector;

    BOOST_CHECK_EQUAL(sizeof(one_hundred_vector), 112);
}

BOOST_AUTO_TEST_CASE(six_hundred_vector_size)
{
    typedef fixed_point_vector<600, 5, 2, 1> six_hundred_vector;

    BOOST_CHECK_EQUAL(sizeof(six_hundred_vector), 608);
}

BOOST_AUTO_TEST_CASE(vector_exact_elements)
{
    typedef fixed_point_vector<10, 5, 2, 1> vector;

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

    BOOST_CHECK_EQUAL(v[0], 0);
    BOOST_CHECK_EQUAL(v[1], 12.0);
    BOOST_CHECK_EQUAL(v[2], 12.25);
    BOOST_CHECK_EQUAL(v[3], -12.5);
    BOOST_CHECK_EQUAL(v[4], 12.75);
    BOOST_CHECK_EQUAL(v[9], 1);
}

BOOST_AUTO_TEST_CASE(vector_inexact_elements)
{
    typedef fixed_point_vector<10, 5, 2, 1> vector;

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

    BOOST_CHECK_EQUAL(v[5], 12.75);
    BOOST_CHECK_EQUAL(v[6], -2.0);
}

BOOST_AUTO_TEST_CASE(vector_truncated_elements)
{
    typedef fixed_point_vector<10, 5, 2, 1> vector;

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

    BOOST_CHECK_EQUAL(v[7], 31.75);
    BOOST_CHECK_EQUAL(v[8], -31.75);
}

BOOST_AUTO_TEST_SUITE_END()
