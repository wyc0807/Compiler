#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lex.h"
ele ELEMENT[100];
int count = 0;//����ch�ļ���
FILE *fp1, *fp2;//fp1���ڴ�txt�ļ���fp2��������html�ļ�
/*
i:id
N:num
I:\int
S:\sum
B:\blank
*****�޸ĺ���ķ�*****
S->$B$
B->TM
T->RU | W{B}{B}{B}
M->B|null
U->_V | ^{B} | null
V->^{B}{B} | {B}
R->id | num | \blank | (B)
W->\int | \sum
*/
int S(int *loca);
int B(int *loca);
int R(int *loca);
int T(int *loca);
int M(int *loca);
int U(int *loca);
int V(int *loca);
int W(int *loca);
void sub(int *a, int *b);
void sup(int *a, int *b);
int match_op(char get)			//ƥ��$,(,),{,},_,^
{
	if (get == ELEMENT[count].type)
	{
		count++;
		return 1;
	}
	else   //����
	{
		printf("Syntax error on %c\n", get);
		return -1;
	}
}
int match_other(char get)	//ƥ��id,num,\blank,\int,\sum
{
	int i = 0;
	if (ELEMENT[count].type == get)
	{
		while (ELEMENT[count].str[i] != '\0')
		{
			i++;
		}
		count++;
		return i;  //���ص��ַ��������﷨�Ƶ��ķ����е��������
	}
	else
	{
		printf("Syntax error on %c\n", get);
		return -1;
	}
}
void txt2html_start(FILE *fp2)
{
	char *s = "<html>\n<head>\n<META content=\"text / html; charset = gb2312\"\n</head>\n<body>\n";
	fprintf(fp2, "%s", s);
}
void txt2html_end(FILE *fp2)
{
	char *s = "</body>\n<html>";
	fprintf(fp2, "%s", s);
}

void main()
{
	int loca[3] = { 500,300,200 };   //������������һ����ʼֵ
	fp1 = fopen("in.txt", "r");	//����Ҫ�Ű���ļ�
	char *ch_in = new char[100];
	fgets(ch_in, 100, fp1);	//�����ַ���
	fclose(fp1);
	printf("%s\n", ch_in);
	Lex_analysis(ch_in, ELEMENT);		//�ʷ�����
	fp2 = fopen("out.html", "w");
	//дhtml�ļ���ͷ��������Ϣ
	char *s = "<html>\n<head>\n<META content=\"text/html; charset = gb2312>\"\n</head>\n<body>\n";
	fprintf(fp2, "%s", s);
	S(loca);  //�﷨���������а����﷨�Ƶ��ķ��룩����S->$B$��ʼ
	char *ss = "</body>\n<html>";//дhtml�ļ��Ľ�β
	fprintf(fp2, "%s", ss);
	fclose(fp2);
}
//fscanf(fp1, "%s", ch_in);
//strcpy(ch_in, "$154^{5}^{5}$");
//Lex_analysis(in, ELEMENT);
//txt2html_start(fp2);
//txt2html_end(fp2);
/*
S->$B$
B->TM
M->B|null
T->RU | W{B}{B}{B}
U->_V | ^{B} | null
V->^{B}{B} | {B}
R->id | num | \blank | (B)
W->\int | \sum
*/

