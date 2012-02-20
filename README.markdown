# Memory
memory, a C++ library for memory magement.

The library defines calloc_and_check() and malloc_and_check(). Not only will it
allocate the wanted amount of memory, but it will also keep track of how much
memory is already allocated. Users can decide a maximum in the memory they want
to use and the library will halt when the limit is reached and {c,m}alloc_and_check()
are called.

A lookup table class is also present. It allow fast


# Compilation
Requirements: [https://github.com/nbigaouette/stdcout](stdcout).

To build and install:

``` bash
$ make gcc shared static install
```

By default, it will be installed in ${HOME}/usr. You can install somewhere else
using DESTDIR:

``` bash
$ make gcc shared static install DESTIR=/usr
```

Library name will be "libmemory".


# Example

## Memory allocation

Here's some basic usage examples:

``` C++
    // Set maximum memory to 2 MB
    allocated_memory.Set_Max_Bytes(MiBytes_to_Bytes(2));
    // 262,144 doubles is ~2MB
    const int N = 262144;
    double *array = (double *) calloc_and_check(N, sizeof(double));
    // Get what is already allocated
    const uint64_t already_allocated_bytes = allocated_memory.Get_Bytes_Allocated();
    // Print memory information (what is allocated and maximum)
    allocated_memory.Print();
    // Make sure we are under the limit
    allocated_memory.Verify_Limit();
```
will result in the following output:

```
Memory allocated:                    2097144 bytes (2.05e+03 KiB, 2 MiB, 0.00195 GiB)
Maximum memory allowed:              2097152 bytes (2.05e+03 KiB, 2 MiB, 0.00195 GiB)
#################################################################################################
Code's expected memory usage: 2097144 bytes, 2.05e+03 KiB, 2 MiB, 0.00195 GiB
Limit: 2097152 bytes, 2.05e+03 KiB, 2 MiB, 0.00195 GiB
#################################################################################################
```

Allocating one more element to the array, or allocating more then 8 bytes using {c,m}alloc_and_check()
will result in:

```
WARNING!!!
    Trying to allocate:                         262144 x 8 bytes = 2097152 bytes
                                               (2.05e+03 KiB, 2 MiB, 0.00195 GiB)
    but memory will be over the limit:
                                2097152 bytes, (2.05e+03 KiB, 2 MiB, 0.00195 GiB)
    Current usage:                    0 bytes, (0 KiB, 0 MiB, 0 GiB)
    Wanted usage:               2097152 bytes, (2.05e+03 KiB, 2 MiB, 0.00195 GiB)
Are you sure you want to continue? [y,N]
```

You can free some memory using **free_me()**. If you pass the number of elements
free-ed, the class will keep track of the deallocation. If not, the class will
still think the memory is allocated, even though it is not:


``` C++
    free_me(array, N);
    int *int_array = (int *) malloc_and_check(2*N, sizeof(int));
```

Note that calloc_and_check() and malloc_and_check() have the same calling convention,
as opposed to calloc() and malloc().


## Binary printing
Values can be converted to a binary std::string representation:
``` C++
    const float  valf = 1.23456789;
    const double vald = 1.23456789;
    const int    vali = 5;
    const std::string valf_string = Float_in_String_Binary(valf);
    const std::string vald_string = Double_in_String_Binary(vald);
    const std::string vali_string = Integer_in_String_Binary(vali);
    std::cout << "Float:  " << valf << " in binary == " << valf_string << "\n";
    std::cout << "Double: " << vald << " in binary == " << vald_string << "\n";
    std::cout << "Int:    " << vali << " in binary == " << vali_string << "\n";
```
will give:

``` C++
Float:  1.23457 in binary == 0 01111111 00111100000011001010010
Double: 1.23457 in binary == 0 01111111111 0011110000001100101001000010100000111101111000011011
Int:    5 in binary == 00000000000000000000000000000101
```

## Lookup tables

``` C++
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
    std::cout << "cos(0)    == " << cos_lut.read(0.0) << " (should be 1)\n";
    std::cout << "cos(pi)   == " << cos_lut.read(std::acos(-1.0)) << " (should be -1)\n";
    std::cout << "cos(pi/2) == " << cos_lut.read(std::acos(-1.0)/2.0) << " (should be 0)\n";

```

gives:

```
Printing table information:
Lookup table information:
    Name:               Cosine lookup table
    Range:              [0, 6.28319]
    Number of points:   10000
    dx:                 0.000628381
    Size:               78.125 kiB
    Pointer:            0xde8840
Printing table content (to standard error stream):
                   0                      1
     0.0006283813689           0.9999998026
      0.001256762738           0.9999992103
      0.001885144107           0.9999982231
      0.002513525475           0.9999968411
      0.003141906844           0.9999950642
      0.003770288213           0.9999928925
      0.004398669582           0.9999903259
[...]
Read some values from lut:
cos(0)    == 1 (should be 1)
cos(pi)   == -1 (should be -1)
cos(pi/2) == -3.877e-12 (should be 0)
```


# License

This code is distributed under the terms of the [GNU General Public License v3 (GPLv3)](http://www.gnu.org/licenses/gpl.html) and is Copyright 2011 Nicolas Bigaouette.

