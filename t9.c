// Thien Trinh
// CSE 374
// Homework 5
// 05/15/2019

/*
 The main program to run the interactive program T9

 main function calls the buildTrie fuction and passes
 in the file Trie and the file name
 Then calls up userInterface function to open up an
 interactive session with the user
 Then frees up the Trie memory

 userInterface create a currentNode pointer to the root
 of Trie object
 Depedning on the input of the user, will past into getWord
 the currentNode pointer and the user input, or free up
 the memory of TrieNode inside the Trie and exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"

void userInterface(Trie* trie);

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "%i: incorrect number of arguments\n", argc);
    exit(EXIT_FAILURE);
  }

  Trie* trie = (Trie*)malloc(sizeof(Trie));
  if (trie == NULL) {
    fprintf(stderr, "Cannot allocated memory for new Trie\n");
    exit(EXIT_FAILURE);
  }

  buildTrie(trie, argv[1]);
  userInterface(trie);

  free(trie);  // Free the Trie itself
  return EXIT_SUCCESS;
}

void userInterface(Trie* trie) {
  printf("Enter \"exit\" to quit.\n");
  printf("Enter Key Sequence (or \"#\" for next word):\n");

  TrieNode* currentNode = trie->root;
  char input[MAX_LINE_LENGTH];
  while (fgets(input, MAX_LINE_LENGTH, stdin)) {
    if (strncmp(input, "exit", 4) == 0) {
      // Reset currentNode back to the trie root to free all TrieNode.
      currentNode = trie->root;
      freeTrieNode(currentNode);
      return;
    }

    currentNode = getWord(currentNode, trie, input);
    printf("Enter Key Sequence (or \"#\" for next word):\n");
  }
}

