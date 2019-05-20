# Thien Trinh
# CSE 374
# Homework 5
# 05/15/2019

# A Makefile that would recompile only the part
# of the t9 program that has been update

# The executable
t9: t9.o trie.o
	gcc -Wall -g -std=c11 -o output t9.o trie.o
trie.o: trie.h trie.c
	gcc -Wall -g -std=c11 -c trie.c
t9.0: trie.h t9.c
	gcc -Wall -g -std=c11 -c t9.c

# Remove built files and backups.
clean:
	rm -f *.o output
