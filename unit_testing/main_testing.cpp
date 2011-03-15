#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MemoryTesting
#include <boost/test/unit_test.hpp>

#include <cmath>

#include "LookUpTable.hpp"
#include "Memory.hpp"

/**
 * To add a new test, create a new .cpp file.
 * You can inspire yourself from "unit_testing/example.cpp".
 * Note that "BOOST_TEST_MODULE" can only be defined once (in
 * this case here) and should not be re-defined in other files.
 *
 * This is based on Boost's Test Library.
 * http://www.boost.org/doc/libs/1_45_0/libs/test/doc/html/index.html
 * You'll need to install boost.
 */


BOOST_AUTO_TEST_CASE(LookupTable)
{
    double (*function)(double) = erf;

    LookUpTable<double> erf_lut(function, 0.0, 7.0, 10000, "erf()");

    BOOST_CHECK(std::abs(erf_lut.read(0.0) - 0.0)       < 1.0e-15);
    BOOST_CHECK(std::abs(erf_lut.read(3.5) - 0.9999993) < 1.0e-7);
    BOOST_CHECK(std::abs(erf_lut.read(6.0) - 1.0)       < 1.0e-15);
}
