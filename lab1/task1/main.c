// #define DLIBNAME "" // of DYNAMIC LIBRARY

#include <stdio.h>

#ifndef DLIBNAME

#include "power.h"
#include "factorial.h"

#else

#include <dlfcn.h>

#endif

int main()
{
#ifdef DLIBNAME
    void *lib_handler;
    lib_handler = dlopen(DLIBNAME, RTLD_LAZY);
    if (!lib_handler)
    {
        printf("Error opening dynamic library: %s\n", dlerror());
        return 1;
    }

    int (*power)(int, int) = dlsym(lib_handler, "power");
    if (!power)
    {
        printf("Error findeing function power: %s\n", dlerror());
        dlclose(lib_handler);
        return 1;
    }

    int (*factorial)(int) = dlsym(lib_handler, "factorial");
    if (!factorial)
    {
        printf("Error findeing function factorial: %s\n", dlerror());
        dlclose(lib_handler);
        return 1;
    }
#endif

    int a = 2,
        b = 3;
    printf("%d to the power of %d is: %d\n", a, b, power(a, b));

    int c = 5;
    printf("Factorial of %d is: %d\n", c, factorial(c));

#ifdef DLIBNAME

    dlclose(lib_handler);
#endif

    return 0;
}
