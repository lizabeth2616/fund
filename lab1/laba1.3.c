#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-9

// Функция для решения квадратного уравнения
void solve_quadratic(double a, double b, double c, double epsilon) {
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < -epsilon) {
        printf("There are no solutions\n");
    } else if (discriminant < epsilon) {
        double x = -b / (2 * a);
        printf("One root: x = %.6f\n", x);
    } else if (a > 0){

        
        double sqrt_discriminant = sqrt(discriminant);
        double x1 = (-b + sqrt_discriminant) / (2 * a);
        double x2 = (-b - sqrt_discriminant) / (2 * a);
        printf("Two roots: x1 = %.6f, x2 = %.6f\n", x1, x2);
    }

    else {
        printf("There are no solutions\n");
    }
}

// Функция для проверки кратности
void check_multiplicity(int num1, int num2) {
    if (num1 % num2 == 0) {
        printf("%d multiple %d\n", num1, num2);
    } else {
        printf("%d not multiple %d\n", num1, num2);
    }
}

// Функция для проверки, могут ли три числа быть сторонами прямоугольного треугольника
void check_right_triangle(double a, double b, double c, double epsilon) {
    double sides[3] = {a, b, c};
    // Сортировка сторон
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (sides[i] > sides[j]) {
                double temp = sides[i];
                sides[i] = sides[j];
                sides[j] = temp;
            }
        }
    }

    // Проверка на прямоугольность с учетом точности
    if (fabs(sides[0] * sides[0] + sides[1] * sides[1] - sides[2] * sides[2]) < epsilon) {
        printf("This is a right-angled triangle\n");
    } else {
        printf("It's not a right-angled triangle.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough arguments\n");
        return 1;
    }

    char *flag = argv[1];

    if (flag[0] != '-' && flag[0] != '/') {
        printf("Invalid flag: the flag must start with the character '-' or '/'\n");
        return 1;
    }

    if (strcmp(flag, "-q") == 0) {
        if (argc != 6) {
            printf("Incorrect number of arguments for the -q flag\n");
            return 1;
        }
        
        double epsilon = atof(argv[2]);
        double a = atof(argv[3]);
        double b = atof(argv[4]);
        double c = atof(argv[5]);

        solve_quadratic(a, b, c, epsilon);
        
        printf("Solutions for the equation with precision %.6f:\n", epsilon);
        solve_quadratic(a, b, c, epsilon);
        solve_quadratic(a, c, b, epsilon);
        solve_quadratic(b, a, c, epsilon);
        solve_quadratic(b, c, a, epsilon);
        solve_quadratic(c, a, b, epsilon);
        solve_quadratic(c, b, a, epsilon);

    } else if (strcmp(flag, "-m") == 0) {
        if (argc != 4) {
            printf("Invalid number of arguments for the -m flag\n");
            return 1;
        }

        int num1 = atoi(argv[2]);
        int num2 = atoi(argv[3]);
        
        if (num1 == 0 || num2 == 0) {
            printf("The numbers must be non-zero for the -m flag\n");
            return 1;
        }

        check_multiplicity(num1, num2);

    } else if (strcmp(flag, "-t") == 0) {
        if (argc != 6) {
            printf("Invalid number of arguments for the -t flag\n");
            return 1;
        }

        double epsilon = atof(argv[2]);
        double a = atof(argv[3]);
        double b = atof(argv[4]);
        double c = atof(argv[5]);
        
        if (a <= 0 || b <= 0 || c <= 0) {
            printf("The sides of the triangle must be positive\n");
            return 1;
        }

        check_right_triangle(a, b, c, epsilon);

    } else {
        printf("Unknown flag: %s\n", flag);
        return 1;
    }

    return 0;
}
