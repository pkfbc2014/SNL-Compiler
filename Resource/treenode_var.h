#pragma once

#include "global_var.h"

//语法树节点和一些相关函数的定义
extern const int maxlen = 20; // 全局变量的使用，带上 extern 关键字

typedef struct treenode
{
	char str[maxlen]; // 当前符号
	token* token; // token序列的一个结点
	treenode* child[NonNum]; // NonNum 比 ReserveNum 多
	int childnum; //当前节点的子节点数量
}treenode;

void initnode(treenode* temp); // 初始化一个新节点