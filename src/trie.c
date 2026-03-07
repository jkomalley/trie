#include "trie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static TrieNode* create_trie_node(char value) {
    TrieNode* node = (TrieNode*)calloc(1, sizeof(TrieNode));
    if (!node) return NULL;
    node->value = value;
    return node;
}

TrieNode* trie_create(void) {
    return create_trie_node('\0');
}

bool trie_insert(TrieNode* root, const char* word) {
    if (!root) return false;
    if (!word) return false;

    TrieNode* cur = root;

    for (int i = 0; word[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)word[i]);
        if (c < 'a' || c > 'z') return false;

        int idx = c - 'a';
        if (!cur->children[idx]) {
            cur->children[idx] = create_trie_node(c);
        }
        if (!cur->children[idx]) return false;
        cur = cur->children[idx];
    }

    cur->endOfWord = true;
    return true;
}

bool trie_search(TrieNode* root, const char* word) {
    if (!root) return false;
    if (!word) return false;

    TrieNode* cur = root;

    for (int i = 0; word[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)word[i]);
        if (c < 'a' || c > 'z') return false;
        int idx = c - 'a';
        if (!cur->children[idx]) return false;
        cur = cur->children[idx];
    }

    return cur->endOfWord;
}

/* Returns true if the node can be freed by its caller. Sets *deleted on match. */
static bool delete_helper(TrieNode* node, const char* word, int depth, bool* deleted) {
    if (word[depth] == '\0') {
        if (node->endOfWord) {
            node->endOfWord = false;
            *deleted = true;
        }
        if (!(*deleted)) return false;
        for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
            if (node->children[i]) return false;
        }
        return true;
    }

    char c = (char)tolower((unsigned char)word[depth]);
    if (c < 'a' || c > 'z') return false;
    int idx = c - 'a';
    if (!node->children[idx]) return false;

    if (delete_helper(node->children[idx], word, depth + 1, deleted)) {
        free(node->children[idx]);
        node->children[idx] = NULL;
        if (!node->endOfWord) {
            for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
                if (node->children[i]) return false;
            }
            return true;
        }
    }
    return false;
}

bool trie_delete(TrieNode* root, const char* word) {
    if (!root) return false;
    if (!word) return false;

    bool deleted = false;
    delete_helper(root, word, 0, &deleted);
    return deleted;
}

void trie_free(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (root->children[i]) trie_free(root->children[i]);
    }
    free(root);
}

static void for_each_helper(TrieNode* node, char* buf, int depth,
                             trie_word_fn cb, void* userdata) {
    if (depth >= TRIE_MAX_WORD_LEN) return;
    if (node->endOfWord) {
        buf[depth] = '\0';
        cb(buf, userdata);
    }
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buf[depth] = (char)('a' + i);
            for_each_helper(node->children[i], buf, depth + 1, cb, userdata);
        }
    }
}

void trie_for_each(TrieNode* root, trie_word_fn cb, void* userdata) {
    if (!root || !cb) return;
    char buf[TRIE_MAX_WORD_LEN + 1];
    for_each_helper(root, buf, 0, cb, userdata);
}

static void print_callback(const char* word, void* userdata) {
    (void)userdata;
    printf("%s\n", word);
}

void trie_print(TrieNode* root) {
    trie_for_each(root, print_callback, NULL);
}

bool trie_starts_with(TrieNode* root, const char* prefix) {
    if (!root || !prefix) return false;

    TrieNode* cur = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)prefix[i]);
        if (c < 'a' || c > 'z') return false;
        int idx = c - 'a';
        if (!cur->children[idx]) return false;
        cur = cur->children[idx];
    }
    return true;
}

size_t trie_count(TrieNode* root) {
    if (!root) return 0;
    size_t count = root->endOfWord ? 1 : 0;
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (root->children[i]) count += trie_count(root->children[i]);
    }
    return count;
}

static int autocomplete_helper(TrieNode* node, char* buf, int depth,
                                char** results, int max_results, int max_word_len,
                                int* count) {
    if (depth >= max_word_len) return 0;
    if (node->endOfWord) {
        buf[depth] = '\0';
        results[*count] = strdup(buf);
        if (!results[*count]) return -1;
        (*count)++;
        if (*count >= max_results) return 0;
    }
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buf[depth] = (char)('a' + i);
            int ret = autocomplete_helper(node->children[i], buf, depth + 1,
                                          results, max_results, max_word_len, count);
            if (ret < 0) return -1;
            if (*count >= max_results) return 0;
        }
    }
    return 0;
}

int trie_autocomplete(TrieNode* root, const char* prefix,
                      char** results, int max_results, int max_word_len) {
    if (!root || !prefix || !results || max_results <= 0 || max_word_len <= 0)
        return -1;

    TrieNode* cur = root;
    char buf[TRIE_MAX_WORD_LEN + 1];
    int prefix_len = 0;

    for (int i = 0; prefix[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)prefix[i]);
        if (c < 'a' || c > 'z') return -1;
        if (prefix_len >= max_word_len) return 0;
        int idx = c - 'a';
        if (!cur->children[idx]) return 0;
        buf[prefix_len++] = c;
        cur = cur->children[idx];
    }

    int count = 0;
    if (autocomplete_helper(cur, buf, prefix_len, results, max_results,
                            max_word_len, &count) < 0) {
        for (int i = 0; i < count; i++) {
            free(results[i]);
            results[i] = NULL;
        }
        return -1;
    }
    return count;
}

void trie_normalize(char* word) {
    if (!word) return;
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = (char)tolower((unsigned char)word[i]);
    }
}
