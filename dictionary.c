#define WORD_MAX_LEN  200
#define MEANING_MAX_LEN  40000
#include<unistd.h>
#include<termio.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<btree.h>
#include"soundex.h"
#include"dictionary.h"

int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

char* autoComplete(BTA* btfile, char* in, char* out){
    memset(out, '\0', strlen(out));
    //nhap du 1 tu -> ko can auto complete
    BTint i;
    // char key[WORD_MAX_LEN];
    
    bfndky(btfile, in, &i);
    //nhap tien to cua 1 tu khac //hie //hien
    while(1){
        if(bnxtky(btfile, out, &i)==0 || strncmp(out, in, strlen(in)) == 0){
            return strdup(out); 
        }
        memset(out, '\0', strlen(out));
    }
    //ko la tien to

    return NULL;
}

//lấy dữ liệu từ file anhviet.txt
int readDictText(BTA* btfile, BTA* btSoundex){
    FILE* fin;
    if((fin=fopen("anhviet.txt", "r"))==NULL){
        printf("Error: ko the mo file tu dien\n");
        return 0;
    }
    printf("Tai du lieu tu file tu dien...\n");
    //biến tmp
    char line[500];
    char word[WORD_MAX_LEN];
    char meaning[MEANING_MAX_LEN];

    int meaning_len=0;
    while(!feof(fin)){
        memset(line, '\0', strlen(line)); //khởi tạo lại biến line
        fgets(line, 500, fin);

        for(int i = 0; i < strlen(line); i++){
            if(line[0]=='@'){
                //khởi tạo lại các biến và lưu word + meaning vao btree
                if(strlen(word)!=0){
                    if(btins(btfile, word, meaning, strlen(meaning)*sizeof(char))==0){
                        addSoundex(btSoundex, word);
                    }
                }
                memset(word,'\0', strlen(word)); //khởi tạo lại biến word
                memset(meaning, '\0', strlen(meaning)); //khởi tạo lại biến mening
                meaning_len=0;

                i++; //bỏ qua '@'
                //lưu word từ line
                for(int j=0; line[i+1]!='/' && line[i]!='\n'; i++, j++)
                    word[j]=line[i];
                word[strlen(word)]='\0';
            }
            while (line[i]=='\n' || line[i]==' ') i++;
            //lưu meaning từ line
            for(; i<strlen(line); i++, meaning_len++){
                meaning[meaning_len]=line[i];
            }
        }
    }
    //lưu từ cuối cùng
    if(btins(btfile, word, meaning, strlen(meaning)*sizeof(char))==0){
        addSoundex(btSoundex, word);
    }
    return 1;
}

//xử lý key sau khi lấy từ bàn phím
char* validWord(char* key){
    if(key[strlen(key)-1]=='\n')
        key[strlen(key)-1]='\0'; //lấy key từ hàm fgets -> ptu cuối = '\n'

    char* tmp = key;
    int l = strlen(key);
    //bỏ kí tự trắng ở cuối key
    while(isspace(tmp[l-1]))
        tmp[--l]='\0';
    //bỏ kí tự trắng ở đầu key
    while (*tmp && isspace(*tmp))
        ++tmp, --l;
    //trả giá trị sau khi xử lý tmp -> key
    memmove(key, tmp, l+1);
    //chuyển về chữ viết thường
    for(int i=0; i<strlen(key); i++){
        key[i] = tolower(key[i]);
    }
    return key;
}

