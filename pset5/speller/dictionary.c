// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#define HASHTABLE_SIZE 65536

// Declaring global variable for linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Hash function by Delipity
int hashfunction(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ tolower(word[i]);
    }
    return hash % HASHTABLE_SIZE;
}

// Declaring global variable for hash table (array of linked lists, each element of array is a node *)
node* hashtable[HASHTABLE_SIZE];

// Word counter varaible
int counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // Finding bucket the word is in
    node *cursor = hashtable[hashfunction(word)];

    while (cursor != NULL)
    {
        // Compare string for every note
        if ((strcasecmp(cursor->word, word)) == 0)
        {
            return true;
        }
        else
        {
            // Continue searching
            cursor = cursor->next;
        }
    }
    // Word is mispelled
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Opens dictionary file
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open dictionary");
        return false;
    }

    // Create array for words to be stored
    char word[LENGTH + 1];

    // Scan dictionary word by word and stores it into variable
    while (fscanf(file, "%s", word) !=EOF)
    {

    // Allocate memory for a node pointer for each new word
    node *new_node = malloc(sizeof(node));

    // Unload dictionary if out of memory
    if (new_node == NULL)
    {
        unload();
        return false;
    }
    // Copy words from dictionary into new node
    strcpy(new_node->word, word);

    // Set new pointer to NULL
    new_node->next = NULL;

    // Hashing word to obtian index
    unsigned int hash = hashfunction(word);

    // Insert new node into linked (head)
    new_node->next = hashtable[hash];
    hashtable[hash] = new_node;

    counter++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Return number of words in load
    return counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int hash = 0; hash < HASHTABLE_SIZE; hash++)
    {
    node* cursor = hashtable[hash];
    while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor-> next;
            free(temp);
        }
    }
    return true;
}
