#pragma once

//一些函数的声明

int getNonIndex(char* s); // 返回非终结符s在非终结符集合中的下标
int getReIndex(char* s); // 返回终结符s在终结符集合中的下标
void cal_first(const char* s); // 计算first集
void cal_follow(const char* s, bool* flag); // 计算follow集
void cal_predict(); // 计算predict集并构造LL1预测分析表
void out_fitstfollow(); //输出first集和follow集到本地
void out_predict(); //输出LL1预测分析表到本地