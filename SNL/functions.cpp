// һЩ����������������first����follow����

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[ReserveNum];

int getNonIndex(char* s) // ���ط��ս��s�ڷ��ս�������е��±꣨�ж�s�Ƿ�Ϊ���ս����
{
	for (int i = 0; i < NonNum; i++)
		if (strcmp(s, Non_symbol[i]) == 0)
			return i;
	return -1;
}

int getReIndex(char* s) // �����ս��s���ս�������е��±�
{
	for (int i = 0; i < ReserveNum; i++)
		if (strcmp(s, Reserved_word[i]) == 0)
			return i;
	return -1;
}

void cal_first(char* s) // ����first��
{
	for (int i = 0; i < NonNum; i++) // ��ʼ������Ϊ0
	{
		firsts[i].num = 0;
	}
	int countEmpty = 0;
	int isEmpty = 0;
	int lenRight = 0; // ����ʽ�Ҳ�����
	int index = getNonIndex(s); // ���ս��s�ڼ����е��±�
	for (int i = 0; i < ProductNum; i++) // ����ÿһ������ʽ
	{
		if (strcmp(Productions[i].left, s) == 0) // ����ʽ��ƥ����
		{
			for (int j = 0; Productions[i].right[j] != "0"; j++) // ����ʽ�Ҳ���"0"��Ϊ������
			{
				char rightj[20];
				strcpy(rightj, Productions[i].right[j]);// ȡ�Ҳ���j������

				// 1.rightj���ս����ֱ�Ӽ���first��
				if (getNonIndex(rightj) == -1)
				{
					first[i][]
				}

			}
		}
	}

}

void cal_follow() // ����follow��
{
	for (int i = 0; i < ReserveNum; i++) // ��ʼ������Ϊ0
	{
		follows[i].num = 0;
	}

}

void out_fitstfollow() //���first����follow��������
{

}