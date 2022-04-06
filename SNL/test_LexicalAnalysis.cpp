//本文件仅用于测试词法分析功能
//push之前应将LL1,func,main加入项目
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"LexicalAnalysis.h"
char getNextChar() {
    char ch = fgetc(fp);
    //while (ch == ' ' || ch == '\n') {
    //    ch = fgetc(fp);
    //}
    return ch;
}
int ungetNextChar() {
    fseek(fp, -(long)sizeof(char), SEEK_CUR);
    return 0;
}
int init_node(node* ptr) {//链表结点初始化
    ptr->Lex = ID;//初始化为标识符
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    ptr->Sem = nullptr;
    return 0;
}
node* getTokenList() {
    if (fp == nullptr) {
        printf("文件打开失败！\n");
        return nullptr;
    }
    node* head = (node*)malloc(sizeof(node));
    state state0 = START;
    init_node(head);

    return head;
}
int main() {
    //FILE* fp;
    //char ch;//取字符后放这里
    //printf("调试词法分析内容：\n");
    //if ((fp = fopen("C:\\Users\\11279\\Desktop\\code.txt", "r+")) == NULL)
    //    //文件打开，取一个字符/字节
    //{
    //    printf("\nCannot open file strike any key exit!");
    //    getchar();//阻塞功能，挂起
    //    exit(1);
    //}

    //int fgetc(FILE * stream);从指定的文件中读一个字符
    //ch = fgetc(fp);//从文件里取一个字节/字符，取出字符后放这里
    //while (ch != EOF)//判断有没有到文件的末尾，EOF文件结束标志
    //{
    //    putchar(ch);//从文件取的字符显示到显示器上
    //    ch = fgetc(fp);//从指定的文件中读一个字符
    //}
    //fclose(fp);
    //printf("\n调试结束\n");
    char test = getNextChar();
    while (test != EOF) {
        if (test == ' ') {
            printf("\n/此处有空格/\n");
        }
            if (test == '\n') {
            printf("\n/此处有换行/\n");
        }
            if (test == '\t') {
                printf("\n此处有Tab/\n");
            }
        printf("%c", test);
        test = getNextChar();
    }
	return 0;
}