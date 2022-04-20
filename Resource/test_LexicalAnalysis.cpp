//���ļ������ڲ��Դʷ���������
//push֮ǰӦ��LL1,func,main������Ŀ
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "LexicalAnalysis.h"

//��������
state a;
error error0;//�ʷ�����
char ch;//ȡ�ַ��������
int Line;//token���ڵ�����
extern int LexicalErrorNum;

FILE* fp = fopen("Code\\code.txt", "r");// �����ļ���дָ��
FILE* w_fp = fopen("Data\\token.txt", "w");// token��д�ļ�

word reservedWords[21] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END},{"array",ARRAY},
{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
{"return",RETURN},{"integer",INTEGER},{"char",CHAR} };	//������


word2 Words[42] = { {PROGRAM,"PROGRAM"},{TYPE,"TYPE"},{VAR,"VAR"},{ENDFILE,"ENDFILE"}, {ERROR,"ERROR"},{ID,"ID"},{INTC,"INTC"},{CHARC,"CHARC"},{ASSIGN,"ASSIGN"},
     {PROCEDURE,"PROCEDURE"},{BEGIN,"BEGIN"},{END,"END"},{ARRAY,"ARRAY"},{EQ,"EQ"},{LT,"LT"},{PLUS,"PLUS"},{MINUS,"MINUS"},{TIMES,"TIMES"},{OVER,"OVER"},
     {OF,"OF"},{RECORD,"RECORD"},{IF,"IF"},{THEN,"THEN"},{ELSE,"ELSE"},{FI,"FI"},{LPAREN,"LPAREN"},{RPAREN,"RPAREN"},{DOT,"DOT"},{COLON,"COLON"},{SEMI,"SEMI"},
     {WHILE,"WHILE"},{DO,"DO"},{ENDWH,"ENDWH"},{READ,"READ"},{WRITE,"WRITE"},{COMMA,"COMMA"},{LMIDPAREN,"LMIDPAREN"},{RMIDPAREN,"RMIDPAREN"},{UNDERANGE,"UNDERANGE"},
     {RETURN,"RETURN"},{INTEGER,"INTEGER"},{CHAR,"CHAR"} };	//������

