#ifndef RBTree
#define RBTree

#define P printf("*file: %s, func: %s, line: %d\n", __FILE__, __func__, __LINE__)

typedef enum {BLACK, RED} COLOR;//node color
typedef enum {NONE, LEFT, RIGHT} SIDE;//parent's L or R child

struct tNode* RBT_init();//initiallize
void insert(struct tNode *, blockBase_Data*);//insert the node to red black tree
struct tNode* find(struct tNode*, blockBase_Data*);//find node by block's name
struct tNode* find_root(struct tNode *);//find red black tree root
int* store_data(struct tNode *);//store blockID to array

#endif
