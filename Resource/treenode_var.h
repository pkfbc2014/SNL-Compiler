#pragma once

//�﷨���ڵ��һЩ��غ����Ķ���

typedef struct treenode
{
	char str[maxlen]; // ��ǰ����
	token* token; // token���е�һ�����
	treenode* child[NonNum]; // NonNum �� ReserveNum ��
	int childnum; //��ǰ�ڵ���ӽڵ�����
}treenode;

void initnode(treenode* temp); // ��ʼ��һ���½ڵ