CC     = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SRC    = src/trie.c
ASAN   = -fsanitize=address,undefined -g

.PHONY: all clean test asan

all: main

main: $(SRC) src/main.c
	$(CC) $(CFLAGS) $^ -o $@

test: $(SRC) src/test.c
	$(CC) $(CFLAGS) $^ -o test_bin
	./test_bin

asan: $(SRC) src/test.c
	$(CC) $(CFLAGS) $(ASAN) $^ -o test_asan
	./test_asan

clean:
	rm -f main test_bin test_asan
