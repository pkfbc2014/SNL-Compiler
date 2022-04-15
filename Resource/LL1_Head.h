#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "treenode_var.h"
#include "LexicalAnalysis.h"

// 43�������ֺ��������ţ������ս������Lex���ձ�
word reservedWords1[42];	//������

//int maxlen = 20; // һ���ַ�����󳤶�
token* nowtoken; // ��ǰָ���token�ڵ�
char* S_stack[256];//����ջ
treenode* G_stack[256];//�﷨��ջ
int N_stack[256];//������ջ
LexType O_stack[256];//������ջ
int S_ptr;//����ջָ��
int G_ptr;//�﷨��ջָ��
int N_ptr;//������ջָ��
int O_ptr;//������ջָ��
treenode* G_pointer;//�﷨ջ��ջָ��

int lineno;//�����к�
int pnum;//����ʽ�������������
treenode* LL1_treeROOT; // LL1�������﷨���������ڵ�
treenode* newnode;//����ʽ������ʹ��

void init_S_stack();//��ʼ������ջ
char* LexToStr(LexType temp);//�ռ���תΪ�ַ���
int getReIndex2(const char* s); // �����ս��s���ս�������е��±�
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