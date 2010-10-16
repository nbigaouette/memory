// **************************************************************
//              Keep track of memory allocation
// **************************************************************

#include <limits> // std::numeric_limits<>::max()
#include <cstdlib> // abort()

#include "Memory.hpp"
// #include "General.hpp"
// #include "Constants.hpp"

const int max_text_width = 97;

Memory_Allocation allocated_memory;
Memory_Allocation memory_to_allocate;

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

void        Print_Factors();

void Print_N_Times(const std::string x, const int N, const bool newline = true);

// **************************************************************
void Print_N_Times(const std::string x, const int N, const bool newline)
{
    for (int i = 0 ; i < N ; i++)
    {
        std_cout << x;
    }

    if (newline)
        std_cout << "\n";
}

// **************************************************************
void * calloc_and_check(unsigned int nb, size_t s, std::string msg)
{
    return calloc_and_check((uint64_t) nb, s, msg);
}
// **************************************************************
void * calloc_and_check(int nb, size_t s, std::string msg)
{
    return calloc_and_check((int64_t) nb, s, msg);
}

// **************************************************************
void * calloc_and_check(int64_t nb, size_t s, std::string msg)
{
    if (nb < 0)
    {
        std_cout << "nb CANNOT be < 0!!!\n" << std::flush;
        abort();
    }
    return calloc_and_check((uint64_t) nb, s, msg);
}

