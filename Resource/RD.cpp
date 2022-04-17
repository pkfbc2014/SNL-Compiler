// 递归下降分析法
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RD_head.h"

token* nowtoken = NULL; // 当前指向的token节点

treenode* RD_analysis(token* tokenhead) // 递归下降分析法主程序，接收token序列头
{
	nowtoken = tokenhead; // 指向token序列的头
	treenode* RD_treeROOT = program(); // 递归下降分析，获得总根节点
	return RD_treeROOT; // 返回总根节点，以便语义分析
}

void initnode(treenode* temp) // 初始化节点
{
	strcpy(temp->str, "\0");
	temp->token = NULL;
	for (int i = 0; i < maxlen; i++)
		temp->child[i] = NULL;
	temp->childnum = 0;
}

treenode* ReadmatchToken(LexType tok) // 匹配当前token与终结符，之后移动指针
{
	if (tok == nowtoken->Lex) // 终结符匹配上了
	{
		treenode* newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			newnode->token = nowtoken;
			nowtoken = nowtoken->next; // token指针后移
			return newnode;
		}
	}
	else
	{
		nowtoken = nowtoken->next; // token指针后移
		return NULL;
	}
}

void movenowtoken() // 指针单纯后移
{
	nowtoken = nowtoken->next;
}

void addChild(treenode* root, treenode* child) // 为根节点root增加孩子节点child
{
	root->child[root->childnum] = child;
	root->childnum++;
}

void printerror(char* message) // 语法分析错误信息输出
{
	printf("%s\n", message);
}

// 每个非终结符是一个函数，以下共58个函数（参考书上只给出了58个，虽然有67个非终结符）
treenode* program()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Program");
		addChild(newnode, programHead());
		addChild(newnode, declarePart());
		addChild(newnode, programBody());
		return newnode;
	}
}

treenode* programHead()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ProgramHead");
		addChild(newnode, ReadmatchToken(PROGRAM));
		addChild(newnode, ReadmatchToken(ID));
		return newnode;
	}
}

treenode* declarePart()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "DeclarePart");
		addChild(newnode, typeDecPart());
		addChild(newnode, varDecPart());
		addChild(newnode, procDecpart());
		return newnode;
	}
}

treenode* programBody()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ProgramBody");
		addChild(newnode,ReadmatchToken(BEGIN));
		addChild(newnode,stmList());
		addChild(newnode,ReadmatchToken(END));
		return newnode;
	}
}

treenode* typeDecPart()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == TYPE)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "TypeDecPart");
			addChild(newnode, typeDec());
		}
	}
	return newnode;
}

treenode* varDecPart()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == VAR)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "VarDecPart");
			addChild(newnode, varDec());
		}
	}
	return newnode;
}

treenode* procDecpart()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == PROCEDURE)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "ProcDecPart");
			addChild(newnode, procDec());
		}
	}
	return newnode;
}

treenode* typeDec()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "TypeDec");
		addChild(newnode, ReadmatchToken(TYPE));
		addChild(newnode, typeDecList());
		return newnode;
	}
}

treenode* typeDecList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "TypeDecList");
		addChild(newnode,ReadmatchToken(ID));
		addChild(newnode,ReadmatchToken(EQ));
		addChild(newnode,typeDef());
		addChild(newnode,ReadmatchToken(SEMI));
		addChild(newnode,typeDecMore());
		return newnode;
	}
}

treenode* typeID()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "TypeID");
		addChild(newnode, ReadmatchToken(ID));
		return newnode;
	}
}

treenode* typeDef()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "TypeDef");
		if ((nowtoken->Lex == INTEGER) || (nowtoken->Lex == CHAR))
		{
			addChild(newnode, baseType());
			return newnode;
		}
		else if ((nowtoken->Lex == ARRAY) || (nowtoken->Lex == RECORD))
		{
			addChild(newnode, structureType());
			return newnode;
		}
		else
		{
			addChild(newnode, ReadmatchToken(ID));
			return newnode;
		}
	}
}

treenode* typeDecMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == ID)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "TypeDecMore");
			addChild(newnode, typeDecList());
		}
	}
	return newnode;
}

treenode* baseType()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "BaseType");
			addChild(newnode, ReadmatchToken(INTEGER));
			return newnode;
		}
	}
	else if (nowtoken->Lex == CHAR)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "BaseType");
			addChild(newnode, ReadmatchToken(CHAR));
			return newnode;
		}
	}
	movenowtoken();
	return newnode;
}

