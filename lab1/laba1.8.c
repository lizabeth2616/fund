#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


// Функция для проверки, что файлы разные
int are_files_different(const char *input, const char *output) {
    char *input_name = basename((char *)input);
    char *output_name = basename((char *)output);
    return strcmp(input_name, output_name) != 0;
}





// Функция для определения минимального основания системы счисления
int determine_min_base(const char *number_str) {
    int max_digit = '0';  
    for (int i = 0; number_str[i] != '\0'; i++) {
        char c = number_str[i];
        
        c = tolower(c);
        
        if (isdigit(c) || isalpha(c)) {
            max_digit = (max_digit > c) ? max_digit : c;
        } 
    }

    if (isdigit(max_digit)) {
        return max_digit - '0' + 1;
    }
    if (isalpha(max_digit)) {
        return max_digit - 'a' + 10 + 1;
    }
}

// Функция для преобразования числа в десятичную систему счисления
long long convert_to_decimal(const char *number_str, int base) {
    long long decimal_value = 0;
    int power = 1;

    for (int i = strlen(number_str) - 1; i >= 0; i--) {
        char c = tolower(number_str[i]);
        
        int digit_value;
        if (isdigit(c)) {
            digit_value = c - '0';
        } else {
            digit_value = c - 'a' + 10;
        }

        decimal_value += digit_value * power;
        power *= base;
    }

    return decimal_value;
}

// Функция для удаления незначащих нулей из строки числа
void remove_leading_zeros(char *str) {
    char *non_zero = str;
    while (*non_zero == '0' && *(non_zero + 1) != '\0') {
        non_zero++;
    }
    
    if (*non_zero == '0' && *(non_zero + 1) == '\0') {
        str[0] = '0';
        str[1] = '\0';
    } else {
        memmove(str, non_zero, strlen(non_zero) + 1); 
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char *flag = argv[1];
   

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }


    //if (!are_files_different(input_file, output_file)) {
       // printf(" the files are the same\n");
       // exit(1);
    //}




    char number_str[100];
    while (fscanf(input_file, "%s", number_str) != EOF) {
        remove_leading_zeros(number_str);
        int min_base = determine_min_base(number_str);
        long long decimal_value = convert_to_decimal(number_str, min_base);
        char decimal_str[100];
        snprintf(decimal_str, sizeof(decimal_str), "%lld", decimal_value);
        fprintf(output_file, "%s %d %s\n", number_str, min_base, decimal_str);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
