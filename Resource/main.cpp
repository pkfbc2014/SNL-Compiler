// 主程序，读入样例程序文件进行分析
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"
#include "RD_head.h"
#include "LL1_Head.h"
#include "SemanticAnalysis.h"
#include "printTree.h"

extern int LexicalErrorNum;

int main()
{
    /*======================================================================*/// 词法分析部分
    token* tokenlisthead = NULL; // token序列头
    tokenlisthead = getTokenList(); // 计算代码的tokenlist
    int error_message = printToken(tokenlisthead); // 将tokenlist输出到本地
    if (!error_message)
        printf("\n词法相关：输出token序列到本地成功！\n");
    if (LexicalErrorNum != 0)
    {
        freetoken(tokenlisthead); // 释放token序列
        return 0;
    }

    /*======================================================================*/// 语法分析部分
    error_message = out_fitstfollow(); // 计算三个集合并输出到本地，LL1分析表存储在运行内存中（全局变量）
    if (error_message)
        printf("\n语法相关：输出first集、follow集、predict集到本地成功！predict集已成功保存到运行内存！\n");

    treenode* RDtreeROOT = RD_analysis(tokenlisthead); // 语法分析 - 递归下降
    treenode* LL1treeROOT = LL1_analysis(tokenlisthead); // 语法分析 - LL1

    error_message = choosePrint(RDtreeROOT, 0); // 打印RD语法树
    if (error_message)
        printf("\n语法相关：RD分析法成功！RD语法树保存到本地成功！\n");
    error_message = choosePrint(LL1treeROOT, 1); // 打印LL1语法树
    if (error_message)
        printf("\n语法相关：LL1分析法成功！LL1语法树保存到本地成功！\n");

    /*======================================================================*/// 语义分析部分
    error_message = semantic_analysis(RDtreeROOT); // 进行语义分析
    if (error_message)
        printf("\n语义相关：语义分析成功！符号表保存到本地成功！\n");

    freetree(RDtreeROOT); // 释放RD语法树
    freetree(LL1treeROOT); // 释放LL1语法树
    freetoken(tokenlisthead); // 释放token序列

    printf("\n程序成功执行，即将退出！\n");
    system("pause");
	return 0;
} 