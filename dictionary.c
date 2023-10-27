// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 100;

int count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int wordlength = strlen(word);
    int total = 0;
    // Improve this hash function
    for (int i = 0; i < wordlength; i++)
    {
        total += tolower(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        printf("The file could not be loaded\n");
        return false;
    }

    // Reset the hash table and count
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    count = 0;

    char word[LENGTH + 1];
    while (fscanf(f, "%s", word) != EOF)
    {
        node *newnode = malloc(sizeof(struct node));
        if (newnode == NULL)
        {
            printf("Not enough memory space\n");
            fclose(f);
            return false;
        }

        strcpy(newnode->word, word);
        newnode->next = NULL; // Set the next pointer to NULL

        int index = hash(word);
        if (table[index] == NULL)
        {
            table[index] = newnode;
        }
        else
        {
            newnode->next = table[index];
            table[index] = newnode;
        }

        count++;
    }
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *temp = table[i];
        while (temp != NULL)
        {
            node *cursor = temp;
            temp = temp->next;
            free(cursor);
        }
    }
    return true;
}
