#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include "treenode_var.h"

using namespace std;

//标识符名称长度
#define IDLENGTH 10

//AttributeIR 中的 kind, 用"char"声明
#define TYPEKIND '0'
#define VARKIND '1'
#define PROCKIND '2'

//Typekind，类型的种类, 用"char"声明
#define INTTY '0'
#define CHARTY '1'
#define BOOLTY '2'
#define RECORDTY '3'
#define ARRAYTY '4'
//#define NAMETY '5'

//AccessKind, 使用"char"声明
#define DIR '0'
#define INDIR '1'

//初试偏移initOff,固定为7
#define INITOFF 7

//语义分析导出文件的路径
#define ERROR_FILE "C:\\Users\\86177\\Desktop\\编译原理课设\\YCompiler_TEST\\file\\error_file_semantic.txt"
#define TABLE_FILE "C:\\Users\\86177\\Desktop\\编译原理课设\\YCompiler_TEST\\file\\SymbolTable.txt"

typedef struct FieldChain {
    char idname[IDLENGTH];//变量名
    struct TypeIR* unitType;//指向标识符的类型内部表示，共有基本5种(intTy, charTy, arrayTy, recordTy, boolTy)和自定义
    int offset;//在记录中的偏移
    struct FieldChain* next;
}fieldChain;

struct ParamTable {
    int entry;//指向形参标识符在符号表中的位置
    struct ParamTable* next;
};

struct TypeIR {
    unsigned int serial;//在属性表中的位置序号
    int size;//类型所占空间大小
    char Typekind;//域中成员的类型，使用宏定义，Typekind(intTy: '0', charTy: '1', boolTy: '2', recordTy: '3', arrayTy: '4')
    union {
        struct {
            struct TypeIR* indexTy;//数组类型有效，指向数组下标类型
            struct TypeIR* elemTy;//数组类型有效，指向数组元素类型
            unsigned int Low;
        };
        fieldChain* body;//记录类型中的域链
    }More;
};


typedef struct attributeIR {
    struct TypeIR* idtype;//指向标识符的类型内部表示，共有基本5种(intTy, charTy, arrayTy, recordTy, boolTy)和自定义
    char kind;//标识符的种类，共有3种(typeKind, varKind, procKind),的kind，使用宏定义，typeKind:'0', varKind:'1', procKind:'2'
    union {
        struct {
            char access;//AccessKind, 使用宏定义，(dir '0', indir '1')
            int level;
            int off;
        }VarAttr;//变量标识符的属性

        struct {
            int level;
            struct ParamTable* param;//参数表
            int code;
            int size;
        }ProcAttr;//过程名标识符的属性
    }More; //标识符的不同类型有不同的属性
}AttributeIR;

typedef struct symbtable {
    char idname[IDLENGTH];
    AttributeIR attrIR;
}SymbTable;//SNL符号表数据结构声明



/***************函数声明区****************/

/******文件输入输出相关操作******/

void InitFile(string file_path);//初始化对应路径下的文件（清空文件）

void PrintFile(string message, string file_path);//字符串打印到相应文件中

/******符号表操作******/
void CreateTable(vector< vector<SymbTable> >& scope, vector<bool>& exit_region, int& ValidTableCount);//创建一个符号表，并插入scope栈

void DestroyTable(vector<bool>& exit_region, int& ValidTableCount);//废除最新的一个有效符号表

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind);//查找标识符是否存在与对应表中

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind);//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region);//登记标识符和其属性到符号表

string TypeIRToString(char TypeKind);//类型种类编号转字符串

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList);//打印类型表和符号表到文件

/******语义错误信息输出相关函数******/
/***标识符***/
void Error_IdentifierDuplicateDec(int line, string sem);// 1.标识符的重复定义，对应书中语义错误(1)

void Error_IdentifierUndeclared(int line, string sem);// 2.无声明的标识符，对应书中语义错误(2)

/***数组***/
void Error_ArraySubscriptLessThanZero(int line, string sem);// 1. 数组声明时下标小于0

void Error_ArraySubscriptOutBounds(int line, string sem1, string sem2);// 2. 数组下标越界，对应书中语义错误(4)

/******解析语法树需要的相关函数******/

void initialize(vector<struct TypeIR*>& TypeList); //初始化三种基本类型

void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"FieldVar"，对应RD中的"fieldVar函数"

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Variable"，对应RD中的"variable函数"

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"MultOp"，对应RD中的"multOp函数"

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Factor"，对应RD中的"factor函数"

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"AddOp"，对应RD中的"addOp函数"

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Term"，对应RD中的"term函数"

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ActParamList"，对应RD中的"actparamList函数"

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Exp"，对应RD中的"exp函数"

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"VariMore"，对应RD中的"variMore函数"

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"AssCall"，对应RD中的"assCall函数"

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ReturnStm"，对应RD中的"returnStm函数"

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"OutputStm"，对应RD中的"outputStm函数"

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"InputStm"，对应RD中的"inputStm函数"

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为""，对应RD中的"loopStm函数"********************************************

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为""，对应RD中的"conditionalStm函数"********************************************

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"StmMore"，对应RD中的"stmMore函数"

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Stm"，对应RD中的"stm函数"

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //根节点名称为"StmList"，对应RD中的"stmList函数"

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"FidMore"，对应RD中的"fidMore函数"

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"FormList"，对应RD中的"formList函数"

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ParamMore"，对应RD中的"paramMore函数"

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"Param"，对应RD中的"param函数"

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ProcBody"，对应RD中的"procBody函数"

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"ParamList"，对应RD中的"paramList函数"

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //根节点名称为"ProcDec"，对应RD中的"procDec函数"

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"VarIDList"，对应RD中的"varIDList函数"

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"VarDecList"，对应RD中的"varDecList函数"

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //根节点名称为"VarDec"，对应RD中的"varDec函数"

void IDMoreParsing(treenode* RD_ROOT, vector<string>& StrVec); //根节点名称为"IDMore"，对应RD中的"IDMore函数"

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"

void IDListParsing(treenode* RD_ROOT, vector<string>& StrVec); //根节点名称为"IDList"，对应RD中的"IDList函数"，StrVec是最后所需要的ID表

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //根节点名称为"RecType"，对应RD中的"recType函数"

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //根节点名称为"ArrayType"，对应RD中的"arrayType函数"

AttributeIR* structureTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"StructureType"，对应RD中的"structureType函数"

AttributeIR* baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //根节点名称为"BaseType"，对应RD中的"baseType函数"

void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"TypeDecMore"，对应RD中的"typeDecMore函数"

AttributeIR* typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"TypeDef"，对应RD中的"typeDef函数"

//void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"TypeID"，对应RD中的"typeID函数"

void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"TypeDecList"，对应RD中的"typeDecList函数"
//****
void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //根节点名称为"TypeDec"，对应RD中的"typeDec函数"
//****

void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //根节点名称为"ProcDecPart"，对应RD中的"procDecpart函数"

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //根节点名称为"VarDecPart"，对应RD中的"varDecPart函数"

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //根节点名称为"ProgramBody"，对应RD中的"programBody函数"

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //根节点名称为"DeclarePart"，对应RD中的"declarePart函数"

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //根节点名称为"ProgramHead"，对应RD中的"programHead函数"

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //解析语法树,根节点名称为"Program"




void semantic_analysis(treenode* RD_ROOT); //语义分析
//*****

