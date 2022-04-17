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
	digraph graph1{
		a->b;
		b->d;
		c->d;
	}
*/

int tetete = 0;

FILE* treefp; // �ļ���дָ��
Queue* head; // ��ͷ�ڱ��ڵ�
Queue* tail; // ��βָ��
int queuenum; // ������Ԫ������
extern const char* Reserved_word[ReserveNum];

void initvar() // ȫ�ֱ�����ʼ��
{
	treefp = NULL;
	head = (Queue*)malloc(sizeof(Queue)); // �����ڱ��ڵ�
	head->node = NULL;
	head->next = NULL;
	tail = head;
	queuenum = 0;
}

void choosePrint(treenode* root, int treetype)
{
	initvar();
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

		system("dot -Tpng LL1Tree.dot -o LL1Tree.png"); // ���нű�
	}
}

void printRDTree(treenode* root) // �﷨�����ڵ㡢�������ͣ�RD�� - 0��LL1�� - 1��
{
	push(root); // ���ڵ����

	while (queuenum != 0) // ѭ��ֱ������Ϊ��
	{
		treenode* temp = pop();
		for (int i = 0; i < temp->childnum; i++)
		{
			if (tetete == 58)
			{
				int kkk = 10;
			}
			if (temp->child[i]->token == NULL) // ���ս�������str
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, temp->child[i]->str);
				printf("%s->%s\n", temp->str, temp->child[i]->str);
				tetete++;
			}
			else // �ս�������token->Lex
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
	push(root); // ���ڵ����

	while (queuenum != 0) // ѭ��ֱ������Ϊ��
	{
		treenode* temp = pop();
		for (int i = 0; i < temp->childnum; i++)
		{
			if (tetete == 58)
			{
				int kkk = 10;
			}
			if (temp->child[i]->token == NULL) // ���ս�������str
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, temp->child[i]->str);
				printf("%s->%s\n", temp->str, temp->child[i]->str);
				tetete++;
			}
			else // �ս�������token->Lex
			{
				fprintf(treefp, "%d~%s->%d~%s\n", temp->index, temp->str, temp->child[i]->index, Reserved_word[temp->child[i]->token->Lex]);
				printf("%s->%s\n", temp->str, Reserved_word[temp->child[i]->token->Lex]);
				tetete++;
			}
			push(temp->child[i]);
		}
	}
}

void push(treenode* node) // ���β����Ԫ�� node
{
	Queue* temp = (Queue*)malloc(sizeof(Queue));
	temp->node = node;
	temp->next = tail->next;
	tail->next = temp;
	tail = tail->next;
	queuenum++;
}

treenode* pop() // ��������Ԫ��
{
	Queue* temp = head->next; // tempָ���ڱ��ڵ��next����ͷ�ڵ�
	head->next = temp->next;
	treenode* message = temp->node; // ����temp��node��Ϣ����Ϊtemp���ϻᱻɾ��
	free(temp);
	queuenum--;
	if (queuenum == 0) // ���⴦��һ��
		tail = head;
	return message;
}