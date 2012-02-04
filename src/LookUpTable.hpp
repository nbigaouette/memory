#ifndef INC_LUT_HPP
#define INC_LUT_HPP

//const bool verbose = true;
const bool verbose = false;

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
    bool is_initialized; // Is the look up table initialized?
    Double (*function)(Double); // Function pointer. Needs to take only one Double parameter and return a Double: function(x)

    public:
    LookUpTable()
    {
        n           = 0;
        range_min   = 0.0;
        range_max   = 0.0;
        dx          = 0.0;
        inv_dx      = 0.0;
        table       = NULL;
        function    = NULL;
        is_initialized = false;
    }

    // **************************************************************
    LookUpTable(Double (*_function)(Double),
                const Double _range_min, const Double _range_max,
                const int _n, const std::string _name, Double *_table = NULL)
    {
        Initialize(_function, _range_min, _range_max, _n, _name, _table);
    }

    // **************************************************************
    LookUpTable(const LookUpTable &other_lut)
    /**
     * Copy constructor. Needed to allocate new memory and preventing double free corruptions.
     */
    {
        Initialize(other_lut.function, other_lut.range_min, other_lut.range_max, other_lut.n, other_lut.name);

        // Now copy the other_lut's table values, only if if was initialized.
        if (other_lut.table != NULL)
            for (int i = 0 ; i < n ; i++)
                Set(i, other_lut.Table(i));
    }

    // **************************************************************
    int     Get_n()                     { return n;         }
    Double  Get_dx()                    { return dx;        }
    Double  Get_inv_dx()                { return inv_dx;    }
    Double  Get_XMin()                  { return range_min; }
    Double  Get_XMax()                  { return range_max; }
    Double  Get_x_from_i(const int i)   { return Double(i)*dx + range_min; }
    const Double* Get_Pointer() const   { return table;     }

    // **************************************************************
    Double Table(const int i) const
    {
#ifdef YDEBUG
        assert(i < n);
#endif // #ifdef YDEBUG
        return table[i];
    }

    // **************************************************************
    void Initialize(Double (*_function)(Double),
                    const Double _range_min, const Double _range_max,
                    const int _n, const std::string _name, Double *_table = NULL)
    {
        is_initialized = true;

        function = _function;
        name        = _name;
        n           = _n;
        range_min   = _range_min;
        range_max   = _range_max;

        if (_table != NULL)
        {
            table   = _table;
        }
        else
        {
            table   = (Double *) calloc_and_check(n, sizeof(Double), "LookUpTable");
        }

        /*
         *  Example:
         *      n   = 6 points
         *      n-1 = 5 intervals
         *      dx = (xmax - xmin) / nb_intervals = (xmax - xmin) / (n-1)
         *                          x               x
         *                  x               x
         *          x
         *  x________________________________________
         *  |       |       |       |       |       |
         * xmin                                    xmax
         */
        dx          = (range_max - range_min) / Double(n-1);
        inv_dx      = Double(1.0) / dx;

        if (verbose)
        {
            Print();

            std_cout << "Building lookup table table \"" << _name << "\"..." << std::flush;
        }
        Double x = 0.0;
        int percentage = 0;

        if (function != NULL)
        {
            assert(table != NULL);

            for (int i = 0 ; i < n ; i++)
            {
                x        = Get_x_from_i(i);
                table[i] = function(x);
                percentage = int(Double(i) / Double(n) * 100.0);
                if (verbose and (percentage % 2) == 0)
                    printf(" %3d %%\b\b\b\b\b\b", percentage);
                fflush(stdout);
            }
        }
        else
        {
            if (verbose)
                std_cout << " Nothing to do since function pointer given is NULL." << std::flush;
        }
        if (verbose)
            std_cout << " Done.   \n" << std::flush;
    }

    // **************************************************************
    void Print()
    {
        Double memsize = Double(n) * sizeof(Double);
        std::string suffix;
        if(memsize >= 1.024e3)
        {
            memsize *= Double(1.0/1.024e3);
            suffix = "ki";
        }
        if(memsize >= 1.024e3)
        {
            memsize *= Double(1.0/1.024e3);
            suffix = "Mi";
        }
        if(memsize >= 1.024e3)
        {
            memsize *= Double(1.0/1.024e3);
            suffix = "Gi";
        }

        std_cout.Clear_Format();
        std_cout
            << "Lookup table information:\n"
            << "    Name:               " << name << "\n"
            << "    Range:              [" << range_min << ", " << range_max << "]\n"
            << "    Number of points:   " << n << "\n"
            << "    dx:                 " << dx << "\n"
            << "    Size:               " << memsize << " " << suffix << "B\n"
            << "    Pointer:            " << table << "\n";
    }

    // **************************************************************
    void Print_Table(const Double factor_x = 1.0, const Double factor_y = 1.0)
    {
        for (int i = 0 ; i < n ; i++)
        {
            //std_cout.Format(20,10,'g'); std_cout << Get_x_from_i(i)*factor_x << "  "; std_cout.Format(20,10,'g'); std_cout << Table(i)*factor_y << "\n";
            //fprintf(stderr, "%20.10g   %20.10g\n", Get_x_from_i(i)*factor_x, Table(i)*factor_y*Get_x_from_i(i));
            fprintf(stderr, "%20.10g   %20.10g\n", Get_x_from_i(i)*factor_x, Table(i)*factor_y);
        }
    }

    // **************************************************************
    inline int Get_i_from_x(const Double x)
    {
        const Double xnorm = (x - range_min)*inv_dx;
        const int i        = int(floor(xnorm));
        return i;
    }

    // **************************************************************
    inline Double read(const Double x)
    /**
     *   Reads the table and returns an interpolated (linear) value at point x.
     */
    {
        const Double xnorm = (x - range_min)*inv_dx;
        const int i        = int(floor(xnorm));
#ifdef YDEBUG
        assert(i < n);
#endif // #ifdef YDEBUG
        return table[i] + (table[i+1]-table[i])*(xnorm-Double(i));
    }

    // **************************************************************
    void Set(const int i, const Double x)
    /**
     *   Set manually the table.
     */
    {
        assert(function == NULL);
        assert(is_initialized);
        assert(i >= 0);
        assert(i <  n);

        table[i] = x;
    }

    // **************************************************************
    void Multiply(const Double x)
    /**
     *   Multiply the content of the table by a constant.
     */
    {
        for (int i = 0 ; i < n ; i++)
        {
            table[i] *= x;
        }
    }

    // **************************************************************
    void Convert_Units(const Double conversion_x, const Double conversion_y)
    /**
     * Convert LUT's units
     */
    {
        range_min   *= conversion_x;
        range_max   *= conversion_x;
        dx          *= conversion_x;
        inv_dx      /= conversion_x;

        for (int i = 0 ; i < n ; i++)
        {
            table[i] *= conversion_y;
        }
    }

    // **************************************************************
    ~LookUpTable()
    {
        free_me(table, n);
    }
};

#endif
