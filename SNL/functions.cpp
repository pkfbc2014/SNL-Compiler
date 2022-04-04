// 一些公共函数，比如求first集、follow集等

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[ReserveNum];

int getNonIndex(char* s) // 返回非终结符s在非终结符集合中的下标（判断s是否为非终结符）
{
	for (int i = 0; i < NonNum; i++)
		if (strcmp(s, Non_symbol[i]) == 0)
			return i;
	return -1;
}

int getReIndex(char* s) // 返回终结符s在终结符集合中的下标
{
	for (int i = 0; i < ReserveNum; i++)
		if (strcmp(s, Reserved_word[i]) == 0)
			return i;
	return -1;
}

void cal_first(char* s) // 计算first集
{
	for (int i = 0; i < NonNum; i++) // 初始化个数为0
	{
		firsts[i].num = 0;
	}
	int countEmpty = 0;
	int isEmpty = 0;
	int lenRight = 0; // 产生式右部长度
	int index = getNonIndex(s); // 非终结符s在集合中的下标
	for (int i = 0; i < ProductNum; i++) // 遍历每一条产生式
	{
		if (strcmp(Productions[i].left, s) == 0) // 产生式左部匹配上
		{
			for (int j = 0; Productions[i].right[j] != "0"; j++) // 产生式右部以"0"作为结束符
			{
				char rightj[20];
				strcpy(rightj, Productions[i].right[j]);// 取右部第j个符号

				// 1.rightj是终结符，直接加入first集
				if (getNonIndex(rightj) == -1)
				{
					first[i][]
				}

			}
		}
	}

}

void cal_follow() // 计算follow集
{
	for (int i = 0; i < ReserveNum; i++) // 初始化个数为0
	{
		follows[i].num = 0;
	}

}

void out_fitstfollow() //输出first集和follow集到本地
{

}