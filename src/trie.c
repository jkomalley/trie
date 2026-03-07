#include "trie.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Create a new node and return it
 * @param value character value for the new node
 * @return new trie node
 */
TrieNode* createTrieNode(char value){
    // allocate memory for the new node
    TrieNode* newNode = (TrieNode*)calloc(1, sizeof(TrieNode));

    // check that the allocation succeeded
    if(!newNode) return NULL;

    // init node values
    newNode->value = value;
    newNode->endOfWord = false;
    for(int index = 0; index < ALPHABET_SIZE; index++){
        newNode->children[index] = NULL;
    }

    return newNode;
}

/**
 * Insert a new word into the trie
 * @param root the root of the Trie to insert into
 * @param word the word to insert
 * @return true upon success, otherwise false
 */
bool insert(TrieNode* root, char* word){
    TrieNode* curNode = root;

    for(int wordIndex = 0; word[wordIndex] != '\0'; wordIndex++){
        if(word[wordIndex] < 'a' || word[wordIndex] > 'z') return false;

        int childIndex = word[wordIndex] - 'a';

        if(!curNode->children[childIndex]){
            curNode->children[childIndex] = createTrieNode(word[wordIndex]);
        }

        curNode = curNode->children[childIndex];
    }

    curNode->endOfWord = true;

    return true;
}

/**
 * Search for a word in the Trie
 * @param root the root of the Trie to search in
 * @param word the word to search for
 * @return true if the word is found, otherwise false
 */
bool search(TrieNode* root, char* word){
    if(!root) return false;
    if(!word) return false;

    TrieNode* curNode = root;

    for(int wordIndex = 0; word[wordIndex] != '\0'; wordIndex++){
        if(word[wordIndex] < 'a' || word[wordIndex] > 'z') return false;
        int letterIndex = word[wordIndex] - 'a';
        if(!curNode->children[letterIndex]) return false;
        curNode = curNode->children[letterIndex];
    }

    return curNode->endOfWord;
}

// Recursive helper: returns true if the node should be freed by its caller.
// Sets *deleted to true if the word was found and unmarked.
static bool _deleteHelper(TrieNode* node, char* word, int depth, bool* deleted){
    if(word[depth] == '\0'){
        if(node->endOfWord){
            node->endOfWord = false;
            *deleted = true;
        }
        // Suggest freeing only if we deleted something and node has no children
        if(!(*deleted)) return false;
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i]) return false;
        }
        return true;
    }

    if(word[depth] < 'a' || word[depth] > 'z') return false;
    int letterIndex = word[depth] - 'a';
    if(!node->children[letterIndex]) return false;

    if(_deleteHelper(node->children[letterIndex], word, depth + 1, deleted)){
        free(node->children[letterIndex]);
        node->children[letterIndex] = NULL;
        // Propagate freeing upward if this node is now a dead-end
        if(!node->endOfWord){
            for(int i = 0; i < ALPHABET_SIZE; i++){
                if(node->children[i]) return false;
            }
            return true;
        }
    }
    return false;
}

/**
 * Delete a word from the Trie
 * @param root the root of the Trie to delete from
 * @param word the word to delete
 * @return true if the word is deleted, otherwise false
 */
bool delete(TrieNode* root, char* word){
    bool deleted = false;
    _deleteHelper(root, word, 0, &deleted);
    return deleted;
}

/**
 * Free the entire Trie structure
 * @param root the root of the Trie to be freed
 */
void freeTrie(TrieNode* root){
    for(int alphaIndex = 0; alphaIndex < ALPHABET_SIZE; alphaIndex++){
        if(root->children[alphaIndex]) freeTrie(root->children[alphaIndex]);
    }
    free(root);
}

// helper func for printTrieEntries
void _printTrieHelper(TrieNode* root, char* prefix, int index, int maxLen){
    if(index >= maxLen) return;
    if(root->endOfWord){
        prefix[index] = '\0';
        printf("%s\n", prefix);
    }
    for(int alphaIndex = 0; alphaIndex < ALPHABET_SIZE; alphaIndex++){
        if(root->children[alphaIndex]){
            prefix[index] = alphaIndex + 'a';
            _printTrieHelper(root->children[alphaIndex], prefix, index + 1, maxLen);
        }
    }
}

/**
 * Print all words in the Trie
 * @param root the root of the Trie to be printed
 */
void printTrieEntries(TrieNode* root){
    char word[256];
    int index = 0;
    _printTrieHelper(root, word, index, 255);
}
