//���ļ������ڲ��Դʷ���������
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"LexicalAnalysis.h"
int main() {
    FILE* fp;
    char ch;//ȡ�ַ��������
    printf("���Դʷ��������ݣ�\n");
    if ((fp = fopen("C:\\Users\\11279\\Desktop\\code.txt", "r+")) == NULL)
        //�ļ��򿪣�ȡһ���ַ�/�ֽ�
    {
        printf("\nCannot open file strike any key exit!");
        getchar();//�������ܣ�����
        exit(1);
    }

    //int fgetc(FILE * stream);��ָ�����ļ��ж�һ���ַ�
    ch = fgetc(fp);//���ļ���ȡһ���ֽ�/�ַ���ȡ���ַ��������
    while (ch != EOF)//�ж���û�е��ļ���ĩβ��EOF�ļ�������־
    {
        putchar(ch);//���ļ�ȡ���ַ���ʾ����ʾ����
        ch = fgetc(fp);//��ָ�����ļ��ж�һ���ַ�
    }
    fclose(fp);
    printf("\n���Խ���\n");
	return 0;
}