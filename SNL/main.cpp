// �����򣬶������������ļ����з���
#include "SemanticAnalysis.h"
#include "head.h"
#include "LexicalAnalysis.h"//�ʷ�����

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