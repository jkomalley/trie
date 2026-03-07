#include "../src/trie.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * libFuzzer entry point.
 *
 * Treats the input buffer as a stream of (op_byte, NUL-terminated word) pairs:
 *   op % 3 == 0  → trie_insert
 *   op % 3 == 1  → trie_search
 *   op % 3 == 2  → trie_delete
 *
 * After processing all pairs, runs trie_autocomplete with the first word seen
 * (if any), then frees everything.
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    TrieNode *root = trie_create();
    if (!root) return 0;

    const uint8_t *p   = data;
    const uint8_t *end = data + size;

    char first_word[TRIE_MAX_WORD_LEN + 1];
    first_word[0] = '\0';
    int seen_first = 0;

    while (p < end) {
        uint8_t op = *p++;
        if (p >= end) break;

        /* Find the NUL terminator for the word, or use the rest of the buffer */
        const uint8_t *nul = (const uint8_t *)memchr(p, '\0', (size_t)(end - p));
        size_t word_len = nul ? (size_t)(nul - p) : (size_t)(end - p);

        if (word_len > TRIE_MAX_WORD_LEN) word_len = TRIE_MAX_WORD_LEN;

        char word[TRIE_MAX_WORD_LEN + 1];
        memcpy(word, p, word_len);
        word[word_len] = '\0';

        p = nul ? nul + 1 : end;

        if (!seen_first && word_len > 0) {
            memcpy(first_word, word, word_len + 1);
            seen_first = 1;
        }

        switch (op % 3) {
            case 0: trie_insert(root, word);  break;
            case 1: trie_search(root, word);  break;
            case 2: trie_delete(root, word);  break;
        }
    }

    if (seen_first) {
        char *results[8];
        int n = trie_autocomplete(root, first_word, results, 8, TRIE_MAX_WORD_LEN);
        for (int i = 0; i < n; i++) free(results[i]);
    }

    trie_free(root);
    return 0;
}
