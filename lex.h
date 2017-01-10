#ifndef _LEX_H_
#define _LEX_H_
typedef struct element{
	char str[10];
	char type;
}ele;
int isdigit(char a);
int ischar(char a);
int Lex_analysis(char* s, ele ELE[]);
void LexError(char *s,int i);
#endif