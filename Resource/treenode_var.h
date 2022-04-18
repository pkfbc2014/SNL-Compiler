#pragma once

#include "global_var.h"

//语法树节点和一些相关函数的定义

typedef struct treenode
{
	char str[maxlen]; // 当前符号
	token* token; // token序列的一个结点
	treenode* child[NonNum]; // NonNum 比 ReserveNum 多
	int childnum; //当前节点的子节点数量
	int index; // 节点编号，方便生成语法树结构图片
}treenode;