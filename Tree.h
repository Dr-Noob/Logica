#ifndef __Tree__
#define __Tree__

#define MAX_HEIGHT 1000
#define MAX_CHAR 200
#define INFINITY (1<<20)

typedef struct Tree Tree;
typedef struct asciinode_struct asciinode;

struct Tree {
  Tree * left;
  Tree * right;
  char* element;
};

struct asciinode_struct {
  asciinode * left;
  asciinode * right;
  //length of the edge from this node to its children
  int edge_length;
  int height;
  int lablen;

  //-1=I am left, 0=I am root, 1=right
  int parent_dir;
  char label[MAX_CHAR];
};

void print_ascii_tree(Tree * t);

#endif
