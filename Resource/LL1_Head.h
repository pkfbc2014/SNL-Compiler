#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

treenode* createNode();

void S_push(const char* str);//����ջѹջ
void S_pop();
void init_S_stack();//��ʼ������ջ
void G_push();//�﷨��ѹջ
treenode* G_pop();

void predict(int a);//����ʽ������
char* LexToStr(LexType temp);//�ռ���תΪ�ַ���
int getReIndex2(LexType temp); // �����ս��s���ս�������е��±�
int nPredict(const char* str,int times);//����ʽ��ģ�崦����
treenode* LL1_analysis(); //LL1������
int prePrint(treenode* root);//��ӡ�﷨��