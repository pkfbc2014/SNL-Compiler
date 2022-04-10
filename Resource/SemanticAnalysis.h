#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream> 

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
#define ARRAYTY '2'
#define RECORDTY '3'
#define BOOLTY '4'

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
    struct TypeIR *idtype;//指向标识符的类型内部表示，共有5种(intTy, charTy, arrayTy, recordTy, boolTy)
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



void semantic_analysis(); //语义分析


