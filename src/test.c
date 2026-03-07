#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int passed = 0;
static int failed = 0;

static void check(const char* name, int condition) {
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

/* --- Callbacks for Group I --- */

static void count_cb(const char* word, void* userdata) {
    (void)word;
    (*(int*)userdata)++;
}

struct collector {
    char* words[16];
    int count;
};

static void collect_cb(const char* word, void* userdata) {
    struct collector* c = (struct collector*)userdata;
    if (c->count < 16) {
        c->words[c->count++] = strdup(word);
    }
}

int main(void) {

    /* --- Group A: Basic correctness --- */
    {
        TrieNode* root = trie_create();

        check("insert returns true", trie_insert(root, "apple") == true);
        check("search finds inserted word", trie_search(root, "apple") == true);
        check("search absent word returns false", trie_search(root, "orange") == false);
        check("insert normalizes uppercase", trie_insert(root, "Apple") == true);
        check("search finds normalized word", trie_search(root, "apple") == true);
        check("insert rejects digit", trie_insert(root, "app1e") == false);
        check("insert rejects space", trie_insert(root, "ap ple") == false);

        trie_free(root);
    }

    /* --- Group B: Prefix/extension regressions --- */
    {
        TrieNode* root = trie_create();
        trie_insert(root, "apple");

        check("search prefix of word returns false", trie_search(root, "app") == false);
        check("search extension of word returns false", trie_search(root, "applesauce") == false);

        trie_insert(root, "app");
        check("prefix found after explicit insert", trie_search(root, "app") == true);
        check("full word still found after prefix insert", trie_search(root, "apple") == true);

        trie_free(root);
    }

    /* --- Group C: Delete behavior --- */
    {
        TrieNode* root = trie_create();
        trie_insert(root, "ball");
        trie_insert(root, "bat");

        check("trie_delete existing word returns true", trie_delete(root, "ball") == true);
        check("search after delete returns false", trie_search(root, "ball") == false);
        check("trie_delete absent word returns false", trie_delete(root, "orange") == false);
        check("trie_delete already-deleted word returns false", trie_delete(root, "ball") == false);

        trie_free(root);
    }

    {
        TrieNode* root = trie_create();
        trie_insert(root, "bat");
        trie_insert(root, "batman");

        check("delete prefix: full word still found",
              trie_delete(root, "bat") == true && trie_search(root, "batman") == true);
        check("delete full word: prefix still found",
              trie_delete(root, "batman") == true && trie_search(root, "bat") == false);

        trie_free(root);
    }

    /* --- Group D: NULL / invalid input guards --- */
    {
        TrieNode* root = trie_create();

        check("trie_insert(NULL, word) returns false", trie_insert(NULL, "word") == false);
        check("trie_insert(root, NULL) returns false", trie_insert(root, NULL) == false);
        check("trie_delete(NULL, word) returns false", trie_delete(NULL, "word") == false);
        check("trie_delete(root, NULL) returns false", trie_delete(root, NULL) == false);
        check("trie_search(NULL, word) returns false", trie_search(NULL, "word") == false);
        check("trie_search(root, NULL) returns false", trie_search(root, NULL) == false);

        trie_free(root);

        trie_free(NULL);
        check("trie_free(NULL) does not crash", 1);

        trie_print(NULL);
        check("trie_print(NULL) does not crash", 1);
    }

    /* --- Group E: Edge cases --- */
    {
        TrieNode* root = trie_create();

        check("insert empty string returns true", trie_insert(root, "") == true);
        check("search empty string returns true", trie_search(root, "") == true);
        check("trie_delete empty string returns true", trie_delete(root, "") == true);
        check("search empty string after delete returns false", trie_search(root, "") == false);

        trie_free(root);
    }

    {
        TrieNode* root = trie_create();

        char long_word[] = "abcdefghijklmnopqrstuvwxyzabcde";
        check("insert long word returns true", trie_insert(root, long_word) == true);
        check("search long word returns true", trie_search(root, long_word) == true);

        trie_free(root);
    }

    {
        TrieNode* root = trie_create();
        check("trie_create returns non-NULL", root != NULL);
        trie_free(root);
    }

    /* --- Group F: trie_starts_with --- */
    {
        TrieNode* root = trie_create();
        trie_insert(root, "apple");
        trie_insert(root, "application");
        trie_insert(root, "ball");

        check("starts_with: prefix exists", trie_starts_with(root, "app") == true);
        check("starts_with: absent prefix returns false", trie_starts_with(root, "xyz") == false);
        check("starts_with: exact word is valid prefix", trie_starts_with(root, "apple") == true);
        check("starts_with: empty prefix always true", trie_starts_with(root, "") == true);
        check("starts_with: uppercase prefix normalized", trie_starts_with(root, "APP") == true);
        check("starts_with(NULL, prefix) returns false", trie_starts_with(NULL, "app") == false);
        check("starts_with(root, NULL) returns false", trie_starts_with(root, NULL) == false);

        trie_free(root);
    }

    /* --- Group G: trie_count --- */
    {
        TrieNode* root = trie_create();

        check("count empty trie is 0", trie_count(root) == 0);

        trie_insert(root, "apple");
        trie_insert(root, "ball");
        trie_insert(root, "bat");
        check("count after 3 inserts is 3", trie_count(root) == 3);

        trie_delete(root, "ball");
        check("count after delete is 2", trie_count(root) == 2);

        trie_insert(root, "apple"); /* duplicate */
        check("count unchanged after duplicate insert", trie_count(root) == 2);

        check("trie_count(NULL) is 0", trie_count(NULL) == 0);

        trie_insert(root, "");
        check("count includes empty string", trie_count(root) == 3);

        trie_free(root);
    }

    /* --- Group H: trie_autocomplete --- */
    {
        TrieNode* root = trie_create();
        trie_insert(root, "app");
        trie_insert(root, "apple");
        trie_insert(root, "application");
        trie_insert(root, "bat");

        char* results[10];
        memset(results, 0, sizeof(results));

        int n = trie_autocomplete(root, "app", results, 10, 255);
        check("autocomplete returns 3 matches", n == 3);
        check("autocomplete result[0] is 'app'",
              n >= 1 && results[0] && strcmp(results[0], "app") == 0);
        check("autocomplete result[1] is 'apple'",
              n >= 2 && results[1] && strcmp(results[1], "apple") == 0);
        check("autocomplete result[2] is 'application'",
              n >= 3 && results[2] && strcmp(results[2], "application") == 0);
        for (int i = 0; i < n; i++) { free(results[i]); results[i] = NULL; }

        n = trie_autocomplete(root, "xyz", results, 10, 255);
        check("autocomplete no match returns 0", n == 0);

        n = trie_autocomplete(root, "app", results, 2, 255);
        check("autocomplete respects max_results", n == 2);
        for (int i = 0; i < n; i++) { free(results[i]); results[i] = NULL; }

        check("autocomplete NULL root returns -1",
              trie_autocomplete(NULL, "app", results, 10, 255) == -1);
        check("autocomplete NULL prefix returns -1",
              trie_autocomplete(root, NULL, results, 10, 255) == -1);
        check("autocomplete NULL results returns -1",
              trie_autocomplete(root, "app", NULL, 10, 255) == -1);
        check("autocomplete max_results=0 returns -1",
              trie_autocomplete(root, "app", results, 0, 255) == -1);

        trie_free(root);
    }

    /* --- Group I: trie_for_each --- */
    {
        TrieNode* root = trie_create();
        trie_insert(root, "cat");
        trie_insert(root, "apple");
        trie_insert(root, "ball");

        /* NULL callback must not crash */
        trie_for_each(root, NULL, NULL);
        check("for_each NULL callback is no-op", 1);

        /* NULL root must not crash */
        trie_for_each(NULL, count_cb, NULL);
        check("for_each NULL root is no-op", 1);

        /* Count all words via callback */
        int word_count = 0;
        trie_for_each(root, count_cb, &word_count);
        check("for_each visits all words", word_count == 3);

        /* Verify lexicographic order */
        struct collector col = {{0}, 0};
        trie_for_each(root, collect_cb, &col);
        check("for_each order: apple first",
              col.count >= 1 && col.words[0] && strcmp(col.words[0], "apple") == 0);
        check("for_each order: ball second",
              col.count >= 2 && col.words[1] && strcmp(col.words[1], "ball") == 0);
        check("for_each order: cat third",
              col.count >= 3 && col.words[2] && strcmp(col.words[2], "cat") == 0);
        for (int i = 0; i < col.count; i++) { free(col.words[i]); col.words[i] = NULL; }

        trie_free(root);
    }

    /* --- Group J: trie_normalize --- */
    {
        char word1[] = "HeLLo";
        trie_normalize(word1);
        check("normalize mixed case", strcmp(word1, "hello") == 0);

        char word2[] = "hello";
        trie_normalize(word2);
        check("normalize already lowercase unchanged", strcmp(word2, "hello") == 0);

        char word3[] = "";
        trie_normalize(word3);
        check("normalize empty string is no-op", strcmp(word3, "") == 0);

        trie_normalize(NULL);
        check("normalize NULL does not crash", 1);
    }

    /* --- Summary --- */
    printf("\n%d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
