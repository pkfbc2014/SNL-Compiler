// һЩ����������������first����follow����

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

int getNonIndex(char* s) // ���ط��ս��s�ڷ��ս�������е��±�
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

void cal_first() // ����first��
{

}

void cal_follow() // ����follow��
{

}

void out_fitstfollow() //���first����follow��������
{

}