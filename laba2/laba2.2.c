#include <stdio.h>
#include <stdarg.h> 
#include <math.h>


double geometric_mean(int count, ...) {  //среднее геометрическое
    if (count <= 0) {
        return NAN; 
    }

    va_list args;
    va_start(args, count);

    double product = 1.0;
    int valid_count = 0;

    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        if (num < 0) {
            va_end(args);
            return NAN;
        }
        product *= num;
        valid_count++;
    }

    va_end(args);

    return pow(product, 1.0 / valid_count);
}


double power(double base, int exp) {
    if (base == 0 && exp == 0) {
        return NAN; 
    }
    if (exp == 0) {
        return 1.0;
    }
    if (exp < 0) {
        return 1.0 / power(base, -exp);
    }

    double result = 1.0;
    double current = base;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= current;
        }
        current *= current;
        exp /= 2;
    }

    return result;
}

int main() {
    printf("Geometric mean: %f\n", geometric_mean(3, 2.0, 8.0, 4.0));
    printf("Geometric mean: %f\n", geometric_mean(4, 1.0, 2.0, 3.0, 4.0));

    double result;

    result = power(2.0, 3);
	if (isnan(result)) {
        printf("Error: 0^0 is undefined.\n");
    } else {
        printf("2.0^3 = %f\n", result);
    }

    result = power(5.0, 0);
	if (isnan(result)) {
        printf("Error: 0^0 is undefined.\n");
    } else {
        printf("5.0^0 = %f\n", result);
    }

    result = power(2.0, -2);
	if (isnan(result)) {
        printf("Error: 0^0 is undefined.\n");
    } else {
        printf("2.0^(-2) = %f\n", result);
    }

    result = power(0.0, 0);
    if (isnan(result)) {
        printf("Error: 0^0 is undefined.\n");
    } else {
        printf("0.0^0 = %f\n", result);
    }

    result = power(2.0, 5);
	if (isnan(result)) {
        printf("Error: 0^0 is undefined.\n");
    } else {
   		printf("2.0^5 = %f\n", result);
	}

    return 0;
}
