#pragma once

#include "treenode_var.h"

typedef struct Queue // ���Ĳ�α���������Ķ���
{
	treenode* node;
	struct Queue* next;
}Queue;

void initvar(); // ȫ�ֱ�����ʼ��
void choosePrint(treenode* root, int treetype);
void printRDTree(treenode* root);
void printLL1Tree(treenode* root);
void push(treenode* node); // ���β����Ԫ�� node
treenode* pop(); // ��������Ԫ��