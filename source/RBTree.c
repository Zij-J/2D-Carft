#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/RBTree.h"

// RBTree Node
struct Node {
    blockBase_Data blockData;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int isRed;
};

// 左旋轉
void leftRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋轉
void rightRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// 修復RBTree after insertion
void fixInsert(struct Node** root, struct Node* z) {
    while (z->parent != NULL && z->parent->isRed) {
        if (z->parent == z->parent->parent->left) {
            struct Node* y = z->parent->parent->right;
            if (y != NULL && y->isRed) {
                z->parent->isRed = 0;
                y->isRed = 0;
                z->parent->parent->isRed = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->isRed = 0;
                z->parent->parent->isRed = 1;
                rightRotate(root, z->parent->parent);
            }
        } else {
            struct Node* y = z->parent->parent->left;
            if (y != NULL && y->isRed) {
                z->parent->isRed = 0;
                y->isRed = 0;
                z->parent->parent->isRed = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->isRed = 0;
                z->parent->parent->isRed = 1;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->isRed = 0;
}

// 插入節點
void insertNode(struct Node** root, blockBase_Data blockData) {
    struct Node* z = (struct Node*)malloc(sizeof(struct Node));
    z->blockData = blockData;
    z->left = NULL;
    z->right = NULL;
    z->isRed = 1;

    struct Node* x = NULL;
    struct Node* y = *root;   

    while (y != NULL) {
        x = y;
        if (z->blockData.blockID < y->blockData.blockID)
            y = y->left;
        else
            y = y->right;
    }

    z->parent = x;
    if (x == NULL)
        *root = z;
    else if (z->blockData.blockID < x->blockData.blockID)
        x->left = z;
    else
        x->right = z;

    fixInsert(root, z);
}

// 將RBTree 轉換成陣列
void treeToArray(struct Node* node, blockBase_Data* array, int* index) {
    if (node != NULL) {
        treeToArray(node->left, array, index);
        array[*index] = node->blockData;
        (*index)++;
        treeToArray(node->right, array, index);
    }
}

// 使用RBTree進行排序
void sortArray(storedBlock_DataBase* IDtoNameBase) {
    struct Node* root = NULL;

    for (int i = 0; i < (*IDtoNameBase)->storedSize; ++i) {
        insertNode(&root, (*IDtoNameBase)->array[i]);
    }
    
    int index = 0;
    treeToArray(root, (*IDtoNameBase)->array, &index);
    
    freeTree(root);
}



