#ifndef INC_LUT_HPP
#define INC_LUT_HPP

#include <string>

#include "Memory.hpp"
#include "StdCout.hpp"

template <class Double>
class LookUpTable
{
    private:
    std::string name;
    int n;              // Table size (number of points)
    Double range_min;   // Minimum value of the sampling range
    Double range_max;   // Maximum value of the sampling range
    Double dx;          // Step size (physical distance between two points)
    Double inv_dx;      // 1/(step size)
    Double *table;      // Array that contains the values

    public:
    LookUpTable()
    {
        n           = 0;
        range_min   = 0.0;
        range_max   = 0.0;
        dx          = 0.0;
        inv_dx      = 0.0;
        table       = NULL;
    }

    // **************************************************************
    LookUpTable(Double (*function)(Double),
                        const Double _range_min, const Double _range_max,
                        const int _n, const std::string _name)
    {
        Initialize(function, _range_min, _range_max, _n, _name);
    }

    // **************************************************************
    int Get_n()
    {
        return n;
    }

    // **************************************************************
    const Double* Get_Pointer() const
    {
        return table;
    }

    // **************************************************************
    Double Table(const int i)
    {
#ifdef YDEBUG
        assert(i < n);
#endif // #ifdef YDEBUG
        return table[i];
    }

    // **************************************************************
    Double Get_Min()
    {
        return range_min;
    }

    // **************************************************************
    Double Get_Max()
    {
        return range_max;
    }

    // **************************************************************
    void Initialize(Double (*function)(Double),
                    const Double _range_min, const Double _range_max,
                    const int _n, const std::string _name)
    {
        name        = _name;
        n           = _n;
        range_min   = _range_min;
        range_max   = _range_max;

        dx          = (range_max - range_min) / Double(n);
        inv_dx      = Double(1.0) / dx;

        table       = (Double *) calloc_and_check(n+1, sizeof(Double), "LookUpTable");

        Print();

        std_cout << "Building lookup table table \"" << _name << "\"..." << std::flush;
        Double x = 0.0;
        int percentage = 0;
        for (int i = 0 ; i <= n ; i++)
        {
            x        = Double(i)*dx + range_min;
            table[i] = function(x);
            percentage = int(Double(i) / Double(n) * 100.0);
            if ((percentage % 2) == 0)
                printf(" %3d %%\b\b\b\b\b\b", percentage);
            fflush(stdout);
        }
        std_cout << " Done.   \n" << std::flush;
    }

    // **************************************************************
    void Print()
    {
        Double memsize = Double(n+1) * sizeof(Double);
        char suffix;
        if(memsize >= 1.024e3)
        {
            memsize *= Double(1.0/1.024e3);
            suffix = 'k';
        }
        if(memsize >= 1.024e3)
        {
            memsize *= Double(1.0/1.024e3);
            suffix = 'M';
        }

        std_cout
            << "Lookup table information:\n"
            << "    Name:               " << name << "\n"
            << "    Range:              [" << range_min << ", " << range_max << "]\n"
            << "    Number of points:   " << n << "\n"
            << "    dx:                 " << dx << "\n"
            << "    Size:               " << memsize << " " << suffix << "\n";
    }

    // **************************************************************
    void Print_Table()
    {
        for (int i = 0 ; i < n ; i++)
        {
            std_cout << i << "  "; std_cout.Format(20,10,'f'); std_cout << Table(i) << "\n";
        }
    }

    // **************************************************************
    inline Double read(const Double x)
    /**
     *   Reads the table and returns an interpolated (linear) value at point x.
     */
    {
        const Double xnorm = (x - range_min)*inv_dx;
        const int i        = int(xnorm);
#ifdef YDEBUG
        assert(i < n);
#endif // #ifdef YDEBUG
        return table[i] + (table[i+1]-table[i])*(xnorm-Double(i));
    }

    // **************************************************************
    ~LookUpTable()
    {
        free_me(table, n);
    }
};

#endif
