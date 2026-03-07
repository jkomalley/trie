CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L
SRC     = src/trie.c
ASAN    = -fsanitize=address,undefined -g
PREFIX  ?= /usr/local
DESTDIR ?=

CLANG      ?= clang
FUZZ_FLAGS  = -fsanitize=fuzzer,address -g
FUZZ_TIME  ?= 30

COVERAGE_FLAGS = --coverage

VERSION = 1.0.0

.PHONY: all demo lib test asan valgrind cppcheck coverage fuzz bench docs ci install uninstall clean

all: demo

demo: $(SRC) examples/demo.c
	$(CC) $(CFLAGS) $^ -o $@

lib: trie.o
	ar rcs libtrie.a trie.o

trie.o: $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC) src/test.c
	$(CC) $(CFLAGS) $^ -o test_bin
	./test_bin

asan: $(SRC) src/test.c
	$(CC) $(CFLAGS) $(ASAN) $^ -o test_asan
	./test_asan

valgrind: $(SRC) src/test.c
	$(CC) $(CFLAGS) $^ -o test_bin
	valgrind --leak-check=full --error-exitcode=1 ./test_bin

cppcheck:
	cppcheck --enable=all --error-exitcode=1 --check-level=exhaustive \
	  --suppress=missingIncludeSystem \
	  src/

coverage: $(SRC) src/test.c
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $^ -o test_cov
	./test_cov
	lcov --capture --directory . --output-file coverage.lcov \
	     --exclude '*/test.c'
	rm -f test_cov *.gcda *.gcno

fuzz: $(SRC) fuzz/fuzz_trie.c
	mkdir -p fuzz/corpus
	$(CLANG) $(CFLAGS) $(FUZZ_FLAGS) $^ -o fuzz_bin
	./fuzz_bin -max_total_time=$(FUZZ_TIME) fuzz/corpus/

bench: $(SRC) benchmarks/bench.c
	$(CC) $(CFLAGS) -O2 $^ -o bench_bin
	./bench_bin

docs:
	doxygen Doxyfile

ci: test asan valgrind cppcheck coverage

install: lib
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -m 644 libtrie.a $(DESTDIR)$(PREFIX)/lib/libtrie.a
	install -m 644 src/trie.h $(DESTDIR)$(PREFIX)/include/trie.h
	sed -e 's|@PREFIX@|$(PREFIX)|g' \
	    -e 's|@VERSION@|$(VERSION)|g' \
	    trie.pc.in > trie.pc
	install -m 644 trie.pc $(DESTDIR)$(PREFIX)/lib/pkgconfig/trie.pc

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libtrie.a
	rm -f $(DESTDIR)$(PREFIX)/include/trie.h
	rm -f $(DESTDIR)$(PREFIX)/lib/pkgconfig/trie.pc

clean:
	rm -f demo test_bin test_asan test_cov fuzz_bin bench_bin trie.o libtrie.a trie.pc
	rm -f *.gcda *.gcno coverage.lcov
	rm -rf coverage_html/ docs/
