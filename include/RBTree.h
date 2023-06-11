#ifndef RBTree
#define RBTree

#define P printf("*file: %s, func: %s, line: %d\n", __FILE__, __func__, __LINE__)

typedef enum {BLACK, RED} COLOR;//node color
typedef enum {NONE, LEFT, RIGHT} SIDE;//parent's L or R child

struct tNode* RBT_init();
void insert(struct tNode *, blockBase_Data*);
struct tNode* find(struct tNode*, blockBase_Data*);
struct tNode* find_root(struct tNode *);
void print_root(struct tNode *);
int* store_data(struct tNode *);

#endif