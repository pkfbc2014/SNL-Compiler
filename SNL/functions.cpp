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
			for (int j = 0; strcmp(Productions[i].right[j],"0") != 0; j++) // ����ʽ�Ҳ���"0"��Ϊ������
			{
				char rightj[20];
				strcpy(rightj, Productions[i].right[j]);// ȡ�Ҳ���j�����ţ����ս�����ս����

				// 1.rightj���ս����ֱ�Ӽ���first��
				if (getNonIndex(rightj) == -1)
				{
					firsts[i].ptr[firsts[i].num] = Productions[i].right[j];
					firsts[i].num++;
					break;
				}

				// 2.rightj�Ƿ��ս������ݹ����
				cal_first(rightj); // ������rightj��first��
				int rightjIndex = getNonIndex(rightj); // ��÷��ս��rightj�ڼ����е��±�
				for (int k = 0; k < firsts[rightjIndex].num; k++) // ��first(rightj)�еķ�$����first(s)
				{
					if (strcmp(firsts[rightjIndex].ptr[k], "$") == 0) // first(rightj)���Ƿ��пղ���ʽ
						isEmpty = 1;
					else
					{
						firsts[index].ptr[firsts[index].num] = firsts[rightjIndex].ptr[k];
						firsts[index].num++;
					}
				}
				if (isEmpty == 0) // rightj����Ϊ$����������
					break;
				else 
				{
					countEmpty += isEmpty;
					isEmpty = 0;
				}
			}
			for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++) // ͳ�Ʋ���ʽ�Ҳ�����
				lenRight++;
			if (countEmpty == lenRight) // ���Ҳ������з��Ŷ����Ƶ������$����$���뵽first(s)��
			{
				firsts[index].ptr[firsts[index].num] = "$";
				firsts[index].num++;
			}
		}
	}
	return;
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