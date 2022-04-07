// 递归下降分析法

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RD_head.h"

void RD_analysis(node* tokenhead) // 递归下降分析法，接收token序列头
{
	nowtoken = tokenhead;

}

void initnode(treenode* temp) // 初始化节点
{
	strcpy(temp->str, "\0");
	temp->token = NULL;
	for (int i = 0; i < maxlen; i++)
		temp->child[i] = NULL;
	temp->childnum = 0;
}

void movetoken() // token序列指针后移
{
	nowtoken = nowtoken->next;
}

bool match() // 匹配token
{
	if ()
	{
		return 1; // 匹配成功
	}
	else
		return 0; //匹配失败，转回原函数并停机，输出错误信息
}

void printerror() // 语法分析错误信息输出
{

}



// 每个非终结符是一个函数，以下共58个函数（参考书上只给出了58个，虽然有67个非终结符）
treenode* program()
{

}
treenode* programHead()
{

}
treenode* declarePart()
{

}
treenode* programBody()
{

}
treenode* typeDecPart()
{

}
treenode* varDecPart()
{

}
treenode* procDecpart()
{

}
treenode* typeDec()
{

}
treenode* typeDecList()
{

}
treenode* typeID()
{

}
treenode* typeDef()
{

}
treenode* typeDecMore()
{

}
treenode* baseType()
{

}
treenode* structureType()
{

}
treenode* arrayType()
{

}
treenode* recType()
{

}
treenode* fieldDecList()
{

}
treenode* IDList()
{

}
treenode* fieldDecMore()
{

}
treenode* IDMore()
{

}
treenode* varDec()
{

}
treenode* varDecList()
{

}
treenode* varIDList()
{

}
treenode* varDecMore()
{

}
treenode* varIDMore()
{

}
treenode* procDec()
{

}
treenode* paramList()
{

}
treenode* procDecPart()
{

}
treenode* procBody()
{

}
treenode* paramDecList()
{

}
treenode* param()
{

}
treenode* paramMore()
{

}
treenode* formList()
{

}
treenode* fidMore()
{

}
treenode* stmList()
{

}
treenode* stm()
{

}
treenode* stmMore()
{

}
treenode* conditionalStm()
{

}
treenode* loopStm()
{

}
treenode* inputStm()
{

}
treenode* outputStm()
{

}
treenode* returnStm()
{

}
treenode* assCall()
{

}
treenode* assignmentRest()
{

}
treenode* callStmRest()
{

}
treenode* variMore()
{

}
treenode* exp()
{

}
treenode* actparamList()
{

}
treenode* actparamMore()
{

}
treenode* term()
{

}
treenode* otherTerm()
{

}
treenode* addOp()
{

}
treenode* factor()
{

}
treenode* otherFactor()
{

}
treenode* multOp()
{

}
treenode* variable()
{

}
treenode* fieldVar()
{

}
treenode* fieldVarMore()
{

}