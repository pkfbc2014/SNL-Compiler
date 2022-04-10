#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
//类型声明
enum state { START = 1, INID, INNUM, DONE, INASSIGN, INCOMMENT, INRANGE, INCHAR};//DFA状态
enum error{NORMAL=1,INASSIGN_ERROR,INRANGE_ERROR,INCHAR_ERROR,ERROR1};//错误类型分别对应无错误,赋值错误，程序结束错误,字符错误，读入无法识别错误
typedef enum//词法类型定义
{	//簿记单词符
	ENDFILE, ERROR,
	//保留字
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,	
	//类型
	INTEGER, CHAR1,
	//多字符单词符号
	ID, INTC, CHARC,
	//特殊符号
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE//数组下标
}LexType;

static struct word //保留字表，便于查阅
{
	
	char Sem[100];   //语义信息
	LexType tok; //词法信息
}reservedWords[21] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
	{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END1},{"array",ARRAY},
	{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
	{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
	{"return",RETURN1},{"integer",INTEGER},{"char",CHAR1} };	//保留字

struct token//链表结点定义
{
	int Lineshow;//单词所在行数
	LexType Lex;
	char Sem[100]={'\0'};//单词语义信息
	token* pre;
	token* next;
};


//变量定义
state a = START;
FILE* fp = fopen("C:\\Users\\11279\\Desktop\\code.txt", "r+");
error error0 = NORMAL;//词法错误
char ch;//取字符后放这里

//函数声明
char getNextChar();//从源文件中读取一个字符
int ungetNextChar();//将文件现有指针向前移动一个字符大小   ....!!!!应增强鲁棒性
int init_node(token* ptr);
int classify(char ch);//自动机所需的对输入字符的分类函数
LexType classify1(char ch);//单分界符的分类函数
token* getTokenList();
int reservedLookup();
int ChainToFile();
