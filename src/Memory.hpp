#ifndef INC_MEMORY_hpp
#define INC_MEMORY_hpp

#include <cstddef>  // size_t
#include <cstdlib>  // free()
#include <climits> // CHAR_BIT
#include <cmath>    // abs()

#ifdef __PGI
#include <boost/cstdint.hpp>
using namespace boost;
#else
#include <stdint.h> // (u)int64_t
// #include <cstdint> // requires C++0x
#endif // #ifdef __PGI

#include "StdCout.hpp"

namespace memory
{
    // See Git_Info.cpp (generated dynamically from Git_Info.cpp_template & Makefile.rules)
    void Log_Git_Info(std::string basename = "");
}

//const double B_to_KiB   = 1.0        / 1024.0;
//const double B_to_MiB   = B_to_KiB   / 1024.0;
//const double B_to_GiB   = B_to_MiB   / 1024.0;
//const double KiB_to_B   = 1.0        /   B_to_KiB;
//const double KiB_to_MiB = KiB_to_B   / 1024.0;
//const double KiB_to_GiB = KiB_to_MiB / 1024.0;
//const double MiB_to_B   = 1.0        /   B_to_MiB;
//const double MiB_to_KiB = 1.0        / KiB_to_MiB;
//const double MiB_to_GiB = 1.0        / 1024.0;
//const double GiB_to_B   = 1.0        /   B_to_GiB;
//const double GiB_to_KiB = 1.0        / KiB_to_GiB;
//const double GiB_to_MiB = 1.0        / MiB_to_GiB;

const double B_to_KiB   = 9.76562500000000e-04;
const double B_to_MiB   = 9.53674316406250e-07;
const double B_to_GiB   = 9.31322574615479e-10;
const double KiB_to_B   = 1024.0;
const double KiB_to_MiB = 9.76562500000000e-04;
const double KiB_to_GiB = 9.53674316406250e-07;
const double MiB_to_B   = 1048576.0;
const double MiB_to_KiB = 1024.0;
const double MiB_to_GiB = 9.76562500000000e-04;
const double GiB_to_B   = 1073741824.0;
const double GiB_to_KiB = 1048576.0;
const double GiB_to_MiB = 1024.0;

double      Bytes_to_KiBytes(const uint64_t bytes);
double      Bytes_to_MiBytes(const uint64_t bytes);
double      Bytes_to_GiBytes(const uint64_t bytes);
uint64_t    KiBytes_to_Bytes(  const double kbytes);
double      KiBytes_to_MiBytes(const double kbytes);
double      KiBytes_to_GiBytes(const double kbytes);
uint64_t    MiBytes_to_Bytes(  const double mbytes);
double      MiBytes_to_KiBytes(const double mbytes);
double      MiBytes_to_GiBytes(const double mbytes);
uint64_t    GiBytes_to_Bytes(  const double gbytes);
double      GiBytes_to_KiBytes(const double gbytes);
double      GiBytes_to_MiBytes(const double gbytes);

inline std::string Bytes_in_String(const uint64_t bytes)
{
    std::ostringstream MyStream;
    MyStream
        << bytes << " bytes ("
            << Bytes_to_KiBytes(bytes) << " KiB, "
            << Bytes_to_MiBytes(bytes) << " MiB, "
            << Bytes_to_GiBytes(bytes) << " GiB)"
        << std::flush;
    return (MyStream.str());
}

void Print_N_Times(const std::string x, const int N, const bool newline = true);

class Memory_Allocation
{
    private:
        uint64_t allocated_bytes;
        uint64_t max_allocated_bytes;

    public:
        Memory_Allocation();
        Memory_Allocation(Memory_Allocation &other);
        Memory_Allocation operator=(const uint64_t right_hand_side);
        Memory_Allocation operator=(Memory_Allocation &right_hand_side);
        Memory_Allocation operator-=(Memory_Allocation &right_hand_side);
        Memory_Allocation operator+=(Memory_Allocation &right_hand_side);
        Memory_Allocation operator-=(const uint64_t right_hand_side);
        Memory_Allocation operator+=(const uint64_t right_hand_side);
        Memory_Allocation operator-(Memory_Allocation &right_hand_side);
        Memory_Allocation operator+(Memory_Allocation &right_hand_side);