treenode* structureType()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == ARRAY)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "StructureType");
			addChild(newnode, arrayType());
			return newnode;
		}
	}
	else if (nowtoken->Lex == RECORD)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "StructureType");
			addChild(newnode, recType());
			return newnode;
		}
	}
	return newnode;
}

treenode* arrayType()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ArrayType");
		addChild(newnode, ReadmatchToken(ARRAY));
		addChild(newnode, ReadmatchToken(LMIDPAREN));
		addChild(newnode, ReadmatchToken(INTC));
		addChild(newnode, ReadmatchToken(UNDERANGE));
		addChild(newnode, ReadmatchToken(INTC));
		addChild(newnode, ReadmatchToken(RMIDPAREN));
		addChild(newnode, ReadmatchToken(OF));
		addChild(newnode, baseType());
	}
	return newnode;
}

treenode* recType()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "RecType");
		addChild(newnode,ReadmatchToken(RECORD));
		addChild(newnode,fieldDecList());
		addChild(newnode,ReadmatchToken(END));
	}
	return newnode;
}

treenode* fieldDecList()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER || nowtoken->Lex == CHAR)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "FieldDecList");
			addChild(newnode, baseType());
			addChild(newnode, IDList());
			addChild(newnode, ReadmatchToken(SEMI));
			addChild(newnode, fieldDecMore());
			return newnode;
		}
	}
	else if (nowtoken->Lex == ARRAY)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "FieldDecList");
			addChild(newnode, arrayType());
			addChild(newnode, IDList());
			addChild(newnode, ReadmatchToken(SEMI));
			addChild(newnode, fieldDecMore());
			return newnode;
		}
	}
	movenowtoken();
	return newnode;
}

treenode* IDList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "IDList");
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, IDMore());
	}
	return newnode;
}

treenode* fieldDecMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER || nowtoken->Lex == CHAR || nowtoken->Lex == ARRAY)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "FieldDecMore");
			addChild(newnode, fieldDecList());
			return newnode;
		}
	}
	return newnode;
}

treenode* IDMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == COMMA)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "IDMore");
			addChild(newnode, ReadmatchToken(COMMA));
			addChild(newnode, IDList());
			return newnode;
		}
	}
	return newnode;
}

treenode* varDec()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "VarDec");
		addChild(newnode, ReadmatchToken(VAR));
		addChild(newnode, varDecList());
	}
	return newnode;
}

treenode* varDecList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "VarDecList");
		addChild(newnode, typeDef());
		addChild(newnode, varIDList());
		addChild(newnode, ReadmatchToken(SEMI));
		addChild(newnode, varDecMore());
	}
	return newnode;
}

treenode* varIDList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "VarIDList");
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, varIDMore());
	}
	return newnode;
}

treenode* varDecMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER ||nowtoken->Lex == CHAR ||nowtoken->Lex == ARRAY ||nowtoken->Lex == RECORD ||nowtoken->Lex == ID)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "VarDecMore");
			addChild(newnode, varDecList());
			return newnode;
		}
	}
	return newnode;
}

treenode* varIDMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == COMMA)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "VarIDMore");
			addChild(newnode, ReadmatchToken(COMMA));
			addChild(newnode, varIDList());
			return newnode;
		}
	}
	return newnode;
}

treenode* procDec()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ProcDec");
		addChild(newnode, ReadmatchToken(PROCEDURE));
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, ReadmatchToken(LPAREN));
		addChild(newnode, paramList());
		addChild(newnode, ReadmatchToken(RPAREN));
		addChild(newnode, ReadmatchToken(SEMI));
		addChild(newnode, procDecPart());
		addChild(newnode, procBody());
		addChild(newnode, procDecpart());
	}
	return newnode;
}

treenode* paramList()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER ||nowtoken->Lex == CHAR ||nowtoken->Lex == ARRAY ||nowtoken->Lex == RECORD ||nowtoken->Lex == VAR ||nowtoken->Lex == ID)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "ParamList");
			addChild(newnode, paramDecList());
			return newnode;
		}
	}
	return newnode;
}

treenode* procDecPart()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ProcDecPart");
		addChild(newnode, declarePart());
	}
	return newnode;
}

treenode* procBody()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ProcBody");
		addChild(newnode, programBody());
	}
	return newnode;
}

treenode* paramDecList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ParamDecList");
		addChild(newnode, param());
		addChild(newnode, paramMore());
	}
	return newnode;
}

