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

int main()
{
    token* tokenlisthead = NULL; // token序列头
    tokenlisthead = getTokenList(); // 计算代码的tokenlist
    int error_message = printToken(tokenlisthead); // 将tokenlist输出到本地
    if (!error_message)
        printf("\n 输出token序列到本地成功！\n");

    out_fitstfollow(); // 计算三个集合并输出到本地，LL1分析表存储在运行内存中（全局变量）

    treenode* RDtreeROOT = RD_analysis(tokenlisthead); // 语法分析 - 递归下降
    //treenode* LL1treeROOT = LL1_analysis(tokenlisthead); // 语法分析 - LL1

    choosePrint(RDtreeROOT, 0); // 打印RD语法树
    //choosePrint(LL1treeROOT, 1); // 打印LL1语法树
    
    semantic_analysis(RDtreeROOT); // 语义分析部分
                                 
    freetree(RDtreeROOT); // 释放RD语法树
    //freetree(LL1treeROOT); // 释放LL1语法树
    
	return 0;
} 