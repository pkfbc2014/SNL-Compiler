//本文件仅用于测试词法分析功能
//push之前应将LL1,func,main加入项目

//1.注意LEXTYPE是否全部用上 2.正常生成行数信息
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LexicalAnalysis.h"
#include "global_var.h"//使用保留字表
char getNextChar() {//取得下个非 空 字 符
    char ch = fgetc(fp);
    //while (ch == ' ' || ch == '\n'||ch=='\t') {
    //   ch = fgetc(fp);
    //}
    return ch;
}
int ungetNextChar() {
    fseek(fp, -(long)sizeof(char), SEEK_CUR);
    return 0;
}
int classify(char ch) {//
    if ((ch > '@' && ch < '[') || (ch > '`' && ch < '{'))
        return 1;//字母
    if (ch >= '0' && ch <= '9')
        return 2;//数字
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' || ch == '[' || ch == ']' || ch == '=' || ch == '<' ||ch==',')
        return 3;//单分界符
    if (ch == ':')
        return 4;//双分界符，接收为：
    if (ch == '{')
        return 5;//注释头符
    if (ch == '.')
        return 6;//数组下标
    if (ch == '\'')
        return 7;//字符状态
    //以下为测试部分
    if (ch == ' ' ||ch == '\t')//字符为空白
        return 8;
    if (ch == '\n')
        return 9;
    if (ch == EOF)
        return 10;
    return 11;
}
LexType classify1(char ch) {
    if (ch == '+') {
        return PLUS;
    }
    else if (ch == '-') {
        return MINUS;
    }
    else if (ch == '*') {
        return TIMES;
    }
    else if (ch == '/') {
        return OVER;
    }
    else if (ch == '(') {
        return LPAREN;
    }
    else if (ch == ')') {
        return RPAREN;
    }
    else if (ch == ';') {
        return SEMI;
    }
    else if (ch == '[') {
        return LMIDPAREN;
    }
    else if (ch == ']') {
        return RMIDPAREN;
    }
    else if (ch == '=') {
        return EQ;
    }
    else if (ch == '<') {
        return LT;
    }
    else if (ch == EOF) {
        return ENDFILE;
    }

}
int init_node(token* ptr) {//链表结点初始化
    ptr->Lex = ID;//初始化为标识符
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    return 0;
}
LexType classify2(char* ptr) {//ID分类函数
    for (int i = 0; i < 21; i++) {
        if (strcmp(reservedWords[i].Sem, ptr) == 0) {
            return reservedWords[i].tok;
        }
    }
    return ID;
}

