// LL1������
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LL1_Head.h"
#include "global_var.h"
#include "func_statement.h"

extern int LL1table[NonNum][ReserveNum];

token* nowtoken; // ��ǰָ���token�ڵ�
char* S_stack[256];//����ջ
treenode* G_stack[256];//�﷨��ջ
int S_ptr;//����ջջ��ָ��
int G_ptr;//�﷨��ջָ��
treenode* G_pointer;//�﷨ջ��ջָ��

int lineno;//�����к�
int pnum;//����ʽ�������������
treenode* LL1_treeROOT; // LL1�������﷨���������ڵ�
treenode* newnode;//����ʽ������ʹ��

word reservedWords1[42] = {//������
	{"ENDFILE",ENDFILE},{"ERROR",ERROR},
	{"PROGRAM",PROGRAM},{"PROCEDURE",PROCEDURE},{"TYPE",PROCEDURE},{"VAR",VAR},{"IF",IF},
	{"THEN",THEN},{"ELSE",ELSE},{"FI",FI},{"WHILE",WHILE},{"DO",DO},{"ENDWH",ENDWH},
	{"BEGIN",BEGIN},{"END",END},{"READ",READ},{"WRITE",WRITE},{"ARRAY",ARRAY},{"OF",OF},
	{"RECORD",RECORD},{"RETURN",RETURN},
	{"INTEGER",INTEGER},{"CHAR",CHAR},
	{"ID",ID},{"INTC",INTC},{"CHARC",CHARC},
	{"ASSIGN",ASSIGN},{"EQ",EQ},{"LT",LT},{"PLUS",PLUS},{"MINUS",MINUS},
	{"TIMES",TIMES},{"OVER",OVER},{"LPAREN",LPAREN},{"RPAREN",RPAREN},{"DOT",DOT},
	{"COLON",COLON},{"SEMI",SEMI},{"COMMA",COMMA},{"LMIDPAREN",LMIDPAREN},{"RMIDPAREN",RMIDPAREN},
	{"UNDERANGE",UNDERANGE}
};	

