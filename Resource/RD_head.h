#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"
#include "treenode_var.h"

const int maxlen = 20; // һ���ַ�����󳤶�
token* nowtoken; // ��ǰָ���token�ڵ�

treenode* RD_treeROOT = NULL; // �ݹ��½����﷨���������ڵ�

void RD_analysis(token* tokenhead); // �ݹ��½�������������token����ͷ

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