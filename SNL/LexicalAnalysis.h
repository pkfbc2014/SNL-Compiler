#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

enum state{START=1};//将对空白状态的处理放入

int test()
{
    FILE* fp;
    char ch;//取字符后放这里
    if ((fp = fopen("c1.txt", "r+")) == NULL)
        //文件打开，取一个字符/字节
    {
        printf("\nCannot open file strike any key exit!");
        getchar();//阻塞功能，挂起
        exit(1);
    }
    //int fgetc(FILE *stream)从指定的文件中读一个字符
    ch = fgetc(fp);//从文件里取一个字节/字符，取出字符后放这里
    while (ch != EOF)//判断有没有到文件的末尾，EOF文件结束标志
    {
        putchar(ch);//从文件取的字符显示到显示器上
        ch = fgetc(fp);//从指定的文件中读一个字符
    }
    fclose(fp);
}
