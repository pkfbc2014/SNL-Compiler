#pragma once

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
#define NAMETY '5'

//AccessKind, 使用"char"声明
#define DIR '0'
#define INDIR '1'

//初试偏移initOff,固定为7
#define INITOFF 7

//语义分析导出文件的路径
#define ERROR_FILE "C:\\Users\\86177\\Desktop\\编译原理课设\\VS_test\\file\\error_file.txt"

//语义错误信息
#define SemanticERROR1 "标识符重复声明错误"

struct fieldChain{
    char idname[IDLENGTH];//变量名
    char unitType;//域中成员的类型，使用宏定义，Typekind(intTy: '0', charTy: '1', arrayTy: '2', recordTy: '3', boolTy: '4')
    int offset;//在记录中的偏移
    struct fieldChain *next;
};

struct ParamTable{
    int entry;//指向形参标识符在符号表中的位置
    struct ParamTable *next;
};

struct TypeIR{
    int size;//类型所占空间大小
    char Typekind;//域中成员的类型，使用宏定义，Typekind(intTy: '0', charTy: '1', arrayTy: '2', recordTy: '3', boolTy: '4')
    union{
        struct{
            struct typeIR *indexTy;//数组类型有效，指向数组下标类型
            struct typeIR *elemTy;//数组类型有效，指向数组元素类型
        };
        fieldChain *body;//记录类型中的域链
    }More;
};


typedef struct attributeIR{
    struct TypeIR *idtype;//指向标识符的类型内部表示，共有基本5种(intTy, charTy, arrayTy, recordTy, boolTy)和自定义
    char kind;//标识符的种类，共有3种(typeKind, varKind, procKind),的kind，使用宏定义，typeKind:'0', varKind:'1', procKind:'2'
    union{
        struct{
            char access;//AccessKind, 使用宏定义，(dir '0', indir '1')
            int level;
            int off;
        }VarAttr;//变量标识符的属性

        struct{
            int level;
            struct ParamTable *param;//参数表
            int code;
            int size;
        }ProcAttr;//过程名标识符的属性
    }More; //标识符的不同类型有不同的属性
}AttributeIR;

typedef struct symbtable{
    char idname[IDLENGTH];
    AttributeIR attrIR;
}SymbTable;//SNL符号表数据结构声明



/***************函数声明区****************/

/******文件输入输出相关操作******/
void PrintFile(string message, string file_path);//字符串打印到相应文件中

/******符号表操作******/
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &ValidTableCount);//创建一个符号表，并插入scope栈

void DestroyTable(vector<bool>& exit_region, int &ValidTableCount);//废除最新的一个有效符号表

bool SearchSingleTable(char *id, vector< vector<SymbTable> > scope, int level);//查找标识符是否存在与对应表中

bool FindEntry(char *id, bool flag, vector< vector<SymbTable> > scope, vector<bool> exit_region);//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region);//登记标识符和其属性到符号表

/******解析语法树需要的相关函数******/

void initialize(vector<struct TypeIR>& TypeList); //初始化三种基本类型

void fieldVarMoreParsing(); //根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"

void fieldVarParsing(); //根节点名称为"FieldVar"，对应RD中的"fieldVar函数"

void variableParsing(); //根节点名称为"Variable"，对应RD中的"variable函数"

void multOpParsing(); //根节点名称为"MultOp"，对应RD中的"multOp函数"

void otherFactorParsing(); //根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"

void factorParsing(); //根节点名称为"Factor"，对应RD中的"factor函数"

void addOpParsing(); //根节点名称为"AddOp"，对应RD中的"addOp函数"

void otherTermParsing(); //根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"

void termParsing(); //根节点名称为"Term"，对应RD中的"term函数"

void actparamMoreParsing(); //根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"

void actparamListParsing(); //根节点名称为"ActParamList"，对应RD中的"actparamList函数"

void expParsing(); //根节点名称为"Exp"，对应RD中的"exp函数"

void variMoreParsing(); //根节点名称为"VariMore"，对应RD中的"variMore函数"

