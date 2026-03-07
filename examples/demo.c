#include "../src/trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    TrieNode* root = trie_create();
    if (!root) {
        fprintf(stderr, "Failed to create trie\n");
        return 1;
    }

    /* Load words from words.txt */
    FILE* f = fopen("examples/words.txt", "r");
    if (!f) {
        fprintf(stderr, "Could not open examples/words.txt\n");
        return 1;
    }

    char line[TRIE_MAX_WORD_LEN + 2];
    int loaded = 0;
    while (fgets(line, sizeof(line), f)) {
        /* strip newline */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        trie_normalize(line);
        if (trie_insert(root, line)) loaded++;
    }
    fclose(f);
    printf("Loaded %d words.\n\n", loaded);

    /* Demo: trie_starts_with */
    const char* prefixes[] = {"app", "bat", "xyz"};
    for (int i = 0; i < 3; i++) {
        printf("trie_starts_with(\"%s\"): %s\n",
               prefixes[i], trie_starts_with(root, prefixes[i]) ? "true" : "false");
    }
    printf("\n");

    /* Demo: trie_count */
    printf("trie_count: %zu words in trie\n\n", trie_count(root));

    /* Demo: trie_autocomplete */
    char* results[10];
    memset(results, 0, sizeof(results));
    int n = trie_autocomplete(root, "app", results, 10, 255);
    printf("trie_autocomplete(\"app\", max=10): %d result(s)\n", n);
    for (int i = 0; i < n; i++) {
        printf("  %s\n", results[i]);
        free(results[i]);
    }
    printf("\n");

    /* Demo: trie_search / trie_delete */
    trie_insert(root, "ball");
    printf("trie_search(\"ball\"): %s\n",
           trie_search(root, "ball") ? "found" : "not found");
    trie_delete(root, "ball");
    printf("After trie_delete(\"ball\"), trie_search: %s\n",
           trie_search(root, "ball") ? "found" : "not found");

    trie_free(root);
    return 0;
}
