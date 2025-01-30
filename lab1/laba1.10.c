#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Функция для преобразования строки в число в десятичной системе счисления
long long convert_to_decimal(const char* num_str, int base) {
    long long result = 0;
    int length = strlen(num_str);
    
    for (int i = 0; i < length; i++) {
        char digit = num_str[i];
        
        // Преобразуем символ в соответствующее значение
        int value;
        if (isdigit(digit)) {
            value = digit - '0'; // Для цифр 0-9
        } else if (isupper(digit)) {
            value = digit - 'A' + 10; // Для букв A-Z
        } else {
            return -1;
        }
        
        if (value >= base) {
            return -1;
        }

        result = result * base + value;
    }

    return result;
}

// Функция для преобразования числа из десятичной системы счисления в строковое представление в нужной системе
void convert_from_decimal(long long num, int base, char* result) {
    char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    if (num == 0) {
        strcpy(result, "0");
        return;
    }
    
    char buffer[65]; 
    int index = 0;
    
    while (num > 0) {
        buffer[index++] = digits[num % base];
        num /= base;
    }
    
    // Переворачиваем строку, чтобы получить правильный порядок
    buffer[index] = '\0';
    int length = index;
    for (int i = 0; i < length; i++) {
        result[i] = buffer[length - 1 - i];
    }
    result[length] = '\0';
}

int main() {
    int base;
    printf("Enter the base of the number system (from 2 to 36): ");
    scanf("%d", &base);

    // Проверка корректности введенного основания
    if (base < 2 || base > 36) {
        printf("Error: base must be between 2 and 36.\n");
        return 1;
    }

    char num_str[65];
    char max_num_str[65];
    long long max_value = -1; // Для хранения максимального значения по модулю
    long long max_abs_value = -1;

    printf("Enter numbers in the string representation (in base %d), or 'Stop' to finish: \n", base);

    while (1) {
        scanf("%s", num_str);
        
        if (strcmp(num_str, "Stop") == 0) {
            break;
        }

        long long decimal_value = convert_to_decimal(num_str, base);
        
        if (decimal_value == -1) {
            printf("Invalid number: %s\n", num_str);
            continue;
        }

        // Ищем максимальное по модулю
        if (abs(decimal_value) > max_abs_value) {
            max_abs_value = abs(decimal_value);
            strcpy(max_num_str, num_str); // Запоминаем строку максимального числа
            max_value = decimal_value;
        }
    }

    if (max_value == -1) {
        printf("No valid numbers were entered.\n");
        return 1;
    }

    printf("The number with the maximum absolute value: %s\n", max_num_str);

    char result[65];
    for (int new_base = 9; new_base <= 36; new_base *= 2) {
        convert_from_decimal(max_value, new_base, result);
        printf("Representation of the number in base %d: %s\n", new_base, result);
    }

    int new_base = 27;
    convert_from_decimal(max_value, new_base, result);
    printf("Representation of the number in base %d: %s\n", new_base, result);
    

    return 0;
}
