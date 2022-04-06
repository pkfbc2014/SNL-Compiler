#pragma once
//AttributeIR 中的 kind, 用"char"声明
#define typeKind '0'
#define varKind '1'
#define procKind '2'

//Typekind，类型的种类, 用"char"声明
#define intTy '0'
#define charTy '1'
#define arrayTy '2'
#define recordTy '3'
#define boolTy '4'

//AccessKind, 使用"char"声明
#define dir '0'
#define indir '1'

struct fieldChain{
    char idname[10];//变量名
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


struct AttributeIR{
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
};

typedef struct symbtable{
    char idname[10];
    struct AttributeIR attrIR;
    struct symbtable * next;
}SymbTable;//SNL符号表数据结构声明

void semantic_analysis(); //语义分析