int S(int *loca)  //S->$B$
{
	int flag;
	int loc;
	flag = match_op('$');
	if (flag == -1) {
		return -1;
	}
	printf("S->$B$\n");
	loc=B(loca);
	flag = match_op('$');
	if (flag == -1)
		return -1;
	else
	{
		printf("Syntax analysis completed\n");
		return 1;
	}
}
int B(int *loca)  //B->TM
{
	int loc;
	printf("B->TM\n");
	loc = T(loca);
	if (loc == -1)
		return -1;
	loc = M(loca);
	return loc;
}
int M(int *loca)   //M->UM | B | null
{//......����ķ��������ķ����Ƶ����м���
	int loc;
	char ch = ELEMENT[count].type;
	if (ch == 'i' || ch == 'I' || ch == 'N' || ch == 'S' || ch == 'B' || ch == '(')
	{
		printf("M->B\n");
		loc=B(loca);
		//count++;  //�˴�������
		return loc;
	}
	else
	{
		printf("M->null\n");
		/*if (ELEMENT[count].type == '$')
		{
			printf("Syntax error\n");
			return -1;
		}*/
		return loca[0];
	}
}
int T(int *loca)  //T->RU | W{B}{B}{B}
{
	int t, t_sub, t_sup, flag;
	int loc, loc_sup, loc_sub;
	if (ELEMENT[count].type == 'i' || ELEMENT[count].type == 'N' || ELEMENT[count].type == 'B' || ELEMENT[count].type == '(')
	{
		printf("T->RU\n");
		t = R(loca);
		if (t == -1)
		{
			printf("Syntax error on T->RU (R)\n");
			return -1;
		}
		loca[0] = t;
		t=U(loca);
		if (t == -1)
		{
			printf("Syntax error on T->RU (U)\n");
			return -1;
		}
		return t;
	}
	else if (ELEMENT[count].type == 'I'|| ELEMENT[count].type == 'S')
	{
		int loca_sub[3], loca_sup[3];
		printf("T->W{B}{B}{B}\n");
		t=W(loca);
		if (t == -1)
		{
			printf("Syntax error on T->W{B}{B}{B} (W) \n");
			return -1;
		}
		loca[0] = t;
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		sub(loca, loca_sub);
		t_sub=B(loca_sub);
		if (t_sub == -1)
		{
			printf("Syntax error on T->W{B}{B}{B} (first B) \n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		sup(loca, loca_sup);
		t_sup = B(loca_sup);
		if (t == -1)
		{
			printf("Syntax error on T->W{B}{B}{B} (second B) \n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		loca[0] = t_sup > t_sub ? t_sup : t_sub;
		t = B(loca);
		if (t == -1)
		{
			printf("Syntax error on T->W{B}{B}{B} (third B) \n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		loca[0] = t;
		return t;
	}
	else
	{
		printf("Syntax error on NO.%d element\n", count++);
	}
}


int U(int *loca)//U->_V | ^{B} | null
{
	int t,t_sup, flag; int loca_sub[3],loca_sup[3];
	if (ELEMENT[count].type == '_')
	{
		printf("U->_V\n");
		match_op('_');
		t=V(loca);
		if (t == -1)
		{
			printf("Syntax error on U->_V  \n");
			return -1;
		}
		return t;		
	}
	if (ELEMENT[count].type == '^')
	{
		printf("U->^{B}\n");
		match_op('^');
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		sup(loca, loca_sup);
		t_sup = B(loca_sup);
		if (t_sup == -1)
		{
			printf("Syntax error on U->^{B}  \n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		return t_sup;
	}
	else
	{
		printf("U->null\n");
		return loca[0];
	}
}
int V(int *loca) //V->^{B}{B} | {B}
{
	int t, t_sub, t_sup, flag;
	int loca_sub[3], loca_sup[3];
	if (ELEMENT[count].type == '^')
	{
		match_op('^');
		printf("V->^{B}{B}\n");
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		sub(loca, loca_sub);
		t_sub = B(loca_sub);
		if (t_sub == -1)
		{
			printf("Syntax error on V->^{B}{B}\n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		flag = match_op('{');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		sup(loca, loca_sup);
		t_sup = B(loca_sup);
		if (t_sup == -1)
		{
			printf("Syntax error on V->^{B}{B}\n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		return t_sub > t_sup ? t_sub : t_sup;
	}
	else if (ELEMENT[count].type == '{')
	{
		match_op('{');
		printf("V->{B}\n");
		sub(loca, loca_sub);
		t = B(loca_sub);
		if (t == -1)
		{
			printf("Syntax error on V->^{B}{B}\n");
			return -1;
		}
		flag = match_op('}');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		return t;
	}
	else
	{
		printf("Syntax error on V->^{B}{B} | {B}\n");
		return -1;
	}
}

int R(int *loca)   //R->id | num | \blank | (B)  
{
	int t, flag;
	if (ELEMENT[count].type == 'i')
	{
		t=match_other('i');
		printf("R->id\n");
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-family:SimHei;font-size:%dpx; font-style:oblique;line-height:100%%;\">%s</span></div>\n", loca[0], loca[1], loca[2], ELEMENT[count - 1].str);
		return loca[0] + loca[2] * 0.5*t;
	}
	else if (ELEMENT[count].type == 'N')
	{
		t=match_other('N');
		printf("R->num\n");
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-family:SimHei;font-size:%dpx; font-style:normal;line-height:100%%;\">%s</span></div>\n", loca[0], loca[1], loca[2], ELEMENT[count - 1].str);
		return loca[0] + loca[2] * 0.5*t;
	}
	else if (ELEMENT[count].type == 'B')
	{
		t=match_other('B');
		printf("R->\\blank\n");
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-family:SimHei;font-size:%dpx; font-style:normal;line-height:100%%;\"></span></div>\n", loca[0], loca[1], loca[2]);
		return loca[0] + loca[2] * 0.5;
	}
	else if (ELEMENT[count].type == '(')
	{
		printf("R->(B)\n");
		match_op('(');
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:90%%;font-weight:100;\">(</span></div>\n", loca[0], loca[1], loca[2]);
		loca[0] = loca[0] + loca[2] * 0.35;
		t = B(loca);
		if (t == -1)
		{
			printf("Syntax error on R->(B)\n");
			return -1;
		}
		flag = match_op(')');
		if (flag == -1)
		{
			printf("Syntax error\n");
			return -1;
		}
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:90%%;font-weight:100;\">)</span></div>\n", loca[0], loca[1], loca[2]);
		loca[0] = loca[0] + loca[2] * 0.2;
		return t + loca[2] * 0.2;
	}
	//ǰ����������кϷ����Ƶ������������Ϊ�﷨�����ڴ˴����д���
	else
	{
		printf("Syntax error at NO.%d element     on R ->id | num | \blank | (B) | \sum{B}{B}{B} | \int{B}{B}{B} \n", count++);
		return -1;
	}
}

int W(int *loca)//W->\int | \sum
{
	int t;
	if (ELEMENT[count].type == 'I')
	{
		match_other('I');
		printf("W->\\int\n");
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:95%%;font-weight:300;\">&int;</span></div>\n", loca[0], loca[1], loca[2]);
		return loca[0] + loca[2]*0.75;
	}
	else if (ELEMENT[count].type == 'S')
	{
		match_other('S'); 
		printf("W->\\sum\n");
		fprintf(fp2, "\t<div style=\"position: absolute; left:%dpx; top:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:98%%;font-weight:300;\">&Sigma;</span></div>\n", loca[0], loca[1], loca[2]);
		return loca[0] + loca[2]*0.75;
	}
	else
	{
		printf("Syntax error at NO.%d element     on W->\int | \sum \n", count++);
		return -1;
	}	
}

//sup�����������ϼǺŵĲ���
void sup(int * a, int *b)
{
	b[0] = a[0];		//������
	b[1] = a[1];	//������
	b[2] = a[2] * 0.35;		//�����С
}
//sub�����������¼ǺŵĲ���
void sub(int * a, int *b)
{
	b[0] = a[0];		//������
	b[1] = a[1] + 0.6*a[2];//������
	b[2] = a[2] * 0.35;		//�����С
}