#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"

const int maxlen = 20; // һ���ַ�����󳤶�
node* nowtoken; // ��ǰָ���token�ڵ�

typedef struct treenode
{
	char str[maxlen]; // ��ǰ����
	node* token; // token���е�һ�����
	treenode* child[NonNum]; // NonNum �� ReserveNum ��
	int childnum; //��ǰ�ڵ���ӽڵ�����
}treenode;

void initnode(treenode* temp); // ��ʼ��һ���½ڵ�

void RD_analysis(node* tokenhead); // �ݹ��½�������������token����ͷ


// ÿ�����ս����һ�����������¹�58���������ο�����ֻ������58������Ȼ��67�����ս����
treenode* program();
treenode* programHead();
treenode* declarePart();
treenode* programBody();
treenode* typeDecPart();
treenode* varDecPart();
treenode* procDecpart();
treenode* typeDec();
treenode* typeDecList();
treenode* typeID();
treenode* typeDef();
treenode* typeDecMore();
treenode* baseType();
treenode* structureType();
treenode* arrayType();
treenode* recType();
treenode* fieldDecList();
treenode* IDList();
treenode* fieldDecMore();
treenode* IDMore();
treenode* varDec();
treenode* varDecList();
treenode* varIDList();
treenode* varDecMore();
treenode* varIDMore();
treenode* procDec();
treenode* paramList();
treenode* procDecPart();
treenode* procBody();
treenode* paramDecList();
treenode* param();
treenode* paramMore();
treenode* formList();
treenode* fidMore();
treenode* stmList();
treenode* stm();
treenode* stmMore();
treenode* conditionalStm();
treenode* loopStm();
treenode* inputStm();
treenode* outputStm();
treenode* returnStm();
treenode* assCall();
treenode* assignmentRest();
treenode* callStmRest();
treenode* variMore();
treenode* exp();
treenode* actparamList();
treenode* actparamMore();
treenode* term();
treenode* otherTerm();
treenode* addOp();
treenode* factor();
treenode* otherFactor();
treenode* multOp();
treenode* variable();
treenode* fieldVar();
treenode* fieldVarMore();