// **************************************************************
void * calloc_and_check(uint64_t nb, size_t s, std::string msg)
{
    void *p = NULL;
    const uint64_t nb_s = nb * s;

    Memory_Allocation mem_temp = allocated_memory;

    mem_temp += nb_s;

    if (!mem_temp.Under_Limit())
    {
        Print_N_Times("#", max_text_width);
        std_cout
            << "WARNING!!!" << std::endl << "    "
            << "Trying to allocate " << nb << " x " << s << " bytes = " << nb_s << "bytes "
            << "("  << double(nb_s) * B_to_KiB << " KiB, "
                    << double(nb_s) * B_to_MiB << " MiB, "
                    << double(nb_s) * B_to_GiB << " GiB)" << std::endl << "    "
            << "but memory will be over the limit: "
            << "("  << allocated_memory.Get_Max_Bytes() << " bytes, "
                    << allocated_memory.Get_Max_KiBytes() << " KiB, "
                    << allocated_memory.Get_Max_MiBytes() << " MiB, "
                    << allocated_memory.Get_Max_GiBytes() << " GiB)" << std::endl << "    "
            << "Current usage: "
                << allocated_memory.Get_Bytes_Allocated() << " bytes, "
                << allocated_memory.Get_KiBytes_Allocated() << " KiB, "
                << allocated_memory.Get_MiBytes_Allocated() << " MiB, "
                << allocated_memory.Get_GiBytes_Allocated() << " GiB)" << std::endl << "    "
            << "Wanted usage: "
                << mem_temp.Get_Bytes_Allocated() << " bytes, "
                << mem_temp.Get_KiBytes_Allocated() << " KiB, "
                << mem_temp.Get_MiBytes_Allocated() << " MiB, "
                << mem_temp.Get_GiBytes_Allocated() << " GiB)" << std::endl;
        if (msg != "")
        {
            std_cout << "    Comment: " << msg << std::endl;
        }

        std::string answer = MemPause("Are you sure you want to continue? [y,N]");
        if ( ! (answer == "y" || answer == "Y"))
        {
            std_cout << "Exiting.\n" << std::flush;
            abort();
        }
        std_cout << "Continuing..." << std::endl << std::flush;
    }

    p = calloc(nb, s);
    if (p == NULL)
    {
        std_cout
            << "Allocation of " << nb << " x " << s << " bytes = " << nb_s
            << " bytes failed" << std::endl
            << "("  << double(nb_s) * B_to_KiB << " KiB, "
                    << double(nb_s) * B_to_MiB << " MiB, "
                    << double(nb_s) * B_to_GiB << " GiB)" << std::endl
            << "p = " << p << std::endl;
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
void * malloc_and_check(unsigned int nb, size_t s, std::string msg)
{
    return malloc_and_check((uint64_t) nb, s, msg);
}
// **************************************************************
void * malloc_and_check(int nb, size_t s, std::string msg)
{
    return malloc_and_check((int64_t) nb, s, msg);
}

// **************************************************************
void * malloc_and_check(int64_t nb, size_t s, std::string msg)
{
    if (nb < 0)
    {
        std_cout << "nb CANNOT be < 0!!!\n" << std::flush;
        abort();
    }
    return malloc_and_check((uint64_t) nb, s, msg);
}

// **************************************************************
void * malloc_and_check(uint64_t nb, size_t s, std::string msg)
{
    void *p = NULL;
    const uint64_t nb_s = nb * s;

    Memory_Allocation mem_temp = allocated_memory;

    mem_temp += nb_s;

    if (!mem_temp.Under_Limit())
    {
        Print_N_Times("#", max_text_width);
        std_cout
            << "WARNING!!!" << std::endl << "    "
            << "Trying to allocate " << nb << " x " << s << " bytes = " << nb_s << "bytes "
            << "("  << double(nb_s) * B_to_KiB << " KiB, "
                    << double(nb_s) * B_to_MiB << " MiB, "
                    << double(nb_s) * B_to_GiB << " GiB)" << std::endl << "    "
            << "but memory will be over the limit: "
            << "("  << allocated_memory.Get_Max_Bytes() << " bytes, "
                    << allocated_memory.Get_Max_KiBytes() << " KiB, "
                    << allocated_memory.Get_Max_MiBytes() << " MiB, "
                    << allocated_memory.Get_Max_GiBytes() << " GiB)" << std::endl << "    "
            << "Current usage: "
                << allocated_memory.Get_Bytes_Allocated() << " bytes, "
                << allocated_memory.Get_KiBytes_Allocated() << " KiB, "
                << allocated_memory.Get_MiBytes_Allocated() << " MiB, "
                << allocated_memory.Get_GiBytes_Allocated() << " GiB)" << std::endl << "    "
            << "Wanted usage: "
                << mem_temp.Get_Bytes_Allocated() << " bytes, "
                << mem_temp.Get_KiBytes_Allocated() << " KiB, "
                << mem_temp.Get_MiBytes_Allocated() << " MiB, "
                << mem_temp.Get_GiBytes_Allocated() << " GiB)" << std::endl;
        if (msg != "")
        {
            std_cout << "    Comment: " << msg << std::endl;
        }

        std::string answer = MemPause("Are you sure you want to continue? [y,N]");
        if ( ! (answer == "y" || answer == "Y"))
        {
            std_cout << "Exiting." << std::endl << std::flush;
            abort();
        }
        std_cout << "Continuing..." << std::endl << std::flush;
    }

    p = malloc(nb * s);
    if (p == NULL)
    {
        std_cout
            << "Allocation of " << nb << " x " << s << " bytes = " << nb_s
            << " bytes failed" << std::endl
            << "("  << double(nb_s) * B_to_KiB << " KiB, "
                    << double(nb_s) * B_to_MiB << " MiB, "
                    << double(nb_s) * B_to_GiB << " GiB)" << std::endl
            << "p = " << p << std::endl;
        if (msg != "")
        {
            std_cout << "Comment: " << msg << std::endl;
        }
        std_cout << "Aborting" << std::endl << std::flush;
        abort();
    }

    allocated_memory += nb_s;

    return p;
}

// **************************************************************
Memory_Allocation::Memory_Allocation()
{
    allocated_bytes     = 0;
    max_allocated_bytes = std::numeric_limits<uint64_t>::max();
}

// **************************************************************
Memory_Allocation::Memory_Allocation(Memory_Allocation &other)
{
    allocated_bytes     = other.Get_Bytes_Allocated();
    max_allocated_bytes = other.Get_Max_Bytes();
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator=(const uint64_t right_hand_side)
{
    allocated_bytes     = right_hand_side;

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator=(Memory_Allocation &right_hand_side)
{
    allocated_bytes     = right_hand_side.Get_Bytes_Allocated();
    max_allocated_bytes = right_hand_side.Get_Max_Bytes();

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator-=(Memory_Allocation &right_hand_side)
{
    allocated_bytes -= right_hand_side.Get_Bytes_Allocated();

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator+=(Memory_Allocation &right_hand_side)
{
    allocated_bytes += right_hand_side.Get_Bytes_Allocated();

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator-=(const uint64_t right_hand_side)
{
    allocated_bytes -= right_hand_side;

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator+=(const uint64_t right_hand_side)
{
    allocated_bytes += right_hand_side;

    return *this;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator-(Memory_Allocation &right_hand_side)
{
    Memory_Allocation temp;

    temp.allocated_bytes = Get_Bytes_Allocated() - right_hand_side.Get_Bytes_Allocated();

    return temp;
}

// **************************************************************
Memory_Allocation Memory_Allocation::operator+(Memory_Allocation &right_hand_side)
{
    Memory_Allocation temp;

    temp.allocated_bytes = Get_Bytes_Allocated() + right_hand_side.Get_Bytes_Allocated();

    return temp;
}

// **************************************************************
bool Memory_Allocation::Under_Limit()
{
    if (allocated_bytes < max_allocated_bytes)
        return true;
    else
        return false;
}

// **************************************************************
void Memory_Allocation::Verify_Limit(const bool verbose)
{
    if (!Under_Limit())
    {
        Print_N_Times("#", max_text_width);
        std_cout
            << "WARNING!!!" << std::endl << "    "
            << "Memory needed:          "
                << Get_Bytes_Allocated()   << " bytes, "
                << Get_KiBytes_Allocated() << " KiB, "
                << Get_MiBytes_Allocated() << " MiB, "
                << Get_GiBytes_Allocated() << " GiB" << std::endl << "    "
            << "will be over the limit: "
                << Get_Max_Bytes()   << " bytes, "
                << Get_Max_KiBytes() << " KiB, "
                << Get_Max_MiBytes() << " MiB, "
                << Get_Max_GiBytes() << " GiB" << std::endl << "    "
            << "Currently using:        "
                << allocated_memory.Get_Bytes_Allocated()   << " bytes, "
                << allocated_memory.Get_KiBytes_Allocated() << " KiB, "
                << allocated_memory.Get_MiBytes_Allocated() << " MiB, "
                << allocated_memory.Get_GiBytes_Allocated() << " GiB" << std::endl;

        std::string answer = MemPause("Are you sure you want to continue? [y,N]");
        if ( ! (answer == "y" || answer == "Y"))
        {
            std_cout << "Exiting." << std::endl << std::flush;
            abort();
        }
        std_cout << "Continuing..." << std::endl << std::flush;
    } else {
        if (verbose)
        {
            Print_N_Times("#", max_text_width);
            std_cout
                << "Code's expected memory usage: "
                    << Get_Bytes_Allocated()   << " bytes, "
                    << Get_KiBytes_Allocated() << " KiB, "
                    << Get_MiBytes_Allocated() << " MiB, "
                    << Get_GiBytes_Allocated() << " GiB" << std::endl
                << "Limit: "
                    << Get_Max_Bytes()   << " bytes, "
                    << Get_Max_KiBytes() << " KiB, "
                    << Get_Max_MiBytes() << " MiB, "
                    << Get_Max_GiBytes() << " GiB" << std::endl;
            Print_N_Times("#", max_text_width);
        }
    }
}

// **************************************************************
uint64_t Memory_Allocation::Get_Bytes_Allocated()
{
    return allocated_bytes;
}

// **************************************************************
double Memory_Allocation::Get_KiBytes_Allocated()
{
    return Bytes_to_KiBytes(Get_Bytes_Allocated());
}

// **************************************************************
double Memory_Allocation::Get_MiBytes_Allocated()
{
    return Bytes_to_MiBytes(Get_Bytes_Allocated());
}

// **************************************************************
double Memory_Allocation::Get_GiBytes_Allocated()
{
    return Bytes_to_GiBytes(Get_Bytes_Allocated());
}

// **************************************************************
void Memory_Allocation::Add_Bytes_Allocated(uint64_t to_add)
{
    allocated_bytes += to_add;
}

// **************************************************************
void Memory_Allocation::Free_Bytes_Allocated(uint64_t bytes_freed)
{
    allocated_bytes -= bytes_freed;
}

// **************************************************************
uint64_t Memory_Allocation::Get_Max_Bytes()
{
    return max_allocated_bytes;
}

// **************************************************************
double Memory_Allocation::Get_Max_KiBytes()
{
    return Bytes_to_KiBytes(max_allocated_bytes);
}

// **************************************************************
double Memory_Allocation::Get_Max_MiBytes()
{
    return Bytes_to_MiBytes(max_allocated_bytes);
}

// **************************************************************
double Memory_Allocation::Get_Max_GiBytes()
{
    return Bytes_to_GiBytes(max_allocated_bytes);
}

// **************************************************************
void Memory_Allocation::Set_Max_Bytes(uint64_t bytes)
{
    max_allocated_bytes = bytes;
}

// **************************************************************
void Memory_Allocation::Set_Max_KiBytes(double kbytes)
{
    max_allocated_bytes = KiBytes_to_Bytes(kbytes);
}

// **************************************************************
void Memory_Allocation::Set_Max_MiBytes(double mbytes)
{
    max_allocated_bytes = MiBytes_to_Bytes(mbytes);
}

// **************************************************************
void Memory_Allocation::Set_Max_GiBytes(double gbytes)
{
    max_allocated_bytes = GiBytes_to_Bytes(gbytes);
}

// **************************************************************
void Memory_Allocation::Print()
{
    std_cout
        << "Memory allocated: "
            << Get_Bytes_Allocated()   << " bytes ("
            << Get_KiBytes_Allocated() << " KiB, "
            << Get_MiBytes_Allocated() << " MiB, "
            << Get_GiBytes_Allocated() << " GiB)" << std::endl
        << "Maximum memory allowed: "
            << Get_Max_Bytes()   << " bytes ("
            << Get_Max_KiBytes() << " KiB, "
            << Get_Max_MiBytes() << " MiB, "
            << Get_Max_GiBytes() << " GiB)" << std::endl;
}

// **************************************************************
double Bytes_to_KiBytes(const uint64_t bytes)
{
    return double(bytes) * B_to_KiB;
}

// **************************************************************
double Bytes_to_MiBytes(const uint64_t bytes)
{
    return double(bytes) * B_to_MiB;
}

// **************************************************************
double Bytes_to_GiBytes(const uint64_t bytes)
{
    return double(bytes) * B_to_GiB;
}

// **************************************************************
uint64_t KiBytes_to_Bytes(const double kbytes)
{
    return (uint64_t) (kbytes * KiB_to_B);
}

// **************************************************************
double KiBytes_to_MiBytes(const double kbytes)
{
    return kbytes * KiB_to_MiB;
}

// **************************************************************
double KiBytes_to_GiBytes(const double kbytes)
{
    return kbytes * KiB_to_GiB;
}

// **************************************************************
uint64_t MiBytes_to_Bytes(const double mbytes)
{
    return (uint64_t) (mbytes * MiB_to_B);
}

// **************************************************************
double MiBytes_to_KiBytes(const double mbytes)
{
    return mbytes * MiB_to_KiB;
}

// **************************************************************
double MiBytes_to_GiBytes(const double mbytes)
{
    return mbytes * MiB_to_GiB;
}

// **************************************************************
uint64_t GiBytes_to_Bytes(const double gbytes)
{
    return (uint64_t) (gbytes * GiB_to_B);
}

// **************************************************************
double GiBytes_to_KiBytes(const double gbytes)
{
    return gbytes * GiB_to_KiB;
}

// **************************************************************
double GiBytes_to_MiBytes(const double gbytes)
{
    return gbytes * GiB_to_MiB;
}

// **************************************************************
void Print_Factors()
{
    std_cout
        << "B_to_KiB   =" << B_to_KiB   << "\n"
        << "B_to_MiB   =" << B_to_MiB   << "\n"
        << "B_to_GiB   =" << B_to_GiB   << "\n"
        << "KiB_to_B   =" << KiB_to_B   << "\n"
        << "KiB_to_MiB =" << KiB_to_MiB << "\n"
        << "KiB_to_GiB =" << KiB_to_GiB << "\n"
        << "MiB_to_B   =" << MiB_to_B   << "\n"
        << "MiB_to_KiB =" << MiB_to_KiB << "\n"
        << "MiB_to_GiB =" << MiB_to_GiB << "\n"
        << "GiB_to_B   =" << GiB_to_B   << "\n"
        << "GiB_to_KiB =" << GiB_to_KiB << "\n"
        << "GiB_to_MiB =" << GiB_to_MiB << "\n"
    ;
}

// ********** End of file ***************************************
