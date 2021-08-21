#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<btree.h>
#include"soundex.h"
#include"dictionary.h"

int main(int argc, char* argv[]){
    //khoi tao btree
    btinit();
    BTA *btfile;
    btfile = btcrt("test_db", 0, FALSE);
    btcrtr(btfile, "btree");
    btdups(btfile, 0);
    // khoi tao bt chua soundex
    BTA* btSoundex;
    btSoundex= btcrt("soundex_db", 0, FALSE);
    btcrtr(btSoundex, "btree");
    btdups(btSoundex, 0);
    //lấy data từ file
    if(readDictText(btfile, btSoundex)==1)
        printf("Tai du lieu thanh cong\n");
    else
        printf("Error: tai du lieu khong thanh cong\n");
    //menu
    int choose=0;
    do{
        printf("\n***************************MENU***************************\n");
        printf("1. Tim kiem.\n");
        printf("2. Them tu.\n");
        printf("3. Xoa tu.\n");
        printf("4. Exit\n");
        printf("**********************************************************\n\n");
        printf("Lua chon = ");
        scanf("%d%*c", &choose);
        switch (choose){
        case 1: //search word
            searchWord(btfile, btSoundex);
            break;
        case 2:
            addWord(btfile, btSoundex);
            break;
        case 3: //xóa từ
            deleteWord(btfile, btSoundex);
            break;
        case 4: //thoat
            printf("Exit\n");
            return 1;
        default:
            printf("Nhap cac lua chon tu 1 -> 4\n");
        }
    } while (1);
}
