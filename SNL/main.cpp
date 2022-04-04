// 主程序，读入样例程序文件进行分析

#include "func_statement.h"
#include "global_var.h"
#include "SemanticAnalysis.h"
#include "LexicalAnalysis.h"//词法分析
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // 输入文件为test.txt

    if ((fp = fopen(input_filename, "r")) == NULL) // 读入test文件
    {
        printf("cannot open the file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }

    //......

    fclose(fp);

    //function1(fp); // 词法分析
    //LL1_analysis(); // 语法分析 - LL1
    //RD_analysis(); //语法分析 - 递归下降
    //semantic_analysis(); //语义分析
    
	return 0;
} 