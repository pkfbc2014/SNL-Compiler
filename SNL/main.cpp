#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // 输入文件为test.txt

    if ((fp = fopen(input_filename, "r")) == NULL)
    {
        printf("cannot open the file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }

    //handle(fp); // 进行词法分析

    fclose(fp);
	return 0;
} 