#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

void init_S_stack();//初始化符号栈
char* LexToStr(LexType temp);//终极符转为字符串
int getReIndex2(LexType temp); // 返回终结符s在终结符集合中的下标
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
int nPredict(const char* str,int times);//产生式的模板处理函数
int prePrint(treenode* root);//打印语法树