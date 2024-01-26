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

    freeTrie(root);

    return 0;
}
