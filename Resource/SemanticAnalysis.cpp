#include"SemanticAnalysis.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "treenode_var.h"
/*
    ���ű���������ת��,��exitջ��ʾ��Ӧ�ֲ������Ƿ��Ѿ��˳�
*/

/******�ļ����������ز���******/
void InitFile(string file_path)//��ʼ����Ӧ·���µ��ļ�������ļ���
{
    ofstream output(file_path);
    output <<"";
    output.close();
    //exit(0);
}

void PrintFile(string message, string file_path)//�ַ�����ӡ����Ӧ�ļ���
{
    fstream foutput;
    //׷��д��,��ԭ�������ϼ���ios::app
    foutput.open(file_path, ios::out | ios::app);
    foutput << message + "\n";
    foutput.close();
    //exit(0);
}

/******���ű����******/
void CreateTable(vector< vector<SymbTable> >& scope, vector<bool>& exit_region, int& ValidTableCount) {//����һ�����ű�������scopeջ
    ValidTableCount++;
    vector<SymbTable> temp_Sym;//�����ձ�
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//��ʾδ�˳��ֲ�����
    return;
}

void DestroyTable(vector<bool>& exit_region, int& ValidTableCount) {//�ϳ����µ�һ����Ч���ű�
    ValidTableCount--;
    if (ValidTableCount < 0) {//������߼�©�����ܻᵼ���������
        cout << "DestroyTable_ERROR (ValidTableCount<0)\n";
        exit(0);
    }
    for (int ix = exit_region.size() - 1; ix >= 0; ix--) {
        if (exit_region[ix] == false) {//�ϳ����ű�����������ĵ�һ��δ�˳��ľֲ�����
            exit_region[ix] = true;
            break;
        }
    }
}

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind) {//���ұ�ʶ���Ƿ�������Ӧ����,kind��ʾ�����ʶ��������('*'��ʾȫ��ƥ��)
    int position = level - 1;
    SymbTable* temp = NULL;
    for (int ix = scope[position].size() - 1; ix >= 0; ix--) {
        if (0 == strcmp(scope[position][ix].idname, id) && (scope[position][ix].attrIR.kind == kind || kind == '*')) {
            temp = &(scope[position][ix]);
            return temp;//���������ʶ���ĵ�ַ
        }
    }
    return temp;//NULL: �����ڣ� �ǿ�: ��Ӧ��ʶ����ַ��Ϣ
}

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind) {//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���
    int level = exit_region.size();//��ʾ�ڼ��㣬level == 0�൱��scopeջ����Ԫ�ء�ʹ�� n = level-1 ����scope[n][x]����exit_region[n]��
    SymbTable* temp = NULL;
    //���ڵ�ǰ���ű��в���
    while (level > 0) {
        if (exit_region[level - 1] == false) {//�ҵ���һ��δ�˳��ľֲ������Ĳ���
            break;
        }
        else {
            level--;
        }
    }
    if (level <= 0) {//������һ����Ч�ľֲ�����
        return NULL;
    }
    temp = SearchSingleTable(id, scope, level, kind);//���ҵ�ǰ���ű�
    if (flag == true && temp == NULL) {//���flag == true���ҵ�ǰ���ű���δ���ҵ������ʶ��id���������������scopeջ
        while (level > 0) {
            if (exit_region[level - 1] == false) {//�ҵ�δ�˳��ľֲ������Ĳ���
                temp = SearchSingleTable(id, scope, level, kind);//���Ҵ˷��ű�
                if (temp != NULL) {
                    break;
                }
            }
            level--;
        }
    }
    return temp;//NULL: �����ڣ� �ǿ�: ��Ӧ��ʶ����ַ��Ϣ
}

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region) {//�ǼǱ�ʶ���������Ե����ű�
    bool present = false;//��¼�Ǽ��Ƿ�ɹ�
    if (FindEntry(id, false, scope, exit_region, '*') == NULL) {
        int level = exit_region.size();//��ʾ�ڼ��㣬level == 0�൱��scopeջ����Ԫ�ء�ʹ�� n = level-1 ����scope[n][x]����exit_region[n]
        while (level > 0) {
            if (exit_region[level - 1] == false) {//�ҵ���һ��δ�˳��ľֲ������Ĳ���
                break;
            }
            else {
                level--;
            }
        }
        if (level <= 0) {//������߼�©�����ܻᵼ���������
            cout << "Enter_ERROR (level<=0)\n";
            exit(0);
        }
        SymbTable temp;
        for (int ix = 0; ix < IDLENGTH; ix++) {//��ֵ��ʶ������
            temp.idname[ix] = id[ix];
        }
        temp.attrIR = Attrib;
        scope[level - 1].push_back(temp);
        present = true;
    }
    else {
        present = false;
    }
    return present;//true:�ɹ��� false:���ɹ�
}

