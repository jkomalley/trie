#include "trie.h"
#include <stdio.h>

int main()
{
    TrieNode* root = createTrie();
    if(!root){
        fprintf(stderr, "Failed to create trie\n");
        return 1;
    }

    insert(root, "apple");
    insert(root, "blue");
    insert(root, "aging");
    insert(root, "ball");

    printTrieEntries(root);

    if(search(root, "ball")){
        printf("ball found :)\n");
    } else {
        printf("ball not found :(\n");
    }

    bool ret = deleteWord(root, "ball");

    if(ret){
        printf("delete successful\n");
    } else {
        printf("delete not successful\n");
    }

    if(search(root, "ball")){
        printf("ball found :)\n");
    } else {
        printf("ball not found :(\n");
    }

    freeTrie(root);

    return 0;
}