char getNextChar() {//ȡ���¸��� �� �� ��
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
    else if (ch == ',') {
        return COMMA;
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

int print_Lex(LexType a){//��ӡ����ʷ���Ϣ
    for (int i = 0; i < 42; i++) {
        if (Words[i].tok==a) {
            //printf("%-15s\t", Words[i].Sem);
            fprintf(w_fp, "%-15s\t", Words[i].Sem);
            return 0;
        }
    }
}

//δ����:��ӿ��ַ���token����?�α��д����޿�token
token* getTokenList() {//δ��ɣ���״̬ת�ƹ����е�tokenͬʱ����;�д���Ĵ�����
    if (fp == nullptr) {
        printf("�ʷ���أ�����Դ�����ļ���ʧ�ܣ�\n");
        return NULL;
    }
    //�����㴦��


    //��������
    a = START;
    error0 = NORMAL;//�ʷ�����
    Line = 1;
    int signal_error = 0;//�Ƿ��Ѿ�������ʷ�������⵽�ʷ�����

    token* head = (token*)malloc(sizeof(token)),*current=head,*next= (token*)malloc(sizeof(token));
    init_node(head); init_node(next);
    current->next = next; next->pre = current;
    next->next = (token*)malloc(sizeof(token));

    state state0 = START;
    char ch = getNextChar();

    //DFA�ַ�������
    char receiver[100]=" ";
    int num = 0;
    LexicalErrorNum = 0;

    //δ��Ӷ����ַ���ѭ��
    //��ӳ���:�������������ļ��ĳ���->INRANGE_ERROR
    //δ���������ʶ��
    while (error0 == NORMAL) {
        switch (state0) {
        case START://��ʼ״̬���������ͣ�INASSIGN_ERROR��
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
                {//����ֻ�ģʽ
                    if (signal_error == 0)
                    {
                        printf("�ʷ���أ���⵽�ʷ�����\n");
                        signal_error = 1;
                    }
                    LexicalErrorNum++;
                    printf("        ERROR%d:��%d,��ֵ����\":=\"ƴд����\n", LexicalErrorNum, Line);
            
                    //������
                    while (ch != ENDFILE && ch != '=')
                    {
                        ch = getNextChar();
                    }
                    if (ch == ENDFILE)//���ļ�����δ������ȷ����
                    {
                        current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                        current->Lineshow = Line;
                        error0 = INRANGE_ERROR;
                        printf("�ʷ���أ��ʷ������ɹ���������⵽�ʷ�����������%d\n", LexicalErrorNum);
                        fclose(fp);//�ر��ļ�ָ��
                        return head;
                    }
                    else
                    {
                        state0 = INASSIGN;
                        receiver[num++] = ch;
                    }
                    break;
                }
            case 5:state0 = INCOMMENT; receiver[num++] = ch; break;//ע��״̬
            case 6:
                receiver[num++] = ch;
                ch = getNextChar();
                if (ch == '.') {
                    state0 = INRANGE; receiver[num++] = ch; break;//�����±����״̬
                }
                else {//��������
                    current->Lex = DOT;  strcpy(current->Sem, "."); current->Lineshow = Line;
                    current = next; next = next->next;//��current��ָ����Ϣ��������
                    next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                    state0 = START; break;//�˴�����Ϊ��ֵ":="��':'����������'='
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
                current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                current->Lineshow = Line;
                error0 = INRANGE_ERROR;
                if (LexicalErrorNum == 0)
                {
                    printf("�ʷ���أ��ʷ������ɹ�������\n");
                    fclose(fp);//�ر��ļ�ָ��
                    return head;
                }
                else
                {
                    printf("�ʷ���أ��ʷ������ɹ���������⵽�ʷ�����������%d\n",LexicalErrorNum);
                    fclose(fp);//�ر��ļ�ָ��
                    return head;
                }

                break;
            default://�����޷�ʶ��
                error0 = ERROR1; 
                LexicalErrorNum++;
                if (signal_error == 0)
                {
                    printf("�ʷ���أ���⵽�ʷ�����\n");
                    signal_error = 1;
                }
                printf("        ERROR%d:��%d,���дʷ�δ�����ַ�,Ϊ:%c��\n", LexicalErrorNum, Line, ch);
                ch = getNextChar();
                state0 = START;
                error0 = NORMAL;
                break;
            }
            break;
        case INID://ID״̬����ʶ��������ֺ�����;�������ͣ���
            ch = getNextChar();
            while (classify(ch) == 1 || classify(ch) == 2)
            {
                receiver[num++] = ch;
                ch = getNextChar();
            }
            receiver[num] = '\0';
            //token�ڵ����ݹ���
            current->Lex = classify2(receiver);
            strcpy(current->Sem, receiver);
            current->Lineshow = Line;
            //if (current->Lex == ID) {
            //    printf("ʶ��ID��ʶ��:%s\n", receiver);
            //}
            //else {
            //    printf("ʶ��ID�Ǳ�ʶ��:%s\n", receiver);
            //} 
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            //��ʶ��ʶ��Token����
            //ungetNextChar();//�˴�������
            state0 = START;
            break;
        case INNUM://����״̬���������ͣ���
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
            //printf("ʶ�����֣�%s\n", receiver);
            //��ʶ��ʶ��Token����
            state0 = START;
            break;
        case INASSIGN://��ֵ״̬���������ͣ���
            current->Lex = ASSIGN; strcpy(current->Sem, ":="); current->Lineshow = Line;
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            //printf("ʶ����ţ� :=\n");
            ch = getNextChar();
            state0 = START;
            break;
        case INCOMMENT://�����������ڣ�������token
            ch = getNextChar();
            while (ch != '}')
                ch = getNextChar();
            ch = getNextChar();
            //printf("ʶ��������ע��");//��Ӱ����һ��token����
            state0 = START;
            break;
        case INRANGE:
            current->Lex = UNDERANGE; strcpy(current->Sem, ".."); current->Lineshow = Line;
            current = next; next = next->next;//��current��ָ����Ϣ��������
            next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
            //printf("ʶ����ţ�..�������±�)\n");
            ch = getNextChar();
            state0 = START;
            break;
        case INCHAR:
            ch = getNextChar();
            if (classify(ch) == 1 || classify(ch) == 2) 
            {
                state0 = DONE;
                //����token
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//��current��ָ����Ϣ��������
                next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                //printf("ʶ���ַ���%c\n",ch);
            }
            else 
            {//����ֻ�ģʽ
                state0 = START;
                if (signal_error == 0)
                {
                    printf("�ʷ���أ���⵽�ʷ�����\n");
                    signal_error = 1;
                }
                LexicalErrorNum++;
                printf("        ERROR%d:��%d,���ַ�������󣬽��ܽ�����ĸ�����֡�\n", LexicalErrorNum, Line);
                while (classify(ch) != 1 && classify(ch) != 2)
                {
                    ch = getNextChar();
                }
                state0 = DONE;
                //����token
                current->Lex = CHARC; current->Lineshow = Line;
                receiver[num++] = ch;
                receiver[num] = '\0';
                strcpy(current->Sem, receiver);
                current = next; next = next->next;//��current��ָ����Ϣ��������
                next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                //printf("ʶ���ַ���%c\n",ch);
            }
            break;
        case DONE:
            if (classify(ch) == 1 || classify(ch) == 2) 
            {//���ַ�ʶ��
                ch = getNextChar();
                if (ch == '\'')
                {
                    state0 = START;
                    ch = getNextChar();
                }
                else 
                {
                    LexicalErrorNum++;
                    printf("        ERROR%d:��%d,���ַ��������Ӧ��'��β��\n", LexicalErrorNum, Line);
                    while (ch != '\''&&ch!=ENDFILE)
                    {
                        ch = getNextChar();
                    }
                    if (ch == ENDFILE)//���ļ�����δ������ȷ����
                    {
                        current->Lex = ENDFILE; strcpy(current->Sem, "ENDFILE"); current->next = NULL;
                        current->Lineshow = Line;
                        error0 = INRANGE_ERROR;
                        printf("�ʷ���أ��ʷ������ɹ���������⵽�ʷ�����������%d\n", LexicalErrorNum);
                        fclose(fp);//�ر��ļ�ָ��
                        return head;
                    }
                    else//��������ȷ����
                    {
                        state0 = START;
                        error0 = NORMAL;
                        ch = getNextChar();
                    }
                }
            }
            else {//���ֽ��
                receiver[num] = '\0';
                current->Lex = classify1(ch); strcpy(current->Sem, receiver); current->Lineshow = Line;
                current = next; next = next->next;//��current��ָ����Ϣ��������
                next->pre = current; next->next = (token*)malloc(sizeof(token));//����next��ָ����Ϣ
                state0 = START;
                //printf("ʶ�𵥷ֽ����%c\n", ch);
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
        printf("\n�ʷ���أ�token.txt�ļ��޷���,�޷����token��Ϣ���ļ�!\n");
        return 0;
    }
        //��token��Ϣͬʱ�����������ļ���
    if (error0 == INRANGE_ERROR) 
    {//tokenʶ�������Ĵ���
        //printf("����\t�ʷ���Ϣ\t������Ϣ\t");
        fprintf(w_fp, "����\t�ʷ���Ϣ\t\t������Ϣ\t");
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
    fclose(w_fp); // ���ļ�ָ��
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