#include "../src/trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WORDS_FILE "examples/words.txt"
#define MAX_WORDS  200000
#define MAX_WORD   256

static double elapsed_ms(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) * 1000.0
         + (double)(end.tv_nsec - start.tv_nsec) / 1e6;
}

static void bench(const char *label, size_t n, double ms) {
    printf("%-20s %7zu words in %8.2f ms  (%10.0f ops/sec)\n",
           label, n, ms, (double)n / (ms / 1000.0));
}

int main(void) {
    /* Load words */
    FILE *f = fopen(WORDS_FILE, "r");
    if (!f) { fprintf(stderr, "cannot open %s\n", WORDS_FILE); return 1; }

    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (!words) { fclose(f); return 1; }

    size_t n = 0;
    char buf[MAX_WORD];
    while (n < MAX_WORDS && fgets(buf, sizeof(buf), f)) {
        size_t len = strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) buf[--len] = '\0';
        if (len == 0) continue;
        words[n] = strdup(buf);
        if (!words[n]) break;
        n++;
    }
    fclose(f);
    printf("Loaded %zu words from %s\n\n", n, WORDS_FILE);

    struct timespec t0, t1;
    TrieNode *root;

    /* Insert */
    root = trie_create();
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t i = 0; i < n; i++) trie_insert(root, words[i]);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    bench("insert", n, elapsed_ms(t0, t1));

    /* Search */
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t i = 0; i < n; i++) trie_search(root, words[i]);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    bench("search", n, elapsed_ms(t0, t1));

    /* Delete */
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t i = 0; i < n; i++) trie_delete(root, words[i]);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    bench("delete", n, elapsed_ms(t0, t1));

    printf("\nsizeof(TrieNode) = %zu bytes\n", sizeof(TrieNode));

    trie_free(root);
    for (size_t i = 0; i < n; i++) free(words[i]);
    free(words);
    return 0;
}
