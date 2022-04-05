// һЩ����������������first����follow����
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[NonNum];
int LL1table[ProductNum][ReserveNum] = { 0 }; // LL1����Ԥ���

int getNonIndex(const char* s) // ���ط��ս��s�ڷ��ս�������е��±꣨�ж�s�Ƿ�Ϊ���ս����
{
	for (int i = 0; i < NonNum; i++)
		if (strcmp(s, Non_symbol[i]) == 0)
			return i;
	return -1;
}

int getReIndex(const char* s) // �����ս��s���ս�������е��±�
{
	for (int i = 0; i < ReserveNum; i++)
		if (strcmp(s, Reserved_word[i]) == 0)
			return i;
	return -1;
}

void cal_first(const char* s) // ����first��
{
	int countEmpty = 0;
	int isEmpty = 0;
	int lenRight = 0; // ����ʽ�Ҳ����ȣ��Ҳ����Ÿ�����
	int index = getNonIndex(s); // ���ս��s�ڼ����е��±�
	for (int i = 0; i < ProductNum; i++) // ����ÿһ������ʽ
	{
		if (strcmp(Productions[i].left, s) == 0) // ����ʽ��ƥ����
		{
			for (int j = 0; strcmp(Productions[i].right[j],"0") != 0; j++) // ����ʽ�Ҳ���"0"��Ϊ������
			{
				// 1.right[j]���ս����ֱ�Ӽ���first��
				if (getNonIndex(Productions[i].right[j]) == -1)
				{
					int NonIndex = getNonIndex(Productions[i].left); // ��ȡ��ǰ����ʽ���ڷ��ս�������е��±�
					if (firsts[NonIndex].flag[getReIndex(Productions[i].right[j])] == false) // right[j]δ�������first��
					{
						firsts[NonIndex].ptr[firsts[NonIndex].num] = Productions[i].right[j];
						firsts[NonIndex].flag[getReIndex(Productions[i].right[j])] = true;
						firsts[NonIndex].num++;
						break;
					}
					else //�Ѿ��������first����ֱ��break
						break;
				}

				// 2.right[j]�Ƿ��ս������ݹ����
				cal_first(Productions[i].right[j]); // ������right[j]��first��
				int rightjIndex = getNonIndex(Productions[i].right[j]); // ��÷��ս��right[j]�ڼ����е��±�
				for (int k = 0; k < firsts[rightjIndex].num; k++) // ��first(right[j])�еķ�$�ս������first(s)
				{
					if (strcmp(firsts[rightjIndex].ptr[k], "$") == 0) // first(right[j])���Ƿ��пղ���ʽ
						isEmpty = 1;
					else
					{
						if (firsts[index].flag[getReIndex(firsts[rightjIndex].ptr[k])] == false) // tempδ�������first��
						{
							firsts[index].ptr[firsts[index].num] = firsts[rightjIndex].ptr[k];
							firsts[index].flag[getReIndex(firsts[rightjIndex].ptr[k])] = true;
							firsts[index].num++;
						}
					}
				}
				if (isEmpty == 0) // right[j]����Ϊ$����������
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
				if (firsts[index].flag[getReIndex("$")] == false) // $δ�������first��
				{
					firsts[index].ptr[firsts[index].num] = "$";
					firsts[index].flag[getReIndex("$")] = true;
					firsts[index].num++;
				}
			}
		}
	}
}

