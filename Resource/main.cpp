// �����򣬶������������ļ����з���
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "RD_head.h"
#include "LL1_Head.h"

int main()
{
    token* tokenlisthead = NULL; // token����ͷ
    tokenlisthead = getTokenList(); // ��������tokenlist

    out_fitstfollow(); // �����������ϲ���������أ�LL1������洢�������ڴ��У�ȫ�ֱ�����

    treenode* RDtreeROOT = RD_analysis(tokenlisthead); //�﷨���� - �ݹ��½�

	return 0;
} 