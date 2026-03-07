CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11
SRC     = src/trie.c
ASAN    = -fsanitize=address,undefined -g
PREFIX  ?= /usr/local
DESTDIR ?=

.PHONY: all demo lib test asan install uninstall clean

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

install: lib
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/include
	install -m 644 libtrie.a $(DESTDIR)$(PREFIX)/lib/libtrie.a
	install -m 644 src/trie.h $(DESTDIR)$(PREFIX)/include/trie.h

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libtrie.a
	rm -f $(DESTDIR)$(PREFIX)/include/trie.h

clean:
	rm -f demo test_bin test_asan trie.o libtrie.a
