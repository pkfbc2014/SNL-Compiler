#pragma once

#include "treenode_var.h"
#include "LexicalAnalysis.h"

void init_S_stack();//��ʼ������ջ
char* LexToStr(LexType temp);//�ռ���תΪ�ַ���
int getReIndex2(LexType temp); // �����ս��s���ս�������е��±�
void S_push(const char* str);//����ջѹջ
void S_pop();
void G_push();//�﷨��ѹջ
treenode* G_pop();
void N_push();//������ѹջ
void N_pop();
void O_push();//������ѹջ
void O_pop();
treenode* LL1_analysis(); //LL1������
void predict(int a);//����ʽ������
int Priosity(LexType op);//���������ȼ�����
treenode* createNode();
int nPredict(const char* str,int times);//����ʽ��ģ�崦����
int prePrint(treenode* root);//��ӡ�﷨��