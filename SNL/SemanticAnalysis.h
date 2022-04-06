#pragma once
//AttributeIR �е� kind, ��"char"����
#define typeKind '0'
#define varKind '1'
#define procKind '2'

//Typekind�����͵�����, ��"char"����
#define intTy '0'
#define charTy '1'
#define arrayTy '2'
#define recordTy '3'
#define boolTy '4'

//AccessKind, ʹ��"char"����
#define dir '0'
#define indir '1'

struct fieldChain{
    char idname[10];//������
    char unitType;//���г�Ա�����ͣ�ʹ�ú궨�壬Typekind(intTy: '0', charTy: '1', arrayTy: '2', recordTy: '3', boolTy: '4')
    int offset;//�ڼ�¼�е�ƫ��
    struct fieldChain *next;
};

struct ParamTable{
    int entry;//ָ���βα�ʶ���ڷ��ű��е�λ��
    struct ParamTable *next;
};

struct TypeIR{
    int size;//������ռ�ռ��С
    char Typekind;//���г�Ա�����ͣ�ʹ�ú궨�壬Typekind(intTy: '0', charTy: '1', arrayTy: '2', recordTy: '3', boolTy: '4')
    union{
        struct{
            struct typeIR *indexTy;//����������Ч��ָ�������±�����
            struct typeIR *elemTy;//����������Ч��ָ������Ԫ������
        };
        fieldChain *body;//��¼�����е�����
    }More;
};


struct AttributeIR{
    struct TypeIR *idtype;//ָ���ʶ���������ڲ���ʾ������5��(intTy, charTy, arrayTy, recordTy, boolTy)
    char kind;//��ʶ�������࣬����3��(typeKind, varKind, procKind),��kind��ʹ�ú궨�壬typeKind:'0', varKind:'1', procKind:'2'
    union{
        struct{
            char access;//AccessKind, ʹ�ú궨�壬(dir '0', indir '1')
            int level;
            int off;
        }VarAttr;//������ʶ��������

        struct{
            int level;
            struct ParamTable *param;//������
            int code;
            int size;
        }ProcAttr;//��������ʶ��������
    }More; //��ʶ���Ĳ�ͬ�����в�ͬ������
};

typedef struct symbtable{
    char idname[10];
    struct AttributeIR attrIR;
    struct symbtable * next;
}SymbTable;//SNL���ű����ݽṹ����

void semantic_analysis(); //�������
