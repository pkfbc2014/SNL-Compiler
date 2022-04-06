#pragma once

/*============================================================*///һЩ��������
char nodekindEnum[9][10] = {
	"ProK","PheadK","TypeK",
	"VarK","ProcDecK","StmLK",
	"DecK","StmtK","ExpK"
};

char decEnum[5][10] = {
	"ArrayK","CharK","IntegerK",
	"RecordK","IdK"
};

char stmtEnum[7][10] = {
	"IfK","WhileK","AssignK","ReadK",
	"WriteK","CallK","ReturnK"
};

char expEnum[3][10] = {
	"OpK","ConstK","IdK",
};
/*============================================================*/

/*============================================================*///�﷨���Ľڵ�
typedef struct kindmember
{
	int dec; // %����ֵ% {�﷨���ڵ���������ͣ��� nodekind = DecK ʱ��Ч}
	int stmt; // %����ֵ% {�﷨���ڵ��������ͣ��� nodekind = StmtK ʱ��Ч}
	int exp; // %����ֵ% {�﷨���ڵ�ı��ʽ���ͣ��� nodekind = ExpK ʱ��Ч}
}kindmember;

typedef struct ArrayAttrmember
{
	int low; // {�����½�}
	int up; // {�����Ͻ�}
	int childType; // {��¼�����Ա����}
}ArrayAttrmember;

typedef struct procAttrmember
{
	int paramt; // %����ֵ% {���̵Ĳ������ͣ�ֵ��or���} 1: valparamtype, 2: varparamtype
}procAttrmember;

typedef struct ExpAttrmember
{
	int op; // %����ֵ% {�﷨���ڵ�����������}
	int val; // {�﷨���ڵ����ֵ�����ڵ�Ϊ���������ӡ�ʱ��Ч}
	int varkind; // %����ֵ% {�������} 1: IdV, 2: ArrayMembV, 3: FieldMembV
	int type; // %����ֵ% {�﷨���ڵ�ļ������} 1: Void, 2: Integer, 3: Boolean
}ExpAttrmember;

typedef struct attrmember
{
	ArrayAttrmember ArrayAttr; // {��¼������������}
	procAttrmember procAttr; // {��¼���̵��������}
	ExpAttrmember ExpAttr; // {��¼���ʽ���������}
}attrmember;

typedef struct treenode
{
	treenode** child; // {ָ�����﷨���ڵ�ָ��}
	treenode* sibling; // {ָ���ֵ��﷨�����ָ��}
	int lineno; // {��¼Դ�����к�}
	int nodekind; // %����ֵ% {��¼�﷨���ڵ�����}
	kindmember kind; // {��¼�﷨���ڵ�ľ������ͣ�Ϊ���ýṹ��}
	int idnum; // {��¼һ���ڵ��еı�־������}
	char** name; // {�ַ������飬�����Ա�ǽڵ��еı�־��������}
	int table; // {ָ�����飬�����Ա�ǽڵ�ĸ�����־���ڷ��ű��е����} ������������
	int type_name; // {��¼�����������ڵ�Ϊ�������ͣ��������������ͱ�־����ʾʱ��Ч} ������������
	attrmember attr; // {��¼�﷨���ڵ���������}
}treenode;
/*============================================================*/