/* trie.c
 *
 * Code found and modified from:
 * http://simplestcodings.blogspot.com/2012/11/trie-implementation-in-c.html
 *
 * Summary of modifcations made: modified trie nodes to store lat and lon data, added new function:TrieSearchPartial
 * to allow searches for partial but unique matches
 */
#include <stdio.h>
#include "trie.h"
#include <stdlib.h>

trieNode_t *TrieCreateNode(char key, float lat, float lon);

/*
 * Creates empty trie pointed to by argument:'root'
 */
void TrieCreate(trieNode_t **root)
{
  *root = TrieCreateNode('\0', 0xffffffff, 0xffffffff);
}

/*
 * Creates a trie node with the provided character:key, latitude:lat and longitude:lon
 * Returns a pointer to the trie node created, or NULL if unable to allocate memory
 */
trieNode_t *TrieCreateNode(char key, float lat, float lon)
{
  trieNode_t *node = NULL;
  node = (trieNode_t *)malloc(sizeof(trieNode_t));

  if(NULL == node)
	{
	  printf("Malloc failed\n");
	  return node;
	}

  node->key = key;
  node->next = NULL;
  node->children = NULL;
  node->latitude = lat;
  node->longitude = lon;
  node->parent= NULL;
  node->prev= NULL;
  return node;
}

/*
 * Adds new entry to trie of argument:'root' for the string provided with argument:'key' with data latitude:lat and longitude:lon
 */
void TrieAdd(trieNode_t **root, char *key, float lat, float lon)
{
  trieNode_t *pTrav = NULL;

  if(NULL == *root)
	{
	  printf("NULL tree\n");
	  return;
	}
#ifdef DEBUG
  printf("\nInserting key %s: \n",key);
#endif
  pTrav = (*root)->children;



  if(pTrav == NULL)
	{
	  /*First Node*/
	  for(pTrav = *root; *key; pTrav = pTrav->children)
		{
		  pTrav->children = TrieCreateNode(*key, 0xffffffff, 0xffffffff);
		  pTrav->children->parent = pTrav;
#ifdef DEBUG
		  printf("Inserting: [%c]\n",pTrav->children->key);
#endif
		  key++;
		}

	  pTrav->children = TrieCreateNode('\0', lat, lon);
	  pTrav->children->parent = pTrav;
#ifdef DEBUG
	  printf("Inserting: [%c]\n",pTrav->children->key);
#endif
	  return;
	}

  if(TrieSearch(pTrav, key))
	{
	  printf("Duplicate!\n");
	  return;
	}

  while(*key != '\0')
	{
	  if(*key == pTrav->key)
		{
		  key++;
#ifdef DEBUG
		  printf("Traversing child: [%c]\n",pTrav->children->key);
#endif
		  pTrav = pTrav->children;
		}
	  else
		break;
	}

  while(pTrav->next)
	{
	  if(*key == pTrav->next->key)
		{
		  key++;
		  TrieAdd(&(pTrav->next), key, lat, lon);
		  return;
		}
	  pTrav = pTrav->next;
	}

  if(*key)
	{
	  pTrav->next = TrieCreateNode(*key, 0xffffffff, 0xffffffff);
	}
  else
	{
	  pTrav->next = TrieCreateNode(*key, lat, lon);
	}

  pTrav->next->parent = pTrav->parent;
  pTrav->next->prev = pTrav;

#ifdef DEBUG
  printf("Inserting [%c] as neighbour of [%c] \n",pTrav->next->key, pTrav->key);
#endif

  if(!(*key))
	return;

  key++;

  for(pTrav = pTrav->next; *key; pTrav = pTrav->children)
	{
	  pTrav->children = TrieCreateNode(*key, 0xffffffff, 0xffffffff);
	  pTrav->children->parent = pTrav;
#ifdef DEBUG
	  printf("Inserting: [%c]\n",pTrav->children->key);
#endif
	  key++;
	}

  pTrav->children = TrieCreateNode('\0', lat, lon);
  pTrav->children->parent = pTrav;
#ifdef DEBUG
  printf("Inserting: [%c]\n",pTrav->children->key);
#endif
  return;
}

/*
 * Searches trie of argument:'root' for the string provided with argument:'key'
 * Returns the terminating trie node if an extact match for 'key' is found; returns NULL otherwise
 */
