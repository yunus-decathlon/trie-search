#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Server.h"
#include <unistd.h>
#include <time.h>
#include <ctype.h>

int *create_map(void) 
{
    int *map = malloc(sizeof(int) * 100);
    for (int i = 0; i < strlen(ALPHABET); i++)
    {
        map[(int)ALPHABET[i] - 32] = i;
    }
    return map;
}

int find_in_alphabet(char c, int **map) {
    // printf("Find in alphabet %c %d\n", c, (*map)[c - 32]);
    return (*map)[c - 32];
    
    /* for (int i = 0; i < strlen(ALPHABET); i++) {
        if (c == ALPHABET[i]) {
            return i;
        }
    }
    return -1; */
}

// Function that returns a new Trie node
struct Trie *getNewTrieNode()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    node->isLeaf = 0;
    node->children = 0;
    node->name = NULL;

    for (int i = 0; i < CHAR_SIZE; i++)
    {
        node->character[i] = NULL;
    }

    return node;
}

// Iterative function to insert a string into a Trie
void insert(struct Trie *head, char *str, int **map)
{
    char *tmp = str;
    int index;
    // start from the root node
    struct Trie *curr = head;
    for(int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
    while (*str)
    {
        if ((*str < 'a' || *str > 'z') && *str != ' ') 
        {
            str++;
            continue;
        }
        index = (*map)[*str - 32];
        // create a new node if the path doesn't exist
        if (curr->character[index] == NULL)
        {
            curr->character[index] = getNewTrieNode();
            curr->children = curr->children + 1;
        }

        // go to the next node
        curr = curr->character[index];

        // move to the next character
        str++;
    }
    curr->name = strdup(tmp);
    printf("Inserted %s\n", tmp);

    // mark the current node as a leaf
    curr->isLeaf = 1;
}

// Iterative function to search a string in a Trie. It returns 1
// if the string is found in the Trie; otherwise, it returns 0.
void search(struct Trie *head, char *str, int **map)
{
    if (str == NULL || strlen(str) == 0) {
        return;
    }
    for(int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
    char *tmp = str;

    // return 0 if Trie is empty
    if (head == NULL)
    {
        printf("Not found 1\n");
        return;
    }

    struct Trie *curr = head;
    while (*str)
    {
        // go to the next node
        curr = curr->character[(*map)[*str - 32]];

        // if the string is invalid (reached end of a path in the Trie)
        if (curr == NULL)
        {
            printf("%s doesn't exist!\n", tmp);
            return;
        }

        // move to the next character
        str++;
    }
    // printf("%s exists!\n", tmp);
    searchAndPrintRec2(curr);
}

// Recursive function to search a string in a Trie. It returns 1
// if the string is found in the Trie; otherwise, it returns 0.
void searchAndPrintRec2(struct Trie *head)
{
    // printf("Not found") if Trie is empty
    if (head == NULL)
    {
        printf("Not found 3\n");
        return;
    }
    if (head->isLeaf == 1)
    {
        printf("Found suggestion: %s\n", head->name);
        ;
    }
    int found = 0;
    for (int i = 0; found < head->children; i++) 
    {
        if (head->character[i]) 
        {
            found = found + 1;
            searchAndPrintRec2(head->character[i]);
        }
    }
}

// Recursive function to search a string in a Trie. It returns 1
// if the string is found in the Trie; otherwise, it returns 0.
void searchAndPrintRec(struct Trie *head, char *curr_prefix)
{
    return;
    // printf("Not found") if Trie is empty
    if (head == NULL)
    {
        printf("Not found 3\n");
        return;
    }
    if (head->isLeaf == 1)
    {
        // printf("Found %s\n", curr_prefix);
        ;
    }
    int found = 0;
    for (int i = 0; found < head->children; i++) 
    {
        if (head->character[i]) 
        {
            found = found + 1;
            char *new_prefix = malloc(sizeof(char) * strlen(curr_prefix) + 2);
            new_prefix[strlen(curr_prefix) + 1] = '\0';
            strcpy(new_prefix, curr_prefix);
            new_prefix[strlen(curr_prefix)] = ALPHABET[i];
            searchAndPrintRec(head->character[i], new_prefix);
            free(new_prefix);
        }
    }
}


// Returns 1 if a given Trie node has any children
int hasChildren(struct Trie *curr)
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (curr->character[i])
        {
            return 1; // child found
        }
    }

    return 0;
}

// Recursive function to delete a string from a Trie
int deletion(struct Trie **curr, char *str, int **map)
{
    // return 0 if Trie is empty
    if (*curr == NULL)
    {
        return 0;
    }

    // if the end of the string is not reached
    if (*str)
    {
        // recur for the node corresponding to the next character in
        // the string and if it returns 1, delete the current node
        // (if it is non-leaf)
        if (*curr != NULL && (*curr)->character[(*map)[*str - 32]] != NULL &&
            deletion(&((*curr)->character[(*map)[*str - 32]]), str + 1, map) &&
            (*curr)->isLeaf == 0)
        {
            if (!hasChildren(*curr))
            {
                free(*curr);
                (*curr) = NULL;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    // if the end of the string is reached
    if (*str == '\0' && (*curr)->isLeaf)
    {
        // if the current node is a leaf node and doesn't have any children
        if (!hasChildren(*curr))
        {
            free(*curr); // delete the current node
            (*curr) = NULL;
            return 1; // delete the non-leaf parent nodes
        }

        // if the current node is a leaf node and has children
        else
        {
            // mark the current node as a non-leaf node (DON'T DELETE IT)
            (*curr)->isLeaf = 0;
            return 0; // don't delete its parent nodes
        }
    }

    return 0;
}

// Trie implementation in C – Insertion, Searching, and Deletion
void createTrie()
{
    int *map;
    map = create_map();
    struct Trie *head = getNewTrieNode();
    if (head == NULL)
    {
        printf("Trie empty!!\n"); // Trie is empty now
    }
    // FILE* stream = fopen("test.csv", "r");
    FILE* stream = fopen("names_only.csv", "r");

    char line[64];
    while (fgets(line, 64, stream) != NULL)
    {
        if (line[0] == '\0' || strlen(line) == 0) {
            continue;
        }
        line[strcspn(line, "\n")] = 0;
        insert(head, line, &map);
        memset(line, 0, 64);
    }
    printf("Indexing finished\n-------------------------\n");
    fclose(stream);
    clock_t start, end;
    double cpu_time_used;
    int BUFFSIZE = 10;
    int n;
    char buff[BUFFSIZE];
    while ((n = read(0, buff, BUFSIZ)) > 0)
    {
        buff[n - 1] = '\0';
        printf("Input: %d %s\n", n - 1, buff);
        start = clock();
        search(head, buff, &map);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Search took %f seconds to execute \n", cpu_time_used);
    }
/*  Performance test   

    char search_string[4];
    strcpy(search_string, "bre");
    start = clock();
    for (int i = 100000; i > 0; i--)
    {
        search(head, search_string, &map);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Search for %s took %f seconds to execute \n", search_string, cpu_time_used);

    char search_string2[3];
    strcpy(search_string2, "br");
    start = clock();
    for (int i = 100000; i > 0; i--)
    {
        search(head, search_string2, &map);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Search for %s took %f seconds to execute \n", search_string2, cpu_time_used); */
}

int main()
{
    createTrie();

}