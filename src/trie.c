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
 * @param wordLen the length of the word
 * @return true if the word is found, otherwise false
 */
bool search(TrieNode* root, char* word, int wordLen){
    if(!root) return false;
    if(!word) return false;
    if(!wordLen) return false;

    TrieNode* curNode = root;

    int letterIndex;

    for(int wordIndex = 0; wordIndex < wordLen; wordIndex++){
        letterIndex = word[wordIndex] - 'a';
        if(curNode->children[letterIndex]){
            curNode = curNode->children[letterIndex];
        } else break;
    }

    if(curNode->endOfWord) return true;

    return false;
}

/**
 * Delete a word from the Trie
 * @param root the root of the Trie to delete from
 * @param word the word to delete
 * @param wordLen the length of the word
 * @return true if the word is deleted, otherwise false
 */
bool delete(TrieNode* root, char* word, int wordLen){
    TrieNode* curNode = root;

    int letterIndex = 0;

    for(int wordIndex = 0; wordIndex < wordLen; wordIndex++){
        letterIndex = word[wordIndex] - 'a';
        if(curNode->children[letterIndex]){
            curNode = curNode->children[letterIndex];
        } else break;
    }

    if(curNode->endOfWord) curNode->endOfWord = false;

    return !curNode->endOfWord;
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
void _printTrieHelper(TrieNode* root, char* prefix, int index){
    if(root->endOfWord){
        prefix[index] = '\0';
        printf("%s\n", prefix);
    }
    for(int alphaIndex = 0; alphaIndex < ALPHABET_SIZE; alphaIndex++){
        if(root->children[alphaIndex]){
            prefix[index] = alphaIndex + 'a';
            _printTrieHelper(root->children[alphaIndex], prefix, index + 1);   
        }
    }
}

/**
 * Print all words in the Trie
 * @param root the root of the Trie to be printed
 */
void printTrieEntries(TrieNode* root){
    char word[30];
    int index = 0;
    _printTrieHelper(root, word, index);
}