        bool        Under_Limit();
        void        Verify_Limit(const bool verbose = true);

        uint64_t    Get_Bytes_Allocated();
        double      Get_KiBytes_Allocated();
        double      Get_MiBytes_Allocated();
        double      Get_GiBytes_Allocated();

        uint64_t    Get_Max_Bytes();
        double      Get_Max_KiBytes();
        double      Get_Max_MiBytes();
        double      Get_Max_GiBytes();
        void        Set_Max_Bytes(uint64_t bytes);
        void        Set_Max_KiBytes(double kbytes);
        void        Set_Max_MiBytes(double mbytes);
        void        Set_Max_GiBytes(double gbytes);

        void        Add_Bytes_Allocated(uint64_t to_add);
        void        Free_Bytes_Allocated(uint64_t bytes_freed);

        void        Print();
};

// Declare extern here and really declare in Memory.cpp
// This allow having "allocated_memory" being global without multiply defined.
extern Memory_Allocation allocated_memory;
extern Memory_Allocation memory_to_allocate;


// **************************************************************
inline std::string MemPause(const std::string &msg = std::string(""))
{
    std::string answer;
    if (msg != std::string(""))
    {
        std_cout << msg << std::endl << std::flush;
    }
    getline(std::cin, answer);

    return answer;
}

// **************************************************************
template <class Float>
std::string Float_in_String_Binary(Float d)
/**
 * Prints binary representation of a floating point (single or double precision)
 * http://www.exploringbinary.com/displaying-the-raw-fields-of-a-floating-point-number/
 */
{
    uint64_t *float_as_int = (uint64_t *) &d;
    const int bit_size = CHAR_BIT*sizeof(Float);

    static const size_t floats_sizes[2] = {32, 64};
    static const size_t floats_exponents_sizes[2] = {8, 11};
    const size_t index = sizeof(d) / 4 - 1; // 0 for single precision, 1 for double precision.

    // +2 to allocate for the (two) spaces between the sign, exponent and mantissa.
    std::string double_in_binary(floats_sizes[index]+2, ' ');
    size_t counter = 0;

    // Print bits by bits
    for (size_t b = 0 ; b <= bit_size-1 ; b++)
    {

        if (b == 1)
            double_in_binary[counter++] = ' ';  // Space after sign field
        if (b == floats_exponents_sizes[index]+1)
            double_in_binary[counter++] = ' ';  // Space after exponent field

        // Get bit, but in reverse order. On Little Endian machines
        // (most of Intel and such), the byte with lower address
        // is the less significant. Since we want to print from
        // the most significant, we iterate from the end.
        if ((*float_as_int >> ((bit_size-1)-b)) & 1)
            double_in_binary[counter++] = '1';
        else
            double_in_binary[counter++] = '0';
    }

    return double_in_binary;
}
// **************************************************************
inline std::string Double_in_String_Binary(double d)
/**
 * Backward compatibility.
 */
{
    return Float_in_String_Binary<double>(d);
}
// **************************************************************

// **************************************************************
template <class Pointer>
void free_me(Pointer &p, const uint64_t nb = 0)
{
    if (p != NULL)
    {
        // Remove bytes from allocated memory count
        allocated_memory -= nb * sizeof(p[0]);

        // Free memory
        free(p);
    }
    p = NULL;
}

// **************************************************************
template <class Pointer>
void free_me_size(Pointer &p, const size_t size_to_remove)
{
    if (p != NULL)
    {
        // Remove bytes from allocated memory count
        allocated_memory -= size_to_remove;

        // Free memory
        free(p);
    }
    p = NULL;
}

