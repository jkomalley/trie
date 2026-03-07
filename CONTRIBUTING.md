# Contributing to trie

## Build Setup

Requirements: `gcc` (or any C11-compatible compiler), `make`.

```sh
git clone https://github.com/jkomalley/trie
cd trie
make test
```

Optional tools for deeper checking:
- `valgrind` — heap profiler / leak checker
- `cppcheck` — static analysis

## Code Style

- **Language:** C11 (`-std=c11`)
- **Naming:** `trie_` prefix for public functions and types; `TRIE_` prefix for macros; `snake_case` throughout
- **Formatting:** 4-space indentation, no tabs
- **Public API:** every public symbol must have a doxygen comment in `src/trie.h`
- **Internal helpers:** declare `static`, no public header entry needed

## Testing

All changes to `src/trie.c` or `src/trie.h` must be accompanied by tests in `src/test.c`.

```sh
make test     # must show 0 failed
make asan     # must complete with no ASAN / UBSan errors
```

Run with Valgrind to verify there are no memory leaks:

```sh
make test
valgrind --leak-check=full --error-exitcode=1 ./test_bin
```

## Pull Request Process

1. Fork the repository and create a feature branch from `main`.
2. Write or update tests covering your change.
3. Ensure `make test` and `make asan` both pass with 0 failures.
4. Update `README.md` if you added or changed a public API function.
5. Add an entry to `CHANGELOG.md` under `[Unreleased]`.
6. Open a pull request — the CI workflow will run automatically.

## Commit Messages

Use the [Conventional Commits](https://www.conventionalcommits.org/) style:

```
feat: add trie_autocomplete function
fix: free orphaned nodes after trie_delete
docs: update README API reference
test: add Group H autocomplete tests
```
