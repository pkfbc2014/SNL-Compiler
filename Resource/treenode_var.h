#pragma once

#include "global_var.h"

//�﷨���ڵ��һЩ��غ����Ķ���

typedef struct treenode
{
	char str[maxlen]; // ��ǰ����
	token* token; // token���е�һ�����
	treenode* child[NonNum]; // NonNum �� ReserveNum ��
	int childnum; //��ǰ�ڵ���ӽڵ�����
	int index; // �ڵ��ţ����������﷨���ṹͼƬ
}treenode;