void callStmRestParsing(); //根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"

void assignmentRestParsing(); //根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"

void assCallParsing(); //根节点名称为"AssCall"，对应RD中的"assCall函数"

void returnStmParsing(); //根节点名称为"ReturnStm"，对应RD中的"returnStm函数"

void outputStmParsing(); //根节点名称为"OutputStm"，对应RD中的"outputStm函数"

void inputStmParsing(); //根节点名称为"InputStm"，对应RD中的"inputStm函数"

void loopStmParsing(); //根节点名称为""，对应RD中的"loopStm函数"********************************************

void conditionalStmParsing(); //根节点名称为""，对应RD中的"conditionalStm函数"********************************************

void stmMoreParsing(); //根节点名称为"StmMore"，对应RD中的"stmMore函数"

void stmParsing(); //根节点名称为"Stm"，对应RD中的"stm函数"

void stmListParsing(); //根节点名称为"StmList"，对应RD中的"stmList函数"

void fidMoreParsing(); //根节点名称为"FidMore"，对应RD中的"fidMore函数"

void formListParsing(); //根节点名称为"FormList"，对应RD中的"formList函数"

void paramMoreParsing(); //根节点名称为"ParamMore"，对应RD中的"paramMore函数"

void paramParsing(); //根节点名称为"Param"，对应RD中的"param函数"

void paramDecListParsing(); //根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"

void procBodyParsing(); //根节点名称为"ProcBody"，对应RD中的"procBody函数"

void procDecPartParsing(); //根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"

void paramListParsing(); //根节点名称为"ParamList"，对应RD中的"paramList函数"

void procDecParsing(); //根节点名称为"ProcDec"，对应RD中的"procDec函数"

void varIDMoreParsing(); //根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"

void varDecMoreParsing(); //根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"

void varIDListParsing(); //根节点名称为"VarIDList"，对应RD中的"varIDList函数"

void varDecListParsing(); //根节点名称为"VarDecList"，对应RD中的"varDecList函数"

void varDecParsing(); //根节点名称为"VarDec"，对应RD中的"varDec函数"

void IDMoreParsing(); //根节点名称为"IDMore"，对应RD中的"IDMore函数"

void fieldDecMoreParsing(); //根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"

void IDListParsing(); //根节点名称为"IDList"，对应RD中的"IDList函数"

void fieldDecListParsing(); //根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"

void recTypeParsing(); //根节点名称为"RecType"，对应RD中的"recType函数"

void arrayTypeParsing(); //根节点名称为"ArrayType"，对应RD中的"arrayType函数"

void structureTypeParsing(); //根节点名称为"StructureType"，对应RD中的"structureType函数"

void baseTypeParsing(); //根节点名称为"BaseType"，对应RD中的"baseType函数"

void typeDecMoreParsing(); //根节点名称为"TypeDecMore"，对应RD中的"typeDecMore函数"

void typeDefParsing(); //根节点名称为"TypeDef"，对应RD中的"typeDef函数"

void typeIDParsing(); //根节点名称为"TypeID"，对应RD中的"typeID函数"

void typeDecListParsing(); //根节点名称为"TypeDecList"，对应RD中的"typeDecList函数"

void typeDecParsing(); //根节点名称为"TypeDec"，对应RD中的"typeDec函数"

void procDecpartParsing(); //根节点名称为"ProcDecPart"，对应RD中的"procDecpart函数"

void varDecPartParsing(); //根节点名称为"VarDecPart"，对应RD中的"varDecPart函数"

void typeDecPartParsing(); //根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"

void programBodyParsing(); //根节点名称为"ProgramBody"，对应RD中的"programBody函数"

void declarePartParsing(); //根节点名称为"DeclarePart"，对应RD中的"declarePart函数"

void programHeadParsing(); //根节点名称为"ProgramHead"，对应RD中的"programHead函数"

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList); //解析语法树,根节点名称为"Program"



void semantic_analysis(); //语义分析


