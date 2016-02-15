//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//: trie.h
//:
//: Code found and modified from:
//: http://simplestcodings.blogspot.com/2012/11/trie-implementation-in-c.html
//:
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef struct trieNode {
  char key;
  char *entry;
  float latitude;
  float longitude;
  struct trieNode *next;
  struct trieNode *prev;
  struct trieNode *children;
  struct trieNode *parent;
} trieNode_t;

extern trieNode_t *root;

void TrieCreate(trieNode_t **root);
trieNode_t* TrieSearch(trieNode_t *root, const char *key);
trieNode_t* TrieSearchPartial(trieNode_t *root, const char *key);
void TrieAdd(trieNode_t **root, char *key, float lat, float lon);
void TrieRemove(trieNode_t **root, char *key);
void TrieDestroy( trieNode_t* root );
