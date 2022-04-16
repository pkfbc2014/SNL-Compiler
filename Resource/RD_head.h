#pragma once

#include "treenode_var.h"

treenode* RD_analysis(token* tokenhead); // 递归下降分析法，接收token序列头
void initnode(treenode* temp); // 初始化节点
treenode* ReadmatchToken(LexType tok); // 匹配当前token与终结符，之后移动指针
void addChild(treenode* root, treenode* child); // 为根节点root增加孩子节点child
void printerror(char* message); // 语法分析错误信息输出
void freetree(treenode* root); // 递归释放以 root 为根节点的树

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