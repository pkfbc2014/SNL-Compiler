// LL1������
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "LL1_Head.h"

word reservedWords1[42] = {
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
};	//������

//int maxlen = 20; // һ���ַ�����󳤶�
S_ptr = -1;//����ջָ��
G_ptr = -1;//�﷨��ջָ��
N_ptr = -1;//������ջָ��
O_ptr = -1;//������ջָ��
G_pointer = NULL;//�﷨ջ��ջָ��

lineno = 0;//�����к�
pnum = -1;//����ʽ�������������
LL1_treeROOT = NULL; // LL1�������﷨���������ڵ�
newnode = NULL;//����ʽ������ʹ��

void init_S_stack() {
	for (int i = 0; i < 256; i++) {
		S_stack[i] = (char*)malloc(sizeof(char) * 20);
	}
}
void initnode(treenode* temp) {
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
void predict(int a)
{
	switch (a)
	{
	case 1:
		S_push("ProgramBody");
		S_push("DeclarePaart");
		S_push("ProgramHead");
		break;
	case 2:
		S_push("ProgramName");
		S_push("PROGRAM");
		newnode = createNode();
		strcpy(newnode->str, "ProgramHead");
		G_pointer = G_pop();
		G_pointer->child[G_pointer->childnum++] = G_pointer;
		G_push(newnode);
		G_push(newnode);
		break;
	case 3:
		S_push("ID");
		newnode = createNode();
		strcpy(newnode -> str, "ProgramName");

		break;
	case 4:
		S_push("FuncDec");
		S_push("VarDec");
		S_push("TypeDec");
		break;
	case 5:
		break;
	case 6:
		S_push("TypeDeclaration");
		break;
	case 7:
		S_push("TypeDeclist");
		S_push("TYPE");
		break;
	case 8:
		newnode=createNode();
		G_pointer = G_pop();
		G_pointer->child[G_pointer->childnum++] = newnode;
		G_push(G_pointer);      //��Ҫѹջ�ֵܽڵ�ʱ��ֱ��ѹջ���ڵ�
		break;
	case 9:
		G_pop();
		break;
	case 10:
		S_push("TypeDecList");
		break;
	case 11:
		S_push("ID");
		break;

	}
}
int Priosity(LexType op)
{
	if (op == END)
		return 0;
	if (op == LT or op == EQ)
		return 1;
	if (op == PLUS or op == MINUS)
		return 2;
	if (op == TIMES or op == OVER)
		return 3;
	printf("ERROR:�﷨����Priosity()���벻�Ϸ���");
	return -1;

}
treenode* LL1_analysis() // LL1������
{
	cal_predict(); // ����predict��������LL1Ԥ�������,����LL1������
	out_predict(); // ���LL1Ԥ�����������

	init_S_stack();
	nowtoken = getTokenList();
	lineno = nowtoken->Lineshow;
	S_push("Program");//�ķ���ʼ��ѹջ
	LL1_treeROOT = (treenode*)malloc(sizeof(treenode));
	initnode(LL1_treeROOT);
	strcpy(LL1_treeROOT->str, "Program");
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);

	while (S_ptr > -1) {//������ջ�ǿ�
		if (getNonIndex(S_stack[S_ptr]) == -1) {//ջ�����ռ���
			if (strcmp(S_stack[S_ptr], LexToStr(nowtoken->Lex)) == 0) {
				S_pop();
				nowtoken = getTokenList();
				lineno = nowtoken->Lineshow;
			}
			else {
				printf("Error:LL1�ռ���ƥ�����");
				exit(0);
				;
			}
		}
		else {
			pnum = LL1table[getNonIndex(S_stack[S_ptr])][getReIndex2(nowtoken->Lex)];
			S_pop();
			predict(pnum);
		}
	}
	if (nowtoken->Lex != ENDFILE) {
		printf("ERROR:�﷨�����У��ļ���ǰ����");
		return LL1_treeROOT;
	}
	else
		return LL1_treeROOT;
}
//int main() {
//	LL1_analysis();
//	return 0;
//}