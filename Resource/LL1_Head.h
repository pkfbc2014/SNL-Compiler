#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

// 43个保留字和其他符号（包括终结符）的Lex对照表
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
};	//保留字

//int maxlen = 20; // 一个字符的最大长度
token* nowtoken; // 当前指向的token节点
char* S_stack[256];//符号栈
treenode* G_stack[256];//语法树栈
int N_stack[256];//操作数栈
LexType O_stack[256];//操作符栈
int S_ptr=-1;//符号栈指针
int G_ptr = -1;//语法树栈指针
int N_ptr = -1;//操作数栈指针
int O_ptr = -1;//操作符栈指针

int lineno = 0;//单词行号
int pnum = -1;//产生式处理函数调用序号
treenode* LL1_treeROOT = NULL; // LL1分析法语法分析数根节点

void init_S_stack();//初始化符号栈
char* LexToStr(LexType temp);//终极符转为字符串
int getReIndex2(const char* s); // 返回终结符s在终结符集合中的下标

void S_push(const char* str);//符号栈压栈
void S_pop();
void G_push();//语法树压栈
void G_pop();
void N_push();//操作数压栈
void N_pop();
void O_push();//操作符压栈
void O_pop();
treenode* LL1_analysis(); //LL1分析法
void predict(int a);//产生式处理函数