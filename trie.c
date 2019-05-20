// Thien Trinh
// CSE 374
// Homework 5
// 05/15/2019

/*
 Define the behavor for each of the three functions for
 object Trie and its helper functions.

 builTrie take into a and Trie and a file name and populate
 the Trie with word inside the file

 getWord take into a pointer TrieNode, a populated Trie,
 and the input char array of the user. Print out the word
 or the corresponding error message, and return the pointer
 TrieNode

 freeTrieNode take a TrieNode, and clean the char array and 
 the TrieNodes it pointed to recurvesively
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"

int getNumber(char c);
int noNumberLeft(int index, char* input);

void buildTrie(Trie* trie, char* fileName) {
  FILE* file = fopen(fileName, "r");
  if (file == NULL) {
    fprintf(stderr, "%s: file does not exist\n", fileName);
    exit(EXIT_FAILURE);
  }

  /*
   Creates an array of char for the word
   Creates a pointer to Trie
   Allocates memory for the TrieNode of the Trie
   Creates a pointer to the TrieNode.
   */
  char temp[MAX_LINE_LENGTH];
  Trie* currentTrie = trie;
  currentTrie->root = (TrieNode*)malloc(sizeof(TrieNode));
  if (currentTrie->root == NULL) {
    fprintf(stderr, "Cannot allocated memory for new TrieNode\n");
    exit(EXIT_FAILURE);
  }
  TrieNode* currentNode = currentTrie->root;

  while (fgets(temp, MAX_LINE_LENGTH, file)) {
    /*
     Replaces the last newline char with the null char
     For each char, changes to the correspond index 
     */
    temp[strlen(temp) - 1] = '\0';
    for (int i = 0; i < strlen(temp); i++) {
      int index = getNumber(temp[i]);
      if (index == -1) {
        fprintf(stderr, "Improper input inside file: Capitalization\n");
        exit(EXIT_FAILURE);
      }

      /*
       If the next TrieNode at the index of the input char is NULL
       creates a new TrieNode at that index
       Move currentNode to the next TrieNode according to the index
       */
      if (currentNode->next[index] == NULL) {
        TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
        if (newNode == NULL) {
          fprintf(stderr, "Cannot allocated memory for new TrieNode\n");
          exit(EXIT_FAILURE);
        }

        currentNode->next[index] = newNode;
      }
      currentNode = currentNode->next[index];
    }

    /*
     Copy the temp char array into an allocated char array
     If the currentNode word is NULL, then paste in the new word
     If not, go down the '#' TrieNode until the next '#' TrieNode
     is NULL
     Allocate memory for a new TrieNode, put in the word, and paste
     it to the currentNode '#' index.
     Reset currentNode back to the root of the Trie.
     */
    int size = strlen(temp);
    char* word = (char*)malloc(size * sizeof(char));
    if (word == NULL) {
      fprintf(stderr, "Cannot allocated memory for new char array\n");
      exit(EXIT_FAILURE);
    }

    strncpy(word, temp, size);
    if (currentNode->word == NULL) {
      currentNode->word = word;
    } else {
      while (currentNode->next[8] != NULL) {
        /* 
         Go all the way down the '#' TrieNode 
         until the next '#' TrieNode is NULL
	*/
        currentNode = currentNode->next[8];
      }

      TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
      if (newNode == NULL) {
        fprintf(stderr, "Cannot allocated memory for new TrieNode\n");
        exit(EXIT_FAILURE);
      }

      newNode->word = word;
      currentNode->next[8] = newNode;
    }
    // Reset the currentNode pointer back to the root of the Trie
    currentNode = currentTrie->root;
  }

  fclose(file);
}


