#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"
#include "treenode_var.h"

const int maxlen = 20; // 一个字符的最大长度
token* nowtoken; // 当前指向的token节点

treenode* RD_treeROOT = NULL; // 递归下降法语法分析数根节点

void RD_analysis(token* tokenhead); // 递归下降分析法，接收token序列头

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