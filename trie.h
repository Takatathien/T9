// Thien Trinh
// CSE 374
// Homework 5
// 05/15/2019

/* 
 Contruct objects called Trie, consisting of multiple
 TrieNode
 Each TrieNode may have an array of char and pointers
 to 9 other TrieNode
 Set up 3 functions for the object Trie
 */
#ifndef TRIE_H
#define TRIE_H

#define MAX_LINE_LENGTH 100
#define MAX_INDEX 9

typedef struct TrieNode {
  char* word;
  struct TrieNode* next[MAX_INDEX];  // 2-9 and "#"
} TrieNode;

typedef struct Trie {
  TrieNode* root;
} Trie;

void buildTrie(Trie* trie, char* fileName);

TrieNode* getWord(TrieNode* currentNode, Trie* trie, char* input);

void freeTrieNode(TrieNode* root);

#endif