// **************************************************************
template <class Integer>
std::string Integer_in_String_Binary(Integer n)
/**
 * Prints binary representation of an integer if any size.
 * Inspired by http://www.exploringbinary.com/displaying-the-raw-fields-of-a-floating-point-number/
 * WARNING: In C/C++, logical right shift of SIGNED integers is compiler dependant. GCC keeps the
 *          sign bit intact (instead of putting a 0).
 *          So ">>" is an arithmetic shift when the integer is signed. Unsigned are not
 *          affected (arithmetic and logical shifts are the same for unsigned integers).
 *          See http://en.wikipedia.org/wiki/Bitwise_operation#Arithmetic_shift
 */
{
                                        // Example 32 bits integers, converted from
                                        // http://www.binaryconvert.com/convert_unsigned_int.html
    const Integer i_zero    =  Integer(0);  // 00000000 00000000 00000000 00000000
    //const Integer i_ones    = ~i_zero;      // 11111111 11111111 11111111 11111111
    const Integer i_one     =  Integer(1);  // 00000000 00000000 00000000 00000001
    //const Integer i_two     =  Integer(2);  // 00000000 00000000 00000000 00000010
    //const Integer i_eigth   =  Integer(8);  // 00000000 00000000 00000000 00001000
    const Integer nb_bits_per_byte    = CHAR_BIT; // Normaly, it is 8, but could be different.
    const Integer nb_bits_per_Integer = sizeof(n)*nb_bits_per_byte;

    // Starting from the LSB being index "0", the MSB is at index "msb_position"
    const Integer msb_position  = nb_bits_per_Integer - i_one;
    const Integer msb           = i_one << msb_position;
    const Integer or_msb        = Integer(~msb);

    std::string integer_in_binary(nb_bits_per_Integer, ' ');
    Integer counter = 0;

    // Note that right shifting a signed integer migth keep the sign bit intact
    // (instead of setting it to 0) because C/C++ is implementation dependant
    // regarding right shift applied to negative signed integers. GCC will do
    // an "arithmetic right shift", meaning dividing the integer by 2. This will
    // keep the number negative (if it was). Because of this, the mask can get
    // screwed. If the Integer type is signed, first right shifting of the
    // mask of one (having an initial value of "msb" == 10000... and thus a
    // negative value) will keep the sign bit (leading to mask == 11000...) but
    // what we want is just to move the mask's bit, not keep the integer
    // reprentation "valid" (we want mask == 01000...). To fix that, after
    // right shifting the mask by one, we "AND" it (using "&") with "or_msb"
    // (or_msb == 01111...) to make sure we forget the sign bit.
    for (Integer mask = msb ; mask != i_zero ; mask = ((mask >> i_one) & or_msb ))
    {
        // If "n"'s bit at position of the mask is 0, print 0, else print 1.
        if ((mask & n) == i_zero) integer_in_binary[counter++] = '0';
        else                      integer_in_binary[counter++] = '1';
    }

    return integer_in_binary;
}

