#pragma once

#include "global_var.h"

//�﷨���ڵ��һЩ��غ����Ķ���

typedef struct treenode
{
	char str[20]; // ��ǰ����
	token* token; // token���е�һ�����
	treenode* child[67]; // NonNum �� ReserveNum ��
	int childnum; //��ǰ�ڵ���ӽڵ�����
}treenode;

void initnode(treenode* temp); // ��ʼ��һ���½ڵ