string TypeIRToString(char TypeKind) {//����������ת�ַ���
    if (TypeKind == '0') { return "Int   "; }
    else if (TypeKind == '1') { return "Char  "; }
    else if (TypeKind == '2') { return "Bool  "; }
    else if (TypeKind == '3') { return "Record"; }
    else if (TypeKind == '4') { return "Array "; }
    return "";
}

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList) {//��ӡ���ͱ�ͷ��ű��ļ�
    const int TypeList_size = TypeList.size();
    string message = "";
    message += "���ͱ�\n���\tSize\tkind\tindexTy\telemTy\tLow\tbody{�������,������,ƫ��}\n";
    for (int ix = 0; ix < TypeList_size; ix++) {//��ӡ���ͱ�
        message += to_string(ix + 1);//���
        message += "\t";
        message += to_string(TypeList[ix]->size);
        message += "\t";
        message += TypeIRToString(TypeList[ix]->Typekind);
        message += "\t";
        if (TypeList[ix]->Typekind == ARRAYTY) {
            message += "   ";  message += to_string(TypeList[ix]->More.indexTy->serial + 1);//indexTy�����
            message += "\t";
            message += "   ";  message += to_string(TypeList[ix]->More.elemTy->serial + 1);//elemTy�����
            message += "\t";
            message += "  ";  message += to_string(TypeList[ix]->More.Low);//indexTy�����
            message += "\t";
            message += " --- ";//Record_body
            message += "\t";
        }
        else if (TypeList[ix]->Typekind == RECORDTY) {
            message += " ---- ";//Array_indexTy
            message += "\t";
            message += " ---- ";//Array_elemTy
            message += "\t";
            message += " --- ";//Array_Low
            message += "\t";
            FieldChain* head = TypeList[ix]->More.body;
            while (head != NULL) {
                message += "{";
                message += to_string(head->unitType->serial + 1);
                message += ", ";
                message += head->idname;
                message += ", ";
                message += to_string(head->offset);
                message += "}  ";
                head = head->next;
            }
        }
        else {//basetype: int,char,bool
            message += " ---- ";//Array_indexTy
            message += "\t";
            message += " ---- ";//Array_elemTy
            message += "\t";
            message += " --- ";//Array_Low
            message += "\t";
            message += " --- ";//Record_body
            message += "\t";
        }
        message += "\n";
    }
    //�������д��ӡscopeջ�Ĵ���*********************************************

    PrintFile(message, TABLE_FILE);
}