extern word2 Words[42];
void init_S_stack() {
	for (int i = 0; i < 256; i++) {
		S_stack[i] = (char*)malloc(sizeof(char) * 20);
		if (S_stack[i] == NULL)
		{
			printf("ERROR:init_S_stack()����");
			exit(0);
		}
	}
}
void initnode(treenode* temp) {
	if (temp == NULL)
	{
		printf("ERROR:initnode()!");
	}
	temp->childnum = 0;
	temp->token = NULL;
}
treenode* createNode() 
{
	treenode* a = (treenode*)malloc(sizeof(treenode));
	if (a == NULL)
		return NULL;
	initnode(a);
	return a;

}
void S_push(const char* str) {
	if (S_ptr<255 && S_ptr>-2)
		strcpy(S_stack[++S_ptr], str);
	else
		printf("S_stack�����±�Խ��");
}
void G_push(treenode* temp) {
	if (G_ptr<255 && G_ptr>-2)
		G_stack[++G_ptr] = temp;
	else
		printf("G_stack�����±�Խ��");
}
void S_pop()
{
	S_ptr--;
}
treenode* G_pop()
{
	if (G_ptr > -1)
		return G_stack[G_ptr--];
	printf("ERROE:G_pop()����");
	return NULL;
}
char* LexToStr(LexType temp) {//�ռ���תΪ�ַ���
	for (int i = 0; i < 42; i++) {
		if (Words[i].tok == temp) {
			return Words[i].Sem;
		}
	}
	printf("Error:LexToStr");
}
int getReIndex2(LexType temp) // �����ս��s���ս�������е��±�
{
	for (int i = 0; i < 42; i++)
		if (temp == reservedWords1[i].tok)
			return i;
	return -1;
}
int nPredict(const char* str, int times)//��ɷ��ս���ڵ㴴������ֵ�����ӣ�ѹ�﷨ջ����
{
	newnode = createNode();
	strcpy(newnode->str, str);
	G_pointer = G_pop();
	G_pointer->child[G_pointer->childnum++] = newnode;
	for (int i = 0; i < times; i++) 
		G_push(newnode);
	return 0;
}
void predict(int a)
{
	switch (a)
	{
	case 1://�׸����ռ�����Ҳ���﷨���ĸ��ڵ㣬�ر�������ѭ����ʼǰ�������)
		S_push("ProgramBody");
		S_push("DeclarePart");
		S_push("ProgramHead");
		break;
	case 2:
		S_push("ProgramName");
		S_push("PROGRAM");
		nPredict("ProgramHead", 2);
		break;
	case 3:
		S_push("ID");
		nPredict("ProgramName", 1);
		break;
	case 4:
		S_push("ProcDec");
		S_push("VarDec");
		S_push("TypeDec");
		nPredict("DeclarePart", 3);
		break;
	case 5:
		nPredict("TypeDec", 0);
		break;
	case 6:
		S_push("TypeDeclaration");
		break;
	case 7:
		S_push("TypeDeclist");
		S_push("TYPE");
		nPredict("TypeDeclaration", 2);
		break;
	case 8:
		S_push("TypeDecMore");
		S_push("SEMI");
		S_push("TypeName");
		S_push("EQ");
		S_push("TypeId");
		nPredict("TypeDecList", 5);
		//��Ҫѹջ�ֵܽڵ�ʱ��ֱ��ѹջ���ڵ�
		break;
	case 9:
		nPredict("TypeDecMore", 0);
		break;
	case 10:
		S_push("TypeDecList");
		nPredict("TypeDecMore", 1);
		break;
	case 11:
		S_push("ID");
		nPredict("TypeDecMore", 1);
		break;
	case 12:
		S_push("BaseType");
		nPredict("TypeName", 1);
		break;
	case 13:
		S_push("StructureType");
		nPredict("TypeName", 1);
		break;
	case 14:
		S_push("ID");
		nPredict("TypeName", 1);
		break;
	case 15:
		S_push("INTEGER");
		nPredict("BaseType", 1);
		break;
	case 16:
		S_push("CHAR");
		nPredict("BaseType", 1);
		break;
	case 17:
		S_push("ARRAYTYPE");
		nPredict("StructureType", 1);
		break;
	case 18:
		S_push("RecType");
		nPredict("StructureType", 1);
		break;
	case 19:
		S_push("BaseType");
		S_push("OF");
		S_push("LMIDPAREN");
		S_push("Top");
		S_push("UNDERANGE");
		S_push("Low");
		S_push("LPAREN");
		S_push("ARRAY");
		nPredict("ArrayType", 8);
		break;
	case 20:
		S_push("INTC");
		nPredict("Low", 1);
		break;
	case 21:
		S_push("INTC");
		nPredict("Top",1);
		break;
	case 22:
		S_push("END");
		S_push("FieldDecList");
		S_push("RECORD");
		nPredict("RecType", 3);
		break;
	case 23:
		S_push("FieldDecMore");
		S_push("SEMI");
		S_push("IdList");
		S_push("BaseType");
		nPredict("FieldDecList", 4);
		break;
	case 24:
		S_push("FieldDecMore");
		S_push("SEMI");
		S_push("IdList");
		S_push("ArrayType");
		nPredict("FieldDecList", 4);
		break;
	case 25:
		nPredict("FieldDecMore", 0);
		break;
	case 26:
		S_push("FieldDecList");
		nPredict("FieldDecMore", 1);
		break;
	case 27:
		S_push("IdMore");
		S_push("ID");
		nPredict("IdList", 2);
		break;
	case 28:
		nPredict("IdMore", 0);
		break;
	case 29:
		S_push("IdList");
		S_push("COMMA");
		nPredict("IdMore", 2);
		break;
	case 30:
		nPredict("VarDec", 0);
		break;
	case 31:
		S_push("VarDeclaration");
		nPredict("VarDec", 1);
		break;
	case 32:
		S_push("VarDecList");
		S_push("VAR");
		nPredict("VarDeclaration", 2);
		break;
	case 33:
		S_push("VarDecMore");
		S_push("SEMI");
		S_push("VarIdList");
		S_push("TypeName");
		nPredict("VarDecList", 4);
		break;
	case 34:
		nPredict("VarDecMore", 0);
		break;
	case 35:
		S_push("VarDecList");
		nPredict("VarDecMore", 1);
		break;
	case 36:
		S_push("VarIdMore");
		S_push("ID");
		nPredict("VarIdList", 2);
		break;
	case 37:
		nPredict("VarIdMore", 0);
		break;
	case 38:
		S_push("VarIdList");
		S_push("COMMA");
		nPredict("VarIdMore",2);
		break;
	case 39:
		nPredict("ProcDec", 0);
		break;
	case 40:
		S_push("ProcDeclaration");
		nPredict("ProcDec", 1);
		break;
	case 41:
		S_push("ProcDecMore");
		S_push("ProcBody");
		S_push("ProcDecPart");
		S_push("SEMI");
		S_push("BaseType");
		S_push("COLON");
		S_push("RPAREN");
		S_push("ParamList");
		S_push("LPAREN");
		S_push("ProcName");
		S_push("PROCEDURE");
		nPredict("ProcDeclaration",11);
		break;
	case 42:
		nPredict("ProcDecMore",0);
		break;
	case 43:
		S_push("ProcDeclaration");
		nPredict("ProcDecMore", 1);
		break;
	case 44:
		S_push("ID");
		nPredict("ProcName",1);
		break;
	case 45:
		nPredict("ParamList", 0);
		break;
	case 46:
		S_push("ParamDecList");
		nPredict("ParamList",1);
		break;
	case 47:
		S_push("ParamMore");
		S_push("Param");
		nPredict("ParaDecList",2);
		break;
	case 48:
		nPredict("ParamMore", 0);
		break;
	case 49:
		S_push("ParamDecList");
		S_push("SEMI");
		nPredict("ParamMore", 2);
		break;
	case 50:
		S_push("FormList");
		S_push("TypeName");
		nPredict("Param", 2);
		break;
	case 51:
		S_push("FormList");
		S_push("TypeName");
		S_push("VAR");
		nPredict("Param",3);
		break;
	case 52:
		S_push("FidMore");
		S_push("ID");
		nPredict("FormList", 2);
		break;
	case 53:
		nPredict("FidMore",0);
		break;
	case 54:
		S_push("FormList");
		S_push("COMMA");
		nPredict("FidMore",2);
		break;
	case 55:
		S_push("DeclarePart");
		nPredict("ProcDecPart", 1);
		break;
	case 56:
		S_push("ProgramBody");
		nPredict("ProcBody", 1);
		break;
	case 57:
		S_push("END");
		S_push("StmList");
		S_push("BEGIN");
		nPredict("ProgramBody", 3);
		break;
	case 58:
		S_push("StmMore");
		S_push("Stm");
		nPredict("StmList", 2);
		break;
	case 59:
		nPredict("StmMore", 0);
		break;
	case 60:
		S_push("StmList");
		S_push("SEMI");
		nPredict("StmMore",2);
		break;
	case 61:
		S_push("ConditionalStm");
		nPredict("Stm", 1);
		break;
	case 62:
		S_push("LoopStm");
		nPredict("Stm", 1);
		break;
	case 63:
		S_push("InputStm");
		nPredict("Stm", 1);
		break;
	case 64:
		S_push("OutputStm");
		nPredict("Stm", 1);
		break;
	case 65:
		S_push("ReturnStm");
		nPredict("Stm", 1);
		break;
	case 66:
		S_push("AssCall");
		S_push("ID");
		nPredict("Stm", 2);
		break;
	case 67:
		S_push("AssignmentRest");
		nPredict("AssCall", 1);
		break;
	case 68:
		S_push("CallStmRest");
		nPredict("AssCall",1);
		break;
	case 69:
		S_push("Exp");
		S_push("EQ");
		S_push("VariMore");
		nPredict("AssignmentRest", 3);
		break;
	case 70:
		S_push("FI");
		S_push("StmList");
		S_push("ELSE");
		S_push("StmList");
		S_push("THEN");
		S_push("RelExp");
		S_push("IF");
		nPredict("ConditionalStm", 7);
		break;
	case 71:
		S_push("EDNWH");
		S_push("StmList");
		S_push("DO");
		S_push("RelExp");
		S_push("WHILE");
		nPredict("LoopStm", 5);
		break;
	case 72:
		S_push("RPAREN");
		S_push("InVar");
		S_push("LPAREN");
		S_push("READ");
		nPredict("InputStm", 4);
		break;
	case 73:
		S_push("ID");
		nPredict("InVar", 1);
		break;
	case 74:
		S_push("RPAREN");
		S_push("Exp");
		S_push("LPAREN");
		S_push("WRITE");
		nPredict("OutputStm", 4);
		break;
	case 75:
		S_push("RETURN");
		nPredict("ReturnStm", 1);
		break;
	case 76:
		S_push("RPAREN");
		S_push("ActParamList");
		S_push("LPAREN");
		nPredict("CallStmRest", 3);
		break;
	case 77:
		nPredict("ActParamList", 0);
		break;
	case 78:
		S_push("ActParamMore");
		S_push("Exp");
		nPredict("ActParamList", 2);
		break;
	case 79:
		nPredict("ActParamMore", 0);
		break;
	case 80:
		S_push("ActParamList");
		S_push("COMMA");
		nPredict("ActParamMore", 2);
		break;
	case 81:
		S_push("OtherRelE");
		S_push("Exp");
		nPredict("RelExp", 2);
		break;
	case 82:
		S_push("Exp");
		S_push("CmpOp");
		nPredict("OtherRelE", 2);
		break;
	case 83:
		S_push("OtherTerm");
		S_push("Term");
		nPredict("Exp",2);
		break;
	case 84:
		nPredict("OtherTerm", 0);
		break;
	case 85:
		S_push("Exp");
		S_push("AddOp");
		nPredict("OtherTerm", 2);
		break;
	case 86:
		S_push("OtherFactor");
		S_push("Factor");
		nPredict("Term", 2);
		break;
	case 87:
		nPredict("OtherFactor", 0);
		break;
	case 88:
		S_push("Term");
		S_push("MultOp");
		nPredict("OtherFactor", 2);
		break;
	case 89:
		S_push("RPAREN");
		S_push("Exp");
		S_push("LPAREN");
		nPredict("Factor", 3);
		break;
	case 90:
		S_push("INTC");
		nPredict("Factor", 1);
		break;
	case 91:
		S_push("Variable");
		nPredict("Factor", 1);
		break;
	case 92:
		S_push("VariMore");
		S_push("ID");
		nPredict("Variable", 2);
		break;
	case 93:
		nPredict("VariMore", 0);
		break;
	case 94:
		S_push("RMIDPAREN");
		S_push("Exp");
		S_push("LMIDPAREN");
		nPredict("VariMore",3);
		break;
	case 95:
		S_push("FieldVar");
		S_push("DOT");
		nPredict("VariMore", 2);
		break;
	case 96:
		S_push("FieldVarMore");
		S_push("ID");
		nPredict("FieldVar", 2);
		break;
	case 97:
		nPredict("FieldVarMore", 0);
		break;
	case 98:
		S_push("RMIDPAREN");
		S_push("Exp");
		S_push("LMIDPAREN");
		nPredict("FieldVarMore", 3);
		break;
	case 99:
		S_push("LT");
		nPredict("CmpOp", 1);
		break;
	case 100:
		S_push("EQ");
		nPredict("CmpOp", 1);
		break;
	case 101:
		S_push("PLUS");
		nPredict("AddOp",1);
		break;
	case 102:
		S_push("MINUS");
		nPredict("AddOp", 1);
		break;
	case 103:
		S_push("TIMES");
		nPredict("MultOp", 1);
		break;
	case 104:
		S_push("OVER");
		nPredict("MultOp", 1);
		break;
	default:
		printf("ERROR:predict()����");

	}
}
int prePrint(treenode* root)
{
	if (root == NULL)
		return 0;
	printf("%s\n", root->str);
	for (int k = 0; k < root->childnum; k--)
		prePrint(root->child[k]);
	return 0;
}
treenode* LL1_analysis() // LL1������
{
	cal_predict(); // ����predict��������LL1Ԥ�������,����LL1������
	out_predict(); // ���LL1Ԥ�����������
	S_ptr = -1;//����ջָ��
	G_ptr = -1;//�﷨��ջָ��
	G_pointer = NULL;//�﷨ջ��ջָ��
	lineno = 0;//�����к�
	pnum = -1;//����ʽ�������������
	LL1_treeROOT = NULL; // LL1�������﷨���������ڵ�
	nowtoken = getTokenList();//��ȡtoken�б�
	printToken(nowtoken);//���token

	init_S_stack();
	lineno = nowtoken->Lineshow;
	S_push("Program");//�ķ���ʼ��ѹջ
	LL1_treeROOT = (treenode*)malloc(sizeof(treenode));
	if (LL1_treeROOT == NULL)
	{
		printf("ERROR:LL1_treeROOT�ڴ�����ʧ��");
		exit(0);
	}
	initnode(LL1_treeROOT);
	strcpy(LL1_treeROOT->str, "Program");
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);

	while (S_ptr > -1) //������ջ�ǿ�
	{
		if (getNonIndex(S_stack[S_ptr]) == -1) {//ջ�����ռ���
			if (strcmp(S_stack[S_ptr], LexToStr(nowtoken->Lex)) == 0) {
				newnode = createNode();//Ϊ�ռ��������﷨���ڵ�
				newnode->token = nowtoken;
				G_pointer = G_pop();//���½ڵ���ΪҶ�ӽڵ���ӵ��﷨����
				G_pointer->child[G_pointer->childnum++] = newnode;

				S_pop();
				nowtoken = nowtoken->next;
				lineno = nowtoken->Lineshow;
			}
			else {
				printf("Error:LL1�ռ���ƥ�����");
				exit(0);
			}
		}
		else {
			pnum = LL1table[getNonIndex(S_stack[S_ptr])][getReIndex2(nowtoken->Lex)];
			S_pop();
			predict(pnum+1);
		}
	}
	if (nowtoken->Lex != ENDFILE) {
		printf("\nERROR:�﷨�����У��ļ���ǰ����");
		return LL1_treeROOT;
	}
	else
	{
		prePrint(LL1_treeROOT);
		return LL1_treeROOT;
	}

}
//int main() {
//	LL1_analysis();
//	return 0;
//}