#include "trie.h"
#include <stdio.h>
#include <stdlib.h>

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

int main(void){

    /* --- Group A: Basic correctness --- */
    {
        TrieNode* root = createTrie();

        check("insert returns true", insert(root, "apple") == true);
        check("search finds inserted word", search(root, "apple") == true);
        check("search absent word returns false", search(root, "orange") == false);
        check("insert rejects uppercase", insert(root, "Apple") == false);
        check("insert rejects digit", insert(root, "app1e") == false);
        check("insert rejects space", insert(root, "ap ple") == false);

        freeTrie(root);
    }

    /* --- Group B: Prefix/extension regressions --- */
    {
        TrieNode* root = createTrie();
        insert(root, "apple");

        check("search prefix of word returns false", search(root, "app") == false);
        check("search extension of word returns false", search(root, "applesauce") == false);

        insert(root, "app");
        check("prefix found after explicit insert", search(root, "app") == true);
        check("full word still found after prefix insert", search(root, "apple") == true);

        freeTrie(root);
    }

    /* --- Group C: Delete behavior --- */
    {
        TrieNode* root = createTrie();
        insert(root, "ball");
        insert(root, "bat");

        check("deleteWord existing word returns true", deleteWord(root, "ball") == true);
        check("search after delete returns false", search(root, "ball") == false);
        check("deleteWord absent word returns false", deleteWord(root, "orange") == false);
        check("deleteWord already-deleted word returns false", deleteWord(root, "ball") == false);

        freeTrie(root);
    }

    {
        TrieNode* root = createTrie();
        insert(root, "bat");
        insert(root, "batman");

        check("delete prefix: full word still found", deleteWord(root, "bat") == true && search(root, "batman") == true);
        check("delete full word: prefix still found", deleteWord(root, "batman") == true && search(root, "bat") == false);

        freeTrie(root);
    }

    /* --- Group D: NULL / invalid input guards --- */
    {
        TrieNode* root = createTrie();

        check("insert(NULL, word) returns false", insert(NULL, "word") == false);
        check("insert(root, NULL) returns false", insert(root, NULL) == false);
        check("deleteWord(NULL, word) returns false", deleteWord(NULL, "word") == false);
        check("deleteWord(root, NULL) returns false", deleteWord(root, NULL) == false);
        check("search(NULL, word) returns false", search(NULL, "word") == false);
        check("search(root, NULL) returns false", search(root, NULL) == false);

        freeTrie(root);

        /* These must not crash */
        freeTrie(NULL);
        check("freeTrie(NULL) does not crash", 1);

        printTrieEntries(NULL);
        check("printTrieEntries(NULL) does not crash", 1);
    }

    /* --- Group E: Edge cases --- */
    {
        TrieNode* root = createTrie();

        check("insert empty string returns true", insert(root, "") == true);
        check("search empty string returns true", search(root, "") == true);
        check("deleteWord empty string returns true", deleteWord(root, "") == true);
        check("search empty string after delete returns false", search(root, "") == false);

        freeTrie(root);
    }

    {
        TrieNode* root = createTrie();

        /* 31-character word (within 255-byte print buffer) */
        char long_word[] = "abcdefghijklmnopqrstuvwxyzabcde";
        check("insert long word returns true", insert(root, long_word) == true);
        check("search long word returns true", search(root, long_word) == true);

        freeTrie(root);
    }

    {
        TrieNode* root = createTrie();
        check("createTrie returns non-NULL", root != NULL);
        freeTrie(root);
    }

    /* --- Summary --- */
    printf("\n%d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
