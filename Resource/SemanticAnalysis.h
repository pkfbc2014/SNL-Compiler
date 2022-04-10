#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream> 

using namespace std;

//��ʶ�����Ƴ���
#define IDLENGTH 10

//AttributeIR �е� kind, ��"char"����
#define TYPEKIND '0'
#define VARKIND '1'
#define PROCKIND '2'

//Typekind�����͵�����, ��"char"����
#define INTTY '0'
#define CHARTY '1'
#define ARRAYTY '2'
#define RECORDTY '3'
#define BOOLTY '4'

//AccessKind, ʹ��"char"����
#define DIR '0'
#define INDIR '1'

//����ƫ��initOff,�̶�Ϊ7
#define INITOFF 7

//������������ļ���·��
#define ERROR_FILE "C:\\Users\\86177\\Desktop\\����ԭ�����\\VS_test\\file\\error_file.txt"

//���������Ϣ
#define SemanticERROR1 "��ʶ���ظ���������"

struct fieldChain{
    char idname[IDLENGTH];//������
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


typedef struct attributeIR{
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
}AttributeIR;

typedef struct symbtable{
    char idname[IDLENGTH];
    AttributeIR attrIR;
}SymbTable;//SNL���ű����ݽṹ����

/***************����������****************/

/******�ļ����������ز���******/
void PrintFile(string message, string file_path);//�ַ�����ӡ����Ӧ�ļ���

/******���ű����******/
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &ValidTableCount);//����һ�����ű�������scopeջ

void DestroyTable(vector<bool>& exit_region, int &ValidTableCount);//�ϳ����µ�һ����Ч���ű�

bool SearchSingleTable(char *id, vector< vector<SymbTable> > scope, int level);//���ұ�ʶ���Ƿ�������Ӧ����

bool FindEntry(char *id, bool flag, vector< vector<SymbTable> > scope, vector<bool> exit_region);//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region);//�ǼǱ�ʶ���������Ե����ű�



void semantic_analysis(); //�������


