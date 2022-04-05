// 一些公共函数，比如求first集、follow集等
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_var.h"

first firsts[NonNum];
follow follows[NonNum];
int LL1table[ProductNum][ReserveNum] = { 0 }; // LL1分析预测表

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

void cal_follow(const char* s, bool* flag) // 计算follow集
{
	int index = getNonIndex(s); // 非终结符s在集合中的下标
	if (flag[index] == false) // 标记这个非终结符已经被找过
		flag[index] = true;
	else
		return;
	for (int i = 0; i < ProductNum; i++)
	{
		int lenRight = 0; // 产生式右部长度（右部符号个数）
		int tempindex = -1;

		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++) // 统计产生式右部长度
			lenRight++;

		int s_index[20] = { 0 }; // 记录s在产生式右部出现的所有位置
		int s_index_num = 0; // 上述数组的尾巴下标（方便索引）

		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++)
		{
			if (strcmp(Productions[i].right[j], s) == 0) // 右部有s，记录在产生式右部的下标
			{
				s_index[s_index_num] = j;
				s_index_num++;
			}
		}

		// 1.存在一个产生式A→αBβ，那么first(β)中除$外的所有符号都在follow(B)中
		for (int j = 0; j < s_index_num; j++)
		{
			tempindex = s_index[j];

			if (tempindex != -1 && tempindex < (lenRight - 1))
			{
				bool hasEmpty = true; // 式β中是否包含$
				while ((tempindex < lenRight - 1) && hasEmpty)
				{
					tempindex += 1;
					const char* beta = Productions[i].right[tempindex];
					if (getNonIndex(beta) == -1) // β是终结符
					{
						if (follows[index].flag[getReIndex(beta)] == false) // β未被加入过follow集
						{
							follows[index].ptr[follows[index].num] = beta;
							follows[index].flag[getReIndex(beta)] = true;
							follows[index].num++;
						}
						hasEmpty = false;
						break;
					}
					else // β是非终结符
					{
						hasEmpty = false;
						int betaindex = getNonIndex(beta); // beta在非终结符集合中的下标
						for (int k = 0; k < firsts[betaindex].num; k++) // 将first(β)中所有除了$的符号加入到follow(B)中
						{
							if (strcmp(firsts[betaindex].ptr[k], "$") == 0)
								hasEmpty = true;
							else
							{
								if (follows[index].flag[getReIndex(firsts[betaindex].ptr[k])] == false) // 这个终结符未加入过follow集
								{
									follows[index].ptr[follows[index].num] = firsts[betaindex].ptr[k];
									follows[index].flag[getReIndex(firsts[betaindex].ptr[k])] = true;
									follows[index].num++;
								}
							}
						}
					}
				}
				// 2.存在一个产生式A→αBβ，且first(β)包含$，那么follow(A)中的所有符号都在follow(B)中
				if (hasEmpty && strcmp(Productions[i].left, s) != 0)
				{
					cal_follow(Productions[i].left, flag);
					int index_A = getNonIndex(Productions[i].left); // 非终结符A在非终结符集合中的下标
					for (int k = 0; k < follows[index_A].num; k++)
					{
						if (follows[index].flag[getReIndex(follows[index_A].ptr[k])] == false) // 即将加入follow(B)的终结符follows[index_A].ptr[k]此前未加入过
						{
							follows[index].ptr[follows[index].num] = follows[index_A].ptr[k];
							follows[index].flag[getReIndex(follows[index_A].ptr[k])] = true;
							follows[index].num++;
						}
					}
				}
			}
			// 3.存在一个产生式A→αB，那么follow(A)中的所有符号都在follow(B)中
			else if (tempindex != -1 && tempindex == lenRight - 1 && strcmp(Productions[i].left, s) != 0)
			{
				cal_follow(Productions[i].left, flag);
				int index_A = getNonIndex(Productions[i].left); // 非终结符A在非终结符集合中的下标
				for (int k = 0; k < follows[index_A].num; k++)
				{
					if (follows[index].flag[getReIndex(follows[index_A].ptr[k])] == false) // 即将加入follow(B)的终结符follows[index_A].ptr[k]此前未加入过
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

	for (int i = 0; i < NonNum; i++) // 计算所有非终结符的follow集
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

void cal_predict() // 计算predict集 - LL1文法
{
	for (int i = 0; i < ProductNum; i++)
	{
		int leftindex = getNonIndex(Productions[i].left); // 左部在非终结符集合中的下标
		int countEmpty = 0;
		// 1.若α不能推出$，则predict(A→α)=first(α)
		for (int j = 0; strcmp(Productions[i].right[j], "0") != 0; j++)
		{
			if (getNonIndex(Productions[i].right[j]) == -1) // 右部是终结符（包括空）
			{
				if (strcmp(Productions[i].right[j], "$") != 0) // 这个终结符不是$
					LL1table[leftindex][getReIndex(Productions[i].right[j])] = i;
				else
					countEmpty++;
				break;
			}
			else  // 右部是非终结符
			{

			}
		}
		// 2.若α能推出$，则predict(A→α)=first(α)∪follow(A)
		if (countEmpty == )
		{

		}
	}
}