// **************************************************************
template <class T, class Integer>
T* alloc_and_check(Integer nb, const bool clear = false, const std::string &msg = "")
/**
 * Template for memory allocation.
 *  -Check that memory is not above a certain threshold.
 *  -Verify that memory allocation succeed
 */
{
    const int _max_text_width = 97;

    const size_t s = sizeof(T);
    const size_t nb_s = nb * s;

    T *p = NULL;

    Memory_Allocation mem_temp = allocated_memory;

    mem_temp += nb_s;

    if (!mem_temp.Under_Limit())
    {
        Print_N_Times("#", _max_text_width);
        std_cout
            << "WARNING!!!\n    "
            << "Trying to allocate:           ";
        std_cout.Format(20,0,'d');
        std_cout << nb << " x " << s << " bytes = " << nb_s << " bytes\n";
        std_cout << "                                               (";
        std_cout.Format(0, 3, 'g');
        std_cout
            << Bytes_to_KiBytes(nb_s) << " KiB, "
            << Bytes_to_MiBytes(nb_s) << " MiB, "
            << Bytes_to_GiBytes(nb_s) << " GiB)\n    "
            << "but memory will be over the limit:\n";
        std_cout << "                   ";
        std_cout.Format(20,0,'d');
        std_cout
            << allocated_memory.Get_Max_Bytes() << " bytes, (";
        std_cout.Format(0, 3, 'g');
        std_cout
            << allocated_memory.Get_Max_KiBytes() << " KiB, "
            << allocated_memory.Get_Max_MiBytes() << " MiB, "
            << allocated_memory.Get_Max_GiBytes() << " GiB)\n    "
            << "Current usage: ";
        std_cout.Format(20,0,'d');
        std_cout
            << allocated_memory.Get_Bytes_Allocated() << " bytes, (";
        std_cout.Format(0, 3, 'g');
        std_cout
            << allocated_memory.Get_KiBytes_Allocated() << " KiB, "
            << allocated_memory.Get_MiBytes_Allocated() << " MiB, "
            << allocated_memory.Get_GiBytes_Allocated() << " GiB)\n    "
            << "Wanted usage:  ";
        std_cout.Format(20,0,'d');
        std_cout
            << mem_temp.Get_Bytes_Allocated() << " bytes, (";
        std_cout.Format(0, 3, 'g');
        std_cout
            << mem_temp.Get_KiBytes_Allocated() << " KiB, "
            << mem_temp.Get_MiBytes_Allocated() << " MiB, "
            << mem_temp.Get_GiBytes_Allocated() << " GiB)\n";
        if (msg != "")
        {
            std_cout << "    Comment: " << msg << std::endl;
        }

        std::string answer = MemPause("Are you sure you want to continue? [y,N]");
        std_cout << std::flush;
        if ( ! (answer == "y" || answer == "Y"))
        {
            std_cout << "Exiting.\n" << std::flush;
            abort();
        }
        std_cout << "Continuing..." << std::endl << std::flush;
    }

    if (clear)
        p = static_cast<T *>(calloc(nb, s));
    else
        p = static_cast<T *>(malloc(nb_s));

    if (p == NULL)
    {
        std_cout << "ERROR!!!\n";
        std_cout << "    Allocation of ";
        std_cout.Format(20,0,'d');
        std_cout << nb << " x " << s << " bytes = " << nb_s << " bytes\n";
        std_cout << "                                               (";
        std_cout.Format(0, 3, 'g');
        std_cout
            << Bytes_to_KiBytes(nb_s) << " KiB, "
            << Bytes_to_MiBytes(nb_s) << " MiB, "
            << Bytes_to_GiBytes(nb_s) << " GiB)\n"
            << "    FAILED!!!\n";
        if (msg != "")
        {
            std_cout << "Comment: " << msg << std::endl;
        }
        std_cout << "Aborting.\n" << std::flush;
        abort();
    }

    allocated_memory += nb_s;

    return p;
}

// **************************************************************
template <class T, class Integer>
T* calloc_and_check(Integer nb, const std::string &msg = "")
/**
 * Template normally used: wrapper around alloc_and_check<T, Integer>()
 */
{
    return alloc_and_check<T, Integer>(nb, true, msg);
}

// **************************************************************
template <class T, class Integer>
T* malloc_and_check(Integer nb, const std::string &msg = "")
/**
 * Template normally used: wrapper around alloc_and_check<T, Integer>()
 */
{
    return alloc_and_check<T, Integer>(nb, false, msg);
}

// **************************************************************
template <class Integer>
void * alloc_and_check(Integer nb, size_t s, const bool clear = false, const std::string &msg = "")
/**
 * For backward compatibility.
 */
{
    void *p = NULL;
    if      (s == 1)
        p = static_cast<void *>(alloc_and_check<uint8_t, Integer>(nb,  clear, msg));
    else if (s == 2)
        p = static_cast<void *>(alloc_and_check<uint16_t, Integer>(nb, clear, msg));
    else if (s == 4)
        p = static_cast<void *>(alloc_and_check<uint32_t, Integer>(nb, clear, msg));
    else if (s == 8)
        p = static_cast<void *>(alloc_and_check<uint64_t, Integer>(nb, clear, msg));
    else
    {
        std_cout << "ERROR: Can't allocate for sizeof() == " << s << std::endl;
        abort();
    }

    return p;
}

// **************************************************************
template <class T>
bool Are_Values_Close(const T val1, const T val2, const double tolerance)
/**
 * Compare two values and return true if they are close enough.
 * Tolerance is a percentage.
 */
{
    const double percent_error = std::abs(double(val1 - val2) / double(val1)) * 100.0;

    if (percent_error < tolerance)
        return true;
    else
        return false;
}


#endif // INC_MEMORY_hpp

// ********** End of file ***************************************