trieNode_t* TrieSearch(trieNode_t *root, const char *key)
{
  trieNode_t *level = root;
  trieNode_t *pPtr = NULL;

  int lvl=0;
  while(1)
	{
	  trieNode_t *found = NULL;
	  trieNode_t *curr;

	  for (curr = level; curr != NULL; curr = curr->next)
		{

#ifdef DEBUG
		  printf("Comparing: [%c] with [%c]\n",curr->key, *key);
#endif

		  if (curr->key == *key)
			{

#ifdef DEBUG
			  printf("Found!\n");
#endif

			  found = curr;
			  lvl++;
			  break;
			}
		}

	  if (found == NULL)
		return NULL;

	  if (*key == '\0')
		{
		  pPtr = curr;
		  return pPtr;
		}

	  level = found->children;
	  key++;
	}
}

/*
 * Searches trie of argument:'root' for the string provided with argument:'key'
 * Returns the terminating trie node if 'key' is a partial, but unique, match for a trie entry; returns NULL otherwise
 */
trieNode_t* TrieSearchPartial(trieNode_t *root, const char *key)
{
  trieNode_t *level = root;
  trieNode_t *pPtr = NULL;
  int is_prefix = 0;
  int lvl=0;

  while(1)
	{
	  trieNode_t *found = NULL;
	  trieNode_t *curr;

	  for (curr = level; curr != NULL; curr = curr->next)
		{

#ifdef DEBUG
		  printf("Comparing: [%c] with [%c]\n",curr->key, *key);
#endif

		  if (curr->key == *key)
			{

#ifdef DEBUG
			  printf("Found!\n");
#endif

			  found = curr;
			  lvl++;
			  break;
			}
		  /* else if will break if search key is a prefix but not unique */
		  else if (is_prefix && curr->next)
			{
			  found = NULL;
			  break;
			}
		  /* else if will continue to travel down the trie in the case that a partial match with a unique trie entry is found */
		  else if ((*key == '\0') && (curr->next == NULL) && (curr->prev == NULL))
			{

#ifdef DEBUG
			  printf("Moving along\n");
#endif

			  found = curr;
			  lvl++;
			  key--;
			  is_prefix = 1;
			  break;
			}
		}

	  if (found == NULL)
		return NULL;

	  if (*key == '\0')
		{
		  pPtr = curr;
		  return pPtr;
		}

	  level = found->children;
	  key++;
	}
}

/*
 * Removes string provided with argument:key from trie of argument:'root'
 */
void TrieRemove(trieNode_t **root, char *key)
{
  trieNode_t *tPtr = NULL;
  trieNode_t *tmp = NULL;

  if(NULL == *root || NULL == key)
	return;

  tPtr = TrieSearch((*root)->children, key);

  if(NULL == tPtr)
	{
	  printf("Key [%s] not found in trie\n", key);
	  return;
	}

#ifdef DEBUG
  printf("Deleting key [%s] from trie\n", key);
#endif

  while(1)
	{
	  if( tPtr->prev && tPtr->next)
		{
		  tmp = tPtr;
		  tPtr->next->prev = tPtr->prev;
		  tPtr->prev->next = tPtr->next;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		  break;
		}
	  else if(tPtr->prev && !(tPtr->next))
		{
		  tmp = tPtr;
		  tPtr->prev->next = NULL;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		  break;
		}
	  else if(!(tPtr->prev) && tPtr->next)
		{
		  tmp = tPtr;
		  tPtr->parent->children = tPtr->next;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		  break;
		}
	  else
		{
		  tmp = tPtr;
		  tPtr = tPtr->parent;
		  tPtr->children = NULL;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		}
	}

#ifdef DEBUG
  printf("Deleted key [%s] from trie\n", key);
#endif
}

/*
 * Destroys trie of argument:'root'
 */
void TrieDestroy( trieNode_t* root )
{
  trieNode_t *tPtr = root;
  trieNode_t *tmp = root;

  while(tPtr)
	{
	  while(tPtr->children)
		tPtr = tPtr->children;

	  if( tPtr->prev && tPtr->next)
		{
		  tmp = tPtr;
		  tPtr->next->prev = tPtr->prev;
		  tPtr->prev->next = tPtr->next;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		}
	  else if(tPtr->prev && !(tPtr->next))
		{
		  tmp = tPtr;
		  tPtr->prev->next = NULL;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		}
	  else if(!(tPtr->prev) && tPtr->next)
		{
		  tmp = tPtr;
		  tPtr->parent->children = tPtr->next;
		  tPtr->next->prev = NULL;
		  tPtr = tPtr->next;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		}
	  else
		{
		  tmp = tPtr;
		  if(tPtr->parent == NULL)
			{
			  /*Root*/
			  free(tmp);
			  return;
			}
		  tPtr = tPtr->parent;
		  tPtr->children = NULL;
#ifdef DEBUG
		  printf("Deleted [%c] \n", tmp->key);
#endif
		  free(tmp);
		}
	}
}

