#include "include/basicSetting.h"
#include "include/texture.h"
#include "include/RBTree.h"
#include <string.h>
#undef main

int main(){
    int op, *arr;
    char c[50];
    blockBase_Data b[8]={[0].blockID=1, [0].blockName="aaa",
                         [1].blockID=2, [1].blockName="bbb",
                         [2].blockID=3, [2].blockName="ccc",
                         [3].blockID=4, [3].blockName="ddd",
                         [4].blockID=5, [4].blockName="eee",
                         [5].blockID=6, [5].blockName="fff",
                         [6].blockID=7, [6].blockName="ggg",
                         [7].blockID=8, [7].blockName="hhh"};
    struct tNode* root= RBT_init();
    while(scanf("%d", &op)!=EOF){
        root=find_root(root);
        if(op==0){
            arr=store_data(root);
            for(int *i=arr; *i!=-1; i++) printf("%d ", *i);
        }
        else if(op>=1 && op<=8) insert(root, &b[op-1]);
        else printf("//op wrong\n");
    }
}