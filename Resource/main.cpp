// �����򣬶������������ļ����з���
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "RD_head.h"
#include "LL1_Head.h"
#include "SemanticAnalysis.h"
#include "printTree.h"

extern int LexicalErrorNum;

int main()
{
    /*======================================================================*/// �ʷ���������
    token* tokenlisthead = NULL; // token����ͷ
    tokenlisthead = getTokenList(); // ��������tokenlist
    int error_message = printToken(tokenlisthead); // ��tokenlist���������
    if (!error_message)
        printf("\n�ʷ���أ����token���е����سɹ���\n");
    if (LexicalErrorNum != 0)
    {
        freetoken(tokenlisthead); // �ͷ�token����
        return 0;
    }

    /*======================================================================*/// �﷨��������
    error_message = out_fitstfollow(); // �����������ϲ���������أ�LL1������洢�������ڴ��У�ȫ�ֱ�����
    if (error_message)
        printf("\n�﷨��أ����first����follow����predict�������سɹ���predict���ѳɹ����浽�����ڴ棡\n");

    treenode* RDtreeROOT = RD_analysis(tokenlisthead); // �﷨���� - �ݹ��½�
    treenode* LL1treeROOT = LL1_analysis(tokenlisthead); // �﷨���� - LL1

    error_message = choosePrint(RDtreeROOT, 0); // ��ӡRD�﷨��
    if (error_message)
        printf("\n�﷨��أ�RD�������ɹ���RD�﷨�����浽���سɹ���\n");
    error_message = choosePrint(LL1treeROOT, 1); // ��ӡLL1�﷨��
    if (error_message)
        printf("\n�﷨��أ�LL1�������ɹ���LL1�﷨�����浽���سɹ���\n");

    /*======================================================================*/// �����������
    error_message = semantic_analysis(RDtreeROOT); // �����������
    if (error_message)
        printf("\n������أ���������ɹ������ű��浽���سɹ���\n");

    freetree(RDtreeROOT); // �ͷ�RD�﷨��
    freetree(LL1treeROOT); // �ͷ�LL1�﷨��
    freetoken(tokenlisthead); // �ͷ�token����

    printf("\n����ɹ�ִ�У������˳���\n");
    system("pause");
	return 0;
} 