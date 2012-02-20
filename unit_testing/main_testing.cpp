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


BOOST_AUTO_TEST_CASE(Memory)
{
    // Set maximum memory to 2 MB
    allocated_memory.Set_Max_Bytes(MiBytes_to_Bytes(2));
    // 262,144 doubles is exactly 2MB, so only allocate 262,143.
    double *array = (double *) calloc_and_check(262143, sizeof(double));
    // Get what is already allocated
    const uint64_t already_allocated_bytes = allocated_memory.Get_Bytes_Allocated();
    // Print memory information (what is allocated and maximum)
    allocated_memory.Print();
    // Make sure we are under the limit
    allocated_memory.Verify_Limit();
}

BOOST_AUTO_TEST_CASE(BinaryStrings)
{
    const float  valf = 1.23456789;
    const double vald = 1.23456789;
    const int    vali = 5;
    const std::string valf_string = Float_in_String_Binary(valf);
    const std::string vald_string = Double_in_String_Binary(vald);
    const std::string vali_string = Integer_in_String_Binary(vali);

    BOOST_CHECK(valf_string == std::string("0 01111111 00111100000011001010010"));
    BOOST_CHECK(vald_string == std::string("0 01111111111 0011110000001100101001000010100000111101111000011011"));
    BOOST_CHECK(vali_string == std::string("00000000000000000000000000000101"));
}


BOOST_AUTO_TEST_CASE(LookupTable)
{
    allocated_memory.Set_Max_Bytes(MiBytes_to_Bytes(3));
    const float  valf = 1.23456789f;
    const double vald = 1.23456789;
    const int    vali = 5;
    const std::string valf_string = Float_in_String_Binary(valf);
    const std::string vald_string = Double_in_String_Binary(vald);
    const std::string vali_string = Integer_in_String_Binary(vali);
    std::cout << "Float:  " << valf << " in binary == " << valf_string << "\n";
    std::cout << "Double: " << vald << " in binary == " << vald_string << "\n";
    std::cout << "Int:    " << vali << " in binary == " << vali_string << "\n";


    // Create object
    LookUpTable<double> cos_lut;
    // Initialize lookup table of f(x) between x == x_min and x == x_max.
    // First argument: function pointer of f(x). Can be **NULL** to initialize later.
    // Second and third arguments: x_min and x_max.
    // Fourth argument: number of points in the lut.
    // Fifth argument: name of lookup table, used when printing information.
    cos_lut.Initialize(std::cos, 0.0, 2.0*std::acos(-1.0), 10000, "Cosine lookup table");
    std::cout << "Printing table information:\n";
    cos_lut.Print();
    std::cout << "Printing table content (to standard error stream):\n";
    cos_lut.Print_Table();
    std::cout << "Read some values from lut:\n";
    const double Pi = std::acos(-1.0);
    std::cout << "cos(0)    == " << cos_lut.read(0.0)    << " (should be 1)\n";
    std::cout << "cos(pi)   == " << cos_lut.read(Pi)     << " (should be -1)\n";
    std::cout << "cos(pi/2) == " << cos_lut.read(Pi/2.0) << " (should be 0)\n";

    BOOST_CHECK(std::abs(cos_lut.read(0.0) - 1.0)   < 1.0e-7);
    BOOST_CHECK(std::abs(cos_lut.read(Pi)  + 1.0)   < 1.0e-7);
    BOOST_CHECK(std::abs(cos_lut.read(Pi/2.0))      < 1.0e-7);
}
