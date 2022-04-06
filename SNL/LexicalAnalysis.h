#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
//类型声明
enum state { START = 1, INID, INNUM, DONE, INASSIGN, INCOMMENT, INRANGE, INCHAR };//DFA
enum LexType{ID=1,RESERVED,INTC,SINGLE,DOUBLE,COM_LEFT,COM_RIGHT,CHAR,SUBSCRIPT};//单词分类的枚举类型：标识符，保留字，无符号整数，单字符分界符，双字符分界符,注释头符，注释结束符，字符起始符和结束符，数组下标界限符
struct node//链表结点定义
{
	int Lineshow;//单词所在行数
	LexType Lex;
	char* Sem;//单词语义信息
	node* pre;
	node* next;
};


//变量定义
state a = START;

FILE* fp = fopen("C:\\Users\\11279\\Desktop\\code.txt", "r+");
char ch;//取字符后放这里

//函数声明
char getNextChar();//从源文件中读取一个字符
int ungetNextChar();//将文件现有指针向前移动一个字符大小   ....!!!!应增强鲁棒性
int init_node(node* ptr);
node* getTokenList();
int reservedLookup();
int ChainToFile();
