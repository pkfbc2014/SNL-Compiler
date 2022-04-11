#pragma once

#include <vector>
#include <string>
#include "treenode_var.h"

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
#define BOOLTY '2'
#define RECORDTY '3'
#define ARRAYTY '4'
#define NAMETY '5'

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
    struct TypeIR *idtype;//ָ���ʶ���������ڲ���ʾ�����л���5��(intTy, charTy, arrayTy, recordTy, boolTy)���Զ���
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

/******�����﷨����Ҫ����غ���******/

void initialize(vector<struct TypeIR>& TypeList); //��ʼ�����ֻ�������

void fieldVarMoreParsing(); //���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"

void fieldVarParsing(); //���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"

void variableParsing(); //���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"

void multOpParsing(); //���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"

void otherFactorParsing(); //���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"

void factorParsing(); //���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"

void addOpParsing(); //���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"

void otherTermParsing(); //���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"

void termParsing(); //���ڵ�����Ϊ"Term"����ӦRD�е�"term����"

void actparamMoreParsing(); //���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"

void actparamListParsing(); //���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"

void expParsing(); //���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"

void variMoreParsing(); //���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"

void callStmRestParsing(); //���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"

void assignmentRestParsing(); //���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"

void assCallParsing(); //���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"

void returnStmParsing(); //���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"

void outputStmParsing(); //���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"

void inputStmParsing(); //���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"

void loopStmParsing(); //���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************

void conditionalStmParsing(); //���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************

void stmMoreParsing(); //���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"

void stmParsing(); //���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"

void stmListParsing(); //���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"

void fidMoreParsing(); //���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"

void formListParsing(); //���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"

void paramMoreParsing(); //���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"

void paramParsing(); //���ڵ�����Ϊ"Param"����ӦRD�е�"param����"

void paramDecListParsing(); //���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"

void procBodyParsing(); //���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"

void procDecPartParsing(); //���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"

void paramListParsing(); //���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"

void procDecParsing(); //���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"

void varIDMoreParsing(); //���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"

void varDecMoreParsing(); //���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"

void varIDListParsing(); //���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"

void varDecListParsing(); //���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"

void varDecParsing(); //���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"

void IDMoreParsing(); //���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"

void fieldDecMoreParsing(); //���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"

void IDListParsing(); //���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"

void fieldDecListParsing(); //���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"

void recTypeParsing(); //���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"

void arrayTypeParsing(); //���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"

void structureTypeParsing(); //���ڵ�����Ϊ"StructureType"����ӦRD�е�"structureType����"

void baseTypeParsing(); //���ڵ�����Ϊ"BaseType"����ӦRD�е�"baseType����"

void typeDecMoreParsing(); //���ڵ�����Ϊ"TypeDecMore"����ӦRD�е�"typeDecMore����"

void typeDefParsing(); //���ڵ�����Ϊ"TypeDef"����ӦRD�е�"typeDef����"

void typeIDParsing(); //���ڵ�����Ϊ"TypeID"����ӦRD�е�"typeID����"

void typeDecListParsing(); //���ڵ�����Ϊ"TypeDecList"����ӦRD�е�"typeDecList����"

void typeDecParsing(); //���ڵ�����Ϊ"TypeDec"����ӦRD�е�"typeDec����"

void procDecpartParsing(); //���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecpart����"

void varDecPartParsing(); //���ڵ�����Ϊ"VarDecPart"����ӦRD�е�"varDecPart����"

void typeDecPartParsing(); //���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"

void programBodyParsing(); //���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"

void declarePartParsing(); //���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"

void programHeadParsing(); //���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList); //�����﷨��,���ڵ�����Ϊ"Program"



void semantic_analysis(); //�������


