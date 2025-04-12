#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum kOpt {
	OPT_SUCCESS,
	OPT_ERROR_OPEN_FILE,
	OPT_ERROR_MEMORY,
	OPT_INVALID_INPUTS
} kOpt;

typedef struct TreeNode {
	char *word;
	int count;
	struct TreeNode *left, *right;
} TreeNode;

typedef struct  NodeArray {
	TreeNode **nodes;
	int size;
	int capacity;
} NodeArray;

TreeNode *insert(TreeNode *node, const char *word);
TreeNode *createNode(const char *word);


char *combineDelimiters(int argc, char *argv[]) {
	size_t total_length = 0;
	for (int i = 2; i < argc; i++) {
		total_length += strlen(argv[i]);
	}

	char *delimiters = (char*)malloc(total_length + 1);
	if (!delimiters){
		return NULL;
	}
	delimiters[0] = '\0';// Начинаем с пустой строки

	for (int i = 2; i < argc; i++) {
		strcat(delimiters, argv[i]);
	}

	return delimiters;
}


kOpt processFile(const char *file_path, const char *delimiters, TreeNode **root) {
	FILE *file = fopen(file_path, "r");
	if (!file) {
		return OPT_ERROR_OPEN_FILE;
	}

	int c;
	char *word = NULL;
	size_t length = 0, capacity = 0;

	while ((c = fgetc(file)) != EOF) {
		if (strchr(delimiters, c)) {
			if (length > 0) {
				word[length] = '\0';
				*root = insert(*root, word);
				if (!*root) {
					if (word) free(word);
					return OPT_ERROR_MEMORY;
				}
				length = 0;
			}
		} else {
			if (length + 1 >= capacity) {
				capacity = capacity == 0 ? 16 : capacity * 2;
				char *tmp = (char*)realloc(word, capacity);
				if (!tmp) {
					free(word);
					return OPT_ERROR_MEMORY;
				}
				word = tmp;
			}
			word[length++] = (char) c;
		}
	}

	if (length > 0) {
		word[length] = '\0';
		*root = insert(*root, word);
		if (!*root) {
			free(word);
			return OPT_ERROR_MEMORY;
		}
	}

	free(word);
	fclose(file);
	return OPT_SUCCESS;
}

TreeNode *insert(TreeNode *node, const char *word) {
	if (node == NULL) return createNode(word);
	int cmp = strcmp(word, node->word);
	if (cmp == 0) {
		node->count++;
	} else if (cmp < 0) {
		node->left = insert(node->left, word);
	} else {
		node->right = insert(node->right, word);
	}
	return node;
}

TreeNode *createNode(const char *word) {
	TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
	if (!node) {
		return NULL;
	}
	node->word = strdup(word);
	node->count = 1;
	node->left = node->right = NULL;
	return node;
}

void inorderPrint(TreeNode *root) {
	if (root) {
		inorderPrint(root->left);
		printf("%s: %d\n", root->word, root->count);
		inorderPrint(root->right);
	}
}

TreeNode *findWord(TreeNode *root, const char *word) {
	if (root == NULL || strcmp(root->word, word) == 0) return root;
	if (strcmp(word, root->word) < 0) return findWord(root->left, word);
	return findWord(root->right, word);
}

TreeNode *findLongest(TreeNode *root) {
	if (root == NULL) return NULL;
	TreeNode *longest = root, *left = findLongest(root->left), *right = findLongest(root->right);
	if (left && strlen(left->word) > strlen(longest->word)) longest = left;
	if (right && strlen(right->word) > strlen(longest->word)) longest = right;
	return longest;
}

TreeNode *findShortest(TreeNode *root) {
	if (root == NULL) return NULL;
	TreeNode *shortest = root, *left = findShortest(root->left), *right = findShortest(root->right);
	if (left && strlen(left->word) < strlen(shortest->word)) shortest = left;
	if (right && strlen(right->word) < strlen(shortest->word)) shortest = right;
	return shortest;
}

