// �����򣬶������������ļ����з���
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "RD_head.h"
#include "LL1_Head.h"
#include "printTree.h"

int main()
{
    token* tokenlisthead = NULL; // token����ͷ
    tokenlisthead = getTokenList(); // ��������tokenlist
    int error_message = printToken(tokenlisthead); // ��tokenlist���������
    if (!error_message)
        printf("\n ���token���е����سɹ���\n");

    out_fitstfollow(); // �����������ϲ���������أ�LL1������洢�������ڴ��У�ȫ�ֱ�����

    treenode* LL1TreeROOT = LL1_analysis();

    // treenode* RDtreeROOT = RD_analysis(tokenlisthead); //�﷨���� - �ݹ��½�

    // �����������

    // choosePrint(RDtreeROOT, 0); // ��ӡRD�﷨��
    // choosePrint(LL1treeROOT, 1); // ��ӡLL1�﷨��

    // freetree(RDtreeROOT); // �ͷ�RD�﷨��
    // freetree(LL1treeROOT); // �ͷ�LL1�﷨��
    
	return 0;
} 