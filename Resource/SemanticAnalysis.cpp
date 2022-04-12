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
void PrintFile(string message, string file_path)//�ַ�����ӡ����Ӧ�ļ���
{
	ofstream output(file_path);
	output << message + "\n";
	output.close();
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

SymbTable* SearchSingleTable(const char *id, vector< vector<SymbTable> > &scope, int level){//���ұ�ʶ���Ƿ�������Ӧ����
    int position = level - 1;
    SymbTable* temp = NULL;
    for(int ix=scope[position].size() - 1; ix>=0; ix--){
        if(0 == strcmp(scope[position][ix].idname, id)/*scope[position][ix].idname == id*/){
            temp = &(scope[position][ix]);
            return temp;//���������ʶ���ĵ�ַ
        }
    }
    return temp;//NULL: �����ڣ� �ǿ�: ��Ӧ��ʶ����ַ��Ϣ
}

SymbTable* FindEntry(const char *id, bool flag, vector< vector<SymbTable> > &scope, vector<bool> exit_region){//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���
    int level = exit_region.size();//��ʾ�ڼ��㣬level == 0�൱��scopeջ����Ԫ�ء�ʹ�� n = level-1 ����scope[n][x]����exit_region[n]��
    SymbTable* temp = NULL;
    //���ڵ�ǰ���ű��в���
    while(level > 0){
        if(exit_region[level - 1] == false){//�ҵ���һ��δ�˳��ľֲ������Ĳ���
            break;
        }else{
            level--;
        }
    }
    if (level <= 0) {//������һ����Ч�ľֲ�����
        return NULL;
    }
    temp = SearchSingleTable(id, scope, level);//���ҵ�ǰ���ű�
    if(flag == true && temp == NULL){//���flag == true���ҵ�ǰ���ű���δ���ҵ������ʶ��id���������������scopeջ
        while(level > 0){
            if(exit_region[level - 1] == false){//�ҵ�δ�˳��ľֲ������Ĳ���
                temp = SearchSingleTable(id, scope, level);//���Ҵ˷��ű�
                if(temp != NULL){
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
    if (FindEntry(id, false, scope, exit_region) == NULL) {
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

/******���������Ϣ�����غ���******/
void Error_IdentifierDuplicateDec(int line, string sem){//��1����ʶ�����ظ�����
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�ظ�����\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {//��2���������ı�ʶ��
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"δ������ʹ��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/******�����﷨����Ҫ����غ���******/

void initialize(vector<struct TypeIR>& TypeList) {//��ʼ�����ֻ�������
    //��ʼ����������
    struct TypeIR int_temp;
    int_temp.size = 2;
    int_temp.Typekind = INTTY;
    TypeList.push_back(int_temp);

    //��ʼ���ַ�����
    struct TypeIR char_temp;
    char_temp.size = 1;
    char_temp.Typekind = CHARTY;
    TypeList.push_back(char_temp);

    //��ʼ����������
    struct TypeIR bool_temp;
    bool_temp.size = 1;
    bool_temp.Typekind = BOOLTY;
    TypeList.push_back(bool_temp);

    return;
}


void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"
    if (RD_ROOT == NULL) { return; }
}

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"
    if (RD_ROOT == NULL) { return; }
}

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"
    if (RD_ROOT == NULL) { return; }
}

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"
    if (RD_ROOT == NULL) { return; }
}

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"
    if (RD_ROOT == NULL) { return; }
}

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"
    if (RD_ROOT == NULL) { return; }
}

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"
    if (RD_ROOT == NULL) { return; }
}

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"
    if (RD_ROOT == NULL) { return; }
}

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Term"����ӦRD�е�"term����"
    if (RD_ROOT == NULL) { return; }
}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"
    if (RD_ROOT == NULL) { return; }
}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"
    if (RD_ROOT == NULL) { return; }
}

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"
    if (RD_ROOT == NULL) { return; }
}

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"
    if (RD_ROOT == NULL) { return; }
}

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"
    if (RD_ROOT == NULL) { return; }
}

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"
    if (RD_ROOT == NULL) { return; }
}

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"
    if (RD_ROOT == NULL) { return; }
}

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"
    if (RD_ROOT == NULL) { return; }
}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"
    if (RD_ROOT == NULL) { return; }
}

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"
    if (RD_ROOT == NULL) { return; }
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************
    if (RD_ROOT == NULL) { return; }
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************
    if (RD_ROOT == NULL) { return; }
}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"
    if (RD_ROOT == NULL) { return; }
}

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"
    if (RD_ROOT == NULL) { return; }
}

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR> TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"
    if (RD_ROOT == NULL) { return; }
}

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"
    if (RD_ROOT == NULL) { return; }
}

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"
    if (RD_ROOT == NULL) { return; }
}

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"
    if (RD_ROOT == NULL) { return; }
}

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Param"����ӦRD�е�"param����"
    if (RD_ROOT == NULL) { return; }
}

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"
    if (RD_ROOT == NULL) { return; }
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"
    if (RD_ROOT == NULL) { return; }
}

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"
    if (RD_ROOT == NULL) { return; }
}

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"
    if (RD_ROOT == NULL) { return; }
}

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"
    if (RD_ROOT == NULL) { return; }
}

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"
    if (RD_ROOT == NULL) { return; }
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"
    if (RD_ROOT == NULL) { return; }
}

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"
    if (RD_ROOT == NULL) { return; }
}

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"
    if (RD_ROOT == NULL) { return; }
}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"
    if (RD_ROOT == NULL) { return; }
}

void IDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"
    if (RD_ROOT == NULL) { return; }
}

void fieldDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"
    if (RD_ROOT == NULL) { return; }
}

void IDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"
    if (RD_ROOT == NULL) { return; }
}

void fieldDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"
    if (RD_ROOT == NULL) { return; }
}

void recTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"
    if (RD_ROOT == NULL) { return; }
}

void arrayTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"
    if (RD_ROOT == NULL) { return; }
}
//--------------------------------------
AttributeIR structureTypeParsing(treenode* RD_ROOT, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"StructureType"����ӦRD�е�"structureType����"
    AttributeIR temp{};

    return temp;
}

AttributeIR baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR> TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"BaseType"����ӦRD�е�"baseType����"
    AttributeIR temp;
    temp.kind = TYPEKIND;
    if (RD_ROOT->child[0]->token->Lex == INTEGER) {
        temp.idtype = &(TypeList[INTTY - '0']);
    }
    else if (RD_ROOT->child[0]->token->Lex == CHAR) {
        temp.idtype = &(TypeList[CHARTY - '0']);
    }
    return temp;
}
//--------------------------------------
void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"TypeDecMore"����ӦRD�е�"typeDecMore����"
    if (RD_ROOT == NULL) { return; }
}


AttributeIR typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR>& TypeList,const int ValidTableCount) {//���ڵ�����Ϊ"TypeDef"����ӦRD�е�"typeDef����"
    AttributeIR temp;
    if (RD_ROOT->child[0]->str == "BaseType") {
        temp = baseTypeParsing(RD_ROOT->child[0], TypeList, ValidTableCount);
    }
    else if (RD_ROOT->child[0]->str == "StructureType") {
        temp = structureTypeParsing(RD_ROOT->child[0], TypeList, ValidTableCount);
    }
    else {
        SymbTable* Sym = FindEntry(RD_ROOT->child[0]->token->Sem, false, scope, exit_region);
        if (Sym == NULL) {//���δ�ҵ��˱�ʶ��
            Error_IdentifierUndeclared(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        }
        else {
            temp = Sym->attrIR;
        }
    }
    return temp;
}
/*
void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int &ValidTableCount) {//���ڵ�����Ϊ"TypeID"����ӦRD�е�"typeID����"
    if (RD_ROOT == NULL) { return; }
}
*/


void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList,const int ValidTableCount) {//���ڵ�����Ϊ"TypeDecList"����ӦRD�е�"typeDecList����"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;//��Ҫ������±�ʶ��
    //RD_ROOT->child[0]: ID, ��ʶ�����Ƹ���
    strcpy(temp.idname, RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: EQ,�޲���
    //RD_ROOT->child[2]:typeDef
    temp.attrIR = typeDefParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[3]: SEMI, ������ţ��޲���
    temp.attrIR.kind = TYPEKIND;//�������
    bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region);//�����ʶ��
    if (flag == false) {//�����ʶ��ʧ�ܣ���ʶ���ظ����壩
        Error_IdentifierDuplicateDec(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
    }
    //RD_ROOT->child[4]: typeDecMore
    typeDecMoreParsing(RD_ROOT->child[4], scope, exit_region, TypeList, ValidTableCount);
    return;
}


void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"TypeDec"����ӦRD�е�"typeDec����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: type���޲���
    //RD_ROOT->child[1]: typeDecList
    typeDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecpart����"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//procDecpart,�½�һ���շ��ű�
    procDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecPart"����ӦRD�е�"varDecPart����"
    if (RD_ROOT == NULL) { return; }
    varDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"
    if (RD_ROOT == NULL) { return; }
    typeDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: Begin,�޲���
    //RD_ROOT->child[1]: stmList
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    DestroyTable(exit_region, ValidTableCount);//RD_ROOT->child[2]: End���ϳ���һ�����ű�
    return;
}

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"
    if (RD_ROOT == NULL) { return; }
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"
    if (RD_ROOT == NULL) { return; }
    //do nothing
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//�����﷨��,���ڵ�����Ϊ"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,�½�һ���շ��ű�
    programHeadParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token��ֻ��ID, INTC, CHARC����������Ϣ���ֱ�Ϊ��ʶ�����ơ��������ַ����Ҷ����ַ�������ʽ

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�

    vector<struct TypeIR> TypeList;//������Ͷ��壬ǰ�����̶����ֱ�Ϊ[0]:int, [1]:char, [2]bool,����ʹ�� TypeList[INTTY - '0'] ������int
    initialize(TypeList);////��ʼ�����ֻ�������
    int ValidTableCount = 0;
    RDTreeParsing(NULL, scope, exit_region, TypeList, ValidTableCount);



    //�˴�ǰ�������﷨��
}