TrieNode* getWord(TrieNode* currentNode, Trie* trie, char* input) {
  if (input[0] == '#') {
    if (strlen(input) == 2) {  // 2 because including the newline char
      if (currentNode->next[8] == NULL) {
        printf("There are no more T9onyms\n");
        return currentNode;
      } else {
        currentNode = currentNode->next[8];
        printf("%s\n", currentNode->word);
        return currentNode;
      }
    } else {
      // Error so resets currentNode back to Trie root
      printf("Improper input\n");
      currentNode = trie->root;
      return currentNode;
    }
  } else {
    // If the input is a array of number char, reset currentNode
    TrieNode* currentNode = trie->root;
    int size = strlen(input) - 1;  // -1 because there is an extra newline char
    int impError = 0;  // Flag for improper input
    int notDirec = 0;  // Flag for word not in directory
    int noT9 = 0;  // Flag for no more T9onyms

    /*
     Start from the Trie root
     Going through the TrieNode according the each input char
     Reach the TrieNode correspond to the end of the input char
     */
    for (int i = 0; i < size; i++) {
      if (currentNode != NULL) {
        // If on of the input char is '#', but NOT at the beginning
        if (input[i] == '#') {
          // Check if indices at input '#' to the end of input is similarly '#'
          if (noNumberLeft(i, input)) {
            if (currentNode->next[8] == NULL) {
              noT9 = 1;
              currentNode = NULL;
            } else {
              currentNode = currentNode->next[8];
            }
          } else {
            // The '#' char is at somewhere in the middle of a number char array
            impError = 1;
            currentNode = NULL;
          }
        } else {
          // Subtrac 2 from input to match index of next[] array
          int index = input[i] - '0' - 2;
          if (currentNode->next[index] == NULL) {
            notDirec = 1;
            currentNode = NULL;
          } else {
            currentNode = currentNode->next[index];
          }
        }
      }
    }

    /*
     Print out the message correspond to the triggered flag
     Reset currentNode to the trie root unless there is a word
     */
    if (currentNode == NULL && noT9 == 1) {
      currentNode = trie->root;
      printf("There are no more T9onyms\n");
    } else if (currentNode == NULL && impError == 1) {
      currentNode = trie->root;
      printf("Improper input\n");
    } else if (currentNode == NULL && notDirec == 1) {
      currentNode = trie->root;
      printf("Not found in current dictionary\n");
    } else if (currentNode->word == NULL) {
      currentNode = trie->root;
      printf("Not found in current dictionary\n");
    } else {
      printf("%s\n", currentNode->word);
    }
    return currentNode;
  }
}


void freeTrieNode(TrieNode* root) {
  for (int i = 0; i < MAX_INDEX; i++) {
    if (root->next[i] != NULL) {
      freeTrieNode(root->next[i]);
    }
  }

  free(root->word);
  root->word = NULL;
  free(root);
  root = NULL;
}

/*
 Helper method that take into a char and return
 the corresponding numbers of T9 setup.
 */
int getNumber(char c) {
  if (c == 'a' || c == 'b' || c == 'c') {
    return 0;
  } else if (c == 'd' || c == 'e' || c == 'f') {
    return 1;
  } else if (c == 'g' || c == 'h' || c == 'i') {
    return 2;
  } else if (c == 'j' || c == 'k' || c == 'l') {
    return 3;
  } else if (c == 'm' || c == 'n' || c == 'o') {
    return 4;
  } else if (c == 'p' || c == 'q' || c == 'r' || c == 's') {
    return 5;
  } else if (c == 't' || c == 'u' || c == 'v') {
    return 6;
  } else if (c == 'w' || c == 'x' || c == 'y' || c == 'z') {
    return 7;
  } else if (c == '#') {
    return 8;
  } else {
    return -1;
  }
}

/*
 Helper method that take an index and an input char
 array
 It will check if from the index to the end of the input
 char array whether there are number left
 (It actually check if there are any number beside # left
 from the index to the end)
 Return 1 if no number is left (all '#')
 Return 0 if there is number left (not all '#')
 */
int noNumberLeft(int index, char* input) {
  for (int i = index; i < strlen(input) - 1; i++) {
    if (input[i] != '#') {
      return 0;
    }
  }
  return 1;
}
