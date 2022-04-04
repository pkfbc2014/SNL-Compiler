// 一些公共函数，比如求first集、follow集等

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

int getNonIndex(char* s) // 返回非终结符s在非终结符集合中的下标
{
	for (int i = 0; i < NonNum; i++)
		if (strcmp(s, Non_symbol[i]) == 0)
			return i;
	return -1;
}

void cal_first() // 计算first集
{

}

void cal_follow() // 计算follow集
{

}

void output_fitst() //输出first集到本地
{

}

void output_follow() //输出follow集到本地
{

}