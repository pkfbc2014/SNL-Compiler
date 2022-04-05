// 一些公共函数，比如求first集、follow集等
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[NonNum];

int getNonIndex(const char* s) // 返回非终结符s在非终结符集合中的下标（判断s是否为非终结符）
{
	for (int i = 0; i < NonNum; i++)
		if (strcmp(s, Non_symbol[i]) == 0)
			return i;
	return -1;
}

int getReIndex(const char* s) // 返回终结符s在终结符集合中的下标
{
	for (int i = 0; i < ReserveNum; i++)
		if (strcmp(s, Reserved_word[i]) == 0)
			return i;
	return -1;
}

void cal_first(const char* s) // 计算first集
{
	int countEmpty = 0;
	int isEmpty = 0;
	int lenRight = 0; // 产生式右部长度（右部符号个数）
	int index = getNonIndex(s); // 非终结符s在集合中的下标
	for (int i = 0; i < ProductNum; i++) // 遍历每一条产生式
	{
		if (strcmp(Productions[i].left, s) == 0) // 产生式左部匹配上
		{
			for (int j = 0; strcmp(Productions[i].right[j],"0") != 0; j++) // 产生式右部以"0"作为结束符
			{
				// 1.right[j]是终结符，直接加入first集
				if (getNonIndex(Productions[i].right[j]) == -1)
				{
					int NonIndex = getNonIndex(Productions[i].left); // 获取当前产生式左部在非终结符集合中的下标
					if (firsts[NonIndex].flag[getReIndex(Productions[i].right[j])] == false) // right[j]未被加入过first集
					{
						firsts[NonIndex].ptr[firsts[NonIndex].num] = Productions[i].right[j];
						firsts[NonIndex].flag[getReIndex(Productions[i].right[j])] = true;
						firsts[NonIndex].num++;
						break;
					}
					else //已经被加入过first集，直接break
						break;
				}

				// 2.right[j]是非终结符，则递归求解
				cal_first(Productions[i].right[j]); // 继续算right[j]的first集
				int rightjIndex = getNonIndex(Productions[i].right[j]); // 获得非终结符right[j]在集合中的下标
				for (int k = 0; k < firsts[rightjIndex].num; k++) // 将first(right[j])中的非$终结符加入first(s)
				{
					if (strcmp(firsts[rightjIndex].ptr[k], "$") == 0) // first(right[j])中是否有空产生式
						isEmpty = 1;
					else
					{
						if (firsts[index].flag[getReIndex(firsts[rightjIndex].ptr[k])] == false) // temp未被加入过first集
						{
							firsts[index].ptr[firsts[index].num] = firsts[rightjIndex].ptr[k];
							firsts[index].flag[getReIndex(firsts[rightjIndex].ptr[k])] = true;
							firsts[index].num++;
						}
					}
				}
				if (isEmpty == 0) // right[j]不能为$，迭代结束
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
				if (firsts[index].flag[getReIndex("$")] == false) // $未被加入过first集
				{
					firsts[index].ptr[firsts[index].num] = "$";
					firsts[index].flag[getReIndex("$")] = true;
					firsts[index].num++;
				}
			}
		}
	}
}

void cal_follow(const char* s) // 计算follow集
{
	
}

void out_fitstfollow() //输出first集和follow集到本地
{
	for (int i = 0; i < NonNum; i++) // 初始化
	{
		firsts[i].num = 0;
		memset(firsts[i].flag, false, sizeof(firsts[i].flag));
		follows[i].num = 0;
		memset(follows[i].flag, false, sizeof(follows[i].flag));
	}

	for (int i = 0; i < NonNum; i++) // 计算所有非终结符的first集
		cal_first(Non_symbol[i]);

	/*for (int i = 0; i < NonNum; i++) // 计算所有非终结符的follow集
		cal_follow(Non_symbol[i]);*/

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

	/*if ((fp = fopen("follow.txt", "w")) == NULL)
	{
		printf("cannot open the follow assemble file\n");
		return;
	}
	for (int i = 0; i < NonNum; i++)
	{
		fputs(Non_symbol[i], fp);
		fputs(":  ", fp);
		for (int j = 0; j < follows[i].num; i++)
		{
			fputs(follows[i].ptr[j], fp);
			fputs(",", fp);
		}
		fputs("\n", fp);
	}
	fclose(fp);
	*/
}