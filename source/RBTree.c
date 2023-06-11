#include "../include/basicSetting.h"
#include "../include/texture.h"
#include "../include/RBTree.h"
#include <string.h>

private struct tNode* create_tNode(blockBase_Data*, struct tNode*, SIDE);
private void check(struct tNode*);
private void left_rotate(struct tNode*);
private void right_rotate(struct tNode*);

int node_count=0;

struct tNode{
    COLOR color;
    blockBase_Data *blockData;
    struct tNode *Lchild;
    struct tNode *Rchild;
    struct tNode *parent;
    SIDE side;//parent's L or R child
};

private struct tNode* create_tNode(blockBase_Data *blockData, struct tNode* Parent, SIDE side){
    //P;
    struct tNode* n=malloc(sizeof(struct tNode));
    n->color= RED, n->blockData=blockData, n->Lchild= NULL;
    n->Rchild= NULL, n->parent= Parent, n->side= side;
    return n;
}

struct tNode* RBT_init(){
    struct tNode *root=create_tNode(NULL, NULL, NONE);
    root->color=BLACK;
    return root;
}

void insert(struct tNode *curNode, blockBase_Data *blockData) {
    if(curNode->blockData != NULL && curNode->blockData->blockName != NULL){
        if(strcmp(blockData->blockName, curNode->blockData->blockName)==0) return;
        else if(strcmp(blockData->blockName, curNode->blockData->blockName)<0){
            if(curNode->Lchild==NULL){
                curNode->Lchild= create_tNode(blockData, curNode, LEFT);
                node_count++;
                check(curNode->Lchild);
            }
            else insert(curNode->Lchild, blockData);
        }
        else{
            if(curNode->Rchild==NULL){
                curNode->Rchild= create_tNode(blockData, curNode, RIGHT);
                node_count++;
                check(curNode->Rchild);
            }
            else insert(curNode->Rchild, blockData);
        }
    }
    else{
        curNode->blockData=blockData;
        node_count++;
        return;
    }
}

struct tNode* find(struct tNode *curNode, blockBase_Data *blockData){
    if(curNode==NULL) return NULL;
    if(strcmp(blockData->blockName, curNode->blockData->blockName)==0) return curNode;
    else if(strcmp(blockData->blockName, curNode->blockData->blockName)>0) find(curNode->Rchild, blockData);
    else if(strcmp(blockData->blockName, curNode->blockData->blockName)<0) find(curNode->Lchild, blockData);
}

private void check(struct tNode *curNode){
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
                right_rotate(curNode);
                check(curNode);
            }
        }
        else{
            if(curNode->parent->side==RIGHT) left_rotate(curNode->parent->parent);
            else{
                curNode=curNode->parent;
                left_rotate(curNode);
                check(curNode);
            }
        }
    }
}

private void left_rotate(struct tNode *curNode){
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Rchild->color;
    curNode->Rchild->color=c;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Rchild;
        else curNode->parent->Rchild=curNode->Rchild;
    }
    //update side
    curNode->Rchild->side=curNode->side;
    curNode->side=LEFT;
    //change relation
    curNode->Rchild->parent=curNode->parent;
    curNode->parent=curNode->Rchild;
    curNode->Rchild=curNode->parent->Lchild;
    if(curNode->Rchild!=NULL){
        curNode->Rchild->parent=curNode;
        curNode->Rchild->side=RIGHT;
    }
    curNode->parent->Lchild=curNode;
}

private void right_rotate(struct tNode *curNode){
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Lchild->color;
    curNode->Lchild->color=c;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Lchild;
        else curNode->parent->Rchild=curNode->Lchild;
    }
    //update side
    curNode->Lchild->side=curNode->side;
    curNode->side=RIGHT;
    //change relation
    curNode->Lchild->parent=curNode->parent;
    curNode->parent=curNode->Lchild;
    curNode->Lchild=curNode->parent->Rchild;
    if(curNode->Lchild!=NULL){
        curNode->Lchild->parent=curNode;
        curNode->Lchild->side=LEFT;
    }
    curNode->parent->Rchild=curNode;
}

struct tNode* find_root(struct tNode *curNode){
    while(curNode->parent!=NULL) curNode=curNode->parent;
    return curNode;
}

int* store_data(struct tNode *curNode){
    int count=0, *block_ID=malloc((node_count+1)*sizeof(int));
    char max_print[50], now_print[49]={0,'\0'};
    struct tNode* M=find_root(curNode);
    curNode=M;
    while(M->Rchild!=NULL) M=M->Rchild;
    strcpy(max_print, M->blockData->blockName);
    while(strcmp(now_print, max_print)<0){
        if(curNode->Lchild!=NULL && strcmp(curNode->Lchild->blockData->blockName, now_print)>0) curNode=curNode->Lchild;
        else if(strcmp(curNode->blockData->blockName, now_print)>0){
            strcpy(now_print, curNode->blockData->blockName);
            *(block_ID+(count++))=curNode->blockData->blockID;
        }
        else if(curNode->Rchild!=NULL && strcmp(curNode->Rchild->blockData->blockName, now_print)>0) curNode=curNode->Rchild;
        else curNode=curNode->parent;
    }
    *(block_ID+count)=-1;//stop
    return block_ID;
}
