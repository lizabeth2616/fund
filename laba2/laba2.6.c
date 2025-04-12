#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define STR_SIZE 1024

typedef enum optRealloc {
	OPT_REALLOC_SUCCESS,
	OPT_REALLOC_FAIL
} optRealloc;

typedef enum kOpt {
	OPT_SUCCESS,
	OPT_INVALID_PARAMETER,
	OPT_ERROR_ALLOC
} kOpt;



bool is_digit_str(const char *str, int size) {
	for (int i = 0; i < size; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

bool is_lower(const char *str, int size) {
	for (int i = 0; i < size; i++) {
		if ((str[i] >= 'A') && (str[i] <= 'Z')) {
			return false;
		}
	}
	return true;
}

bool is_upper(const char *str, int size) {
	for (int i = 0; i < size; i++) {
		if ((str[i] >= 'a') && (str[i] <= 'z')) {
			return false;
		}
	}
	return true;
}

kOpt FromRoman(const char *number_str, int *num) {
	int size = strlen(number_str);
	if (!size) {
		return OPT_INVALID_PARAMETER;
	}

	*num = 0;
	int sign;
	if (number_str[0] == '-') {
		sign = -1;
	} else {
		sign = 1;
	}

	for (int i = size - 1; i >= 0; i--) {
		char symbol = number_str[i];
		switch (symbol) {
			case 'I':
				if (number_str[i + 1] == 'X' || number_str[i + 1] == 'V') {
					(*num)--;
				} else {
					(*num)++;
				}
				break;
			case 'V':
				(*num) += 5;
				break;
			case 'X':
				if (number_str[i + 1] == 'L' || number_str[i + 1] == 'C') {
					(*num) -= 10;
				} else {
					(*num) += 10;
				}
				break;
			case 'L':
				(*num) += 50;
				break;
			case 'C':
				if (number_str[i + 1] == 'D' || number_str[i + 1] == 'M') {
					(*num) -= 100;
				} else {
					(*num) += 100;
				}
				break;
			case 'M':
				(*num) += 1000;
				break;
			default:
				break;
		}
	}

	*num *= sign;
	return OPT_SUCCESS;
}

kOpt FromZeckendorf(const char *number_z, unsigned int *number) {
	int size = strlen(number_z);
	if (!size) {
		return OPT_INVALID_PARAMETER;
	}

	unsigned int num1 = 0;
	unsigned int num2 = 0;
	unsigned int cur = 1;
	*number = 0;
	for (int i = 0; i < size - 1; i++) {
		char symb = number_z[i];
		if (symb == '1') {
			(*number) += cur;
		}
		num1 = num2;
		num2 = cur;
		cur = num1 + num2;
	}

	return OPT_SUCCESS;
}

kOpt ToDecimal(char *number_str, int base, int *dec_number, int flag) {
	if (base < 2 || base > 36) {
		base = 10;
	}

	long long decimal_number = 0;
	int power = strlen(number_str) - 1;
	bool start = true;
	bool is_minus = false;
	for (int i = 0; i < strlen(number_str); i++) {
		char digit = number_str[i];
		if (digit == '-' && start) {
			power--;
			start = false;
			is_minus = true;
			continue;
		}

		int digit_value;
		if (isdigit(digit)) {
			digit_value = digit - '0';
		} else if (isalpha(digit)) {
			if (flag) {
				digit_value = digit - 'A' + 10;
			} else {
				digit_value = digit - 'a' + 10;
			}
		} else {
			return OPT_INVALID_PARAMETER;
		}

		decimal_number += digit_value * pow(base, power);
		power--;
		start = false;
	}

	if (decimal_number > INT_MAX) {
		return OPT_INVALID_PARAMETER;
	}

	*dec_number = is_minus ? -1 * decimal_number : decimal_number;
	return OPT_SUCCESS;
}


int overfscanf(FILE *stream, const char *format, ...) {
	if (!stream) {
		return -1;
	}

	int size_format = strlen(format);
	if (!size_format) {
		printf("There are no format to scan.\n");
		return -1;
	}

	int argc = 0;
	for (int i = 0; i < size_format; i++) {
		if (format[i] == '%' &&  format[i + 1] != '%') {
			argc++;
		}
	}

	va_list ptr;
	va_start(ptr, format);

	int index = 0;
	char *flag = NULL;
	int count_values = 0;
	for (int i = 0; i < argc; i++) {
		int size_flag = 1;
		int capacity_flag = 2;

		while (format[index] != '%' && index < size_format) {
			index++;
		}

		if (index >= size_format){
			break;
		}

		flag = (char *) malloc(sizeof(char) * (capacity_flag + 2));
		if (!flag) {
			printf("Error malloc memory.\n");
			va_end(ptr);
			return OPT_ERROR_ALLOC;
		}

		flag[0] = '%';
		index++;
		while ((isdigit(format[index])) || (isalpha(format[index])) && (index < size_format)) {
			flag[size_flag] = format[index];
			size_flag++;

			if (size_flag == capacity_flag - 1) {
				capacity_flag *= 2;
				char *new_flag = (char *) realloc(flag, capacity_flag);
				if (!new_flag){
					free(flag);
					printf("Can`t realloc memory!\n");
					va_end(ptr);
					return OPT_ERROR_ALLOC;
				}
				flag = new_flag;
			}
			index++;
		}

		flag[size_flag] = '\0';

		if (!strcmp(flag, "%Ro\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			char tmp[STR_SIZE];
			count_values += fscanf(stream, "%s", tmp);
			FromRoman(tmp, arg_ptr);
		} else if (!strcmp(flag, "%Zr\0")) {
			unsigned int *arg_ptr = va_arg(ptr, unsigned int *);
			char tmp[STR_SIZE];
			count_values += fscanf(stream, "%s", tmp);
			FromZeckendorf(tmp, arg_ptr);
		} else if (!strcmp(flag, "%Cv\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			int base = va_arg(ptr, int);
			char tmp[STR_SIZE];
			count_values += fscanf(stream, "%s", tmp);
			int size = strlen(tmp);

			if (is_upper(tmp, size) && !is_digit_str(tmp, size)) {
				printf("Letters are in upper case.\n");
				(*arg_ptr) = 0;
			} else {
				ToDecimal(tmp, base, arg_ptr, 0);
			}
		} else if (!strcmp(flag, "%CV\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			int base = va_arg(ptr, int);
			char tmp[STR_SIZE];
			count_values += fscanf(stream, "%s", tmp);
			int size = strlen(tmp);

			if (is_lower(tmp, size) && !is_digit_str(tmp, size)) {
				printf("Letters are in lower case.\n");
				(*arg_ptr) = 0;
			} else {
				ToDecimal(tmp, base, arg_ptr, 1);
			}
		} else {
			void *tmp_arg = va_arg(ptr, void*);
			count_values += fscanf(stream, flag, tmp_arg);
			char c;
			while (c = fgetc(stream) != ' ');
			if (c == ' '){
				ungetc(' ', stream);
			}
		}
		free(flag);
		flag = NULL;
	}

	if (flag){
		free(flag);
	}

	va_end(ptr);
	return count_values;
}

int oversscanf(char *buf, const char *format, ...) {
	if (!buf) {
		return -1;
	}

	int size_format = strlen(format);
	if (!size_format) {
		printf("There are no format to scan.\n");
		return -1;
	}

	int argc = 0;
	for (int i = 0; i < size_format; i++) {
		if (format[i] == '%') {
			if (format[i + 1] != '%') {
				argc++;
			}
		}
	}

	va_list ptr;
	va_start(ptr, format);
	int index = 0;
	int buf_index = 0;
	char *flag = NULL;
	int count_values = 0;

	for (int i = 0; i < argc; i++) {
		int size_flag = 1;
		int capacity_flag = 2;
		while (format[index] != '%' && index < size_format) {
			index++;
		}

		flag = (char *) malloc(sizeof(char) * (capacity_flag + 2));
		if (!flag) {
			printf("Error malloc memory.\n");
			return OPT_ERROR_ALLOC;
		}

		flag[0] = '%';
		index++;
		while ((isdigit(format[index])) || (isalpha(format[index])) && (index < size_format)) {
			char symbol = format[index];
			flag[size_flag] = symbol;
			size_flag++;

			if (size_flag == capacity_flag - 1) {
				capacity_flag *= 2;
				char *new_flag = (char *) realloc(flag, capacity_flag);
				if (!new_flag){
					free(flag);
					printf("Can`t realloc memory.\n");
					va_end(ptr);
					return OPT_ERROR_ALLOC;
				}
				flag = new_flag;
			}
			index++;
		}

		flag[size_flag] = '\0';

		if (!strcmp(flag, "%Ro\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			char tmp[STR_SIZE];
			count_values += sscanf(buf_index + buf, "%s", tmp);
			buf_index += strlen(tmp) + 1;
			FromRoman(tmp, arg_ptr);
		} else if (!strcmp(flag, "%Zr\0")) {
			unsigned int *arg_ptr = va_arg(ptr, unsigned int *);
			char tmp[STR_SIZE];
			count_values += sscanf(buf_index + buf, "%s", tmp);
			buf_index += strlen(tmp) + 1;
			FromZeckendorf(tmp, arg_ptr);
		} else if (!strcmp(flag, "%Cv\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			int base = va_arg(ptr, int);
			char number_str[STR_SIZE];
			count_values += sscanf(buf_index + buf, "%s", number_str);
			int size = strlen(number_str);
			buf_index += size + 1;

			if (is_upper(number_str, size) && !is_digit_str(number_str, size)) {
				printf("Letters are in upper case.\n");
				(*arg_ptr) = 0;
			} else {
				ToDecimal(number_str, base, arg_ptr, 0);
			}

		} else if (!strcmp(flag, "%CV\0")) {
			int *arg_ptr = va_arg(ptr, int *);
			int base = va_arg(ptr, int);
			char number_str[STR_SIZE];
			count_values += sscanf(buf_index + buf, "%s", number_str);
			int size = strlen(number_str);
			buf_index += size + 1;

			if (is_lower(number_str, size) && !is_digit_str(number_str, size)) {
				printf("Letters are in lower case.\n");
				(*arg_ptr) = 0;
			} else {
				ToDecimal(number_str, base, arg_ptr, 1);
			}

		} else {
			void *tmp_arg = va_arg(ptr, void *);
			count_values += sscanf(buf_index + buf, flag, tmp_arg);
			buf_index += strlen(tmp_arg) + 1;
		}

		free(flag);
		flag = NULL;
	}

	va_end(ptr);
	return count_values;
}



int main(int argc, char *argv[]) {

	char *input_file = argv[1];
	FILE *file = fopen(input_file, "r");
	if (file == NULL) {
		fprintf(stderr, "Error of opening file.\n");
		return 1;
	}

	char s[STR_SIZE];
	int num1, num2, num3;
	unsigned int num4;
	if (overfscanf(file, "%CV %Zr %3s %Cv %Ro", &num1, 16, &num4, s, &num2, 16, &num3) == OPT_ERROR_ALLOC) {
		printf("Error alloc memory.\n");
		fclose(file);
		return 1;
	}
	printf("Results: %d %d %s %d %d\n", num1, num4, s, num2, num3);

	if (oversscanf("012DEF 010011 ABOBA abc MCXXVIII", "%CV %Zr %s %Cv %Ro", &num1, 16, &num4, s, &num2, 36, &num3) == OPT_ERROR_ALLOC) {
		printf("Error alloc memory.\n");
		fclose(file);
		return 1;
	}
	printf("Results: %d %d %s %d %d\n", num1, num4, s, num2, num3);

	if (oversscanf("XV XVX 1010", "%Ro %Ro %CV", &num1, &num2, &num3, 2) == OPT_ERROR_ALLOC) {
		printf("Error alloc memory.\n");
		fclose(file);
		return 1;
	}
	printf("Results: %d %d %d\n", num1, num2, num3);
	fclose(file);
}
