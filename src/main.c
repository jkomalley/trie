#include "trie.h"
#include <stdio.h>

int main()
{
    TrieNode* root = createTrieNode('\0');

    insert(root, "apple");
    insert(root, "blue");
    insert(root, "aging");
    insert(root, "ball");

    printTrieEntries(root);

    if(search(root, "ball", 5)){
        printf("ball found :)\n");
    } else {
        printf("ball not found :(\n");
    }

    bool ret = delete(root, "ball", 5);

    if(ret){
        printf("delete successful\n");
    } else {
        printf("delete not successful\n");
    }

    if(search(root, "ball", 5)){
        printf("ball found :)\n");
    } else {
        printf("ball not found :(\n");
    }

    freeTrie(root);

    return 0;
}
