//this code haven't finished debug yet...

#include "RBTree.h"

PRIVATE struct tNode* create_tNode(int data, struct tNode* Parent, SIDE side){
    struct tNode* n=malloc(sizeof(struct tNode));
    n->color= RED, n->data= data, n->Lchild= NULL;
    n->Rchild= NULL, n->parent= Parent, n->side= side;
    return n;
}

struct tNode* RBT_init(){
    struct tNode *root=create_tNode(-1, NULL, NONE);
    root->color=BLACK;
    return root;
}

void insert(struct tNode *curNode, int newData) {
    P;
    if(curNode->data > -1){
        if(newData < curNode->data){
            if(curNode->Lchild==NULL){
                curNode->Lchild= create_tNode(newData, curNode, LEFT);
                print_data(curNode),check(curNode->Lchild);
            }
            else insert(curNode->Lchild, newData);
        }
        else{
            if(curNode->Rchild==NULL){
                curNode->Rchild= create_tNode(newData, curNode, RIGHT);
                print_data(curNode),check(curNode->Rchild);
            }
            else insert(curNode->Rchild, newData);
        }
    }
    else{
        curNode->data=newData;
        return;
    }
}

struct tNode* find(struct tNode *curNode, int target){
    if(curNode==NULL) return NULL;
    if(target == curNode->data) return curNode;
    else if(target > curNode->data) find(curNode->Rchild,target);
    else if(target < curNode->data) find(curNode->Lchild,target);
}

PRIVATE void check(struct tNode *curNode){
    if(curNode->parent==NULL){
        curNode->color=BLACK, curNode->side=NONE;
        return;
    }
    if(curNode->parent->color==RED){
        if(curNode->parent->side==LEFT && curNode->parent->parent->Rchild!=NULL && curNode->parent->parent->Rchild->color==RED){
            curNode->parent->color=BLACK;
            curNode->parent->parent->color=RED;
            curNode->parent->parent->Rchild->color=BLACK;
            check(curNode->parent->parent);
        }
        else if(curNode->parent->side==RIGHT && curNode->parent->parent->Lchild!=NULL && curNode->parent->parent->Lchild->color==RED){
            curNode->parent->color=BLACK;
            curNode->parent->parent->color=RED;
            curNode->parent->parent->Lchild->color=BLACK;
            check(curNode->parent->parent);
        }
        else if(curNode->side==LEFT){
            if(curNode->parent->side==LEFT) right_rotate(curNode->parent->parent);
            else{
                curNode=curNode->parent;
                //printf("RL1 data: %d\n", curNode->data);
                right_rotate(curNode);
                //printf("RL2 data: %d\n", curNode->data);
                check(curNode);
            }
        }
        else{
            if(curNode->parent->side==RIGHT) left_rotate(curNode->parent->parent);
            else{
                curNode=curNode->parent;
                //printf("LR1 data: %d\n", curNode->data);
                left_rotate(curNode);
                //printf("LR2 data: %d\n", curNode->data);
                check(curNode);
            }
        }
    }
}

PRIVATE void left_rotate(struct tNode *curNode){
    P;
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Rchild->color;
    curNode->Rchild->color=c;
    //update side
    curNode->Rchild->side=curNode->side;
    curNode->side=LEFT;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Rchild;
        else curNode->parent->Rchild=curNode->Rchild;
    }
    curNode->Rchild->parent=curNode->parent;
    curNode->parent=curNode->Rchild;
    curNode->Rchild=curNode->parent->Lchild;
    if(curNode->Rchild!=NULL){
        curNode->Rchild->parent=curNode;
        curNode->Rchild->side=RIGHT;
    }
    curNode->parent->Lchild=curNode;
    print_data(curNode);
}

PRIVATE void right_rotate(struct tNode *curNode){
    P;
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Lchild->color;
    curNode->Lchild->color=c;
    //update side
    curNode->Lchild->side=curNode->side;
    curNode->side=RIGHT;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Lchild;
        else curNode->parent->Rchild=curNode->Lchild;
    }
    curNode->Lchild->parent=curNode->parent;
    curNode->parent=curNode->Lchild;
    curNode->Lchild=curNode->parent->Rchild;
    if(curNode->Lchild!=NULL){
        curNode->Lchild->parent=curNode;
        curNode->Lchild->side=LEFT;
    }
    curNode->parent->Rchild=curNode;
    print_data(curNode);
}

struct tNode* find_root(struct tNode *curNode){
    while(curNode->parent!=NULL) curNode=curNode->parent;
    return curNode;
}

void print_root(struct tNode *curNode){
    curNode=find_root(curNode);
    printf("root: %d\n", curNode->data);
}

void print_data(struct tNode *curNode){
    int max_print, now_print=-1, count=0;
    struct tNode* M=find_root(curNode);
    curNode=M;
    while(M->Rchild!=NULL) M=M->Rchild;
    max_print=M->data;
    print_root(curNode);
    printf("Max: %d, RBTree data:\n", max_print);
    while(now_print<max_print){
        if(curNode->Lchild!=NULL && curNode->Lchild->data > now_print) curNode=curNode->Lchild;
        else if(curNode->data > now_print){
            now_print=curNode->data;
            printf("%d\t%d\t%x\tp:%x\tL:%x\tR:%x\n", now_print,curNode->color,curNode,curNode->parent,curNode->Lchild,curNode->Rchild);
            //printf("%d",curNode->data);
            //(++count)%10==0 ? printf("\n") : printf("\t");
        }
        else if(curNode->Rchild!=NULL && curNode->Rchild->data > now_print) curNode=curNode->Rchild;
        else curNode=curNode->parent;
    }
    printf("\n");
}
