#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

/* Version */
#define TRIE_VERSION_MAJOR 1
#define TRIE_VERSION_MINOR 0
#define TRIE_VERSION_PATCH 0
#define TRIE_VERSION "1.0.0"

/** Only lowercase English letters a–z are stored. */
#define TRIE_ALPHABET_SIZE 26

/** Maximum word length accepted by the API (bytes, excluding NUL). */
#define TRIE_MAX_WORD_LEN 255

/** Callback type for trie_for_each(). */
typedef void (*trie_word_fn)(const char* word, void* userdata);

/** Trie node structure. */
typedef struct TrieNode {
    char value;
    struct TrieNode* children[TRIE_ALPHABET_SIZE];
    bool endOfWord;
} TrieNode;

/**
 * Create an empty root node for a new trie.
 * @return New root node, or NULL on allocation failure.
 */
TrieNode* trie_create(void);

/**
 * Insert a word into the trie.
 * Uppercase letters are normalized to lowercase. Any other non-alphabetic
 * character causes immediate rejection.
 * @param root  Root of the trie.
 * @param word  NUL-terminated word to insert.
 * @return true on success; false on NULL args, invalid character, or alloc failure.
 */
bool trie_insert(TrieNode* root, const char* word);

/**
 * Search for a word in the trie.
 * Uppercase letters are normalized to lowercase before searching.
 * @param root  Root of the trie.
 * @param word  NUL-terminated word to search for.
 * @return true if the exact word was previously inserted, otherwise false.
 */
bool trie_search(TrieNode* root, const char* word);

/**
 * Delete a word from the trie, freeing now-orphaned nodes.
 * Uppercase letters are normalized to lowercase before deletion.
 * @param root  Root of the trie.
 * @param word  NUL-terminated word to delete.
 * @return true if the word existed and was deleted, false otherwise.
 */
bool trie_delete(TrieNode* root, const char* word);

/**
 * Free the entire trie structure.
 * @param root  Root of the trie (may be NULL).
 */
void trie_free(TrieNode* root);

/**
 * Print all words stored in the trie to stdout, one per line, in
 * lexicographic order.
 * @param root  Root of the trie (may be NULL).
 */
void trie_print(TrieNode* root);

/**
 * Test whether any word in the trie begins with the given prefix.
 * An empty prefix always matches (returns true for any non-NULL root).
 * @param root    Root of the trie.
 * @param prefix  NUL-terminated prefix (a–z, or uppercase normalized).
 * @return true if at least one matching word exists, false otherwise.
 */
bool trie_starts_with(TrieNode* root, const char* prefix);

/**
 * Count the number of words stored in the trie.
 * @param root  Root of the trie (may be NULL).
 * @return Number of inserted words.
 */
size_t trie_count(TrieNode* root);

/**
 * Collect all words that begin with @p prefix into a caller-supplied array.
 *
 * Memory ownership: each written element of @p results is a heap-allocated
 * string that the caller must free(). On error, any partially allocated
 * strings are freed and the array is left in an unspecified state.
 *
 * @param root          Root of the trie.
 * @param prefix        NUL-terminated prefix to search for.
 * @param results       Output array of at least @p max_results char* pointers.
 * @param max_results   Maximum results to return; must be > 0.
 * @param max_word_len  Maximum word length to include; must be > 0.
 * @return Number of results written on success, 0 if none, or -1 on invalid args.
 */
int trie_autocomplete(TrieNode* root, const char* prefix,
                      char** results, int max_results, int max_word_len);

/**
 * Invoke @p cb for every word in the trie in lexicographic order.
 * @p userdata is forwarded unchanged to each invocation.
 * @param root      Root of the trie (may be NULL).
 * @param cb        Callback; if NULL the function is a no-op.
 * @param userdata  Opaque pointer forwarded to @p cb.
 */
void trie_for_each(TrieNode* root, trie_word_fn cb, void* userdata);

/**
 * Normalize a word in-place by converting every character to lowercase.
 * @param word  NUL-terminated string to normalize (may be NULL — no-op).
 */
void trie_normalize(char* word);

#endif /* TRIE_H */
