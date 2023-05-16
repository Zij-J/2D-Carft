#include <stdio.h>
#include <stdlib.h>
#define PRIVATE static
#define P printf("file: %s, func: %s, line: %d\n", __FILE__, __func__, __LINE__)

typedef enum {BLACK, RED} COLOR;//node color
typedef enum {NONE, LEFT, RIGHT} SIDE;//parent's L or R child

struct tNode{
    COLOR color;
    int data;
    struct tNode *Lchild;
    struct tNode *Rchild;
    struct tNode *parent;
    SIDE side;//parent's L or R child
};

PRIVATE struct tNode* create_tNode(int, struct tNode*, SIDE);
struct tNode* RBT_init();
void insert(struct tNode *, int);
struct tNode* find(struct tNode*, int);
PRIVATE void check(struct tNode*);
PRIVATE void left_rotate(struct tNode*);
PRIVATE void right_rotate(struct tNode*);
struct tNode* find_root(struct tNode *);
void print_root(struct tNode *);
void print_data(struct tNode *);