/******���������Ϣ�����غ���******/
/***��ʶ��***/
void Error_IdentifierDuplicateDec(int line, string sem) {// 1.��ʶ�����ظ����壬��Ӧ�����������(1)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�ظ�����\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {// 2.�������ı�ʶ������Ӧ�����������(2)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"δ������ʹ��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***����***/
void Error_ArraySubscriptLessThanZero(int line, string sem) {// 1. ��������ʱ�±�С��0����������������ʷ�������������������ʱ�±�С��0��
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   �����±�\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�Ƿ���С��0��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}
void Error_ArraySubscriptOutBounds(int line, string sem1, string sem2) {// 2. �����±�Խ�磬��Ӧ�����������(4)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   �����±�Խ��(\"";
    ErrorMessage += sem1;
    ErrorMessage += "\" > \"";
    ErrorMessage += sem2;
    ErrorMessage += "\")\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/******�����﷨����Ҫ����غ���******/

void initialize(vector<struct TypeIR*>& TypeList) {//��ʼ�����ֻ�������
    //��ʼ����������
    struct TypeIR* int_temp = new (struct TypeIR);
    int_temp->size = 2;
    int_temp->Typekind = INTTY;
    int_temp->serial = TypeList.size();
    TypeList.push_back(int_temp);

    //��ʼ���ַ�����
    struct TypeIR* char_temp = new (struct TypeIR);
    char_temp->size = 1;
    char_temp->Typekind = CHARTY;
    char_temp->serial = TypeList.size();
    TypeList.push_back(char_temp);

    //��ʼ����������
    struct TypeIR* bool_temp = new (struct TypeIR);
    bool_temp->size = 1;
    bool_temp->Typekind = BOOLTY;
    bool_temp->serial = TypeList.size();
    TypeList.push_back(bool_temp);

    return;
}


void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"
    if (RD_ROOT == NULL) { return; }
}

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"
    if (RD_ROOT == NULL) { return; }
}

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"
    if (RD_ROOT == NULL) { return; }
}

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"
    if (RD_ROOT == NULL) { return; }
}

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"
    if (RD_ROOT == NULL) { return; }
}

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"
    if (RD_ROOT == NULL) { return; }
}

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"
    if (RD_ROOT == NULL) { return; }
}

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"
    if (RD_ROOT == NULL) { return; }
}

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Term"����ӦRD�е�"term����"
    if (RD_ROOT == NULL) { return; }
}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"
    if (RD_ROOT == NULL) { return; }
}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"
    if (RD_ROOT == NULL) { return; }
}

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"
    if (RD_ROOT == NULL) { return; }
}

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"
    if (RD_ROOT == NULL) { return; }
}

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"
    if (RD_ROOT == NULL) { return; }
}

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"
    if (RD_ROOT == NULL) { return; }
}

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"
    if (RD_ROOT == NULL) { return; }
}

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"
    if (RD_ROOT == NULL) { return; }
}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"
    if (RD_ROOT == NULL) { return; }
}

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"
    if (RD_ROOT == NULL) { return; }
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************
    if (RD_ROOT == NULL) { return; }
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************
    if (RD_ROOT == NULL) { return; }
}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"
    if (RD_ROOT == NULL) { return; }
}

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"
    if (RD_ROOT == NULL) { return; }
}

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"
    if (RD_ROOT == NULL) { return; }
}

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"
    if (RD_ROOT == NULL) { return; }
}

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"
    if (RD_ROOT == NULL) { return; }
}

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"
    if (RD_ROOT == NULL) { return; }
}

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Param"����ӦRD�е�"param����"
    if (RD_ROOT == NULL) { return; }
}

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"
    if (RD_ROOT == NULL) { return; }
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"
    if (RD_ROOT == NULL) { return; }
}

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"
    if (RD_ROOT == NULL) { return; }
}

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"
    if (RD_ROOT == NULL) { return; }
}

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"
    if (RD_ROOT == NULL) { return; }
}

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"
    if (RD_ROOT == NULL) { return; }
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"
    if (RD_ROOT == NULL) { return; }
}

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"
    if (RD_ROOT == NULL) { return; }
}

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"
    if (RD_ROOT == NULL) { return; }
}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"
    if (RD_ROOT == NULL) { return; }
}


void IDMoreParsing(treenode* RD_ROOT, vector<string>& StrVec) {//���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, �޲���
    //RD_ROOT->child[1]: IDList()
    IDListParsing(RD_ROOT->child[1], StrVec);
    return;
}

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]:fieldDecList()
    return fieldDecListParsing(RD_ROOT->child[0], TypeList);
}