//tìm 1 từ trong từ điển
void searchWord(BTA* btfile, BTA* btSoundex){
    char key[WORD_MAX_LEN];
    char meaning[MEANING_MAX_LEN];
    do{
        printf("Nhap tu can tim kiem: ");
//------------------------------------    
        int count = 0;
        int ch;

        while (1){
            if((ch=getch())=='\n') break; //kết thúc
            if(ch==127){ //delete 
                if(count==0) continue;
                putchar('\b');
                printf("%c[0K", 27); //????
                key[--count]='\b'; //xoa phan tu ngay truoc va count = count-1

                continue;
            }
            if(ch=='\t'){//auto complete
                if(count==0) continue; //neu chua nhap key -> giu nguyen
                char output[WORD_MAX_LEN];
                autoComplete(btfile, key, output);
                if(output!=NULL){//nếu từ vừa nhập là tiền tố của 1 từ 
                    for(; count<strlen(output); count++){
                        putchar(output[count]);
                        key[count]=output[count];
                    }
                }
                memset(output, '\0', strlen(output));
                continue;
            }
            putchar(ch); 
            key[count]=ch;
            count++;
        }
        key[count]='\0';
//--------------------------------
        // fgets(key, WORD_MAX_LEN*sizeof(char), stdin);
        validWord(key);
        int meaningLength=0;
        if(btsel(btfile, key, meaning, MEANING_MAX_LEN, &meaningLength)==0){
           printf("\n%s\n", meaning);
           break;
        }else{
            printf("\nKhong tim thay\n");
            char list[WORD_MAX_LEN*1000];
            if(searchSoundex(btSoundex, key, list)){
                printf("\ngoi y:\n%s\n", list);
            }else{
                break;
            }
            memset(list, '\0', strlen(list));
        }
    }while(1);
    memset(key, '\0', strlen(key));
    memset(meaning, '\0', strlen(meaning));
}

//xoa key khoi tu dien
void deleteWord(BTA* btfile, BTA* btSoundex){
    printf("Nhap tu can xoa: ");
    char key[WORD_MAX_LEN];
    fgets(key, WORD_MAX_LEN*sizeof(char), stdin);
    validWord(key);

    switch (bdelky(btfile, key)){
    case 0:
        deleteSoundex(btSoundex, key);
        printf("Xoa thanh cong\n");
        break;
    case QNOKEY:
        printf("Tu can xoa ko ton tai\n");
        break;
    default:
        printf("Xay ra loi\n");
        break;
    }
    memset(key, '\0', strlen(key));
}

// add word từ bàn phím.
void addWord(BTA* btfile, BTA* btSoundex){
    //nhập từ
    printf("Nhap tu can them: ");
    char key[WORD_MAX_LEN];
    fgets(key, 50, stdin);
    validWord(key);

    char meaning[MEANING_MAX_LEN];
    char newMeaning[MEANING_MAX_LEN];
    //check tồn tại
    int meaningLength=0;
    if(btsel(btfile, key, meaning, MEANING_MAX_LEN, &meaningLength)==0){
        //nếu đã tồn tại -> show meaning -> có muốn thêm meaning?
        printf("Tu nay da ton tai, meaning: \n%s\n", meaning);
        printf("Ban van muon them tu (y/n): ");
        char choose[20];
        scanf("%s%*c", choose);
        do{
            int check = 1;
            switch (choose[0]){
            case 'Y':
            case'y':
                printf("Nhap nghia cua banj: ");
                fgets(newMeaning, MEANING_MAX_LEN*sizeof(char), stdin);
                if(strlen(newMeaning)+strlen(meaning)>MEANING_MAX_LEN){
                    printf("Khong the add them nghia (>meaning_max_len)");
                    return;
                }
                strcat(meaning, newMeaning);
                if(btupd(btfile, key, meaning, strlen(meaning))==0){
                    printf("Cap nhat thanh cong!\n");
                }else{
                    printf("Cap nhat ko thanh cong\n");
                }
                check=1;
                break; 
            case 'n':
            case 'N':
                check=1;
                // return; //không add thêm nghĩa -> thoát hàm
                break;
            default:
                check=0;
                printf("Nhap y or n: ");
                memset(choose, 0, strlen(choose));
                scanf("%s%*c", choose);
                break;
            }  
            if(check==1) break;
        }while(1);
        
    }else{
        //nếu ko tồn tại -> nhập meaning -> add vào dict
        printf("Meaning: ");
        fgets(newMeaning, MEANING_MAX_LEN*sizeof(char), stdin);
        if(btins(btfile, key, newMeaning, strlen(newMeaning)*sizeof(char))==0){
            addSoundex(btSoundex, key);
        }
    }
    memset(key, '\0', strlen(key));
    memset(meaning,'\0', strlen(meaning));
    memset(newMeaning,'\0', strlen(newMeaning));
}