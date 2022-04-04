#pragma once

//一些函数的声明

int getNonIndex(char* s); // 返回非终结符s在非终结符集合中的下标
void cal_first(); // 计算first集
void cal_follow(); // 计算follow集
void out_fitstfollow(); //输出first集和follow集到本地

void LL1_analysis(); //LL1分析法
void RD_analysis(); //递归下降分析法