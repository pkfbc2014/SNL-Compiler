//本文件仅用于测试词法分析功能
//push之前应将LL1,func,main加入项目
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "LexicalAnalysis.h"

//变量定义
state a;
error error0;//词法错误
char ch;//取字符后放这里
int Line;//token所在的行数
extern int LexicalErrorNum;

FILE* fp = fopen("Code\\code.txt", "r");// 代码文件读写指针
FILE* w_fp = fopen("Data\\token.txt", "w");// token读写文件

word reservedWords[21] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END},{"array",ARRAY},
{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
{"return",RETURN},{"integer",INTEGER},{"char",CHAR} };	//保留字


word2 Words[42] = { {PROGRAM,"PROGRAM"},{TYPE,"TYPE"},{VAR,"VAR"},{ENDFILE,"ENDFILE"}, {ERROR,"ERROR"},{ID,"ID"},{INTC,"INTC"},{CHARC,"CHARC"},{ASSIGN,"ASSIGN"},
     {PROCEDURE,"PROCEDURE"},{BEGIN,"BEGIN"},{END,"END"},{ARRAY,"ARRAY"},{EQ,"EQ"},{LT,"LT"},{PLUS,"PLUS"},{MINUS,"MINUS"},{TIMES,"TIMES"},{OVER,"OVER"},
     {OF,"OF"},{RECORD,"RECORD"},{IF,"IF"},{THEN,"THEN"},{ELSE,"ELSE"},{FI,"FI"},{LPAREN,"LPAREN"},{RPAREN,"RPAREN"},{DOT,"DOT"},{COLON,"COLON"},{SEMI,"SEMI"},
     {WHILE,"WHILE"},{DO,"DO"},{ENDWH,"ENDWH"},{READ,"READ"},{WRITE,"WRITE"},{COMMA,"COMMA"},{LMIDPAREN,"LMIDPAREN"},{RMIDPAREN,"RMIDPAREN"},{UNDERANGE,"UNDERANGE"},
     {RETURN,"RETURN"},{INTEGER,"INTEGER"},{CHAR,"CHAR"} };	//保留字

char getNextChar() {//取得下个非 空 字 符
    char ch = fgetc(fp);
    //while (ch == ' ' || ch == '\n'||ch=='\t') {
    //   ch = fgetc(fp);
    //}
    return ch;
}