//未处理:添加空字符的token处理?课本中处理并无空token
token* getTokenList() {//未完成：在状态转移过程中的token同时生成;有错误的处理方法
    if (fp == nullptr) {
        printf("文件打开失败！\n");
        return nullptr;
    }
    //链表结点处理
    token* head = (token*)malloc(sizeof(token)),*current=head,*next= (token*)malloc(sizeof(token));
    init_node(head); init_node(next);
    current->next = next; next->pre = current;
    next->next = (token*)malloc(sizeof(token));

    state state0 = START;
    char ch = getNextChar();

    //DFA字符接收器
    char receiver[100]=" ";
    int num = 0;

    //未添加读新字符的循环
    //添加出口:读完整个程序文件的出口->INRANGE_ERROR
    //未添加行数的识别
    while (error0 == NORMAL) {//未发生错误且未识别到程序结束
        switch (state0) {//处理逻辑：先看状态，再看输入
        case START:
            num = 0; receiver[0] = '\0';
            switch (classify(ch)) {
            case 1:
                state0 = INID; receiver[num++] = ch; break;
            case 2:
                state0 = INNUM; receiver[num++] = ch; break;
            case 3:
                state0 = DONE; receiver[num++] = ch; break;
            case 4:
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '=') {
                    state0 = INASSIGN;
                    receiver[num++] = ch;
                    break;
                }
                else {
                    state0 = START;  error0 = INASSIGN_ERROR; break;//此处错误为赋值":="中':'后所跟并非'='
                }
            case 5:state0 = INCOMMENT; receiver[num++] = ch; break;//注释状态
            case 6:
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '.') {
                    state0 = INRANGE; receiver[num++] = ch; break;//数组下标界限状态
                }
                else {
                    current->Lex = ENDFILE; current->Lineshow = Line; current->next = NULL;
                    state0 = START; error0 = INRANGE_ERROR;  break;//此处错误为赋值":="中':'后所跟并非'='
                }
            case 7://无需录入
                state0 = INCHAR; break;
            case 8://空白认为是上一个输入状态的截止,空格空行换行等都在此处理，其余状态的输入空格也退回到此处处理
                ch = getNextChar();
                break;
            case 9://换行
                ch = getNextChar();
                Line++;
                break;
            case 10://正常识别到文件结束符
                current->Lex = ENDFILE; current->next = NULL;
                current->Lineshow = Line;
                error0 = INRANGE_ERROR;
                break;
            default://输入无法识别
                error0 = ERROR1; receiver[num++] = ch; break;
            }
            break;
        case INID://ID状态可以识别出ID，保留字和类型
            ch = getNextChar();
            while (classify(ch) == 1 || classify(ch) == 2)
            {
                receiver[num++] = ch;
                ch = getNextChar();
            }
            receiver[num] = '\0';
            //token节点内容构建
            current->Lex = classify2(receiver); 
            if (current->Lex == ID) {//ID才有语义信息
                strcpy(current->Sem, receiver);
            }
            current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            if (current->Lex == ID) {
                printf("识别到ID标识符:%s\n", receiver);
            }
            else {
                printf("识别到ID非标识符:%s\n", receiver);
            }
            //标识符识别，Token建立
            ungetNextChar();
            state0 = START;
            break;
        case INNUM:
            ch = getNextChar();
            while (classify(ch) == 2) {
                receiver[num++] = ch;
                ch = getNextChar();
            }
            receiver[num] = '\0';
            //token节点内容构建
            strcpy(current->Sem, receiver);
            current->Lex = INTC; current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            printf("识别数字：%s\n", receiver);
            //标识符识别，Token建立
            ungetNextChar();
            state0 = START;
            break;
        case INASSIGN:
            current->Lex = ASSIGN; current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            printf("识别符号： :=\n");
            state0 = START;
            break;
        case INCOMMENT://包含符号在内，不生成token
            ch = getNextChar();
            while (ch != '}')
                ch = getNextChar();
            printf("识别并已跳过注释");//不影响下一轮token生成
            state0 = START;
            break;
        case INRANGE:
            current->Lex = UNDERANGE; current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            printf("识别符号：..（数组下标)\n");
            state0 = START;
            break;
        case INCHAR:
            ch = getNextChar();
            if (classify(ch) == 1 || classify(ch) == 2) {
                state0 = DONE;
                //构建token
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//新current的指针信息是完整的
                next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                printf("识别字符：%c\n",ch);
            }
            else {
                state0 = START;
                error0 = INCHAR_ERROR;
            }
            break;
        case DONE:
            if (classify(ch) == 1 || classify(ch) == 2) {//单字符识别
                ch = getNextChar();
                if (ch == '\'')
                    state0 = START;
                else {
                    error0 = INCHAR_ERROR;
                    state0 = START;
                }
            }
            else {//单分界符
                current->Lex = classify1(ch); current->Lineshow = Line;
                current = next; next = next->next;//新current的指针信息是完整的
                next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                state0 = START;
                printf("识别单分界符：%c\n", ch);
                ch = getNextChar();
            }
            break;
        default:
            break;
        } 
    }
    switch (error0) {
    case NORMAL:
        printf("NORMAL:token序列未识别到文件结束符");
        return head;
        break;
    case INASSIGN_ERROR:
        printf("INASSIGN_ERROR:赋值符号书写错误");
        return head;
        break;
    case INRANGE_ERROR:
        printf("INRANGE_ERROR:token识别程序正常识别到文件结束符结束");
        return head;
        break;
    case INCHAR_ERROR:
        printf("INCHAR_ERROR:字符书写出错");
        return head;
        break;
    case ERROR1:
        printf("ERROR1:读入无法识别的字符,字符为:%c",ch);
        return head;
        break;
    default:
        printf("token识别程序异常退出");
        return head;
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
    //char ab[100] = { '\0' };
    //char abc[100] = { 'a','b','c','d','\0' };
    //strcpy(ab, abc);
    //printf("%s", ab);
    token* head = getTokenList();
    if (error0 == INRANGE_ERROR) {//token识别正常的处理
        printf("\n行数\t词法信息\t语义信息\t");
 /*       while (head->Lex != ENDFILE) {
            printf("%-10d\n")
        }*/
    }

	return 0;
}