treenode* param()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == INTEGER ||nowtoken->Lex == CHAR ||nowtoken->Lex == ARRAY ||nowtoken->Lex == RECORD ||nowtoken->Lex == ID)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "Param");
			addChild(newnode, typeDef());
			addChild(newnode, formList());
			return newnode;
		}
	}
	else if (nowtoken->Lex == VAR)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "Param");
			addChild(newnode, ReadmatchToken(VAR));
			addChild(newnode, typeDef());
			addChild(newnode, formList());
			return newnode;
		}
	}
	movenowtoken();
	return newnode;
}

treenode* paramMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == SEMI)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "ParamMore");
			addChild(newnode, ReadmatchToken(SEMI));
			addChild(newnode, paramDecList());
			return newnode;
		}
	}
	return newnode;
}

treenode* formList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "FormList");
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, fidMore());
	}
	return newnode;
}

treenode* fidMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == COMMA)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "FidMore");
			addChild(newnode, ReadmatchToken(COMMA));
			addChild(newnode, formList());
			return newnode;
		}
	}
	return newnode;
}

treenode* stmList()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "StmList");
		addChild(newnode, stm());
		addChild(newnode, stmMore());
	}
	return newnode;
}

treenode* stm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Stm");
		if (nowtoken->Lex == IF)
		{
			addChild(newnode, conditionalStm());
			return newnode;
		}
		else if (nowtoken->Lex == WHILE)
		{
			addChild(newnode, loopStm());
			return newnode;
		}
		else if (nowtoken->Lex == READ)
		{
			addChild(newnode, inputStm());
			return newnode;
		}
		else if (nowtoken->Lex == WRITE)
		{
			addChild(newnode, outputStm());
			return newnode;
		}
		else if (nowtoken->Lex == RETURN)
		{
			addChild(newnode, returnStm());
			return newnode;
		}
		else if (nowtoken->Lex == ID)
		{
			addChild(newnode, ReadmatchToken(ID));
			addChild(newnode, assCall());
			return newnode;
		}
	}
	return NULL;
}

treenode* stmMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == SEMI)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "StmMore");
			addChild(newnode, ReadmatchToken(SEMI));
			addChild(newnode, stmList());
			return newnode;
		}
	}
	return newnode;
}

treenode* conditionalStm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ConditionalStm");
		addChild(newnode, ReadmatchToken(IF));
		addChild(newnode, exp());
		if (nowtoken->Lex == LT)
			addChild(newnode, ReadmatchToken(LT));
		else if (nowtoken->Lex == EQ)
			addChild(newnode, ReadmatchToken(EQ));
		else
		{
			movenowtoken();
			; // 输出错误信息 ???
		}
		addChild(newnode, exp());
		addChild(newnode, ReadmatchToken(THEN));
		addChild(newnode, stmList());
		addChild(newnode, ReadmatchToken(ELSE));
		addChild(newnode, stmList());
		addChild(newnode, ReadmatchToken(FI));
		return newnode;
	}
}

treenode* loopStm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "LoopStm");
		addChild(newnode, ReadmatchToken(WHILE));
		addChild(newnode, exp());
		if (nowtoken->Lex == LT)
			addChild(newnode, ReadmatchToken(LT));
		else if(nowtoken->Lex == EQ)
			addChild(newnode, ReadmatchToken(EQ));
		else
		{
			movenowtoken();
			; // 输出错误信息 ???
		}
		addChild(newnode, exp());
		addChild(newnode, ReadmatchToken(DO));
		addChild(newnode, stmList());
		addChild(newnode, ReadmatchToken(ENDWH));
		return newnode;
	}
}

treenode* inputStm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "InputStm");
		addChild(newnode, ReadmatchToken(READ));
		addChild(newnode, ReadmatchToken(LPAREN));
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, ReadmatchToken(RPAREN));
	}
	return newnode;
}

treenode* outputStm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "OutputStm");
		addChild(newnode, ReadmatchToken(WRITE));
		addChild(newnode, ReadmatchToken(LPAREN));
		addChild(newnode, exp());
		addChild(newnode, ReadmatchToken(RPAREN));
	}
	return newnode;
}

treenode* returnStm()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "ReturnStm");
		addChild(newnode, ReadmatchToken(RETURN));
	}
	return newnode;
}

treenode* assCall()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == LMIDPAREN ||nowtoken->Lex == DOT ||nowtoken->Lex == ASSIGN)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "AssCall");
			addChild(newnode, assignmentRest());
			return newnode;
		}
	}
	else if (nowtoken->Lex == LPAREN)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "AssCall");
			addChild(newnode, callStmRest());
			return newnode;
		}
	}
	return newnode;
}

