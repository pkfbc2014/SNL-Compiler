#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "global_var.h"
//类型声明
enum state { START = 1, INID, INNUM, DONE, INASSIGN, INCOMMENT, INRANGE, INCHAR};//DFA状态
enum error{NORMAL=1,INASSIGN_ERROR,INRANGE_ERROR,INCHAR_ERROR,ERROR1};//错误类型分别对应无错误,赋值错误，程序结束错误,字符错误，读入无法识别错误

//头文件放声明，定义放在函数中，不可以放在函数之外
struct word //保留字表，便于查阅
{
	char Sem[100];   //语义信息
	LexType tok; //词法信息
};	//保留字

struct word2 //词法信息表,含有定义的所有词法类型
{
	LexType tok; //词法信息
	char Sem[100]; //词法信息
};	//保留字


//函数声明
char getNextChar();//从源文件中读取一个字符
int ungetNextChar();//将文件现有指针向前移动一个字符大小   ....!!!!应增强鲁棒性
int init_node(token* ptr);
int classify(char ch);//自动机所需的对输入字符的分类函数
LexType classify1(char ch);//单分界符的分类函数
LexType classify2(char* ptr);//ID的分类函数
int print_Lex(LexType a);//打印输出词法信息
token* getTokenList();
int reservedLookup();
int ChainToFile();
