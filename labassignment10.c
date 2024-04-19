#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    int count;
};

// Initializes a trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->isEndOfWord = false;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts a word into the trie
void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *crawler = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!crawler->children[index]) {
            crawler->children[index] = createTrieNode();
        }
        crawler = crawler->children[index];
    }
    crawler->isEndOfWord = true;
    crawler->count++;
}

// Computes the number of occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode *root, const char *word) {
    struct TrieNode *crawler = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!crawler->children[index]) {
            return 0; // Word not found
        }
        crawler = crawler->children[index];
    }
    if (crawler != NULL && crawler->isEndOfWord) {
        return crawler->count;
    }
    return 0;
}

// Deallocates memory used by trie nodes
void deallocateTrie(struct TrieNode *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deallocateTrie(root->children[i]);
    }
    free(root);
}

// Reads the dictionary from a file and returns the number of words read
int readDictionary(const char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    // Create and populate trie with dictionary words
    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    // Test the trie with some words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    // Deallocate memory used by the trie
    deallocateTrie(root);

    return 0;
}
