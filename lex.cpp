#include"lex.h"
#include<string.h>
#include<stdio.h>
/*
符号		state
起始$		  0
char		  1
digit		  2
(,),{,},_,^   3
\blank		  4
\sum,\int	  5
结束$	      6
number		  7
id			  8
*/
char* GetId(char* s, int *i)
{
	char temp[100];
	int j=0;
	while (ischar(s[*i]) || isdigit(s[*i]))
	{
		temp[j] = s[*i];
		(*i)++; j++;
	}
	temp[j] = '\0';
	return temp;
}
char* GetNumber(char* s, int *i)
{
	char temp[100];
	int j=0;
	while (isdigit(s[*i]))
	{
		temp[j] = s[*i];
		(*i)++; j++;
	}
	temp[j] = '\0';
	return temp;
}
int isdigit(char a)
{
	if (a >= '0'&&a <= '9')
		return 1;
	return 0;
}
int ischar(char a)
{
	if ((a >= 'a'&&a <= 'z') || (a >= 'A'&&a <= 'Z'))
		return 1;
	return 0;
}
int Lex_analysis(char* s,ele ELEMENT[])
{
	char temp[6];
	int i = 0, j = 0, k = 0, state;   //i负责读入串s的计数，j负责ELEMENT数组的计数
	int len = strlen(s);
	int flag = 1;
	char c,c1;//c记录读到s的某个字符
	c = s[i];
	if (c == '$')   //开始的$符
	{
		state = 0;
		strcpy(ELEMENT[j].str, "$");
		ELEMENT[j].type='$';
		i++; j++;
	}
	else
	{
		LexError(s, i);
		printf("Missing start $.\n");
		return -1;
	}
	while (flag)
	{
		switch (state)   //state表示读入各种字符后所处的状态，便于进行下一步处理
		{
		case 0:
			c = s[i];
			c1 = s[i + 1];
			if (ischar(c))
				state = 1;
			else if (isdigit(c))
				state = 2;
			else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '_' || c == '^')
				state = 3;
			else if (c == '\\'&&c1 == 'b')
				state = 4;
			else if (c == '\\')
				state = 5;
			else if (c == '$')
				state = 6;
			else if (c == '\0')
			{
				printf("Lexical error!Missing end $.\n");
				return -1;
			}
			else 
			{
				LexError(s, i);
				return -1;
			}
			break;
		case 1:
			strcpy(ELEMENT[j].str, GetId(s, &i));  //GetId函数读入表示id的串
			ELEMENT[j].type='i';//i为id
			j++;
			state = 6;
			break;
		case 2:
			strcpy(ELEMENT[j].str, GetNumber(s, &i));  //GetNumber函数读入数字串
			ELEMENT[j].type = 'N'; //N为数字
			j++;
			state = 6;
			break;
		case 3:
			temp[0] = c; temp[1] = '\0';
			strcpy(ELEMENT[j].str, temp);
			ELEMENT[j].type = c;
			i++; j++;
			state = 0;
			break;
		case 4:
			i++;
			for (k = 0; k < 5; k++)
				temp[k] = s[i + k];
			temp[k] = '\0';
			if (strcmp(temp, "blank") == 0)
			{
				strcpy(ELEMENT[j].str, "\\blank");
				ELEMENT[j].type = 'B';
				j++;
				i += 5;
				state = 0;
				break;
			}
			else
			{
				LexError(s, i);
				return -1;
			}
		case 5:  //匹配\sum，\int
			i++;
			for (k = 0; k < 3; k++)
				temp[k] = s[i + k];
			temp[k] = '\0';
			if (strcmp(temp, "sum") == 0)
			{
				strcpy(ELEMENT[j].str, "\\sum");
				ELEMENT[j].type = 'S';
				j++;
				i += 3;
			}
			else if (strcmp(temp, "int") == 0)
			{
				strcpy(ELEMENT[j].str, "\\int");
				ELEMENT[j].type = 'I';
				j++;
				i += 3;			
			}
			else
			{
				LexError(s, i);
				return -1;
			}
			state = 0;
			break;
		case 6:
			if (c == '$')
			{
				strcpy(ELEMENT[j].str, "$");
				ELEMENT[j].type = '$';
				j++;
				flag = 0;
				state = 0;
			}
			else if (c != '\0')
			{
				state = 0;
			}
			else
			{
				LexError(s, i);
				printf(" missing $");
				return -1;
			}
			break;
		}
	}
	for (k = 0; k < j; k++)
		printf("%c", ELEMENT[k].type);
	printf("\nsyntax:\n");
	return j;
}
void LexError(char *s, int i)
{
	printf("Lexical error on character:%c.\n", s[i]);
}

/*if (s[0] != '$') {
printf("Missing $ at the beginning!");
return;
}
i++;
while (s[i] && s[i] != '$')
{
if (isdigit(s[i]) == 1) {
while (isdigit(s[i]) == 1)
i++;
strcpy(out[j], "number");
j++;
}
else if (ischar(s[i]) == 1) {
while (ischar(s[i]) == 1 || isdigit(s[i]) == 1)
i++;
strcpy(out[j], "id");
j++;
}
else if (s[i] == '\\')
{
for (k = 1; k <= 3; k++) {
temp[k - 1] = s[i + k];
if (k == 3)
temp[k] = '\0';
}
if (!strcmp(temp, "int"))
{
i += 4;
strcpy(out[j], "INT");
j++;
}
else if (!strcmp(temp, "sum"))
{
i += 4;
strcpy(out[j], "SUM");
j++;
}

}
else if (s[i] == '\b')
{
for (k = 1; k <= 4; k++) {
temp[k - 1] = s[i + k];
if (k == 4)
temp[k] = '\0';
}
if (!strcmp(temp, "lank"))
{
i += 5;
strcpy(out[j], "BLANK");
j++;
}
}
if ((s[i] == '(') || (s[i] == ')') || (s[i] == '{') || (s[i] == '}') || (s[i] == '_') || (s[i] == '^'))
{
temp[0] = s[i];
temp[1] = '\0';
strcpy(out[j], temp);
j++;
}
i++;
}*/