void cal_follow(const char* s, bool* flag) // ����follow��
{
	int index = getNonIndex(s); // ���ս��s�ڼ����е��±�
	if (flag[index] == false) // ���������ս���Ѿ����ҹ�
		flag[index] = true;
	else
		return;
	for (int i = 0; i < ProductNum; i++)
	{
		int lenRight = 0; // ����ʽ�Ҳ����ȣ��Ҳ����Ÿ�����
		int tempindex = -1;

		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++) // ͳ�Ʋ���ʽ�Ҳ�����
			lenRight++;

		int s_index[20] = { 0 }; // ��¼s�ڲ���ʽ�Ҳ����ֵ�����λ��
		int s_index_num = 0; // ���������β���±꣨����������

		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++)
		{
			if (strcmp(Productions[i].right[j], s) == 0) // �Ҳ���s����¼�ڲ���ʽ�Ҳ����±�
			{
				s_index[s_index_num] = j;
				s_index_num++;
			}
		}

		// 1.����һ������ʽA����B�£���ôfirst(��)�г�$������з��Ŷ���follow(B)��
		for (int j = 0; j < s_index_num; j++)
		{
			tempindex = s_index[j];

			if (tempindex != -1 && tempindex < (lenRight - 1))
			{
				bool hasEmpty = true; // ʽ�����Ƿ����$
				while ((tempindex < lenRight - 1) && hasEmpty)
				{
					tempindex += 1;
					const char* beta = Productions[i].right[tempindex];
					if (getNonIndex(beta) == -1) // �����ս��
					{
						if (follows[index].flag[getReIndex(beta)] == false) // ��δ�������follow��
						{
							follows[index].ptr[follows[index].num] = beta;
							follows[index].flag[getReIndex(beta)] = true;
							follows[index].num++;
						}
						hasEmpty = false;
						break;
					}
					else // ���Ƿ��ս��
					{
						hasEmpty = false;
						int betaindex = getNonIndex(beta); // beta�ڷ��ս�������е��±�
						for (int k = 0; k < firsts[betaindex].num; k++) // ��first(��)�����г���$�ķ��ż��뵽follow(B)��
						{
							if (strcmp(firsts[betaindex].ptr[k], "$") == 0)
								hasEmpty = true;
							else
							{
								if (follows[index].flag[getReIndex(firsts[betaindex].ptr[k])] == false) // ����ս��δ�����follow��
								{
									follows[index].ptr[follows[index].num] = firsts[betaindex].ptr[k];
									follows[index].flag[getReIndex(firsts[betaindex].ptr[k])] = true;
									follows[index].num++;
								}
							}
						}
					}
				}
				// 2.����һ������ʽA����B�£���first(��)����$����ôfollow(A)�е����з��Ŷ���follow(B)��
				if (hasEmpty && strcmp(Productions[i].left, s) != 0)
				{
					cal_follow(Productions[i].left, flag);
					int index_A = getNonIndex(Productions[i].left); // ���ս��A�ڷ��ս�������е��±�
					for (int k = 0; k < follows[index_A].num; k++)
					{
						if (follows[index].flag[getReIndex(follows[index_A].ptr[k])] == false) // ��������follow(B)���ս��follows[index_A].ptr[k]��ǰδ�����
						{
							follows[index].ptr[follows[index].num] = follows[index_A].ptr[k];
							follows[index].flag[getReIndex(follows[index_A].ptr[k])] = true;
							follows[index].num++;
						}
					}
				}
			}
			// 3.����һ������ʽA����B����ôfollow(A)�е����з��Ŷ���follow(B)��
			else if (tempindex != -1 && tempindex == lenRight - 1 && strcmp(Productions[i].left, s) != 0)
			{
				cal_follow(Productions[i].left, flag);
				int index_A = getNonIndex(Productions[i].left); // ���ս��A�ڷ��ս�������е��±�
				for (int k = 0; k < follows[index_A].num; k++)
				{
					if (follows[index].flag[getReIndex(follows[index_A].ptr[k])] == false) // ��������follow(B)���ս��follows[index_A].ptr[k]��ǰδ�����
					{
						follows[index].ptr[follows[index].num] = follows[index_A].ptr[k];
						follows[index].flag[getReIndex(follows[index_A].ptr[k])] = true;
						follows[index].num++;
					}
				}
			}
		}
	}
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
		cal_first(Non_symbol[i]);

	for (int i = 0; i < NonNum; i++) // �������з��ս����follow��
	{
		bool flag[NonNum];
		memset(flag, false, sizeof(flag));
		cal_follow(Non_symbol[i], flag);
	}

	FILE* fp;
	if ((fp = fopen("first.txt", "w")) == NULL)
	{
		printf("cannot open the first assemble file\n");
		return;
	}
	for (int i = 0; i < NonNum; i++)
	{
		fprintf(fp, "%s:  ", Non_symbol[i]);
		for (int j = 0; j < firsts[i].num; j++)
			fprintf(fp, "%s,", firsts[i].ptr[j]);
		fprintf(fp, "\n");
	}
	fclose(fp);

	if ((fp = fopen("follow.txt", "w")) == NULL)
	{
		printf("cannot open the follow assemble file\n");
		return;
	}
	for (int i = 0; i < NonNum; i++)
	{
		fprintf(fp, "%s:  ", Non_symbol[i]);
		for (int j = 0; j < follows[i].num; j++)
			fprintf(fp, "%s,", follows[i].ptr[j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void cal_predict() // ����predict�� - LL1�ķ�
{
	for (int i = 0; i < ProductNum; i++)
	{
		int leftindex = getNonIndex(Productions[i].left); // ���ڷ��ս�������е��±�
		int countEmpty = 0;
		// 1.���������Ƴ�$����predict(A����)=first(��)
		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++)
		{
			if (getNonIndex(Productions[i].right[j]) == -1) // �Ҳ����ս���������գ�
			{
				if (strcmp(Productions[i].right[j], "$") != 0) // ����ս������$
					LL1table[leftindex][getReIndex(Productions[i].right[j])] = i;
				else
					countEmpty++;
				break;
			}
			else  // �Ҳ��Ƿ��ս��
			{

			}
		}
		// 2.�������Ƴ�$����predict(A����)=first(��)��follow(A)
		if (countEmpty == )
		{

		}
	}
}