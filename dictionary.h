#include<btree.h>
#include"soundex.h"

//getch() lấy kí tự từ bàn phím
int getch(void);
//auto complete;
char* autoComplete(BTA* btfile, char* in, char* out);
//lấy dữ liệu từ file anhviet.txt
int readDictText(BTA* btfile, BTA* btSoundex);
//xử lý key sau khi lấy từ bàn phím
char* validWord(char* key);
//tìm 1 từ trong từ điển
void searchWord(BTA* btfile, BTA* btSoundex);
//xoa key khoi tu dien
void deleteWord(BTA* btfile, BTA* btSoundex);
// add word từ bàn phím.
void addWord(BTA* btfile, BTA* btSoundex);
