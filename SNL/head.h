#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cal_first(); // 计算first集
void cal_follow(); // 计算follow集
void output_fitst(); //输出first集到本地
void output_follow(); //输出follow集到本地

void LL1_analysis(); //LL1分析法
void RD_analysis(); //递归下降分析法

char Non_symbol[67][20]; // 67个非终结符
char Reserved_word[42][20]; // 42个保留字和其他符号