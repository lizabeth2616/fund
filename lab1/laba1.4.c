#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Функция для проверки, что файлы разные
int are_files_different(const char *input, const char *output) {
    char *input_name = basename((char *)input);
    char *output_name = basename((char *)output);
    return strcmp(input_name, output_name) != 0;
}




void remove_digits_from_line(char *line, FILE *output) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isdigit(line[i])) {
            fputc(line[i], output);
        }
    }
}

void count_latin_letters_in_line(char *line, FILE *output) {
    int count = 0;  
    for (int i = 0; line[i] != '\0'; i++) {
        if (isalpha(line[i]) && (line[i] >= 'a' && line[i] <= 'z' || line[i] >= 'A' && line[i] <= 'Z')) {
            count++;  
        }
    }

    fprintf(output, "Number of Latin letters: %d\n", count);
}

void count_special_symbols_in_line(char *line, FILE *output) {
    int count = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (!(isalpha(line[i]) || isdigit(line[i]) || isspace(line[i]))) {
            count++;
        }
    }
    fprintf(output, "%d\n", count);
}

void replace_with_ascii_hex_in_line(char *line, FILE *output) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isdigit(line[i])) {
            fprintf(output, "%X  ", (unsigned char)line[i]);
        } else {
            fputc(line[i], output);
        }
    }
    fprintf(output, "\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Insufficient arguments\n");
        return 1;
    }

    char *flag = argv[1];
    char *input_file = argv[2];
    char *output_file = NULL;

    if (argc > 3) {
        output_file = argv[3];
    } else {
        output_file = malloc(strlen(input_file) + 5);
        strcpy(output_file, "out_");
        strcat(output_file, input_file);
    }


    if (!are_files_different(input_file, output_file)) {
        printf(" the files are the same\n");
        exit(1);
    }



    FILE *input = fopen(input_file, "r");
    if (input == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Failed to open output file\n");
        fclose(input);
        return 1;
    }
    
    char line[1024];  // Буфер для чтения строк
    while (fgets(line, sizeof(line), input)) {
        if (strcmp(flag, "-d") == 0 || strcmp(flag, "/d") == 0) {
            remove_digits_from_line(line, output);
        } else if (strcmp(flag, "-i") == 0 || strcmp(flag, "/i") == 0) {
            count_latin_letters_in_line(line, output);
        } else if (strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0) {
            count_special_symbols_in_line(line, output);
        } else if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) {
            replace_with_ascii_hex_in_line(line, output);
        } else {
            printf("Unknown flag: %s\n", flag);
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    fclose(input);
    fclose(output);

    printf("The result has been written to the file: %s\n", output_file);

    return 0;
}