void IDListParsing(treenode* RD_ROOT, vector<string>& StrVec) {//���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"��StrVec���������Ҫ��ID��
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    StrVec.push_back(RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: IDMore()
    IDMoreParsing(RD_ROOT->child[1], StrVec);
    return;
}

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"
    if (RD_ROOT == NULL) { return NULL; }
    fieldChain* temp = NULL;
    //RD_ROOT->child[0]: BaseType �� ArrayType,����unitType
    if (RD_ROOT->child[0]->child[0]->token->Lex == INTEGER || RD_ROOT->child[0]->child[0]->token->Lex == CHAR) {
        temp = new fieldChain;
        if (RD_ROOT->child[0]->child[0]->token->Lex == INTEGER) {
            temp->unitType = TypeList[INTTY - '0'];//ָ��INTGER����
        }
        else {//RD_ROOT->child[0]->child[0]->token->Lex == CHAR
            temp->unitType = TypeList[CHARTY - '0'];//ָ��CHAR����
        }
    }
    else if (RD_ROOT->child[0]->child[0]->token->Lex == ARRAY) {
        temp = new fieldChain;
        AttributeIR* tempAttr = arrayTypeParsing(RD_ROOT->child[0]->child[0], TypeList);
        if (tempAttr == NULL) {
            delete tempAttr;
            delete temp;
            return NULL;
        }
        else {
            temp->unitType = tempAttr->idtype;
            delete tempAttr;
        }
    }
    else {
        return NULL;
    }
    //RD_ROOT->child[1]:IDList(),����idname
    vector<string> StrVec;
    IDListParsing(RD_ROOT->child[1], StrVec);//�õ�IDList�е�����ID��������StrVec
    if (StrVec.size() == 0) {//ID����Ϊ0
        delete temp;
        return NULL;
    }
    else {
        strcpy(temp->idname, StrVec[0].c_str());
        fieldChain* rear = temp;
        for (int ix = 1; ix < StrVec.size(); ix++) {//������ID������ΪfieldChain������������
            fieldChain* tempID = new fieldChain;
            tempID->unitType = temp->unitType;//����ID������ͬ������
            strcpy(tempID->idname, StrVec[ix].c_str());
            rear->next = tempID;
            rear = tempID;
        }
        //RD_ROOT->child[2]:SEMI���޲���
        //RD_ROOT->child[3]:fieldDecMore()
        rear->next = fieldDecMoreParsing(RD_ROOT->child[3], TypeList);
    }
    return temp;
}

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START �½�һ��TypeIR��������TypeList------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = RECORDTY;//��¼����Typeind
    //RD_ROOT->child[0]: RECORD, �޲���
    //RD_ROOT->child[1]: fieldDecList()
    fieldChain* body = fieldDecListParsing(RD_ROOT->child[1], TypeList);
    //RD_ROOT->child[2]: END, �޲���
    if (body == NULL) {
        delete temp;
        return NULL;
    }
    tempTypeIR->More.body = body;//��¼����body
    int size = 0;
    int offset = 0;
    while (body != NULL) {
        size += body->unitType->size;
        body->offset = offset;
        offset = size;
        body = body->next;
    }
    tempTypeIR->size = size;//��¼����size
    tempTypeIR->serial = TypeList.size();

    TypeList.push_back(tempTypeIR);
    //END �½�һ��TypeIR��������TypeList------------------------------------------------------------------------------------
    temp->idtype = TypeList[TypeList.size() - 1];
    return temp;
}

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START �½�һ��TypeIR��������TypeList------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = ARRAYTY;//��������Typeind
    tempTypeIR->More.indexTy = TypeList[INTTY - '0'];//�����±�����indexTYһ������������
    //RD_ROOT->child[0]: ARRAY, �޲���
    //RD_ROOT->child[1]: LMIDPAREN('['), �޲���
    //RD_ROOT->child[2]:INTC
    string str1 = RD_ROOT->child[2]->token->Sem;
    auto str1_n = std::strtol(str1.data(), nullptr, 10);//�ַ���ת ����Ϊ10 ������
    if (str1_n < 0) {
        Error_ArraySubscriptLessThanZero(RD_ROOT->child[2]->token->Lineshow, RD_ROOT->child[2]->token->Sem);//��������ʱ�±�С��0
        delete temp;
        return NULL;
    }
    else {
        tempTypeIR->More.Low = str1_n;//��������Low
    }
    //RD_ROOT->child[3]: UNDERANGE(".."), �޲���
    //RD_ROOT->child[4]: ���ں��漸�в���
    //RD_ROOT->child[5]: RMIDPAREN(']'), �޲���
    //RD_ROOT->child[6]: OF, �޲���
    //RD_ROOT->child[7]: baseType()
    if (RD_ROOT->child[7]->child[0]->token->Lex == INTEGER) {//��������elemTY
        tempTypeIR->More.elemTy = TypeList[INTTY - '0'];
    }
    else if (RD_ROOT->child[7]->child[0]->token->Lex == CHAR) {
        tempTypeIR->More.elemTy = TypeList[CHARTY - '0'];
    }
    //RD_ROOT->child[4]:INTCstring str2 = RD_ROOT->child[2]->token->Sem;
    string str2 = RD_ROOT->child[4]->token->Sem;
    auto str2_n = std::strtol(str2.data(), nullptr, 10);//�ַ���ת ����Ϊ10 ������
    if (str2_n < 0) {
        Error_ArraySubscriptLessThanZero(RD_ROOT->child[4]->token->Lineshow, RD_ROOT->child[4]->token->Sem);//��������ʱ�±�С��0
        delete temp;
        return NULL;
    }
    else if (str2_n < str1_n) {
        Error_ArraySubscriptOutBounds(RD_ROOT->child[2]->token->Lineshow, RD_ROOT->child[2]->token->Sem, RD_ROOT->child[4]->token->Sem);//�����±�Խ��
        delete temp;
        return NULL;
    }
    else {
        tempTypeIR->size = (str2_n - str1_n + 1) * tempTypeIR->More.elemTy->size;//��������size = ����Ԫ�ظ��� * ����Ԫ�سߴ�
    }
    tempTypeIR->serial = TypeList.size();
    TypeList.push_back(tempTypeIR);


    //END �½�һ��TypeIR��������TypeList------------------------------------------------------------------------------------
    temp->idtype = TypeList[TypeList.size() - 1];
    return temp;
}

