// �����򣬶������������ļ����з���

#include "head.h"

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // �����ļ�Ϊtest.txt

    if ((fp = fopen(input_filename, "r")) == NULL)
    {
        printf("cannot open the file!\n"); // �ļ���ʧ��
        exit(0); // ��������
    }

    //function1(fp); // �ʷ�����
    //LL1_analysis(); // �﷨���� - LL1
    //RD_analysis(); //�﷨���� - �ݹ��½�
    //function2(); //�������
    fclose(fp);
	return 0;
} 