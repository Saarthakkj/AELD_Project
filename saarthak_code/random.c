#include <stdio.h>
#include <complex.h>

void some_function(double complex *c , double complex *d) {
    printf("Real part: %f\n", creal(*c)); 
    printf("Imaginary part: %f\n", cimag(*c));
    printf("Real part: %f\n", creal(*d)); 
    printf("Imaginary part: %f\n", cimag(*d));
}

int main() {
    printf("hello world\n");
    double complex c = 1.0 + 2.0 * I;
    double complex c2 = 2.0 + 1.0 * I;
    some_function(&c , &c2);
    return 0;
}
