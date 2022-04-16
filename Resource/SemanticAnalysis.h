#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include "treenode_var.h"

using namespace std;

//��ʶ�����Ƴ���
#define IDLENGTH 10
//��WrongID������ű���ʾ��ʶ�������Ƿ���ֻ���ڹ��̲�����ʶ�����ظ�����
#define WRONGID "*Unknown"

//AttributeIR �е� kind, ��"char"����
#define TYPEKIND '0'
#define VARKIND '1'
#define PROCKIND '2'

/*
Typekind�����͵�����, ��"char"������ǰ�����̶�����������ģ�����Ϊ���ͱ��ʼ��ʱ��ǰ���������˳����ˣ�
����ʹ��TypeList[INTTY - '0']��ʾINTTY���͵��ڲ���ʾ
        TypeList[CHARTY - '0']��ʾCHARTY���͵��ڲ���ʾ
        TypeList[BOOLTY - '0']��ʾBOOLTY���͵��ڲ���ʾ
���ԣ�����һ�£�������Ը��ĵĻ�������س������������������ʼ������һ�����
*/
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

//������������ļ��������Ϣ
//�յ�Ԫ�������Ϣ
#define EMPTYSPACE "/"
//Tab��
#define TAB_CSV ","
#define TAB_XLS "\t"
//���������Ϣ���·��
#define ERROR_FILE "../file/ErrorSemantic.txt"
//���ͱ�ͷ��ű����·����.csv�ļ���.xls�ļ�������EXCEL��Tab�ֱ�Ϊ","��"\t"��
#define TABLE_FILE_CSV "../file/SymbolTable.csv"
#define TABLE_FILE_XLS "../file/SymbolTable.xls"

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

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind, const int ProcLevel);//���ұ�ʶ���Ƿ�������Ӧ����

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind, const int ProcLevel);//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���

int FindProc(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const int ProcLevel);//FindProc()����body��Ѱ����ȷ�Ĺ��̱�ʶ��

bool Enter(const char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region, char kind, const int ProcLevel);//�ǼǱ�ʶ���������Ե����ű�

string TypeIRToString(char TypeKind);//����������ת�ַ���

string KindToString(char TypeKind);//Kind������ת�ַ���


/******�����﷨����Ҫ����غ���******/

void initialize(vector<struct TypeIR*>& TypeList); //��ʼ�����ֻ�������

//�����ͱ������Type�����Ƿ��Ѿ����ڴ��ڣ����ڵĻ��������ַ�����򷵻ؿ�; Typekind�Ǵ����ʶ�������ͣ��������ͻ��߼�¼���ͣ�
struct TypeIR* WhetherTypeDuplicate(vector<struct TypeIR*>& TypeList, struct TypeIR* Type, char Typekind);

struct TypeIR* WhetherResaultValid(struct TypeIR* TypeP1, struct TypeIR* TypeP2, Token* tok); //����TypeP1��TypeP2�Ƿ�ƥ�䣬�������������ƥ������ͻ���NULL(��ƥ��)

struct TypeIR* fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, Token* Arraytok, struct TypeIR* ArrayTy); //���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"

struct TypeIR* fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, Token* tok); //���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"

struct TypeIR* variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"

struct TypeIR* otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, Token*& tok); //���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"

struct TypeIR* factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"

struct TypeIR* otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, Token*& tok); //���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"

struct TypeIR* termParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Term"����ӦRD�е�"term����"

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec); //���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec); //���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"

struct TypeIR* expParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"

struct TypeIR* variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, Token* tok); //���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, Token* IDTok); //���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, SymbTable* VarSym); //���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, Token* IDtok, const int ValidTableCount);//���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region); //���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"

void fidMoreParsing(treenode* RD_ROOT, vector<Token*>& token); //���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"

void formListParsing(treenode* RD_ROOT, vector<Token*>& token); //���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"

struct ParamTable* paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"

struct ParamTable* paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"Param"����ӦRD�е�"param����"

struct ParamTable* paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"

struct ParamTable* paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount); //���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"

void varIDMoreParsing(treenode* RD_ROOT, vector<Token*>& token);//���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount);//���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"

void varIDListParsing(treenode* RD_ROOT, vector<Token*>& token);//���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount);//���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount);//���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"

void IDMoreParsing(treenode* RD_ROOT, vector<Token*>& token); //���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<Token*>& token); //���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"

void IDListParsing(treenode* RD_ROOT, vector<Token*>& token); //���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"��StrVec���������Ҫ��ID��

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<Token*> &token); //���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"

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

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList); //���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"

void programHeadParsing(treenode* RD_ROOT); //���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount); //�����﷨��,���ڵ�����Ϊ"Program"




void semantic_analysis(treenode* RD_ROOT); //�������


//2022_4_16