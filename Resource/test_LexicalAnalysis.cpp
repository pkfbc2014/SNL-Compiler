//���ļ������ڲ��Դʷ���������
//push֮ǰӦ��LL1,func,main������Ŀ
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LexicalAnalysis.h"
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
int classify(char ch) {
    if ((ch > '@' && ch < '[') || (ch > '`' && ch < '{'))
        return 1;//��ĸ
    if (ch >= '0' && ch <= '9')
        return 2;//����
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' || ch == '[' || ch == ']' || ch == '=' || ch == '<' || ch == EOF || ch == ' ')
        return 3;//���ֽ��
    if (ch == ':')
        return 4;//˫�ֽ��������Ϊ��
    if (ch == '{')
        return 5;//ע��ͷ��
    if (ch == '.')
        return 6;//�����±�
    if (ch == ',')
        return 7;//�ַ�״̬
    return 8;
}
int init_node(node* ptr) {//�������ʼ��
    ptr->Lex = ID;//��ʼ��Ϊ��ʶ��
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    ptr->Sem = nullptr;
    return 0;
}
node* getTokenList() {//δ��ɣ���״̬ת�ƹ����е�tokenͬʱ����;�д���Ĵ�����
    if (fp == nullptr) {
        printf("�ļ���ʧ�ܣ�\n");
        return nullptr;
    }
    //�����㴦��
    node* head = (node*)malloc(sizeof(node)),*current=head,*next= (node*)malloc(sizeof(node));
    init_node(head); init_node(next);
    head->next = next;
    head->Sem =(char*)malloc(sizeof(char)*100);//SemΪһ�����ָ��

    state state0 = START;
    char ch = getNextChar();
    //δ��Ӷ����ַ���ѭ��
    switch (state0) {//�����߼����ȿ�״̬���ٿ�����
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
                state0 = START; error0 = INASSIGN_ERROR; break;//�˴�����Ϊ��ֵ":="��':'����������'='
            }
        case 5:state0 = INCOMMENT; break;//ע��״̬
        case 6:
            if (getNextChar() == '.') {
                state0 = INRANGE; break;//�����±����״̬
            }
            else {
                state0 = START; error0 = INRANGE_ERROR; break;//�˴�����Ϊ��ֵ":="��':'����������'='
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

	return 0;
}




dasdiaoihfohiasfoiaoi�������i