int ungetNextChar() 
{
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
    else if (ch == ',') {
        return COMMA;
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

int print_Lex(LexType a){//打印输出词法信息
    for (int i = 0; i < 42; i++) {
        if (Words[i].tok==a) {
            //printf("%-15s\t", Words[i].Sem);
            fprintf(w_fp, "%-15s\t", Words[i].Sem);
            return 0;
        }
    }
}

//未处理:添加空字符的token处理?课本中处理并无空token
token* getTokenList() {//未完成：在状态转移过程中的token同时生成;有错误的处理方法
    if (fp == nullptr) {
        printf("词法相关：输入源代码文件打开失败！\n");
        return NULL;
    }
    //链表结点处理


    //变量定义
    a = START;
    error0 = NORMAL;//词法错误
    Line = 1;
    int signal_error = 0;//是否已经输出“词法分析检测到词法错误”

    token* head = (token*)malloc(sizeof(token)),*current=head,*next= (token*)malloc(sizeof(token));
    init_node(head); init_node(next);
    current->next = next; next->pre = current;
    next->next = (token*)malloc(sizeof(token));

    state state0 = START;
    char ch = getNextChar();

    //DFA字符接收器
    char receiver[100]=" ";
    int num = 0;
    LexicalErrorNum = 0;

    //未添加读新字符的循环
    //添加出口:读完整个程序文件的出口->INRANGE_ERROR
    //未添加行数的识别
    while (error0 == NORMAL) {
        switch (state0) {
        case START://初始状态；错误类型：INASSIGN_ERROR，
            num = 0; receiver[0] = '\0';
            switch (classify(ch)) {
            case 1:
                state0 = INID; receiver[num++] = ch; break;
            case 2:
                state0 = INNUM; receiver[num++] = ch; break;
            case 3:
                state0 = DONE; receiver[num++] = ch; break;
            case 4://INASSIGN_ERROR
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '=')
                {
                    state0 = INASSIGN;
                    receiver[num++] = ch;
                    break;
                }
                else 
                {//进入恐慌模式
                    if (signal_error == 0)
                    {
                        printf("词法相关：检测到词法错误：\n");
                        signal_error = 1;
                    }
                    LexicalErrorNum++;
                    printf("        ERROR%d:行%d,赋值符号\":=\"拼写有误。\n", LexicalErrorNum, Line);
            
                    //错误处理
                    while (ch != ENDFILE && ch != '=')
                    {
                        ch = getNextChar();
                    }
                    if (ch == ENDFILE)//至文件结束未读到正确输入
                    {
                        current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                        current->Lineshow = Line;
                        error0 = INRANGE_ERROR;
                        printf("词法相关：词法分析成功结束！检测到词法错误数量：%d\n", LexicalErrorNum);
                        fclose(fp);//关闭文件指针
                        return head;
                    }
                    else
                    {
                        state0 = INASSIGN;
                        receiver[num++] = ch;
                    }
                    break;
                }
            case 5:state0 = INCOMMENT; receiver[num++] = ch; break;//注释状态
            case 6:
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '.') {
                    state0 = INRANGE; receiver[num++] = ch; break;//数组下标界限状态
                }
                else {//其他符号
                    current->Lex = DOT;  strcpy(current->Sem, "."); current->Lineshow = Line;
                    current = next; next = next->next;//新current的指针信息是完整的
                    next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                    state0 = START; break;//此处错误为赋值":="中':'后所跟并非'='
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
                current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                current->Lineshow = Line;
                error0 = INRANGE_ERROR;
                if (LexicalErrorNum == 0)
                {
                    printf("词法相关：词法分析成功结束！\n");
                    fclose(fp);//关闭文件指针
                    return head;
                }
                else
                {
                    printf("词法相关：词法分析成功结束！检测到词法错误数量：%d\n",LexicalErrorNum);
                    fclose(fp);//关闭文件指针
                    return head;
                }

                break;
            default://输入无法识别
                error0 = ERROR1; 
                LexicalErrorNum++;
                if (signal_error == 0)
                {
                    printf("词法相关：检测到词法错误：\n");
                    signal_error = 1;
                }
                printf("        ERROR%d:行%d,含有词法未定义字符,为:%c。\n", LexicalErrorNum, Line, ch);
                ch = getNextChar();
                state0 = START;
                error0 = NORMAL;
                break;
            }
            break;
        case INID://ID状态可以识别出保留字和类型;错误类型：无
            ch = getNextChar();
            while (classify(ch) == 1 || classify(ch) == 2)
            {
                receiver[num++] = ch;
                ch = getNextChar();
            }
            receiver[num] = '\0';
            //token节点内容构建
            current->Lex = classify2(receiver);
            strcpy(current->Sem, receiver);
            current->Lineshow = Line;
            //if (current->Lex == ID) {
            //    printf("识别到ID标识符:%s\n", receiver);
            //}
            //else {
            //    printf("识别到ID非标识符:%s\n", receiver);
            //} 
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            //标识符识别，Token建立
            //ungetNextChar();//此处有问题
            state0 = START;
            break;
        case INNUM://数字状态；错误类型：无
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
            //printf("识别数字：%s\n", receiver);
            //标识符识别，Token建立
            state0 = START;
            break;
        case INASSIGN://赋值状态；错误类型：无
            current->Lex = ASSIGN; strcpy(current->Sem, ":="); current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            //printf("识别符号： :=\n");
            ch = getNextChar();
            state0 = START;
            break;
        case INCOMMENT://包含符号在内，不生成token
            ch = getNextChar();
            while (ch != '}')
                ch = getNextChar();
            ch = getNextChar();
            //printf("识别并已跳过注释");//不影响下一轮token生成
            state0 = START;
            break;
        case INRANGE:
            current->Lex = UNDERANGE; strcpy(current->Sem, ".."); current->Lineshow = Line;
            current = next; next = next->next;//新current的指针信息是完整的
            next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
            //printf("识别符号：..（数组下标)\n");
            ch = getNextChar();
            state0 = START;
            break;
        case INCHAR:
            ch = getNextChar();
            if (classify(ch) == 1 || classify(ch) == 2) 
            {
                state0 = DONE;
                //构建token
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//新current的指针信息是完整的
                next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                //printf("识别字符：%c\n",ch);
            }
            else 
            {//进入恐慌模式
                state0 = START;
                if (signal_error == 0)
                {
                    printf("词法相关：检测到词法错误：\n");
                    signal_error = 1;
                }
                LexicalErrorNum++;
                printf("        ERROR%d:行%d,单字符输入错误，仅能接收字母或数字。\n", LexicalErrorNum, Line);
                while (classify(ch) != 1 && classify(ch) != 2)
                {
                    ch = getNextChar();
                }
                state0 = DONE;
                //构建token
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//新current的指针信息是完整的
                next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                //printf("识别字符：%c\n",ch);
            }
            break;
        case DONE:
            if (classify(ch) == 1 || classify(ch) == 2) 
            {//单字符识别
                ch = getNextChar();
                if (ch == '\'')
                {
                    state0 = START;
                    ch = getNextChar();
                }
                else 
                {
                    LexicalErrorNum++;
                    printf("        ERROR%d:行%d,单字符输入错误，应以'结尾。\n", LexicalErrorNum, Line);
                    while (ch != '\''&&ch!=ENDFILE)
                    {
                        ch = getNextChar();
                    }
                    if (ch == ENDFILE)//至文件结束未读到正确输入
                    {
                        current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                        current->Lineshow = Line;
                        error0 = INRANGE_ERROR;
                        printf("词法相关：词法分析成功结束！检测到词法错误数量：%d\n", LexicalErrorNum);
                        fclose(fp);//关闭文件指针
                        return head;
                    }
                    else//读到了正确输入
                    {
                        state0 = START;
                        error0 = NORMAL;
                        ch = getNextChar();
                    }
                }
            }
            else {//单分界符
                receiver[num] = '\0';
                current->Lex = classify1(ch); strcpy(current->Sem, receiver); current->Lineshow = Line;
                current = next; next = next->next;//新current的指针信息是完整的
                next->pre = current; next->next = (token*)malloc(sizeof(token));//完善next的指针信息
                state0 = START;
                //printf("识别单分界符：%c\n", ch);
                ch = getNextChar();
            }
            break;
        default:
            break;
        }
    }
}

int printToken(token* head)
{
    if (w_fp == NULL) 
    {
        printf("\n词法相关：token.txt文件无法打开,无法输出token信息到文件!\n");
        return 0;
    }
        //将token信息同时输出到界面和文件中
    if (error0 == INRANGE_ERROR) 
    {//token识别正常的处理
        //printf("行数\t词法信息\t语义信息\t");
        fprintf(w_fp, "行数\t词法信息\t\t语义信息\t");
        while (head->next != NULL) {
            //printf("\n%d\t", head->Lineshow);
            fprintf(w_fp,"\n%d\t", head->Lineshow);

            print_Lex(head->Lex);

            //printf("%-15s", head->Sem);
            fprintf(w_fp, "%-15s", head->Sem);
            head = head->next;
        }
        //printf("\n%d\t", head->Lineshow);
        fprintf(w_fp, "\n%d\t", head->Lineshow);

        print_Lex(head->Lex);

        //printf("%-15s", head->Sem);
        fprintf(w_fp, "%-15s", head->Sem);
        head = head->next;
    }   
    fclose(w_fp); // 关文件指针
    return 0;
}

void freetoken(token* head)
{
    while (head->next != NULL) {
        head = head->next;
        free(head->pre);
    }
    free(head);
}