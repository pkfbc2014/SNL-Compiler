#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "treenode_var.h"
#include "printTree.h"

/*
	将语法树输出到本地，根据树生成dot脚本，再调用 Graphviz 插件根据生成的dot脚本作图
	Graphviz下载链接: http://www.graphviz.org/gallery/
	使用教程: https://www.jianshu.com/p/6d9bbbbf38b1
	dot脚本的简单语法格式如下: 
	digraph graph{
		a->b;
		b->d;
		c->d;
	}
*/

FILE* treefp = NULL; // 文件读写指针
Queue* head = NULL; // 队尾指针
Queue* tail = NULL; // 队尾指针

void choosePrint(treenode* root, int treetype)
{
	if (treetype == 0) // 打印RD树
	{	
		if ((treefp = fopen("RDtree.dot", "w")) == NULL)
		{
			printf("cannot open the RDtree.dot file\n");
			return;
		}

		fprintf(treefp, "digraph graphRD{\n");
		printRDTree(root);
		fprintf(treefp, "}");
		fclose(treefp);

		//system("dot -Tpng RDtree.dot -o RDtree.png"); // 运行脚本
	}
	else // 打印LL1树
	{
		if ((treefp = fopen("LL1tree.dot", "w")) == NULL)
		{
			printf("cannot open the LL1tree.dot file\n");
			return;
		}

		fprintf(treefp, "digraph graphLL1{\n");
		printLL1Tree(root);
		fprintf(treefp, "}");
		fclose(treefp);

		//system("dot -Tpng LL1tree.dot -o LL1tree.png"); // 运行脚本
	}
}

void printRDTree(treenode* root) // 语法树根节点、树的类型（RD树 - 0，LL1树 - 1）
{
	head = (Queue*)malloc(sizeof(Queue)); // 根节点入队
	head->node = root;

	while (!isEmpty())
	{
		treenode* temp = pop();
		for (int i = 0; i < temp->childnum; i++)
		{
			fprintf(treefp, "\t%s->%s\n", temp->str, temp->child[i]->str);
			push(temp->child[i]);
		}
	}
}

void printLL1Tree(treenode* root)
{

}

bool isEmpty() // 判断队列是否为空
{
	if (head == tail) // 队列为空
		return true;
	else // 队列不为空
		return false;
}

void push(treenode* node) // 向队尾增加元素 node
{
	Queue* temp = (Queue*)malloc(sizeof(Queue));
	temp->node = node;
	tail->next = temp;
	tail = tail->next;
}

treenode* pop() // 弹出队首元素
{
	Queue* temp = head;
	head = head->next;
	treenode* flag = temp->node;
	free(temp);
	return flag;
}