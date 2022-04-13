#pragma once
#define _CRT_SECURE_NO_WARNINGS

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
//#define NAMETY '5'

//AccessKind, ʹ��"char"����
#define DIR '0'
#define INDIR '1'

//����ƫ��initOff,�̶�Ϊ7
#define INITOFF 7

//������������ļ���·��
#define ERROR_FILE "C:\\Users\\86177\\Desktop\\����ԭ�����\\YCompiler_TEST\\file\\error_file_semantic.txt"
#define TABLE_FILE "C:\\Users\\86177\\Desktop\\����ԭ�����\\YCompiler_TEST\\file\\SymbolTable.txt"

typedef struct FieldChain {
    char idname[IDLENGTH];//������
    struct TypeIR* unitType;//ָ���ʶ���������ڲ���ʾ�����л���5��(intTy, charTy, arrayTy, recordTy, boolTy)���Զ���
    int offset;//�ڼ�¼�е�ƫ��
    struct FieldChain* next;
}fieldChain;

struct ParamTable {
    int entry;//ָ���βα�ʶ���ڷ��ű��е�λ��
    struct ParamTable* next;
};

struct TypeIR {
    unsigned int serial;//�����Ա��е�λ�����
    int size;//������ռ�ռ��С
    char Typekind;//���г�Ա�����ͣ�ʹ�ú궨�壬Typekind(intTy: '0', charTy: '1', boolTy: '2', recordTy: '3', arrayTy: '4')
    union {
        struct {
            struct TypeIR* indexTy;//����������Ч��ָ�������±�����
            struct TypeIR* elemTy;//����������Ч��ָ������Ԫ������
            unsigned int Low;
        };
        fieldChain* body;//��¼�����е�����
    }More;
};


typedef struct attributeIR {
    struct TypeIR* idtype;//ָ���ʶ���������ڲ���ʾ�����л���5��(intTy, charTy, arrayTy, recordTy, boolTy)���Զ���
    char kind;//��ʶ�������࣬����3��(typeKind, varKind, procKind),��kind��ʹ�ú궨�壬typeKind:'0', varKind:'1', procKind:'2'
    union {
        struct {
            char access;//AccessKind, ʹ�ú궨�壬(dir '0', indir '1')
            int level;
            int off;
        }VarAttr;//������ʶ��������

        struct {
            int level;
            struct ParamTable* param;//������
            int code;
            int size;
        }ProcAttr;//��������ʶ��������
    }More; //��ʶ���Ĳ�ͬ�����в�ͬ������
}AttributeIR;

typedef struct symbtable {
    char idname[IDLENGTH];
    AttributeIR attrIR;
}SymbTable;//SNL���ű����ݽṹ����



/***************����������****************/

/******�ļ����������ز���******/

void InitFile(string file_path);//��ʼ����Ӧ·���µ��ļ�������ļ���

void PrintFile(string message, string file_path);//�ַ�����ӡ����Ӧ�ļ���

/******���ű����******/
void CreateTable(vector< vector<SymbTable> >& scope, vector<bool>& exit_region, int& ValidTableCount);//����һ�����ű�������scopeջ

void DestroyTable(vector<bool>& exit_region, int& ValidTableCount);//�ϳ����µ�һ����Ч���ű�

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind);//���ұ�ʶ���Ƿ�������Ӧ����

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind);//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region);//�ǼǱ�ʶ���������Ե����ű�

string TypeIRToString(char TypeKind);//����������ת�ַ���

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList);//��ӡ���ͱ�ͷ��ű��ļ�

/******���������Ϣ�����غ���******/
/***��ʶ��***/
void Error_IdentifierDuplicateDec(int line, string sem);// 1.��ʶ�����ظ����壬��Ӧ�����������(1)

void Error_IdentifierUndeclared(int line, string sem);// 2.�������ı�ʶ������Ӧ�����������(2)

/***����***/
void Error_ArraySubscriptLessThanZero(int line, string sem);// 1. ��������ʱ�±�С��0

void Error_ArraySubscriptOutBounds(int line, string sem1, string sem2);// 2. �����±�Խ�磬��Ӧ�����������(4)

/******�����﷨����Ҫ����غ���******/

void initialize(vector<struct TypeIR*>& TypeList); //��ʼ�����ֻ�������

void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Term"����ӦRD�е�"term����"

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Param"����ӦRD�е�"param����"

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"

void IDMoreParsing(treenode* RD_ROOT, vector<string>& StrVec); //���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"

void IDListParsing(treenode* RD_ROOT, vector<string>& StrVec); //���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"��StrVec���������Ҫ��ID��

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"

AttributeIR* structureTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"StructureType"����ӦRD�е�"structureType����"

AttributeIR* baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"BaseType"����ӦRD�е�"baseType����"

void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeDecMore"����ӦRD�е�"typeDecMore����"

AttributeIR* typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeDef"����ӦRD�е�"typeDef����"

//void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeID"����ӦRD�е�"typeID����"

void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeDecList"����ӦRD�е�"typeDecList����"
//****
void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeDec"����ӦRD�е�"typeDec����"
//****

void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecpart����"

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"VarDecPart"����ӦRD�е�"varDecPart����"

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //�����﷨��,���ڵ�����Ϊ"Program"




void semantic_analysis(treenode* RD_ROOT); //�������
//*****

