#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_SIZE 256

void ToLowercase(char* str) {
	for (int i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
}

int ConvertSS(int num, int base) {
	int number = 0, k = 1;
	while (num > 0) {
		int remainder = num % base;
		number += remainder * k;
		num /= base;
		k *= 10;
	}
	return number;
}

char* Strdup(const char* kstr) {
	size_t len = strlen(kstr) + 1;
	char* copy = malloc(len);
	if (copy != NULL) {
		memcpy(copy, kstr, len);
	}
	return copy;
}

char** ReadTokens(const char* kfilename, int* count) {
	FILE* file = fopen(kfilename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error of openning file %s.\n", kfilename);
		return NULL;
	}

	char** tokens = NULL;
	char buffer[MAX_TOKEN_SIZE];
	*count = 0;

	while (fscanf(file, "%s", buffer) == 1) {
		tokens = realloc(tokens, (*count + 1) * sizeof(char*));
		if (tokens == NULL) {
			fprintf(stderr, "Memory allocation error");
			fclose(file);
			return NULL;
		}
		tokens[*count] = Strdup(buffer);
		if (tokens[*count] == NULL) {
			fprintf(stderr, "Token memory allocation error");
			fclose(file);
			return NULL;
		}
		(*count)++;
	}

	fclose(file);
	return tokens;
}

int WriteTokensToFileForR(char** tokens1, int count1, char** tokens2, int count2, const char* koutput_filename) {
	FILE* output_file = fopen(koutput_filename, "w");
	if (output_file == NULL) {
		fprintf(stderr, "Error of openning file %s.\n", koutput_filename);
		return -1;
	}

	int i = 0, j = 0;
	while (i < count1 || j < count2) {
		if (i < count1) {
			fprintf(output_file, "%s", tokens1[i++]);
			if (i < count1 || j < count2) {
				fprintf(output_file, " ");
			}
		}
		if (j < count2) {
			fprintf(output_file, "%s", tokens2[j++]);
			if (i < count1 || j < count2) {
				fprintf(output_file, " ");
			}
		}
	}

	fclose(output_file);
	return 0;
}

int WriteTokensToFileForA(char** tokens, int count, const char* koutput_filename) {
	FILE* output_file = fopen(koutput_filename, "w");
	if (output_file == NULL) {
		fprintf(stderr, "Error of openning file %s.\n", koutput_filename);
		return -1;
	}

	for (int i = 0; i < count; i++) {
		char* token = tokens[i];
		if ((i + 1) % 10 == 0) {
			ToLowercase(token);
			for (int j = 0; token[j]; j++) {
				int digit = token[j] - '0';
				fprintf(output_file, "%d", ConvertSS(digit, 4));
			}
		} else if (((i + 1) % 5 == 0) && ((i + 1) % 10 != 0)) {
			for (int j = 0; token[j]; j++) {
				int digit = token[j] - '0';
				fprintf(output_file, "%d", ConvertSS(digit, 8));
			}
		} else if (((i + 1) % 2 == 0) && ((i + 1) % 10 != 0)) {
			ToLowercase(token);
			fprintf(output_file, "%s", token);
		} else {
			fprintf(output_file, "%s", token);
		}

		if (i < count - 1) {
			fprintf(output_file, " ");
		}
	}

	fclose(output_file);
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "There are not enough arguments.\n");
		return 1;
	}

	const char* kflag = argv[1];

	if (strcmp(kflag, "-r") == 0) {
		if (argc != 5) {
			fprintf(stderr, "Incorrect number of arguments for the flag -r.\n");
			return 1;
		}

		int count1, count2;
		char** tokens1 = ReadTokens(argv[2], &count1);
		char** tokens2 = ReadTokens(argv[3], &count2);

		if (tokens1 == NULL || tokens2 == NULL) {
			fprintf(stderr, "Error of reading files.\n");
			return 1;
		}

		if (WriteTokensToFileForR(tokens1, count1, tokens2, count2, argv[4]) != 0) {
			for (int i = 0; i < count1; i++) {
				free(tokens1[i]);
			}
			for (int i = 0; i < count2; i++) {
				free(tokens2[i]);
			}
			free(tokens1);
			free(tokens2);
			return 1;
		}

		for (int i = 0; i < count1; i++) {
			free(tokens1[i]);
		}
		for (int i = 0; i < count2; i++) {
			free(tokens2[i]);
		}
		free(tokens1);
		free(tokens2);
	} else if (strcmp(kflag, "-a") == 0) {
		if (argc != 4) {
			fprintf(stderr, "Incorrect number of arguments for the flag -а.\n");
			return 1;
		}

		int count;
		char** tokens = ReadTokens(argv[2], &count);

		if (tokens == NULL) {
			fprintf(stderr, "Error of reading file.");
			return 1;
		}

		if (WriteTokensToFileForA(tokens, count, argv[3]) != 0) {
			for (int i = 0; i < count; i++){
				free(tokens[i]);
			}
			free(tokens);
			return 1;
		}

		for (int i = 0; i < count; i++){
			free(tokens[i]);
		}
		free(tokens);
	} else {
		fprintf(stderr, "Unsupported flag: %s. It must be '-r' or '-a'\n", kflag);
		return 1;
	}

	return 0;
}