kOpt AddToArray(NodeArray *array, TreeNode *node) {
	if (array->size >= array->capacity) {
		array->capacity *= 2;
		TreeNode **new_nodes = (TreeNode **)realloc(array->nodes, array->capacity * sizeof(TreeNode *));
		if (new_nodes == NULL) {
			return OPT_ERROR_MEMORY;
		}
		array->nodes = new_nodes;
	}
	array->nodes[array->size++] = node;
	return OPT_SUCCESS;
}

kOpt TraverseTree(TreeNode *root, NodeArray *array) {
	if (root == NULL) return OPT_SUCCESS;
	if (AddToArray(array, root) != OPT_SUCCESS) {
		return OPT_ERROR_MEMORY;
	}

	if (TraverseTree(root->left, array) != OPT_SUCCESS) {
		return OPT_ERROR_MEMORY;
	}

	if (TraverseTree(root->right, array) != OPT_SUCCESS) {
		return OPT_ERROR_MEMORY;
	}
}

int CompareNodes(const void *a, const void *b) {
	TreeNode *nodeA = *(TreeNode **)a;
	TreeNode *nodeB = *(TreeNode **)b;
	return nodeB->count - nodeA->count;
}

TreeNode **FindTopN(TreeNode *root, long int n) {
	NodeArray array;
	array.capacity = 10;
	array.size = 0;
	array.nodes = (TreeNode **)malloc(array.capacity * sizeof(TreeNode *));

	if (array.nodes == NULL) {
		return NULL;
	}

	if (TraverseTree(root, &array) != OPT_SUCCESS) {
		free(array.nodes);
		return NULL;
	}

	qsort(array.nodes, array.size, sizeof(TreeNode *), CompareNodes);

	TreeNode **topN = (TreeNode **)malloc(n * sizeof(TreeNode *));
	if (topN == NULL) {
		free(array.nodes);
		return NULL;
	}

	for (int i = 0; i < n && i < array.size; i++) {
		topN[i] = array.nodes[i];
	}

	free(array.nodes);
	return topN;
}

