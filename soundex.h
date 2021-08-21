void soundex(char* name, char* s);
//add them key vao bt soundex.
int addSoundex(BTA* btSoundex, char* word);
//tìm list của soundex
int searchSoundex(BTA* btSoundex, char* key, char* list);
//xóa soundex
int deleteSoundex(BTA* btSoundex, char* word);
