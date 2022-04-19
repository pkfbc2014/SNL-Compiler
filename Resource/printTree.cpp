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

FILE* treefp; // �ļ���дָ��
Queue* head; // ��ͷ�ڱ��ڵ�
Queue* tail; // ��βָ��
int queuenum; // ������Ԫ������
extern const char* Reserved_word[ReserveNum];

void initvar() // ȫ�ֱ�����ʼ��
{
	treefp = NULL;
	head = (Queue*)malloc(sizeof(Queue)); // �����ڱ��ڵ�
	if (head == NULL)
	{
		printf("�ڱ��ڵ�����ռ�ʧ��\n");
		return;
	}
	head->node = NULL;
	head->next = NULL;
	tail = head;
	queuenum = 0;
}

int choosePrint(treenode* root, int treetype)
{
	initvar();
	if (treetype == 0) // ��ӡRD��
	{	
		if ((treefp = fopen("RDtree.dot", "w")) == NULL)
		{
			printf("cannot open the RDtree.dot file\n");
			return 0;
		}

		fprintf(treefp, "digraph graphRD{\n");
		printRDTree(root);
		fprintf(treefp, "}");
		fclose(treefp);
		
		system("dot -Tpng RDtree.dot -o RDtree.png"); // ���нű�
	}
	else // ��ӡLL1��
	{
		if ((treefp = fopen("LL1tree.dot", "w")) == NULL)
		{
			printf("cannot open the LL1tree.dot file\n");
			return 0;
		}

		fprintf(treefp, "digraph graphLL1{\n");
		printLL1Tree(root);
		fprintf(treefp, "}");
		fclose(treefp);

		system("dot -Tpng LL1Tree.dot -o LL1Tree.png"); // ���нű�
	}
	free(head); // �ͷ��ڱ��ڵ�
	return 1;
}

void printRDTree(treenode* root) // �﷨�����ڵ㡢�������ͣ�RD�� - 0��LL1�� - 1��
{
	int numnull = 0; // �պ��ӽڵ��ţ�ÿ��++

	push(root); // ���ڵ����

	while (queuenum != 0) // ѭ��ֱ������Ϊ��
	{
		treenode* temp = pop();
		if (temp == NULL) // �սڵ㲻��ȡ��ֱ������
			continue;
		for (int i = 0; i < temp->childnum; i++)
		{
			if (temp->child[i] == NULL) // �պ��ӣ�Ҳ���
			{
				fprintf(treefp, "%s_%d->NULL_%d\n", temp->str, temp->index, numnull);
				numnull++;
			}
			else if (temp->child[i]->token == NULL) // ���ս�������str
				fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, temp->child[i]->str, temp->child[i]->index);
			else // �ս�������token->Lex
			{
				if (temp->child[i]->token->Lex == ID || temp->child[i]->token->Lex == CHARC) // ֱ�����Sem
					fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, temp->child[i]->token->Sem, temp->child[i]->index);
				else if (temp->child[i]->token->Lex == INTC) // ����Ҫ����intǰ׺����Ϊdot���Բ��������ֿ�ͷ
					fprintf(treefp, "%s_%d->int%s_%d\n", temp->str, temp->index, temp->child[i]->token->Sem, temp->child[i]->index);
				else // �Ƿֺš��������ֵģ�dot���Բ���������������Ӧ���ս������
					fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, Reserved_word[temp->child[i]->token->Lex], temp->child[i]->index);
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
			if (temp->child[i]->token == NULL) // ���ս�������str
				fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, temp->child[i]->str,temp->child[i]->index);
			else // �ս�������token->Lex
			{
				if (temp->child[i]->token->Lex == ID || temp->child[i]->token->Lex == CHARC) // ֱ�����Sem
					fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, temp->child[i]->token->Sem, temp->child[i]->index);
				else if (temp->child[i]->token->Lex == INTC) // ����Ҫ����intǰ׺����Ϊdot���Բ��������ֿ�ͷ
					fprintf(treefp, "%s_%d->int%s_%d\n", temp->str, temp->index, temp->child[i]->token->Sem, temp->child[i]->index);
				else // �Ƿֺš��������ֵģ�dot���Բ���������������Ӧ���ս������
					fprintf(treefp, "%s_%d->%s_%d\n", temp->str, temp->index, Reserved_word[temp->child[i]->token->Lex], temp->child[i]->index);
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