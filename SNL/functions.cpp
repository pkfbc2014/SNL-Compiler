// һЩ����������������first����follow����
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[NonNum];

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
				char rightj[20]; // ���ڽ����̬��
				strcpy(rightj, Productions[i].right[j]);// ȡ�Ҳ���j�����ţ����ս�����ս����

				// 1.rightj���ս����ֱ�Ӽ���first��
				if (getNonIndex(rightj) == -1)
				{
					if (firsts[i].flag[getReIndex(rightj)] == false) // rightjδ�������first��
					{
						firsts[i].ptr[firsts[i].num] = rightj;
						firsts[i].num++;
						firsts[i].flag[getReIndex(rightj)] = true;
						break;
					}
					else //�Ѿ��������first����ֱ��break
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
}

void cal_follow(char* s) // ����follow��
{
	
}

void out_fitstfollow() //���first����follow��������
{
	for (int i = 0; i < NonNum; i++) // ��ʼ��
	{
		firsts[i].num = 0;
		memset(firsts[i].flag, false, sizeof(firsts[i].flag));
		follows[i].num = 0;
		memset(follows[i].flag, false, sizeof(follows[i].flag));
	}

	for (int i = 0; i < NonNum; i++) // �������з��ս����first��
	{
		char temp[20];
		strcpy(temp, Non_symbol[i]); // �����̬��
		cal_first(temp);
	}

	for (int i = 0; i < NonNum; i++) // �������з��ս����follow��
	{
		char temp[20];
		strcpy(temp, Non_symbol[i]); // �����̬��
		cal_follow(temp);
	}

	FILE* fp;
	if ((fp = fopen("first.txt", "w")) == NULL)
	{
		printf("cannot open the first assemble file\n");
		return;
	}
	for (int i = 0; i < NonNum; i++)
	{
		fputs(Non_symbol[i], fp);
		fputs(":\n", fp);
		for (int j = 0; j < firsts[i].num; i++)
		{
			fputs(firsts[i].ptr[j], fp);
			fputs(",", fp);
		}
		fputs("\n", fp);
	}
	fclose(fp);

	if ((fp = fopen("follow.txt", "w")) == NULL)
	{
		printf("cannot open the follow assemble file\n");
		return;
	}
	for (int i = 0; i < NonNum; i++)
	{
		fputs(Non_symbol[i], fp);
		fputs(":\n", fp);
		for (int j = 0; j < follows[i].num; i++)
		{
			fputs(follows[i].ptr[j], fp);
			fputs(",", fp);
		}
		fputs("\n", fp);
	}
	fclose(fp);
}