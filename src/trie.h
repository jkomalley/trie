#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

#define ALPHABET_SIZE 26 // only lowercase english characters

// Trie node structure
typedef struct TrieNode
{
    char value;
    struct TrieNode* children[ALPHABET_SIZE];
    bool endOfWord;
} TrieNode;

// Trie function prototypes

/**
 * Create an empty root node for a new Trie
 * @return new trie root node, or NULL on allocation failure
 */
TrieNode* createTrie(void);

/**
 * Insert a new word into the trie
 * @param root the root of the Trie to insert into
 * @param word the word to insert
 * @return true upon success, otherwise false
 */
bool insert(TrieNode* root, char* word);

/**
 * Search for a word in the Trie
 * @param root the root of the Trie to search in
 * @param word the word to search for
 * @return true if the word is found, otherwise false
 */
bool search(TrieNode* root, char* word);

/**
 * Delete a word from the Trie
 * @param root the root of the Trie to delete from
 * @param word the word to delete
 * @return true if the word is deleted, otherwise false
 */
bool deleteWord(TrieNode* root, char* word);

/**
 * Free the entire Trie structure
 * @param root the root of the Trie to be freed
 */
void freeTrie(TrieNode* root);

/**
 * Print all words in the Trie
 * @param root the root of the Trie to be printed
 */
void printTrieEntries(TrieNode* root);

#endif /* TRIE_H */