int treeDepth(TreeNode *root) {
	if (root == NULL) return 0;
	int left_depth = treeDepth(root->left);
	int right_depth = treeDepth(root->right);
	return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

void freeTree(TreeNode *root) {
	if (root) {
		freeTree(root->left);
		freeTree(root->right);
		free(root->word);
		free(root);
	}
}

void saveTree(TreeNode *root, FILE *file) {
	if (root == NULL) {
		fprintf(file, "(null)\n");
		return;
	}
	fprintf(file, "%s %d\n", root->word, root->count);
	saveTree(root->left, file);
	saveTree(root->right, file);
}

TreeNode *loadTree(FILE *file) {
	char buffer[1024];
	if (fscanf(file, "%1023s", buffer) != 1 || strcmp(buffer, "(null)") == 0) return NULL;

	int count;
	fscanf(file, "%d", &count);
	TreeNode *node = createNode(buffer);
	if (!node) return NULL;

	node->count = count;
	node->left = loadTree(file);
	node->right = loadTree(file);
	return node;
}

void printTree(TreeNode *node, int depth) {
	if (node == NULL) return;

	printTree(node->right, depth + 1);
	for (int i = 0; i < depth; i++) {
		printf("    ");
	}
	printf("%s (%d)\n", node->word, node->count);
	printTree(node->left, depth + 1);
}

int CountNodes(TreeNode *root) {
	if (root == NULL) {
		return 0;
	}

	return 1 + CountNodes(root->left) + CountNodes(root->right);
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage: %s <file_path> <delimiter> <delimiter> ....\n", argv[0]);
		return OPT_INVALID_INPUTS;
	}

	char *delimiters = combineDelimiters(argc, argv);
	if (!delimiters){
		fprintf(stderr, "Error alloc memory.\n");
		return 1;
	}

	TreeNode *root = NULL;
	int tmp = processFile(argv[1], delimiters, &root);
	if (tmp == OPT_ERROR_MEMORY){
		fprintf(stderr, "Error alloc memory.\n");
		free(delimiters);
		return OPT_ERROR_MEMORY;
	} else if (tmp == OPT_ERROR_OPEN_FILE){
		fprintf(stderr, "Error open file\n");
		free(delimiters);
		return OPT_ERROR_OPEN_FILE;
	}

	char *choice = NULL;
	size_t choice_len = 0;
	do {
		printf("1. Print word frequencies\n");
		printf("2. Find word count\n");
		printf("3. Print top N frequent words\n");
		printf("4. Find longest and shortest words\n");
		printf("5. Calculate tree depth\n");
		printf("6. Save tree to file\n");
		printf("7. Load tree from file\n");
		printf("8. Print tree\n");
		printf("9. Exit\n\n");
		printf("Enter your choice: ");

		if (getline(&choice, &choice_len, stdin) == -1) {
			printf("Error reading input.\n");
			continue;
		}

		choice[strcspn(choice, "\n")] = 0;

		if (strlen(choice) > 1) {
			printf("Incorrect input.\n\n");
			continue;
		}

		switch (choice[0]) {
			case '1':
				inorderPrint(root);
				break;
			case '2':
				printf("Enter word: ");
				char *word = NULL;
				size_t len = 0;
				getline(&word, &len, stdin);
				word[strcspn(word, "\n")] = 0;
				TreeNode *found = findWord(root, word);
				printf("%s: %d\n", word, found ? found->count : 0);
				free(word);
				break;
			case '3':
				printf("Enter N: ");
				char *input = NULL;
				size_t leni = 0;

				if (getline(&input, &leni, stdin) != -1) {
					input[strcspn(input, "\n")] = 0;
					char *endptr;
					long n = strtol(input, &endptr, 10);
					if (*endptr == '\0' && n > 0) {
						TreeNode **topWords;
						int count_words = CountNodes(root);

						if (count_words < n) {
							n = count_words;
						}
						topWords = FindTopN(root, n);

						if (topWords == NULL) {
							fprintf(stderr, "Error alloc memory.\n");
							return OPT_ERROR_MEMORY;
						}

						for (int i = 0; i < n; i++) {
							if (topWords[i] != NULL) {
								printf("%s: %d\n", topWords[i]->word, topWords[i]->count);
							}
						}

						free(topWords);
					} else {
						printf("Invalid input for N.\n");
					}
				} else {
					printf("Error reading input.\n");
				}

				free(input);
				break;
			case '4':
				printf("Longest word: %s\n", findLongest(root)->word);
				printf("Shortest word: %s\n", findShortest(root)->word);
				break;
			case '5':
				printf("Tree depth: %d\n", treeDepth(root));
				break;
			case '6': {
				FILE *file = fopen("tree.txt", "w");
				if (!file) {
					fprintf(stderr, "Error open file\n");
					break;
				}
				saveTree(root, file);
				fclose(file);
				printf("Tree saved to tree.txt\n");
				break;
			}
			case '7': {
				FILE *file = fopen("tree.txt", "r");
				if (!file) {
					fprintf(stderr, "Error open file\n");
					free(delimiters);
					free(choice);
					freeTree(root);
					return OPT_ERROR_OPEN_FILE;
				}
				freeTree(root);
				root = loadTree(file);
				if (!root){
					fprintf(stderr,"Error alloc memory\n");
					free(delimiters);
					free(choice);
					freeTree(root);
					fclose(file);
					return OPT_ERROR_MEMORY;
				}
				fclose(file);
				printf("Tree loaded from tree.txt\n");
				break;
			}
			case '8':
				printTree(root, 0);
				break;
			case '9':
				printf("Exiting...\n");
				break;
			default:
				printf("Incorrect input\n");
				break;
		}
		printf("\n");
	} while (strcmp(choice, "9") != 0);

	free(delimiters);
	free(choice);
	freeTree(root);
	return OPT_SUCCESS;
}
