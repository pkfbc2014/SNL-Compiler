#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

enum state{START=1};//���Կհ�״̬�Ĵ������

int test()
{
    FILE* fp;
    char ch;//ȡ�ַ��������
    if ((fp = fopen("c1.txt", "r+")) == NULL)
        //�ļ��򿪣�ȡһ���ַ�/�ֽ�
    {
        printf("\nCannot open file strike any key exit!");
        getchar();//�������ܣ�����
        exit(1);
    }
    //int fgetc(FILE *stream)��ָ�����ļ��ж�һ���ַ�
    ch = fgetc(fp);//���ļ���ȡһ���ֽ�/�ַ���ȡ���ַ��������
    while (ch != EOF)//�ж���û�е��ļ���ĩβ��EOF�ļ�������־
    {
        putchar(ch);//���ļ�ȡ���ַ���ʾ����ʾ����
        ch = fgetc(fp);//��ָ�����ļ��ж�һ���ַ�
    }
    fclose(fp);
}
