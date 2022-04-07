#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"

const int maxlen = 20; // 一个字符的最大长度
node* nowtoken; // 当前指向的token节点

typedef struct treenode
{
	char str[maxlen]; // 当前符号
	node* token; // token序列的一个结点
	treenode* child[NonNum]; // NonNum 比 ReserveNum 多
	int childnum; //当前节点的子节点数量
}treenode;

void initnode(treenode* temp); // 初始化一个新节点

void RD_analysis(node* tokenhead); // 递归下降分析法，接收token序列头


// 每个非终结符是一个函数，以下共58个函数（参考书上只给出了58个，虽然有67个非终结符）
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