AttributeIR* structureTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"StructureType"����ӦRD�е�"structureType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = NULL;
    if (RD_ROOT->child[0]->child[0]->token->Lex == ARRAY) {
        //RD_ROOT->child[0]: arrayType();
        temp = arrayTypeParsing(RD_ROOT->child[0], TypeList);
    }
    else if (RD_ROOT->child[0]->child[0]->token->Lex == RECORD) {
        //RD_ROOT->child[0]: recType();
        temp = recTypeParsing(RD_ROOT->child[0], TypeList);
    }
    return temp;
}

AttributeIR* baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"BaseType"����ӦRD�е�"baseType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = NULL;
    if (RD_ROOT->child[0]->token->Lex == INTEGER) {
        temp = new AttributeIR;
        temp->kind = TYPEKIND;
        temp->idtype = TypeList[INTTY - '0'];
    }
    else if (RD_ROOT->child[0]->token->Lex == CHAR) {
        temp = new AttributeIR;
        temp->kind = TYPEKIND;
        temp->idtype = TypeList[CHARTY - '0'];
    }
    return temp;
}

void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeDecMore"����ӦRD�е�"typeDecMore����"
    if (RD_ROOT == NULL) { return; }
    typeDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
}


AttributeIR* typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeDef"����ӦRD�е�"typeDef����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = NULL;
    if (0 == strcmp(RD_ROOT->child[0]->str, "BaseType")) {
        temp = baseTypeParsing(RD_ROOT->child[0], TypeList);
    }
    else if (0 == strcmp(RD_ROOT->child[0]->str, "StructureType")) {
        temp = structureTypeParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    }
    else {
        SymbTable* Sym = FindEntry(RD_ROOT->child[0]->token->Sem, false, scope, exit_region, TYPEKIND);
        if (Sym == NULL) {//���δ�ҵ��˱�ʶ��
            Error_IdentifierUndeclared(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        }
        else {
            temp = new AttributeIR;
            *temp = Sym->attrIR;
        }
    }
    return temp;
}
/*
void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeID"����ӦRD�е�"typeID����"
    if (RD_ROOT == NULL) { return; }
}
*/


