// �����򣬶������������ļ����з���

#include "func_statement.h"
#include "global_var.h"
#include "SemanticAnalysis.h"
#include "LexicalAnalysis.h"//�ʷ�����
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // �����ļ�Ϊtest.txt

    if ((fp = fopen(input_filename, "r")) == NULL) // ����test�ļ�
    {
        printf("cannot open the file!\n"); // �ļ���ʧ��
        exit(0); // ��������
    }

    //......

    fclose(fp);

    //function1(fp); // �ʷ�����
    //LL1_analysis(); // �﷨���� - LL1
    //RD_analysis(); //�﷨���� - �ݹ��½�
    //semantic_analysis(); //�������
    
	return 0;
} 