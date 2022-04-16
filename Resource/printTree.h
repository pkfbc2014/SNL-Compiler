#pragma once

#include "treenode_var.h"

typedef struct Queue // 树的层次遍历法所需的队列
{
	treenode* node;
	struct Queue* next;
}Queue;

void choosePrint(treenode* root, int treetype);
void printRDTree(treenode* root);
void printLL1Tree(treenode* root);
bool isEmpty(); // 判断队列是否为空
void push(treenode* node); // 向队尾增加元素 node
treenode* pop(); // 弹出队首元素