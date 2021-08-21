#define WORD_MAX_LEN  200
#define MEANING_MAX_LEN  40000

#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<btree.h>
#include"soundex.h"
//--------------------------------------------------------
// FUNCTION soundex
//--------------------------------------------------------
// BTA* btSoundex;
//hien
void soundex(char* name, char* s) //H
{
    int si = 1; 
    char c;

    //                 ABCDEFGHIJKLMNOPQRSTUVWXYZ
    char mappings[] = "01230120022455012623010202";

    s[0] = toupper(name[0]); //gan ki tu dau tien cua name cho s

    for(int i = 1, l = strlen(name); i < l; i++) // tim 3 ki tu tiep theo
    {
        c = toupper(name[i]) - 65; //A=65

        if(c >= 0 && c <= 25)
        {
            if(mappings[(int)c] != '0')
            {
                if(mappings[(int)c] != s[si-1]) //hiien H1
                {
                    s[si] = mappings[(int)c];
                    si++;
                }

                if(si > 3) //neu du 4 ki tu -> out
                {
                    break;
                }
            }
        }
    }

    if(si <= 3) //neu s dang co tu 3 ki tu tro xuong -> them 0 vao ben phai
    {
        while(si <= 3)
        {
            s[si] = '0';
            si++;
        }
    }
}

//add them key vao bt soundex.
int addSoundex(BTA* btSoundex, char* word){
    char s[5];
    soundex(word, s);
    s[4]='\0';
    char list[WORD_MAX_LEN*1000]; //danh sach cac tu ung voi soundex
    char tmp[WORD_MAX_LEN]; //bien tmp luu word
    strcpy(tmp, word);
    strcat(tmp, "\n");
    int listLength;
    if(btsel(btSoundex, s, list, WORD_MAX_LEN*sizeof(char)*1000, &listLength)==0){
        //soundex đã tồn tại, chèn thêm vào list.
        strcat(list, tmp);
        btupd(btSoundex, s, list, strlen(list));
    }else{
        memset(list, '0', strlen(list));
        strcat(list, tmp);
        btins(btSoundex, s, list, strlen(list));
    }
    // printf("%s -> %s \n", word, s);
    memset(word, '\0', strlen(word));
    memset(tmp, '\0', strlen(tmp));
    memset(list, '\0', strlen(list));
    return 1;
}

int searchSoundex(BTA* btSoundex, char* key, char* list){
    char s[5];
    soundex(key, s);
    int listLength;
    if(btsel(btSoundex, s, list, WORD_MAX_LEN*sizeof(char)*1000, &listLength)==0){
        return 1;
    }else
        return 0;
}

int deleteSoundex(BTA* btSoundex, char* word){
    char s[5];
    soundex(word, s);
    s[4]='\0';
    char list[WORD_MAX_LEN*1000]; //danh sach cac tu ung voi soundex
    int listLength=0;
    btsel(btSoundex, s, list, WORD_MAX_LEN*sizeof(char)*1000, &listLength);
    //xoa word trong list.
    char tmp[WORD_MAX_LEN];
    for(int i=0; i<strlen(list); i++){
        for(int j=0; list[i]!='\n';i++, j++){
            tmp[j]=list[i];
        }
        if(strcmp(word, tmp)==0){
            for(i=i-strlen(tmp); i<strlen(list)-(strlen(tmp)+1); i++){
                list[i]=list[i+strlen(tmp)+1];
            }
            for(; i<strlen(list); i++){
                list[i]='\0';
            }
            break;
        }
        memset(tmp, '\0', strlen(tmp));
    }
    //update key, value
    btupd(btSoundex, s, list, strlen(list));
    
    memset(word, '\0', strlen(word));
    memset(tmp, '\0', strlen(tmp));
    memset(list, '\0', strlen(list));
    return 1;
}