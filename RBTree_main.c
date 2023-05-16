#include "RBTree.h"

int main(){
    int op, num;

    struct tNode* root= RBT_init();
    while(scanf("%d", &op)!=EOF){
        root=find_root(root);
        switch(op){
            case 1:
                scanf("%d", &num);
                insert(root, num);
                break;
            case 2:
                print_data(root);
                break;
            default:
                printf("//op wrong\n");
        }
    }
    P;
}
