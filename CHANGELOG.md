# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2026-03-07

### Added
- `trie_starts_with`: test whether any word begins with a given prefix
- `trie_count`: return the number of words stored in the trie
- `trie_autocomplete`: collect all words beginning with a prefix into a caller-owned array
- `trie_for_each`: iterate all words in lexicographic order via a callback
- `trie_normalize`: convert a word to lowercase in-place
- Version macros: `TRIE_VERSION`, `TRIE_VERSION_MAJOR`, `TRIE_VERSION_MINOR`, `TRIE_VERSION_PATCH`
- `TRIE_MAX_WORD_LEN` macro (255)
- `trie_word_fn` callback typedef
- `lib`, `install`, `uninstall` Makefile targets
- Full doxygen comments for all public symbols
- CI workflow (build, test, ASAN, cppcheck, Valgrind)
- Automated release workflow triggered by `v*.*.*` tags
- GitHub issue templates and pull request template
- `CONTRIBUTING.md`, `CHANGELOG.md`, `SECURITY.md`, `CODE_OF_CONDUCT.md`
- `examples/demo.c` demonstrating the full API
- `.clang-tidy` configuration

### Changed
- **Breaking:** All public symbols renamed to `trie_` prefix / `snake_case`:
  - `createTrie` → `trie_create`
  - `insert` → `trie_insert`
  - `search` → `trie_search`
  - `deleteWord` → `trie_delete`
  - `freeTrie` → `trie_free`
  - `printTrieEntries` → `trie_print`
  - `ALPHABET_SIZE` → `TRIE_ALPHABET_SIZE`
- `trie_insert`, `trie_search`, `trie_delete`: uppercase letters are now silently normalized to lowercase instead of being rejected
- Demo source moved from `src/main.c` to `examples/demo.c`
- Word list moved from `data/words.txt` to `examples/words.txt`

[Unreleased]: https://github.com/jkomalley/trie/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/jkomalley/trie/releases/tag/v1.0.0
