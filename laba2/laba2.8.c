#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LEN 100 

int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1; 
}

char digit_to_char(int digit) {
    if (digit >= 0 && digit <= 9) return '0' + digit;
    return 'A' + digit - 10;
}

bool is_valid_number_in_base(char* num, int base) {
    for (int i = 0; num[i] != '\0'; i++) {
        int digit = char_to_digit(num[i]);
        if (digit < 0 || digit >= base) {
            return false; 
        }
    }
    return true;
}

void add_two_numbers_in_base(char* num1, char* num2, int base, char* result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    
    int i = len1 - 1, j = len2 - 1, k = 0, carry = 0;

    memset(result, 0, MAX_LEN + 2);
    
    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? char_to_digit(num1[i--]) : 0;
        int digit2 = (j >= 0) ? char_to_digit(num2[j--]) : 0;
        
        int sum = digit1 + digit2 + carry;
        result[k++] = digit_to_char(sum % base);
        carry = sum / base;
    }
    
    for (int x = 0; x < k / 2; x++) {
        char temp = result[x];
        result[x] = result[k - x - 1];
        result[k - x - 1] = temp;
    }
    
    result[k] = '\0';
    
    char* non_zero_result = result;
    while (*non_zero_result == '0' && *(non_zero_result + 1) != '\0') {
        non_zero_result++;
    }
    
    if (non_zero_result != result) {
        memmove(result, non_zero_result, strlen(non_zero_result) + 1);
    }
}

void add_numbers_in_base(int base, char* result, int count, ...) {
    va_list args;
    va_start(args, count);

    char temp_result[MAX_LEN + 2];
    char* current_num;
    strcpy(result, "0");

    for (int i = 0; i < count; i++) {
        current_num = va_arg(args, char*);
        if (!is_valid_number_in_base(current_num, base)) {
            printf("Error: Number %s is not valid in base %d\n", current_num, base);
            va_end(args);
            return;
        }
        add_two_numbers_in_base(result, current_num, base, temp_result);
        strcpy(result, temp_result);
    }

    va_end(args);
}

int main() {
    char result[MAX_LEN + 2];
    
    // Пример 1: корректные числа в шестнадцатеричной системе
    add_numbers_in_base(16, result, 3, "12", "3A7", "0A07");
    printf("Result: %s\n", result);

    // Пример 2: число не в правильной системе счисления
    add_numbers_in_base(16, result, 2, "1G", "2A"); // G не допустима в 16-ричной системе

    return 0;
}
