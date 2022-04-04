// �����򣬶������������ļ����з���
#include "SemanticAnalysis.h"
#include "head.h"

char Non_symbol[67][20]; // 67�����ս��
char Reserved_word[42][20]; // 42�������ֺ���������
int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // �����ļ�Ϊtest.txt

    /*===================================================================*/
    if ((fp = fopen("Non_symbol.txt", "r")) == NULL) // ������ս���ļ�
    {
        printf("cannot open the Non_symbol file!\n"); // �ļ���ʧ��
        exit(0); // ��������
    }
    int i = 0;
    while (fgets(Non_symbol[i], 20, fp) != NULL)
    {
        Non_symbol[i][strlen(Non_symbol[i]) - 1] = '\0'; // ȥ����β���з�
        i++;
    }
    fclose(fp);

    /*===================================================================*/
    if ((fp = fopen("Reserved_word.txt", "r")) == NULL) // ���뱣�����ļ�
    {
        printf("cannot open the Reserved_word file!\n"); // �ļ���ʧ��
        exit(0); // ��������
    }
    i = 0;
    while (fgets(Reserved_word[i], 20, fp) != NULL)
    {
        Reserved_word[i][strlen(Reserved_word[i]) - 1] = '\0'; // ȥ����β���з�
        i++;
    }
    fclose(fp);

    /*===================================================================*/
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