//
//обработка символов и строк
//
#ifndef __STR_H
#define __STR_H

char str_char_ENGup(char chr);
int str_char_find1st(char *s,char chr);
//
//строку в число
//
//int str_strBIN2BYTE (char *value,char *str); нет
int str_strBIN2WORD (short *value,char *str);
int str_strBIN2DWORD(long *value,char *str);

int str_strHEX2BYTE (char *value,char *str);
int str_strHEX2WORD (short *value,char *str);
int str_strHEX2DWORD(long *value,char *str);

//int str_strDEC2BYTE (char *value,char *str); нет
int str_strDEC2WORD (short *value,char *str);
long str_strDEC2DWORD(char *str);

//число в строку
char *str_BYTE2strBIN(char *s,char value);
char *str_BYTE2strHEX(char *s,char value);
char *str_BYTE2strDEC(char *s,char value);

//char *str_WORD2strBIN(char *s,short value); нет
//char *str_WORD2strHEX(char *s,short value); нет
char *str_WORD2strDEC(char *s,short value);

char *str_DWORD2strBIN(char *s,long value);
char *str_DWORD2strHEX(char *s,long value);
//char *str_DWORD2strDEC(char *s,long value); нет
char *str_DWORD2strDECnf(char *str,long value);

#endif