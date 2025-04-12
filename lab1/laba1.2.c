#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_INTERATION 47
#define PI 3.1415

long double factorial(int n) {
	long double res = 1.0;
	for (int i = 2; i <= n; i++) {
		res *= i;
	}
	return res;
}

double my_lim_e(double epsilon) {
	int n = 1;
    double prev;
    double cur = pow(1.0 + 1.0 / n, n);
    n++; 
    do
    {
        prev = cur;
        cur = pow(1.0 + 1.0 / n, n);
        n++;
    } while (fabs(cur - prev) > epsilon || fabs(cur - prev) == 0);
    return cur;
}

double my_lim_pi(double epsilon) {
	double pi = 4;
	double prev_pi = 0.0;
	int n = 1;
	while (fabs(pi - prev_pi) > epsilon) {
		prev_pi = pi;
		pi *= 4.0 * ((n + 1) * n) / pow(2 * n + 1, 2);
		n++;
	}
	return pi;
}

double my_lim_ln_2(double epsilon) {
	int n = 1;
    double prev;
    double cur = n * (pow(2.0, 1.0 / n) - 1);
    n++;
    do
    {
        prev = cur;
        cur = n * (pow(2.0, 1.0 / n) - 1);
        n++;
    } while ((fabs(cur - prev) > epsilon));
    return cur;
}

double my_lim_kor_2(double epsilon) {
	double prev;
    double cur = -0.5;
    do
    {
        prev = cur;
        cur = prev - pow(prev, 2.0) / 2.0 + 1.0;
    } while (fabs(cur - prev) > epsilon);
    return cur;
}

double my_lim_y(double epsilon) {  
	long double gamma = 0.0;
	long double prev_gamma = 0.0;
	int m = 1;
	long double fact_m = 1;
	while (1) {
		long double sum = 0.0;
		long double fact_k = 1;
		for (int k = 1; k <= m; k++) {
			fact_k *= k;
			long double binom = fact_m / (fact_k * factorial(m - k));
			sum += binom * (k % 2 ? -1.0 : 1.0) * log(fact_k) / k;

		}
		if (m == 1) {
			prev_gamma = -100;
		} else {
			prev_gamma = gamma;
		}
		gamma = sum;
		if (m >= MAX_INTERATION) {
			return gamma;
		}
		if (fabsl(gamma - prev_gamma) < epsilon) {
			break;
		}
		m++;
		fact_m *= m;
	}

	return gamma;
}

double my_row_e(long double epsilon) {
	double sum = 1.0;
	double term = 1.0;
	int n = 1;
	
	while (term > epsilon) {
		term /= n; 
		sum += term;
		n++;
	}
	
	return sum;
}

double my_row_pi(long double epsilon) {
	double sum = 0.0;
	double term;
	int n = 1;
    int sign = 1;
    double dop_number = 1.0;
	
	do {
        term = sign / dop_number; 
		sum += term;
        dop_number += 2.0; 
        sign = -sign;
		n++;
    } while (fabs(term) >= epsilon);
	
	return sum * 4;
}

double my_row_ln_2(double epsilon) {
	double sum = 0.0;
    double term;
    int n = 1;

    do {
        term = (n % 2 == 0 ? -1 : 1) * (1.0 / n);
        sum += term;
        n++;
    } while (fabs(term) > epsilon);

    return sum;
}

double my_row_kor_2(long double epsilon) {
	double sum = 0.0;
	double term;
    double log_2 = log(2.0);
	int k = 2;
    double help_del = 1.0 / 2.0;
	
    do {
        help_del /= 2.0;
        term = help_del * log_2; 
		sum += term;
        k++;
    } while (fabs(term) > epsilon);
	
	return exp(sum);
}

double my_row_y(long double epsilon) {
	long double gamma = 0;
	long double term = 0;
	long double gamma_prev;
	int k = 2;

	while (1) {
		gamma_prev = gamma;
		term = (1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k);
		gamma += term;
		if (fabsl(term) < epsilon && term != 0) {
			break;
		}

		k++;
	}

	return gamma - PI * PI / 6.0;
}


double f_e(double x) {
    return log(x) - 1;
}

double f_pi(double x) {
    return cos(x) + 1;
}

