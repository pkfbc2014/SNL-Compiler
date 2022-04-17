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
	digraph graph1{
		a->b;
		b->d;
		c->d;
	}
*/

int tetete = 0;

FILE* treefp; // 文件读写指针
Queue* head; // 队头哨兵节点
Queue* tail; // 队尾指针
int queuenum; // 队列中元素数量
extern const char* Reserved_word[ReserveNum];

void initvar() // 全局变量初始化
{
	treefp = NULL;
	head = (Queue*)malloc(sizeof(Queue)); // 建立哨兵节点
	head->node = NULL;
	head->next = NULL;
	tail = head;
	queuenum = 0;
}

void choosePrint(treenode* root, int treetype)
{
	initvar();
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

		system("dot -Tpng LL1Tree.dot -o LL1Tree.png"); // 运行脚本
	}
}

void printRDTree(treenode* root) // 语法树根节点、树的类型（RD树 - 0，LL1树 - 1）
{
	push(root); // 根节点入队

	while (queuenum != 0) // 循环直到队列为空
	{
		treenode* temp = pop();
		for (int i = 0; i < temp->childnum; i++)
		{
			if (tetete == 58)
			{
				int kkk = 10;
			}
			if (temp->child[i]->token == NULL) // 非终结符，输出str
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, temp->child[i]->str);
				printf("%s->%s\n", temp->str, temp->child[i]->str);
				tetete++;
			}
			else // 终结符，输出token->Lex
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, Reserved_word[temp->child[i]->token->Lex]);
				printf("%s->%s\n", temp->str, Reserved_word[temp->child[i]->token->Lex]);
				tetete++;
			}
			push(temp->child[i]);
		}
	}
}

void printLL1Tree(treenode* root)
{
	push(root); // 根节点入队

	while (queuenum != 0) // 循环直到队列为空
	{
		treenode* temp = pop();
		for (int i = 0; i < temp->childnum; i++)
		{
			if (tetete == 58)
			{
				int kkk = 10;
			}
			if (temp->child[i]->token == NULL) // 非终结符，输出str
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, temp->child[i]->str);
				printf("%s->%s\n", temp->str, temp->child[i]->str);
				tetete++;
			}
			else // 终结符，输出token->Lex
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, Reserved_word[temp->child[i]->token->Lex]);
				printf("%s->%s\n", temp->str, Reserved_word[temp->child[i]->token->Lex]);
				tetete++;
			}
			push(temp->child[i]);
		}
	}
}

void push(treenode* node) // 向队尾增加元素 node
{
	Queue* temp = (Queue*)malloc(sizeof(Queue));
	temp->node = node;
	temp->next = tail->next;
	tail->next = temp;
	tail = tail->next;
	queuenum++;
}

treenode* pop() // 弹出队首元素
{
	Queue* temp = head->next; // temp指向哨兵节点的next，即头节点
	head->next = temp->next;
	treenode* message = temp->node; // 保存temp的node信息，因为temp马上会被删除
	free(temp);
	queuenum--;
	if (queuenum == 0) // 特殊处理一下
		tail = head;
	return message;
}