#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "treenode_var.h"
#include "LexicalAnalysis.h"

// 43个保留字和其他符号（包括终结符）的Lex对照表
word reservedWords1[42];	//保留字

//int maxlen = 20; // 一个字符的最大长度
token* nowtoken; // 当前指向的token节点
char* S_stack[256];//符号栈
treenode* G_stack[256];//语法树栈
int N_stack[256];//操作数栈
LexType O_stack[256];//操作符栈
int S_ptr;//符号栈指针
int G_ptr;//语法树栈指针
int N_ptr;//操作数栈指针
int O_ptr;//操作符栈指针
treenode* G_pointer;//语法栈弹栈指针

int lineno;//单词行号
int pnum;//产生式处理函数调用序号
treenode* LL1_treeROOT; // LL1分析法语法分析数根节点
treenode* newnode;//产生式处理函数使用

void init_S_stack();//初始化符号栈
char* LexToStr(LexType temp);//终极符转为字符串
int getReIndex2(const char* s); // 返回终结符s在终结符集合中的下标
void S_push(const char* str);//符号栈压栈
void S_pop();
void G_push();//语法树压栈
treenode* G_pop();
void N_push();//操作数压栈
void N_pop();
void O_push();//操作符压栈
void O_pop();
treenode* LL1_analysis(); //LL1分析法
void predict(int a);//产生式处理函数
int Priosity(LexType op);//操作符优先级函数
treenode* createNode();