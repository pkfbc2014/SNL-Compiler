//���ļ������ڲ��Դʷ���������
//push֮ǰӦ��LL1,func,main������Ŀ

//1.ע��LEXTYPE�Ƿ�ȫ������ 2.��������������Ϣ
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LexicalAnalysis.h"
#include "global_var.h"//ʹ�ñ����ֱ�
char getNextChar() {//ȡ���¸��� �� �� ��
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
int classify(char ch) {//�޿հ�
    if ((ch > '@' && ch < '[') || (ch > '`' && ch < '{'))
        return 1;//��ĸ
    if (ch >= '0' && ch <= '9')
        return 2;//����
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' || ch == '[' || ch == ']' || ch == '=' || ch == '<' || ch == EOF)
        return 3;//���ֽ��
    if (ch == ':')
        return 4;//˫�ֽ��������Ϊ��
    if (ch == '{')
        return 5;//ע��ͷ��
    if (ch == '.')
        return 6;//�����±�
    if (ch == '\'')
        return 7;//�ַ�״̬
    return 8;
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
int init_node(token* ptr) {//�������ʼ��
    ptr->Lex = ID;//��ʼ��Ϊ��ʶ��
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    return 0;
}


token* getTokenList() {//δ��ɣ���״̬ת�ƹ����е�tokenͬʱ����;�д���Ĵ�����
    if (fp == nullptr) {
        printf("�ļ���ʧ�ܣ�\n");
        return nullptr;
    }
    //�����㴦��
    token* head = (token*)malloc(sizeof(token)),*current=head,*next= (token*)malloc(sizeof(token));
    init_node(head); init_node(next);
    current->next = next; next->pre = current;
    next->next = (token*)malloc(sizeof(token));

    state state0 = START;
    char ch = getNextChar();

    //DFA�ַ�������
    char receiver[100]=" ";
    int num = 0;

    //δ��Ӷ����ַ���ѭ��
    //��ӳ���:�������������ļ��ĳ���->INRANGE_ERROR
    //
    //while(error0==NORMAL&&)
    switch (state0) {//�����߼����ȿ�״̬���ٿ�����
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
                state0 = START;  error0 = INASSIGN_ERROR; break;//�˴�����Ϊ��ֵ":="��':'����������'='
            }
        case 5:state0 = INCOMMENT;receiver[num++] = ch; break;//ע��״̬
        case 6:
            receiver[num++] = ch;
            ch = getNextChar();
            if (ch == '.') {
                state0 = INRANGE; receiver[num++] = ch; break;//�����±����״̬
            }
            else {
                state0 = START; error0 = INRANGE_ERROR;  break;//�˴�����Ϊ��ֵ":="��':'����������'='
            }
        case 7://����¼��
            state0 = INCHAR; break;
        default://�����޷�ʶ��
            error0 = ERROR1; receiver[num++] = ch; break;
        }
        break;
    case INID:
        ch = getNextChar();
        while (classify(ch) == 1 || classify(ch) == 2)
        {
            receiver[num++] = ch;
            ch = getNextChar();
        }
        receiver[num] = '\0';
        //token�ڵ����ݹ���
        strcpy(current->Sem,receiver);
        current->Lex = ID;
        current = next; next = next->next;//��current��ָ����Ϣ��������
        next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
        printf("ʶ���ʶ����%s\n", receiver);
        //��ʶ��ʶ��Token����
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
        //token�ڵ����ݹ���
        strcpy(current->Sem, receiver);
        current->Lex = INTC;
        current = next; next = next->next;//��current��ָ����Ϣ��������
        next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
        printf("ʶ�����֣�%s\n", receiver);
        //��ʶ��ʶ��Token����
        ungetNextChar();
        state0 = START;
        break;
    case INASSIGN:
        current->Lex = ASSIGN;
        current = next; next = next->next;//��current��ָ����Ϣ��������
        next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
        printf("ʶ����ţ� :=\n");
        state0 = START;
        break;
    case INCOMMENT://�����������ڣ�������token
        ch = getNextChar();
        while (ch != '}')
            ch = getNextChar();
        printf("ʶ��������ע��");//��Ӱ����һ��token����
        state0 = START;
        break;
    case INRANGE:
        current->Lex = UNDERANGE;
        current = next; next = next->next;//��current��ָ����Ϣ��������
        next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
        printf("ʶ����ţ�..�������±�)\n");
        state0 = START;
        break;
    case INCHAR:
        ch = getNextChar();
        if (classify(ch) == 1 || classify(ch) == 2) {
            state0 = DONE;
            //����token
            current->Lex = CHARC;
            receiver[num++] = ch;
            receiver[num] = '\0';
            strcpy(current->Sem, receiver);
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            printf("ʶ���ַ���..�������±�)\n");
        }
        else {
            state0 = START;
            error0 = INCHAR_ERROR;
        }
        break;
    case DONE:
        if (classify(ch) == 1 || classify(ch) == 2) {//���ַ�ʶ��
            ch = getNextChar();
            if (ch == '\'')
                state0 = START;
            else {
                error0 = INCHAR_ERROR;
                state0 = START;
            }
        }
        else {//���ֽ��
            current->Lex = classify1(ch);
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            state0 = START;
            printf("ʶ�𵥷ֽ����%c�������±�)\n",ch);
        }
        break;
    default:
        break;
    }
    return head;
}
int main() {
    //FILE* fp;
    //char ch;//ȡ�ַ��������
    //printf("���Դʷ��������ݣ�\n");
    //if ((fp = fopen("C:\\Users\\11279\\Desktop\\code.txt", "r+")) == NULL)
    //    //�ļ��򿪣�ȡһ���ַ�/�ֽ�
    //{
    //    printf("\nCannot open file strike any key exit!");
    //    getchar();//�������ܣ�����
    //    exit(1);
    //}

    //int fgetc(FILE * stream);��ָ�����ļ��ж�һ���ַ�
    //ch = fgetc(fp);//���ļ���ȡһ���ֽ�/�ַ���ȡ���ַ��������
    //while (ch != EOF)//�ж���û�е��ļ���ĩβ��EOF�ļ�������־
    //{
    //    putchar(ch);//���ļ�ȡ���ַ���ʾ����ʾ����
    //    ch = fgetc(fp);//��ָ�����ļ��ж�һ���ַ�
    //}
    //fclose(fp);
    //printf("\n���Խ���\n");


    /*char test = getNextChar();
    while (test != EOF) {
        if (test == ' ') {
            printf("\n/�˴��пո�/\n");
        }
            if (test == '\n') {
            printf("\n/�˴��л���/\n");
        }
            if (test == '\t') {
                printf("\n�˴���Tab/\n");
            }
        printf("%c", test);
        test = getNextChar();
    }*/
    //char ab[100] = { '\0' };
    //char abc[100] = { 'a','b','c','d','\0' };
    //strcpy(ab, abc);
    //printf("%s", ab);
    
	return 0;
}