// 主程序，读入样例程序文件进行分析
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "RD_head.h"
#include "LL1_Head.h"

int main()
{
    token* tokenlisthead = NULL; // token序列头
    tokenlisthead = getTokenList(); // 计算代码的tokenlist

    out_fitstfollow(); // 计算三个集合并输出到本地，LL1分析表存储在运行内存中（全局变量）

    treenode* RDtreeROOT = RD_analysis(tokenlisthead); //语法分析 - 递归下降

	return 0;
} 