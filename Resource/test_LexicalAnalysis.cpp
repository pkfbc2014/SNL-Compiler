//本文件仅用于测试词法分析功能
//push之前应将LL1,func,main加入项目
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LexicalAnalysis.h"
char getNextChar() {//取得下个非 空 字 符
    char ch = fgetc(fp);
    while (ch == ' ' || ch == '\n'||ch=='\t') {
       ch = fgetc(fp);
    }
    return ch;
}
int ungetNextChar() {
    fseek(fp, -(long)sizeof(char), SEEK_CUR);
    return 0;
}
int classify(char ch) {
    if ((ch > '@' && ch < '[') || (ch > '`' && ch < '{'))
        return 1;//字母
    if (ch >= '0' && ch <= '9')
        return 2;//数字
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' || ch == '[' || ch == ']' || ch == '=' || ch == '<' || ch == EOF || ch == ' ')
        return 3;//单分界符
    if (ch == ':')
        return 4;//双分界符，接收为：
    if (ch == '{')
        return 5;//注释头符
    if (ch == '.')
        return 6;//数组下标
    if (ch == ',')
        return 7;//字符状态
    return 8;
}
int init_node(node* ptr) {//链表结点初始化
    ptr->Lex = ID;//初始化为标识符
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    ptr->Sem = nullptr;
    return 0;
}
node* getTokenList() {//未完成：在状态转移过程中的token同时生成;有错误的处理方法
    if (fp == nullptr) {
        printf("文件打开失败！\n");
        return nullptr;
    }
    //链表结点处理
    node* head = (node*)malloc(sizeof(node)),*current=head,*next= (node*)malloc(sizeof(node));
    init_node(head); init_node(next);
    head->next = next;
    head->Sem =(char*)malloc(sizeof(char)*100);//Sem为一数组的指针

    state state0 = START;
    char ch = getNextChar();
    //未添加读新字符的循环
    switch (state0) {//处理逻辑：先看状态，再看输入
    case START:
        switch (classify(ch)) {
        case 1:
            state0 = INID; break;
        case 2:
            state0 = INNUM; break;
        case 3:
            state0 = DONE; break;
        case 4:
            if (getNextChar() == '=') {
                state0 = INASSIGN; break;
            }
            else {
                state0 = START; error0 = INASSIGN_ERROR; break;//此处错误为赋值":="中':'后所跟并非'='
            }
        case 5:state0 = INCOMMENT; break;//注释状态
        case 6:
            if (getNextChar() == '.') {
                state0 = INRANGE; break;//数组下标界限状态
            }
            else {
                state0 = START; error0 = INRANGE_ERROR; break;//此处错误为赋值":="中':'后所跟并非'='
            }
        case 7:
            state0 = INCHAR; break;
        default:
            error0 = ERROR; break;
        }
        break;
    case INID:
        while (classify(ch) == 1 || classify(ch) == 2)
            ch = getNextChar();
        break;
    default:
        break;
    }
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


    /*char test = getNextChar();
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
    }*/

	return 0;
}




dasdiaoihfohiasfoiaoi发呆红佛【i