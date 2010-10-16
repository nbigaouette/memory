#ifndef INC_MEMORY_hpp
#define INC_MEMORY_hpp

#include <cstddef>  // size_t
#include <stdint.h> // uint64_t
#include <cstdlib>  // free()

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
        std_cout << msg << std::endl;
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
