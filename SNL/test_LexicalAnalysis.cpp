//���ļ������ڲ��Դʷ���������
//push֮ǰӦ��LL1,func,main������Ŀ
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
int init_node(node* ptr) {//�������ʼ��
    ptr->Lex = ID;//��ʼ��Ϊ��ʶ��
    ptr->next = ptr->pre = nullptr;
    ptr->Lineshow = -1;
    ptr->Sem = nullptr;
    return 0;
}
node* getTokenList() {
    if (fp == nullptr) {
        printf("�ļ���ʧ�ܣ�\n");
        return nullptr;
    }
    node* head = (node*)malloc(sizeof(node));
    state state0 = START;
    init_node(head);

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
    char test = getNextChar();
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
    }
	return 0;
}