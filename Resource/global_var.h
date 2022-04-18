#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
//һЩȫ�ֱ���

constexpr int NonNum = 67; // ���ս������
constexpr int ReserveNum = 43; //����������
constexpr int ProductNum = 104; // ����ʽ����
constexpr int maxloop = 40; // ��follow�������ѭ������
constexpr int maxlen = 20; // һ���ַ�����󳤶�

typedef enum//�ʷ����Ͷ���
{	//���ǵ��ʷ�
	ENDFILE, ERROR,

	//������,�����ֺ��������ڱ�ʶ�����룬��ʶ����ID��״̬��ʶ��������֣����ͣ���ID
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END, READ, WRITE, ARRAY, OF,
	RECORD, RETURN,
	//����
	INTEGER, CHAR,

	//���ַ����ʷ���
	ID, INTC, CHARC,

	//�������
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE//�����±�
}LexType;

struct token//�����㶨��
{
	int Lineshow;//������������
	LexType Lex;
	char Sem[100];//����������Ϣ
	token* pre;
	token* next;
};

typedef struct first
{
	int num; // �÷��ս����first����Ԫ�ظ���
	const char* ptr[maxlen]; // ָ������ս��
	bool flag[ReserveNum + 1]; // ��֤�������ظ�Ԫ�أ���һ��$��λ��
}first;

typedef struct follow
{
	int num; // �÷��ս����follow����Ԫ�ظ���
	const char* ptr[maxlen]; // ָ������ս��
	bool flag[ReserveNum + 1]; // ��֤�������ظ�Ԫ�أ���һ��$��λ��
}follow;

typedef struct production // ����ʽ����
{
	const char left[maxlen]; // ����ʽ��
	const char* right[10]; // ����ʽ�Ҳ�
}production;