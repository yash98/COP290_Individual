Using user defined Libraries
In the previous assignment, you implemented arbitrary precision arithmetic. In this assignment, you will use the same to implement complex arithmetic which works with numbers having arbitrary precison.

Step One: Create a library called arbprecision (An elegant short note on using ar  and the verbose version)

Step Two: Implement complex number arithmetic and link arbprecision. This should support addition, subtraction, abs, product, quotient.

Step Three: Note the difference in run time for your implementation of complex arithmetic (using arbprecision) and the standard C library implementation.

Step Four: Plot the observations in GNUPlot by calling GNUPlot from the C file where you may need to use this.

```c
FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
The key requirement is that a make should show the graphs/plots. 
```

Example C code for complex numbers is given below:

```c
#include <stdio.h>      /* Standard Library of Input and Output */
#include <complex.h>    /* Standard Library of Complex Numbers */

int main() {

    double complex z1 = 1.0 + 3.0 * I;
    double complex z2 = 1.0 - 4.0 * I;

    printf("Working with complex numbers:\n\v");

    printf("Starting values: Z1 = %.2f + %.2fi\tZ2 = %.2f %+.2fi\n", creal(z1), cimag(z1), creal(z2), cimag(z2));

    double complex sum = z1 + z2;
    printf("The sum: Z1 + Z2 = %.2f %+.2fi\n", creal(sum), cimag(sum));

    double complex difference = z1 - z2;
    printf("The difference: Z1 - Z2 = %.2f %+.2fi\n", creal(difference), cimag(difference));

    double complex product = z1 * z2;
    printf("The product: Z1 x Z2 = %.2f %+.2fi\n", creal(product), cimag(product));

    double complex quotient = z1 / z2;
    printf("The quotient: Z1 / Z2 = %.2f %+.2fi\n", creal(quotient), cimag(quotient));

    double complex conjugate = conj(z1);
    printf("The conjugate of Z1 = %.2f %+.2fi\n", creal(conjugate), cimag(conjugate));

    return 0;
}
```