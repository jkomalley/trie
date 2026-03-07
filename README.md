# trie

A trie (prefix tree) data structure library in C11. Memory-safe, ASAN-clean, and fully tested.

**Version:** 1.0.0

---

## Features

- Insert, search, and delete words
- Prefix lookup (`trie_starts_with`)
- Word count (`trie_count`)
- Autocomplete (`trie_autocomplete`)
- Iterate all words in lexicographic order (`trie_for_each`)
- Input normalization (`trie_normalize`)

**Limitations:**
- Accepts lowercase English letters `a`–`z` only; uppercase is silently normalized
- Maximum word length: 255 bytes
- Not thread-safe

---

## Build

```sh
make            # build demo binary (./demo)
make test       # build and run test suite
make asan       # build and run tests with AddressSanitizer + UBSan
make valgrind   # run tests under Valgrind (requires valgrind)
make cppcheck   # run static analysis (requires cppcheck)
make ci         # run all of the above (mirrors GitHub Actions)
make lib        # build libtrie.a static library
make install    # install libtrie.a and trie.h (PREFIX=/usr/local by default)
make uninstall  # remove installed files
make clean      # remove build artifacts
```

Override install prefix:

```sh
make install PREFIX=/usr/local DESTDIR=/staging
```

---

## Quick Start

```c
#include "trie.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    TrieNode* root = trie_create();

    trie_insert(root, "apple");
    trie_insert(root, "application");
    trie_insert(root, "bat");

    printf("search apple: %d\n", trie_search(root, "apple"));
    printf("starts with app: %d\n", trie_starts_with(root, "app"));
    printf("word count: %zu\n", trie_count(root));

    char* results[10];
    int n = trie_autocomplete(root, "app", results, 10, 255);
    for (int i = 0; i < n; i++) {
        printf("  %s\n", results[i]);
        free(results[i]);
    }

    trie_free(root);
    return 0;
}
```

Compile against the installed library:

```sh
gcc -std=c11 my_program.c -ltrie -o my_program
```

---

## API Reference

### `trie_create`

```c
TrieNode* trie_create(void);
```

Allocates and returns a new empty trie root node. Returns `NULL` on allocation failure.

---

### `trie_insert`

```c
bool trie_insert(TrieNode* root, const char* word);
```

Inserts `word` into the trie. Uppercase letters are normalized to lowercase. Any non-alphabetic character (digit, space, etc.) causes immediate rejection.

**Returns:** `true` on success; `false` if `root` or `word` is `NULL`, if `word` contains an invalid character, or on allocation failure.

---

### `trie_search`

```c
bool trie_search(TrieNode* root, const char* word);
```

Searches for an exact match of `word`. Uppercase letters are normalized before searching.

**Returns:** `true` if `word` was previously inserted; `false` otherwise.

---

### `trie_delete`

```c
bool trie_delete(TrieNode* root, const char* word);
```

Removes `word` from the trie and frees any nodes that are no longer needed. Uppercase letters are normalized before deletion.

**Returns:** `true` if the word existed and was removed; `false` otherwise.

---

### `trie_free`

```c
void trie_free(TrieNode* root);
```

Frees the entire trie. Passing `NULL` is safe.

---

### `trie_print`

```c
void trie_print(TrieNode* root);
```

Prints all words to `stdout`, one per line, in lexicographic order. Passing `NULL` is safe.

---

### `trie_starts_with`

```c
bool trie_starts_with(TrieNode* root, const char* prefix);
```

Tests whether any word in the trie begins with `prefix`. An empty prefix returns `true` for any non-NULL root.

**Returns:** `true` if at least one matching word exists; `false` if `root` or `prefix` is `NULL`, the prefix contains an invalid character, or no match exists.

---

### `trie_count`

```c
size_t trie_count(TrieNode* root);
```

Counts all words stored in the trie. Passing `NULL` returns `0`.

---

### `trie_autocomplete`

```c
int trie_autocomplete(TrieNode* root, const char* prefix,
                      char** results, int max_results, int max_word_len);
```

Collects up to `max_results` words beginning with `prefix` into `results`.

**Memory ownership:** Each non-NULL element written into `results` is a heap-allocated string that the **caller must `free()`**. On error, any partially allocated strings are freed before returning.

| Parameter | Description |
|---|---|
| `root` | Root of the trie |
| `prefix` | NUL-terminated prefix to search for |
| `results` | Output array of at least `max_results` `char*` pointers |
| `max_results` | Maximum results to return; must be > 0 |
| `max_word_len` | Skip words longer than this; must be > 0 |

**Returns:** Number of results written on success, `0` if no matches, or `-1` on invalid arguments.

---

### `trie_for_each`

```c
void trie_for_each(TrieNode* root, trie_word_fn cb, void* userdata);
```

Calls `cb(word, userdata)` for every word in the trie, in lexicographic order. Passing `NULL` for `root` or `cb` is safe (no-op).

```c
typedef void (*trie_word_fn)(const char* word, void* userdata);
```

---

### `trie_normalize`

```c
void trie_normalize(char* word);
```

Converts every character in `word` to lowercase in-place. Passing `NULL` is safe.

---

## Macros

| Macro | Value | Description |
|---|---|---|
| `TRIE_VERSION` | `"1.0.0"` | Version string |
| `TRIE_VERSION_MAJOR` | `1` | Major version |
| `TRIE_VERSION_MINOR` | `0` | Minor version |
| `TRIE_VERSION_PATCH` | `0` | Patch version |
| `TRIE_ALPHABET_SIZE` | `26` | Number of children per node |
| `TRIE_MAX_WORD_LEN` | `255` | Maximum word length (bytes) |

---

## License

[MIT](LICENSE)
