#pragma once

/*============================================================*///一些索引数组
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

/*============================================================*///语法树的节点
typedef struct kindmember
{
	int dec; // %索引值% {语法树节点的声明类型，当 nodekind = DecK 时有效}
	int stmt; // %索引值% {语法树节点的语句类型，当 nodekind = StmtK 时有效}
	int exp; // %索引值% {语法树节点的表达式类型，当 nodekind = ExpK 时有效}
}kindmember;

typedef struct ArrayAttrmember
{
	int low; // {数组下界}
	int up; // {数组上界}
	int childType; // {记录数组成员类型}
}ArrayAttrmember;

typedef struct procAttrmember
{
	int paramt; // %索引值% {过程的参数类型，值参or变参} 1: valparamtype, 2: varparamtype
}procAttrmember;

typedef struct ExpAttrmember
{
	int op; // %索引值% {语法树节点的运算符单词}
	int val; // {语法树节点的数值，当节点为“数字因子”时有效}
	int varkind; // %索引值% {变量类别} 1: IdV, 2: ArrayMembV, 3: FieldMembV
	int type; // %索引值% {语法树节点的检查类型} 1: Void, 2: Integer, 3: Boolean
}ExpAttrmember;

typedef struct attrmember
{
	ArrayAttrmember ArrayAttr; // {记录数组的相关属性}
	procAttrmember procAttr; // {记录过程的相关属性}
	ExpAttrmember ExpAttr; // {记录表达式的相关属性}
}attrmember;

typedef struct treenode
{
	treenode** child; // {指向子语法树节点指针}
	treenode* sibling; // {指向兄弟语法树结点指针}
	int lineno; // {记录源程序行号}
	int nodekind; // %索引值% {记录语法树节点类型}
	kindmember kind; // {记录语法树节点的具体类型，为共用结构体}
	int idnum; // {记录一个节点中的标志符个数}
	char** name; // {字符串数组，数组成员是节点中的标志符的名字}
	int table; // {指针数组，数组成员是节点的各个标志符在符号表中的入口} ？？？？？？
	int type_name; // {记录类型名，当节点为声明类型，且类型是由类型标志符表示时有效} ？？？？？？
	attrmember attr; // {记录语法树节点其他属性}
}treenode;
/*============================================================*/