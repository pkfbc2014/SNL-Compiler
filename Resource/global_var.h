#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
//一些全局变量

constexpr int NonNum = 67; // 非终结符数量
constexpr int ReserveNum = 43; //保留字数量
constexpr int ProductNum = 104; // 产生式数量
constexpr int maxloop = 40; // 求follow集的最大循环次数
constexpr int maxlen = 20; // 一个字符的最大长度

typedef enum//词法类型定义
{	//簿记单词符
	ENDFILE, ERROR,

	//保留字,保留字和类型属于标识符范畴，标识符（ID）状态课识别出保留字，类型，和ID
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END, READ, WRITE, ARRAY, OF,
	RECORD, RETURN,
	//类型
	INTEGER, CHAR,

	//多字符单词符号
	ID, INTC, CHARC,

	//特殊符号
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE//数组下标
}LexType;

struct token//链表结点定义
{
	int Lineshow;//单词所在行数
	LexType Lex;
	char Sem[100];//单词语义信息
	token* pre;
	token* next;
};

typedef struct first
{
	int num; // 该非终结符的first集中元素个数
	const char* ptr[maxlen]; // 指向各个终结符
	bool flag[ReserveNum + 1]; // 保证集合无重复元素（多一个$的位）
}first;

typedef struct follow
{
	int num; // 该非终结符的follow集中元素个数
	const char* ptr[maxlen]; // 指向各个终结符
	bool flag[ReserveNum + 1]; // 保证集合无重复元素（多一个$的位）
}follow;

typedef struct production // 产生式类型
{
	const char left[maxlen]; // 产生式左部
	const char* right[10]; // 产生式右部
}production;