double f_pi_prime(double x) {
    return -sin(x);
}

double f_ln_2(double x) {
    return exp(x) - 2;
}

double f_kor_2(double x) {
    return x * x - 2;
}

double f_y(double x) {
    return log(x) - 1;
}

double bisection_method(double (*func)(double), double a, double b, double eps) {
    long double midpoint;
    if (func(a) * func(b) >= 0.0) {
        printf("Error: the function does not change the sign on the specified segment: [%.2lf, %.2lf].\n", a, b);
        return NAN;
    }

    while ((b - a) / 2.0 > eps) {
        midpoint = (a + b) / 2.0;
        
        if (func(midpoint) == 0.0) {
            return midpoint;
        } else if (func(a) * func(midpoint) < 0.0) {
            b = midpoint;
        } else {
            a = midpoint;
        }
    }

    return (a + b) / 2.0; // Возвращаем среднюю точку как корень
}

double newton_method(double x0, double (*func)(double), double (*func_prime)(double), double epsilon) {
    double x1;
    
    do {
        x1 = x0 - func(x0) / func_prime(x0);
        if (fabs(x1 - x0) < epsilon) {
            break;
        }
        x0 = x1;
    } while (1);

    return x1;
}

bool IsPrime(int n) {
	if (n < 2) {
		return false;
	}
	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

double ProdResult(int t) {
	double product = 1.0;
	for (int p = 2; p <= t; p++) {
		if (IsPrime(p)) {
			product *= (double)(p - 1) / p;
		}
	}
	return product;
}

long double my_equation_y(double epsilon) { 
	double t = 2.0;  
	double prev_x = 0.0;

	for (int i = 0; i < 1000; i++) {
		double product = ProdResult((int)t);

		double current_x = log(t) * product;

		if (fabs(current_x - prev_x) < epsilon) {
			return current_x;
		}

		prev_x = current_x;
		t += 1;
	}

	return prev_x;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("The input is like this: %s <x> <epsilon>\n", argv[0]); 
		return 1;
	}
	
	double epsilon = atof(argv[1]);
	
	if (epsilon <= 0.000000001) {
		printf("Must not е <= 0.000000001\n");
		return 1;
	}

    printf("Accuracy: %0.15f\n\n", epsilon);
	
    double a = 0.0;
    double b = 100.0; 

	printf("e: ");
    double result_lim_e = my_lim_e(epsilon);
    printf("%f; ", result_lim_e);
	double result_row_e = my_row_e(epsilon);
    printf("%f; ", result_row_e);
    double result_equation_e = bisection_method(f_e, a, b, epsilon);
    printf("%f", result_equation_e);

    

    printf("\nln_2: ");
	double result_lim_ln_2 = my_lim_ln_2(epsilon);
    printf("%f; ", result_lim_ln_2);
    double result_row_ln_2 = my_row_ln_2(epsilon);
    printf("%f; ", result_row_ln_2);
    double result_equation_ln_2 = bisection_method(f_ln_2, 0.0, 2.0, epsilon);
    printf("%f", result_equation_ln_2);

    printf("\nkor_2: ");
	double result_lim_kor_2 = my_lim_kor_2(epsilon);
    printf("%f; ", result_lim_kor_2);
    double result_row_kor_2 = my_row_kor_2(epsilon);
    printf("%f; ", result_row_kor_2);
    double result_equation_kor_2 = bisection_method(f_kor_2, a, b, epsilon);
    printf("%f", result_equation_kor_2);

    printf("\ny: ");
	double result_lim_y = my_lim_y(epsilon);
    printf("%f; ", result_lim_y);
    double result_row_y = my_row_y(epsilon);
    printf("%f; ", result_row_y);
    double result_equation_y = my_equation_y(epsilon);
    printf("%f", result_equation_y);

    printf("\npi: ");
	double result_lim_pi = my_lim_pi(epsilon);
    printf("%f; ", result_lim_pi);
	double result_row_pi = my_row_pi(epsilon);
    printf("%f; ", result_row_pi);
    double result_equation_pi = newton_method(1.0, f_pi, f_pi_prime, epsilon);
    printf("%f\n", result_equation_pi);
}
