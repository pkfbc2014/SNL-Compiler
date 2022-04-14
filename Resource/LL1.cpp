// LL1分析法
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
		printf("S_stack数组下标越界");
}
void G_push(treenode* temp) {
	if (G_ptr<255 && G_ptr>-2)
		G_stack[++G_ptr] = temp;
	else
		printf("G_stack数组下标越界");
}
void S_pop() {
	S_ptr--;
}
char* LexToStr(LexType temp) {//终极符转为字符串
	for (int i = 0; i < 42; i++) {
		if (Words[i].tok == temp) {
			return Words[i].Sem;
		}
	}
	printf("Error:LexToStr");
}
int getReIndex2(LexType temp) // 返回终结符s在终结符集合中的下标
{
	for (int i = 0; i < 42; i++)
		if (temp == reservedWords1[i].tok)
			return i;
	return -1;
}
treenode* LL1_analysis() // LL1分析法
{
	cal_predict(); // 计算predict集并构造LL1预测分析表,生成LL1分析表
	out_predict(); // 输出LL1预测分析表到本地

	init_S_stack();
	nowtoken = getTokenList();
	lineno = nowtoken->Lineshow;
	S_push("Program");//文法开始符压栈
	LL1_treeROOT = (treenode*)malloc(sizeof(treenode));
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);
	G_push(LL1_treeROOT);

	while (S_ptr > -1) {//当符号栈非空
		if (getNonIndex(S_stack[S_ptr]) == -1) {//栈顶是终极符
			if (strcmp(S_stack[S_ptr], LexToStr(nowtoken->Lex)) == 0) {
				S_pop();
				nowtoken = getTokenList();
				lineno = nowtoken->Lineshow;
			}
			else {
				printf("Error:LL1终极符匹配出错");
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
		printf("ERROR:语法分析中，文件提前结束");
		return LL1_treeROOT;
	}
}
//int main() {
//	LL1_analysis();
//	return 0;
//}