void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeDecList"����ӦRD�е�"typeDecList����"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;//��Ҫ������±�ʶ��
    //RD_ROOT->child[0]: ID, ��ʶ�����Ƹ���
    strcpy(temp.idname, RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: EQ,�޲���
    //RD_ROOT->child[2]:typeDef
    AttributeIR* tempAttr = typeDefParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    if (tempAttr != NULL) {//���ΪNULL�Ļ��������typeDefParsing������typeDefParsing�еĺ����б����������ﲻ��Ҫ����
        temp.attrIR = *tempAttr;
        delete tempAttr;
        //RD_ROOT->child[3]: SEMI, ������ţ��޲���
        temp.attrIR.kind = TYPEKIND;//�������
        bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region);//�����ʶ��
        if (flag == false) {//�����ʶ��ʧ�ܣ���ʶ���ظ����壩
            Error_IdentifierDuplicateDec(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        }
    }
    //RD_ROOT->child[4]: typeDecMore
    typeDecMoreParsing(RD_ROOT->child[4], scope, exit_region, TypeList);
    return;
}


void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeDec"����ӦRD�е�"typeDec����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: type���޲���
    //RD_ROOT->child[1]: typeDecList
    typeDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    return;
}


void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecpart����"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//procDecpart,�½�һ���շ��ű�
    procDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecPart"����ӦRD�е�"varDecPart����"
    if (RD_ROOT == NULL) { return; }
    varDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"
    if (RD_ROOT == NULL) { return; }
    typeDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    return;
}

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: Begin,�޲���
    //RD_ROOT->child[1]: stmList
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    DestroyTable(exit_region, ValidTableCount);//RD_ROOT->child[2]: End���ϳ���һ�����ű�
    return;
}

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"
    if (RD_ROOT == NULL) { return; }
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"
    if (RD_ROOT == NULL) { return; }
    //do nothing
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//�����﷨��,���ڵ�����Ϊ"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,�½�һ���շ��ű�
    programHeadParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token��ֻ��ID, INTC, CHARC����������Ϣ���ֱ�Ϊ��ʶ�����ơ��������ַ����Ҷ����ַ�������ʽ

void semantic_analysis(treenode* RD_ROOT) {
    //�������
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�
    vector<struct TypeIR*> TypeList;//������Ͷ��壬ǰ�����̶����ֱ�Ϊ[0]:int, [1]:char, [2]bool,����ʹ�� TypeList[INTTY - '0'] ������int
    int ValidTableCount = 0;

    //��ʼ������
    InitFile(ERROR_FILE);//��մ洢���������Ϣ���ļ�
    InitFile(TABLE_FILE);//��մ洢���ͱ�ͷ��ű���ļ�
    initialize(TypeList);////��ʼ�����ֻ�������

    //�����﷨��
    RDTreeParsing(RD_ROOT, scope, exit_region, TypeList, ValidTableCount);

    //��ӡ���ͱ�ͷ��ű��ļ�
    PrintTable(scope, TypeList);
    return;
}
