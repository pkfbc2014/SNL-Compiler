#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "treenode_var.h"
#include "printTree.h"

/*
	���﷨����������أ�����������dot�ű����ٵ��� Graphviz ����������ɵ�dot�ű���ͼ
	Graphviz��������: http://www.graphviz.org/gallery/
	ʹ�ý̳�: https://www.jianshu.com/p/6d9bbbbf38b1
	dot�ű��ļ��﷨��ʽ����: 
	digraph graph{
		a->b;
		b->d;
		c->d;
	}
*/

FILE* treefp = NULL; // �ļ���дָ��
Queue* head = NULL; // ��βָ��
Queue* tail = NULL; // ��βָ��

void choosePrint(treenode* root, int treetype)
{
	if (treetype == 0) // ��ӡRD��
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

		//system("dot -Tpng RDtree.dot -o RDtree.png"); // ���нű�
	}
	else // ��ӡLL1��
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

		//system("dot -Tpng LL1tree.dot -o LL1tree.png"); // ���нű�
	}
}

void printRDTree(treenode* root) // �﷨�����ڵ㡢�������ͣ�RD�� - 0��LL1�� - 1��
{
	head = (Queue*)malloc(sizeof(Queue)); // ���ڵ����
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

bool isEmpty() // �ж϶����Ƿ�Ϊ��
{
	if (head == tail) // ����Ϊ��
		return true;
	else // ���в�Ϊ��
		return false;
}

void push(treenode* node) // ���β����Ԫ�� node
{
	Queue* temp = (Queue*)malloc(sizeof(Queue));
	temp->node = node;
	tail->next = temp;
	tail = tail->next;
}

treenode* pop() // ��������Ԫ��
{
	Queue* temp = head;
	head = head->next;
	treenode* flag = temp->node;
	free(temp);
	return flag;
}