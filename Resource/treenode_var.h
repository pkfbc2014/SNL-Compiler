#pragma once

#include "global_var.h"

//�﷨���ڵ��һЩ��غ����Ķ���
extern const int maxlen = 20; // ȫ�ֱ�����ʹ�ã����� extern �ؼ���

typedef struct treenode
{
	char str[maxlen]; // ��ǰ����
	token* token; // token���е�һ�����
	treenode* child[NonNum]; // NonNum �� ReserveNum ��
	int childnum; //��ǰ�ڵ���ӽڵ�����
}treenode;

void initnode(treenode* temp); // ��ʼ��һ���½ڵ