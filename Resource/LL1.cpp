// LL1������
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "LL1_Head.h"


void init_S_stack() {
	for (int i = 0; i < 256; i++) {
		S_stack[i] = (char*)malloc(sizeof(char) * 20);
	}
}
void initnode(treenode* temp) {
	temp->childnum = 0;
	temp->token = NULL;
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
void S_pop() {
	S_ptr--;
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
treenode* LL1_analysis() // LL1������
{
	cal_predict(); // ����predict��������LL1Ԥ�������,����LL1������
	out_predict(); // ���LL1Ԥ�����������

	init_S_stack();
	nowtoken = getTokenList();
	lineno = nowtoken->Lineshow;
	S_push("Program");//�ķ���ʼ��ѹջ
	LL1_treeROOT = (treenode*)malloc(sizeof(treenode));
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
}
//int main() {
//	LL1_analysis();
//	return 0;
//}