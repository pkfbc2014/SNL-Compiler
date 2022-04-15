#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

// 43�������ֺ��������ţ������ս������Lex���ձ�
word reservedWords1[42] = { 
	{"ENDFILE",ENDFILE},{"ERROR",ERROR},
	{"PROGRAM",PROGRAM},{"PROCEDURE",PROCEDURE},{"TYPE",PROCEDURE},{"VAR",VAR},{"IF",IF},
	{"THEN",THEN},{"ELSE",ELSE},{"FI",FI},{"WHILE",WHILE},{"DO",DO},{"ENDWH",ENDWH},
	{"BEGIN",BEGIN},{"END",END},{"READ",READ},{"WRITE",WRITE},{"ARRAY",ARRAY},{"OF",OF},
	{"RECORD",RECORD},{"RETURN",RETURN},
	{"INTEGER",INTEGER},{"CHAR",CHAR},
	{"ID",ID},{"INTC",INTC},{"CHARC",CHARC},
	{"ASSIGN",ASSIGN},{"EQ",EQ},{"LT",LT},{"PLUS",PLUS},{"MINUS",MINUS},
	{"TIMES",TIMES},{"OVER",OVER},{"LPAREN",LPAREN},{"RPAREN",RPAREN},{"DOT",DOT},
	{"COLON",COLON},{"SEMI",SEMI},{"COMMA",COMMA},{"LMIDPAREN",LMIDPAREN},{"RMIDPAREN",RMIDPAREN},
	{"UNDERANGE",UNDERANGE}
};	//������

//int maxlen = 20; // һ���ַ�����󳤶�
token* nowtoken; // ��ǰָ���token�ڵ�
char* S_stack[256];//����ջ
treenode* G_stack[256];//�﷨��ջ
int N_stack[256];//������ջ
LexType O_stack[256];//������ջ
int S_ptr=-1;//����ջָ��
int G_ptr = -1;//�﷨��ջָ��
int N_ptr = -1;//������ջָ��
int O_ptr = -1;//������ջָ��

int lineno = 0;//�����к�
int pnum = -1;//����ʽ�������������
treenode* LL1_treeROOT = NULL; // LL1�������﷨���������ڵ�

void init_S_stack();//��ʼ������ջ
char* LexToStr(LexType temp);//�ռ���תΪ�ַ���
int getReIndex2(const char* s); // �����ս��s���ս�������е��±�

void S_push(const char* str);//����ջѹջ
void S_pop();
void G_push();//�﷨��ѹջ
void G_pop();
void N_push();//������ѹջ
void N_pop();
void O_push();//������ѹջ
void O_pop();
treenode* LL1_analysis(); //LL1������
void predict(int a);//����ʽ������