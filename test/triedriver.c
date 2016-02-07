/*triedriver.c*/
/*
 * To Compile : gcc -o trie trie.c test/triedriver.c (-DDEBUG)
 * To run: ./trie
 *
 * Code found and modified from:
 * http://simplestcodings.blogspot.com/2012/11/trie-implementation-in-c.html
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "../trie.h"
 
int main()
{
    trieNode_t *root;
    trieNode_t *result;
    printf("Trie Example\n");
     
    /*Create a trie*/
    TrieCreate(&root);
     
    TrieAdd(&root, "seattle", 1.0, 1.1);
    TrieAdd(&root, "tacoma", 2.0, 2.1);
    TrieAdd(&root, "seatac", 3.0, 3.1);
    TrieAdd(&root, "kent", 4.0, 4.1);
    TrieRemove(&root, "shelton");
    TrieAdd(&root, "shelton", 5.0, 5.1);
    TrieRemove(&root, "shelton");
    TrieAdd(&root, "seat", 5.0, 5.1);
    TrieAdd(&root, "bellevue", 6.0, 6.1);
    TrieAdd(&root, "spokane", 7.0, 7.1);

    /* Exact match with trie entry, should return value associated with "seattle" */
    result = TrieSearchPartial(root->children, "seattle");
    if (result)
        printf("Found \"seattle\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("seattle not found\n");

    /* Exact match with trie entry, should return value associated with "tacoma" */
    result = TrieSearchPartial(root->children, "tacoma");
    if (result)
        printf("Found \"tacoma\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("tacoma not found\n");

    /* Exact match with trie entry, should return value associated with "seat", not "seattle" or "seatac" */
    result = TrieSearchPartial(root->children, "seat");
    if (result)
        printf("Found \"seat\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("seat not found\n");

    /* Partial match with unique entry, should return value associated with "seattle" */
    result = TrieSearchPartial(root->children, "seatt");
    if (result)
        printf("Found \"seatt\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("seatt not found\n");

    /* Partial match with multiple entries, should return nothing */
    result = TrieSearchPartial(root->children, "sea");
    if (result)
        printf("Found \"sea\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("sea not found\n");

     /* No match, should return nothing */
   result = TrieSearchPartial(root->children, "tokyo");
    if (result)
        printf("Found \"tokyo\", lat:%f lon:%f\n", result->latitude, result->longitude);
    else
        printf("tokyo not found\n");

 
    /*Destroy the trie*/
    TrieDestroy(root);
}