treenode* assignmentRest()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "AssignmentRest");
		if (nowtoken->Lex == LMIDPAREN || nowtoken->Lex == DOT)
			addChild(newnode, variMore());
		addChild(newnode, ReadmatchToken(COLON));
		addChild(newnode, exp());
	}
	return newnode;
}

treenode* callStmRest()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "CallStmRest");
		addChild(newnode, ReadmatchToken(LPAREN));
		addChild(newnode, actparamList());
		addChild(newnode, ReadmatchToken(RPAREN));
		return newnode;
	}
}

treenode* variMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == LMIDPAREN)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "VariMore");
			addChild(newnode, ReadmatchToken(LMIDPAREN));
			addChild(newnode, exp());
			addChild(newnode, ReadmatchToken(RMIDPAREN));
		}
	}
	else if (nowtoken->Lex == DOT)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "VariMore");
			addChild(newnode, ReadmatchToken(DOT));
			addChild(newnode, fieldVar());
		}
	}
	return newnode;
}

treenode* exp()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Exp");
		addChild(newnode, term());
		addChild(newnode, otherTerm());
		return newnode;
	}
}

treenode* actparamList()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == LPAREN || nowtoken->Lex == INTC || nowtoken->Lex == ID)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		initnode(newnode);
		strcpy(newnode->str, "ActParamList");
		addChild(newnode, exp());
		addChild(newnode, actparamMore());
	}
	return newnode;
}

treenode* actparamMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == COMMA)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		initnode(newnode);
		strcpy(newnode->str, "ActParamMore");
		addChild(newnode, ReadmatchToken(COMMA));
		addChild(newnode, actparamList());
	}
	return newnode;
}

treenode* term()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Term");
		addChild(newnode, factor());
		addChild(newnode, otherFactor());
		return newnode;
	}
}

treenode* otherTerm()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == PLUS || nowtoken->Lex == MINUS)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		initnode(newnode);
		strcpy(newnode->str, "OtherTerm");
		addChild(newnode, addOp());
		addChild(newnode, exp());
	}
	return newnode;
}

treenode* addOp() // 回头看
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == PLUS)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "AddOp");
			addChild(newnode, ReadmatchToken(PLUS));
			return newnode;
		}
	}
	else if (nowtoken->Lex == MINUS)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "AddOp");
			addChild(newnode, ReadmatchToken(MINUS));
			return newnode;
		}
	}
	return newnode;
}

treenode* factor()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Factor");
	}
	if (nowtoken->Lex == INTC)
	{
		addChild(newnode, ReadmatchToken(INTC));
		return newnode;
	}
	else if (nowtoken->Lex == LPAREN)
	{
		addChild(newnode, ReadmatchToken(LPAREN));
		addChild(newnode, exp());
		addChild(newnode, ReadmatchToken(RPAREN));
		return newnode;
	}
	else if (nowtoken->Lex == ID)
	{
		addChild(newnode, variable());
		return newnode;
	}
	return NULL;
}

treenode* otherFactor()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == TIMES || nowtoken->Lex == OVER)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "OtherFactor");
			addChild(newnode, multOp());
			addChild(newnode, term());
		}
	}
	return newnode;
}

treenode* multOp()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == TIMES)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "MultOp");
			addChild(newnode, ReadmatchToken(TIMES));
			return newnode;
		}
	}
	else if (nowtoken->Lex == OVER)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "MultOp");
			addChild(newnode, ReadmatchToken(OVER));
			return newnode;
		}
	}
	return newnode;
}

treenode* variable()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "Variable");
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, variMore());
		return newnode;
	}
}

treenode* fieldVar()
{
	treenode* newnode = (treenode*)malloc(sizeof(treenode));
	if (newnode == NULL)
		return NULL;
	else
	{
		initnode(newnode);
		strcpy(newnode->str, "FieldVar");
		addChild(newnode, ReadmatchToken(ID));
		addChild(newnode, fieldVarMore());
		return newnode;
	}
}

treenode* fieldVarMore()
{
	treenode* newnode = NULL;
	if (nowtoken->Lex == LMIDPAREN)
	{
		newnode = (treenode*)malloc(sizeof(treenode));
		if (newnode == NULL)
			return NULL;
		else
		{
			initnode(newnode);
			strcpy(newnode->str, "FieldVarMore");
			addChild(newnode, ReadmatchToken(LMIDPAREN));
			addChild(newnode, exp());
			addChild(newnode, ReadmatchToken(RMIDPAREN));
		}
	}
	return newnode;
}