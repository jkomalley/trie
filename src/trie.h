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
 * Create a new node and return it
 * @param value character value for the new node
 * @return new trie node
 */
TrieNode* createTrieNode(char value);

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
 * @param wordLen the length of the word
 * @return true if the word is found, otherwise false
 */
bool search(TrieNode* root, char* word, int wordLen);

/**
 * Delete a word from the Trie
 * @param root the root of the Trie to delete from
 * @param word the word to delete
 * @param wordLen the length of the word
 * @return true if the word is deleted, otherwise false
 */
bool delete(TrieNode* root, char* word, int wordLen);

/**
 * Free the entire Trie structure
 * @param root the root of the Trie to be freed
 */
void freeTrie(TrieNode* root);

void printTrieEntries(TrieNode* root);

#endif /* TRIE_H */
