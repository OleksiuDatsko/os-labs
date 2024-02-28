#include "factorial.h"

int factorial(int a) {
    int result = 1;
    for (int i = 2; i <= a; ++i) {
        result *= i;
    }
    return result;
}
