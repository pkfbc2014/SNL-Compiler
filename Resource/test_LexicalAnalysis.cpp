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
        return 1;//��ĸ
    if (ch >= '0' && ch <= '9')
        return 2;//����
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' || ch == '[' || ch == ']' || ch == '=' || ch == '<' ||ch==',')
        return 3;//���ֽ��
    if (ch == ':')
        return 4;//˫�ֽ��������Ϊ��
    if (ch == '{')
        return 5;//ע��ͷ��
    if (ch == '.')
        return 6;//�����±�
    if (ch == '\'')
        return 7;//�ַ�״̬
    //����Ϊ���Բ���
    if (ch == ' ' ||ch == '\t')//�ַ�Ϊ�հ�
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
int init_node(token* ptr) {//�������ʼ��
    ptr->Lex = ID;//��ʼ��Ϊ��ʶ��
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    return 0;
}
LexType classify2(char* ptr) {//ID���ຯ��
    for (int i = 0; i < 21; i++) {
        if (strcmp(reservedWords[i].Sem, ptr) == 0) {
            return reservedWords[i].tok;
        }
    }
    return ID;
}

//δ����:��ӿ��ַ���token����?�α��д����޿�token
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
    //δ���������ʶ��
    while (error0 == NORMAL) {//δ����������δʶ�𵽳������
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
            case 5:state0 = INCOMMENT; receiver[num++] = ch; break;//ע��״̬
            case 6:
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '.') {
                    state0 = INRANGE; receiver[num++] = ch; break;//�����±����״̬
                }
                else {
                    current->Lex = ENDFILE; current->Lineshow = Line; current->next = NULL;
                    state0 = START; error0 = INRANGE_ERROR;  break;//�˴�����Ϊ��ֵ":="��':'����������'='
                }
            case 7://����¼��
                state0 = INCHAR; break;
            case 8://�հ���Ϊ����һ������״̬�Ľ�ֹ,�ո���л��еȶ��ڴ˴�������״̬������ո�Ҳ�˻ص��˴�����
                ch = getNextChar();
                break;
            case 9://����
                ch = getNextChar();
                Line++;
                break;
            case 10://����ʶ���ļ�������
                current->Lex = ENDFILE; current->next = NULL;
                current->Lineshow = Line;
                error0 = INRANGE_ERROR;
                break;
            default://�����޷�ʶ��
                error0 = ERROR1; receiver[num++] = ch; break;
            }
            break;
        case INID://ID״̬����ʶ���ID�������ֺ�����
            ch = getNextChar();
            while (classify(ch) == 1 || classify(ch) == 2)
            {
                receiver[num++] = ch;
                ch = getNextChar();
            }
            receiver[num] = '\0';
            //token�ڵ����ݹ���
            current->Lex = classify2(receiver); 
            if (current->Lex == ID) {//ID����������Ϣ
                strcpy(current->Sem, receiver);
            }
            current->Lineshow = Line;
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            if (current->Lex == ID) {
                printf("ʶ��ID��ʶ��:%s\n", receiver);
            }
            else {
                printf("ʶ��ID�Ǳ�ʶ��:%s\n", receiver);
            }
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
            current->Lex = INTC; current->Lineshow = Line;
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            printf("ʶ�����֣�%s\n", receiver);
            //��ʶ��ʶ��Token����
            ungetNextChar();
            state0 = START;
            break;
        case INASSIGN:
            current->Lex = ASSIGN; current->Lineshow = Line;
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
            current->Lex = UNDERANGE; current->Lineshow = Line;
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
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//��current��ָ����Ϣ��������
                next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                printf("ʶ���ַ���%c\n",ch);
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
                current->Lex = classify1(ch); current->Lineshow = Line;
                current = next; next = next->next;//��current��ָ����Ϣ��������
                next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                state0 = START;
                printf("ʶ�𵥷ֽ����%c\n", ch);
                ch = getNextChar();
            }
            break;
        default:
            break;
        } 
    }
    switch (error0) {
    case NORMAL:
        printf("NORMAL:token����δʶ���ļ�������");
        return head;
        break;
    case INASSIGN_ERROR:
        printf("INASSIGN_ERROR:��ֵ������д����");
        return head;
        break;
    case INRANGE_ERROR:
        printf("INRANGE_ERROR:tokenʶ���������ʶ���ļ�����������");
        return head;
        break;
    case INCHAR_ERROR:
        printf("INCHAR_ERROR:�ַ���д����");
        return head;
        break;
    case ERROR1:
        printf("ERROR1:�����޷�ʶ����ַ�,�ַ�Ϊ:%c",ch);
        return head;
        break;
    default:
        printf("tokenʶ������쳣�˳�");
        return head;
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
    token* head = getTokenList();
    if (error0 == INRANGE_ERROR) {//tokenʶ�������Ĵ���
        printf("\n����\t�ʷ���Ϣ\t������Ϣ\t");
 /*       while (head->Lex != ENDFILE) {
            printf("%-10d\n")
        }*/
    }

	return 0;
}