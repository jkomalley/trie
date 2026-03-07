#include "trie.h"
#include <stdio.h>
#include <string.h>

static int passed = 0;
static int failed = 0;

static void check(const char* name, int condition){
    if(condition){
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

int main(){
    TrieNode* root = createTrieNode('\0');

    // Insert + search (found)
    insert(root, "ball");
    check("search inserted word", search(root, "ball"));

    // Search for absent word
    check("search absent word returns false", !search(root, "cat"));

    // False-positive regression: prefix of inserted word
    insert(root, "apple");
    check("search prefix of inserted word returns false", !search(root, "app"));

    // False-positive regression: extension of inserted word
    check("search extension of inserted word returns false", !search(root, "balling"));

    // Prefix word inserted explicitly should be found
    insert(root, "app");
    check("search prefix after explicit insert", search(root, "app"));
    check("search longer word still found after prefix insert", search(root, "apple"));

    // Delete + verify not found
    check("delete existing word returns true", delete(root, "ball"));
    check("search deleted word returns false", !search(root, "ball"));

    // Delete non-existent word returns false
    check("delete absent word returns false", !delete(root, "xyz"));

    // Delete already-deleted word returns false
    check("delete already-deleted word returns false", !delete(root, "ball"));

    // Empty string
    // insert("") marks root endOfWord; search("") finds it — consistent behaviour
    insert(root, "");
    check("search empty string consistent with insert", search(root, ""));
    delete(root, "");
    check("search empty string after delete", !search(root, ""));

    // Long word (> 30 chars): insert + search
    char longword[] = "abcdefghijklmnopqrstuvwxyzabcde"; // 31 chars
    check("insert long word returns true", insert(root, longword));
    check("search long word returns true", search(root, longword));

    freeTrie(root);

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
