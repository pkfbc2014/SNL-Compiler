#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

treenode* createNode();

void S_push(const char* str);//符号栈压栈
void S_pop();
void init_S_stack();//初始化符号栈
void G_push(treenode* temp);//语法树压栈
treenode* G_pop();
void initnode_LL1(treenode* temp);
treenode* createNode();

void predict(int a);//产生式处理函数
char* LexToStr(LexType temp);//终极符转为字符串
int getReIndex2(LexType temp); // 返回终结符s在终结符集合中的下标
int nPredict(const char* str,int times);//产生式的模板处理函数
treenode* LL1_analysis(token* head); //LL1分析法
int prePrint(treenode* root);//打印语法树