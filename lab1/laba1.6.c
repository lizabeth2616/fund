#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef enum kOpts { OPT_A, OPT_B, OPT_C, OPT_D } kOpts;

double FunctionA(double x) {
    return (x == 0) ? 1.0 : log(1 + x) / x;
}

double FunctionB(double x) {
    return exp(-x * x / 2);
}

double FunctionC(double x) {
    return -log(1 - x);
}

double FunctionD(double x) {
    return (x == 0) ? 1.0 : pow(x, x);
}

double Integral(double (*f)(double), double epsilon){
    double a = 0.0, b = 1.0;
    double n = 1.0;
    double integral_prev = 0.0;
    double integral_curr = 0.0;
    double h;
    double summ;

    do {
        n *= 2;
        h = (b - a) / n;
        summ = 0.0;

        for (int i = 0; i < n; i++) {
            double x_middle = a + (i + 0.5) * h;
            summ += f(x_middle);
        }

        integral_curr = summ * h;

        if (fabs(integral_curr - integral_prev) < epsilon) {
            break;
        }

        integral_prev = integral_curr;

    } while (1);

    return integral_curr;
}

double MyStrtod(const char* str, char** end) {   //перевод строки в число
	while (isspace((unsigned char)*str)) {
		str++;
	}

	int sign = 1;
	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	double integer_part = 0.0;
	while (isdigit((unsigned char)*str)) {
		integer_part = integer_part * 10 + (*str - '0');
		str++;
	}

	double fraction_part = 0.0;
	if (*str == '.') {
		str++;
		double divisor = 10.0;
		while (isdigit((unsigned char)*str)) {
			fraction_part += (*str - '0') / divisor;
			divisor *= 10;
			str++;
		}
	}

	double result = sign * (integer_part + fraction_part);

	if (end) {
		*end = (char*)str;
	}


	
	return result;
}

int CheckFlag(const char* kflag, const char* valid_flag) {
    return (kflag[0] == '-' || kflag[0] == '/') && strcmp(&kflag[1], valid_flag) == 0;
}

int GetOpts(int argc, char** argv, kOpts* option, double* epsilon) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <a/b/c/d> <epsilon>\n", argv[0]);
		return 1;
	}

	char *end, *end1;
	*epsilon = MyStrtod(argv[2], &end1);
	if (*end1 != '\0') {
		return 2;
	}

	char* flags[4] = {"a", "b", "c", "d"};

	for (int i = 0; i < 4; i++) {
		if (CheckFlag(argv[1], flags[i])) {
			*option = i;
			return 0;
		}
	}

	return 3;
}

int main(int argc, char* argv[]) {
    kOpts opt = 0;
    double epsilon;

    double (*functions[4])(double) = {FunctionA, FunctionB, FunctionC, FunctionD};

    int res = GetOpts(argc, argv, &opt, &epsilon);
	switch (res) {
		case 0:
			break;
		case 1:
			printf("Incorrect number of arguments.\n");
			return 1;
		case 2:
			printf("Error: %s aren't a double number.\n", argv[2]);
			return 1;
		case 3:
			printf("Unknown flag: %s\n", argv[1]);
			return 1;
		default:
			printf("Unknown error.\n");
			return 1;
	}

    printf("Integral for the flag %s: %.10f\n", argv[1], Integral(functions[opt], epsilon));

    return 0;
}
