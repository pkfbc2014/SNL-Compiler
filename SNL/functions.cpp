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
			for (int j = 0; strcmp(Productions[i].right[j],"0") != 0; j++) // 产生式右部以"0"作为结束符
			{
				char rightj[20];
				strcpy(rightj, Productions[i].right[j]);// 取右部第j个符号（非终结符和终结符）

				// 1.rightj是终结符，直接加入first集
				if (getNonIndex(rightj) == -1)
				{
					firsts[i].ptr[firsts[i].num] = Productions[i].right[j];
					firsts[i].num++;
					break;
				}

				// 2.rightj是非终结符，则递归求解
				cal_first(rightj); // 继续算rightj的first集
				int rightjIndex = getNonIndex(rightj); // 获得非终结符rightj在集合中的下标
				for (int k = 0; k < firsts[rightjIndex].num; k++) // 将first(rightj)中的非$加入first(s)
				{
					if (strcmp(firsts[rightjIndex].ptr[k], "$") == 0) // first(rightj)中是否有空产生式
						isEmpty = 1;
					else
					{
						firsts[index].ptr[firsts[index].num] = firsts[rightjIndex].ptr[k];
						firsts[index].num++;
					}
				}
				if (isEmpty == 0) // rightj不能为$，迭代结束
					break;
				else 
				{
					countEmpty += isEmpty;
					isEmpty = 0;
				}
			}
			for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++) // 统计产生式右部长度
				lenRight++;
			if (countEmpty == lenRight) // 即右部的所有符号都能推导或就是$，则将$加入到first(s)中
			{
				firsts[index].ptr[firsts[index].num] = "$";
				firsts[index].num++;
			}
		}
	}
	return;
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