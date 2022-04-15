#pragma once

// ȫ�ֱ�����ʹ�ã����� extern �ؼ���
extern const int NonNum = 67; // ���ս������
extern const int ReserveNum = 43; //����������
extern const int ProductNum = 104; // ����ʽ����
extern const int maxloop = 40; // ��follow�������ѭ������

extern int LL1table[NonNum][ReserveNum]; // LL1����Ԥ�����ʼ����Ϊ-1

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
	char Sem[100] = { '\0' };//����������Ϣ
	token* pre;
	token* next;
};

typedef struct first
{
	int num; // �÷��ս����first����Ԫ�ظ���
	const char* ptr[20]; // ָ������ս��
	bool flag[ReserveNum + 1]; // ��֤�������ظ�Ԫ�أ���һ��$��λ��
}first;

typedef struct follow
{
	int num; // �÷��ս����follow����Ԫ�ظ���
	const char* ptr[20]; // ָ������ս��
	bool flag[ReserveNum + 1]; // ��֤�������ظ�Ԫ�أ���һ��$��λ��
}follow;

typedef struct production // ����ʽ����
{
	const char left[20]; // ����ʽ��
	const char* right[10]; // ����ʽ�Ҳ�
}production;