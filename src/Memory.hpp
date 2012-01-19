#ifndef INC_MEMORY_hpp
#define INC_MEMORY_hpp

#include <cstddef>  // size_t
#include <cstdlib>  // free()
#include <climits> // CHAR_BIT

#ifdef __PGI
#include <boost/cstdint.hpp>
using namespace boost;
#else
#include <stdint.h> // (u)int64_t
// #include <cstdint> // requires C++0x
#endif // #ifdef __PGI

#include "StdCout.hpp"

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
inline std::string MemPause(std::string msg = std::string(""))
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
std::string Double_in_String_Binary(double d);
std::string Float_in_String_Binary(float d);

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
void * calloc_and_check(uint64_t     nb, size_t s, std::string msg = "");
void * calloc_and_check(int64_t      nb, size_t s, std::string msg = "");
void * calloc_and_check(unsigned int nb, size_t s, std::string msg = "");
void * calloc_and_check(         int nb, size_t s, std::string msg = "");

// **************************************************************
void * malloc_and_check(uint64_t     nb, size_t s, std::string msg = "");
void * malloc_and_check(int64_t      nb, size_t s, std::string msg = "");
void * malloc_and_check(unsigned int nb, size_t s, std::string msg = "");
void * malloc_and_check(         int nb, size_t s, std::string msg = "");

#endif // INC_MEMORY_hpp

// ********** End of file ***************************************
