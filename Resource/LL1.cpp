// LL1������
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "LL1_Head.h"


//������.Hͷ�ļ�������ʱ�ѷ����ڴ�ռ䣻
//����ֻ���ڳ�ʼ��ʱ���ַ���ֵ�������ں������Ķ���ʱ���ַ���ֵ
//.hͷ�ļ���ֻ�ɷ����������ɷŶ���(�ǲ����Ƽ���)���������ı�����cpp����Ҫ��ֵ����Ӧ�ں����У������Ǻ�����
//.h����������Ӧ���Ǳ��ڶ�.cpp�ļ�����Ҫ�ģ�������ˣ�Ӧ�ŵ�.cpp�к����ⶨ��
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
	}
}
void initnode(treenode* temp) {
	if (temp == NULL)
	{
		printf("ERROR:initnode()!");
		return NULL;
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
	case 1:
		S_push("ProgramBody");
		S_push("DeclarePaart");
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
		break;
	case 5:
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
		nPredict("")
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
				newnode = createNode();//Ϊ�ռ��������﷨���ڵ�
				newnode->token = nowtoken;
				G_pointer = G_pop();//���½ڵ���ΪҶ�ӽڵ���ӵ��﷨����
				G_pointer->child[G_pointer->childnum++] = newnode;
				S_pop();
				nowtoken = getTokenList();
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