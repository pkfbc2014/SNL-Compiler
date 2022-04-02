// 主程序，读入样例程序文件进行分析

#include "head.h"

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // 输入文件为test.txt

    if ((fp = fopen(input_filename, "r")) == NULL)
    {
        printf("cannot open the file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }

    //handle(fp); // 词法分析

    fclose(fp);
	return 0;
} 