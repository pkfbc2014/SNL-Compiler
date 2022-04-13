#include"SemanticAnalysis.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "treenode_var.h"
/*
    符号表管理采用跳转法,以exit栈表示对应局部化区是否已经退出
*/

/******文件输入输出相关操作******/
void InitFile(string file_path)//初始化对应路径下的文件（清空文件）
{
    ofstream output(file_path);
    output <<"";
    output.close();
    //exit(0);
}

void PrintFile(string message, string file_path)//字符串打印到相应文件中
{
    fstream foutput;
    //追加写入,在原来基础上加了ios::app
    foutput.open(file_path, ios::out | ios::app);
    foutput << message + "\n";
    foutput.close();
    //exit(0);
}

/******符号表操作******/
void CreateTable(vector< vector<SymbTable> >& scope, vector<bool>& exit_region, int& ValidTableCount) {//创建一个符号表，并插入scope栈
    ValidTableCount++;
    vector<SymbTable> temp_Sym;//创建空表
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//表示未退出局部化区
    return;
}

void DestroyTable(vector<bool>& exit_region, int& ValidTableCount) {//废除最新的一个有效符号表
    ValidTableCount--;
    if (ValidTableCount < 0) {//程序的逻辑漏洞可能会导致这个问题
        cout << "DestroyTable_ERROR (ValidTableCount<0)\n";
        exit(0);
    }
    for (int ix = exit_region.size() - 1; ix >= 0; ix--) {
        if (exit_region[ix] == false) {//废除符号表从下往上数的第一个未退出的局部化区
            exit_region[ix] = true;
            break;
        }
    }
}

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind) {//查找标识符是否存在与对应表中,kind表示所需标识符的种类('*'表示全部匹配)
    int position = level - 1;
    SymbTable* temp = NULL;
    for (int ix = scope[position].size() - 1; ix >= 0; ix--) {
        if (0 == strcmp(scope[position][ix].idname, id) && (scope[position][ix].attrIR.kind == kind || kind == '*')) {
            temp = &(scope[position][ix]);
            return temp;//返回这个标识符的地址
        }
    }
    return temp;//NULL: 不存在， 非空: 对应标识符地址信息
}

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind) {//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找
    int level = exit_region.size();//表示第几层，level == 0相当于scope栈中无元素。使用 n = level-1 访问scope[n][x]或者exit_region[n]，
    SymbTable* temp = NULL;
    //先在当前符号表中查找
    while (level > 0) {
        if (exit_region[level - 1] == false) {//找到第一个未退出的局部化区的层数
            break;
        }
        else {
            level--;
        }
    }
    if (level <= 0) {//不存在一个有效的局部化区
        return NULL;
    }
    temp = SearchSingleTable(id, scope, level, kind);//查找当前符号表
    if (flag == true && temp == NULL) {//如果flag == true，且当前符号表内未查找到所需标识符id，则继续查找整个scope栈
        while (level > 0) {
            if (exit_region[level - 1] == false) {//找到未退出的局部化区的层数
                temp = SearchSingleTable(id, scope, level, kind);//查找此符号表
                if (temp != NULL) {
                    break;
                }
            }
            level--;
        }
    }
    return temp;//NULL: 不存在， 非空: 对应标识符地址信息
}

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region) {//登记标识符和其属性到符号表
    bool present = false;//记录登记是否成功
    if (FindEntry(id, false, scope, exit_region, '*') == NULL) {
        int level = exit_region.size();//表示第几层，level == 0相当于scope栈中无元素。使用 n = level-1 访问scope[n][x]或者exit_region[n]
        while (level > 0) {
            if (exit_region[level - 1] == false) {//找到第一个未退出的局部化区的层数
                break;
            }
            else {
                level--;
            }
        }
        if (level <= 0) {//程序的逻辑漏洞可能会导致这个问题
            cout << "Enter_ERROR (level<=0)\n";
            exit(0);
        }
        SymbTable temp;
        for (int ix = 0; ix < IDLENGTH; ix++) {//赋值标识符名称
            temp.idname[ix] = id[ix];
        }
        temp.attrIR = Attrib;
        scope[level - 1].push_back(temp);
        present = true;
    }
    else {
        present = false;
    }
    return present;//true:成功， false:不成功
}

string TypeIRToString(char TypeKind) {//类型种类编号转字符串
    if (TypeKind == '0') { return "Int   "; }
    else if (TypeKind == '1') { return "Char  "; }
    else if (TypeKind == '2') { return "Bool  "; }
    else if (TypeKind == '3') { return "Record"; }
    else if (TypeKind == '4') { return "Array "; }
    return "";
}

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList) {//打印类型表和符号表到文件
    const int TypeList_size = TypeList.size();
    string message = "";
    message += "类型表\n序号\tSize\tkind\tindexTy\telemTy\tLow\tbody{类型序号,变量名,偏移}\n";
    for (int ix = 0; ix < TypeList_size; ix++) {//打印类型表
        message += to_string(ix + 1);//序号
        message += "\t";
        message += to_string(TypeList[ix]->size);
        message += "\t";
        message += TypeIRToString(TypeList[ix]->Typekind);
        message += "\t";
        if (TypeList[ix]->Typekind == ARRAYTY) {
            message += "   ";  message += to_string(TypeList[ix]->More.indexTy->serial + 1);//indexTy的序号
            message += "\t";
            message += "   ";  message += to_string(TypeList[ix]->More.elemTy->serial + 1);//elemTy的序号
            message += "\t";
            message += "  ";  message += to_string(TypeList[ix]->More.Low);//indexTy的序号
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
    //在这里编写打印scope栈的代码*********************************************

    PrintFile(message, TABLE_FILE);
}

/******语义错误信息输出相关函数******/
/***标识符***/
void Error_IdentifierDuplicateDec(int line, string sem) {// 1.标识符的重复定义，对应书中语义错误(1)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"重复定义\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {// 2.无声明的标识符，对应书中语义错误(2)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"未声明就使用\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***数组***/
void Error_ArraySubscriptLessThanZero(int line, string sem) {// 1. 数组声明时下标小于0（这是冗余情况，词法分析不允许数组声明时下标小于0）
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   数组下标\"";
    ErrorMessage += sem;
    ErrorMessage += "\"非法（小于0）\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}
void Error_ArraySubscriptOutBounds(int line, string sem1, string sem2) {// 2. 数组下标越界，对应书中语义错误(4)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   数组下标越界(\"";
    ErrorMessage += sem1;
    ErrorMessage += "\" > \"";
    ErrorMessage += sem2;
    ErrorMessage += "\")\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/******解析语法树需要的相关函数******/

void initialize(vector<struct TypeIR*>& TypeList) {//初始化三种基本类型
    //初始化整数类型
    struct TypeIR* int_temp = new (struct TypeIR);
    int_temp->size = 2;
    int_temp->Typekind = INTTY;
    int_temp->serial = TypeList.size();
    TypeList.push_back(int_temp);

    //初始化字符类型
    struct TypeIR* char_temp = new (struct TypeIR);
    char_temp->size = 1;
    char_temp->Typekind = CHARTY;
    char_temp->serial = TypeList.size();
    TypeList.push_back(char_temp);

    //初始化布尔类型
    struct TypeIR* bool_temp = new (struct TypeIR);
    bool_temp->size = 1;
    bool_temp->Typekind = BOOLTY;
    bool_temp->serial = TypeList.size();
    TypeList.push_back(bool_temp);

    return;
}


void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"
    if (RD_ROOT == NULL) { return; }
}

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"FieldVar"，对应RD中的"fieldVar函数"
    if (RD_ROOT == NULL) { return; }
}

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Variable"，对应RD中的"variable函数"
    if (RD_ROOT == NULL) { return; }
}

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"MultOp"，对应RD中的"multOp函数"
    if (RD_ROOT == NULL) { return; }
}

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"
    if (RD_ROOT == NULL) { return; }
}

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Factor"，对应RD中的"factor函数"
    if (RD_ROOT == NULL) { return; }
}

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"AddOp"，对应RD中的"addOp函数"
    if (RD_ROOT == NULL) { return; }
}

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"
    if (RD_ROOT == NULL) { return; }
}

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Term"，对应RD中的"term函数"
    if (RD_ROOT == NULL) { return; }
}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"
    if (RD_ROOT == NULL) { return; }
}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ActParamList"，对应RD中的"actparamList函数"
    if (RD_ROOT == NULL) { return; }
}

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Exp"，对应RD中的"exp函数"
    if (RD_ROOT == NULL) { return; }
}

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"VariMore"，对应RD中的"variMore函数"
    if (RD_ROOT == NULL) { return; }
}

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"
    if (RD_ROOT == NULL) { return; }
}

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"
    if (RD_ROOT == NULL) { return; }
}

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"AssCall"，对应RD中的"assCall函数"
    if (RD_ROOT == NULL) { return; }
}

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ReturnStm"，对应RD中的"returnStm函数"
    if (RD_ROOT == NULL) { return; }
}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"OutputStm"，对应RD中的"outputStm函数"
    if (RD_ROOT == NULL) { return; }
}

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"InputStm"，对应RD中的"inputStm函数"
    if (RD_ROOT == NULL) { return; }
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为""，对应RD中的"loopStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为""，对应RD中的"conditionalStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"StmMore"，对应RD中的"stmMore函数"
    if (RD_ROOT == NULL) { return; }
}

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Stm"，对应RD中的"stm函数"
    if (RD_ROOT == NULL) { return; }
}

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"StmList"，对应RD中的"stmList函数"
    if (RD_ROOT == NULL) { return; }
}

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"FidMore"，对应RD中的"fidMore函数"
    if (RD_ROOT == NULL) { return; }
}

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"FormList"，对应RD中的"formList函数"
    if (RD_ROOT == NULL) { return; }
}

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ParamMore"，对应RD中的"paramMore函数"
    if (RD_ROOT == NULL) { return; }
}

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Param"，对应RD中的"param函数"
    if (RD_ROOT == NULL) { return; }
}

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"
    if (RD_ROOT == NULL) { return; }
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ProcBody"，对应RD中的"procBody函数"
    if (RD_ROOT == NULL) { return; }
}

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"
    if (RD_ROOT == NULL) { return; }
}

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"ParamList"，对应RD中的"paramList函数"
    if (RD_ROOT == NULL) { return; }
}

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProcDec"，对应RD中的"procDec函数"
    if (RD_ROOT == NULL) { return; }
}

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"
    if (RD_ROOT == NULL) { return; }
}

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"VarIDList"，对应RD中的"varIDList函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"VarDecList"，对应RD中的"varDecList函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"VarDec"，对应RD中的"varDec函数"
    if (RD_ROOT == NULL) { return; }
}


void IDMoreParsing(treenode* RD_ROOT, vector<string>& StrVec) {//根节点名称为"IDMore"，对应RD中的"IDMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, 无操作
    //RD_ROOT->child[1]: IDList()
    IDListParsing(RD_ROOT->child[1], StrVec);
    return;
}

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]:fieldDecList()
    return fieldDecListParsing(RD_ROOT->child[0], TypeList);
}

void IDListParsing(treenode* RD_ROOT, vector<string>& StrVec) {//根节点名称为"IDList"，对应RD中的"IDList函数"，StrVec是最后所需要的ID表
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    StrVec.push_back(RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: IDMore()
    IDMoreParsing(RD_ROOT->child[1], StrVec);
    return;
}

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"
    if (RD_ROOT == NULL) { return NULL; }
    fieldChain* temp = NULL;
    //RD_ROOT->child[0]: BaseType 或 ArrayType,决定unitType
    if (RD_ROOT->child[0]->child[0]->token->Lex == INTEGER || RD_ROOT->child[0]->child[0]->token->Lex == CHAR) {
        temp = new fieldChain;
        if (RD_ROOT->child[0]->child[0]->token->Lex == INTEGER) {
            temp->unitType = TypeList[INTTY - '0'];//指向INTGER类型
        }
        else {//RD_ROOT->child[0]->child[0]->token->Lex == CHAR
            temp->unitType = TypeList[CHARTY - '0'];//指向CHAR类型
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
    //RD_ROOT->child[1]:IDList(),决定idname
    vector<string> StrVec;
    IDListParsing(RD_ROOT->child[1], StrVec);//得到IDList中的所有ID，并存入StrVec
    if (StrVec.size() == 0) {//ID数量为0
        delete temp;
        return NULL;
    }
    else {
        strcpy(temp->idname, StrVec[0].c_str());
        fieldChain* rear = temp;
        for (int ix = 1; ix < StrVec.size(); ix++) {//将所有ID都生成为fieldChain，并接入链表
            fieldChain* tempID = new fieldChain;
            tempID->unitType = temp->unitType;//所有ID都是相同的类型
            strcpy(tempID->idname, StrVec[ix].c_str());
            rear->next = tempID;
            rear = tempID;
        }
        //RD_ROOT->child[2]:SEMI，无操作
        //RD_ROOT->child[3]:fieldDecMore()
        rear->next = fieldDecMoreParsing(RD_ROOT->child[3], TypeList);
    }
    return temp;
}

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"RecType"，对应RD中的"recType函数"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START 新建一个TypeIR，并插入TypeList------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = RECORDTY;//记录类型Typeind
    //RD_ROOT->child[0]: RECORD, 无操作
    //RD_ROOT->child[1]: fieldDecList()
    fieldChain* body = fieldDecListParsing(RD_ROOT->child[1], TypeList);
    //RD_ROOT->child[2]: END, 无操作
    if (body == NULL) {
        delete temp;
        return NULL;
    }
    tempTypeIR->More.body = body;//记录类型body
    int size = 0;
    int offset = 0;
    while (body != NULL) {
        size += body->unitType->size;
        body->offset = offset;
        offset = size;
        body = body->next;
    }
    tempTypeIR->size = size;//记录类型size
    tempTypeIR->serial = TypeList.size();

    TypeList.push_back(tempTypeIR);
    //END 新建一个TypeIR，并插入TypeList------------------------------------------------------------------------------------
    temp->idtype = TypeList[TypeList.size() - 1];
    return temp;
}

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"ArrayType"，对应RD中的"arrayType函数"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START 新建一个TypeIR，并插入TypeList------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = ARRAYTY;//数组类型Typeind
    tempTypeIR->More.indexTy = TypeList[INTTY - '0'];//数组下标类型indexTY一定是整数类型
    //RD_ROOT->child[0]: ARRAY, 无操作
    //RD_ROOT->child[1]: LMIDPAREN('['), 无操作
    //RD_ROOT->child[2]:INTC
    string str1 = RD_ROOT->child[2]->token->Sem;
    auto str1_n = std::strtol(str1.data(), nullptr, 10);//字符串转 基数为10 的整数
    if (str1_n < 0) {
        Error_ArraySubscriptLessThanZero(RD_ROOT->child[2]->token->Lineshow, RD_ROOT->child[2]->token->Sem);//数组声明时下标小于0
        delete temp;
        return NULL;
    }
    else {
        tempTypeIR->More.Low = str1_n;//数组类型Low
    }
    //RD_ROOT->child[3]: UNDERANGE(".."), 无操作
    //RD_ROOT->child[4]: 放在后面几行操作
    //RD_ROOT->child[5]: RMIDPAREN(']'), 无操作
    //RD_ROOT->child[6]: OF, 无操作
    //RD_ROOT->child[7]: baseType()
    if (RD_ROOT->child[7]->child[0]->token->Lex == INTEGER) {//数组类型elemTY
        tempTypeIR->More.elemTy = TypeList[INTTY - '0'];
    }
    else if (RD_ROOT->child[7]->child[0]->token->Lex == CHAR) {
        tempTypeIR->More.elemTy = TypeList[CHARTY - '0'];
    }
    //RD_ROOT->child[4]:INTCstring str2 = RD_ROOT->child[2]->token->Sem;
    string str2 = RD_ROOT->child[4]->token->Sem;
    auto str2_n = std::strtol(str2.data(), nullptr, 10);//字符串转 基数为10 的整数
    if (str2_n < 0) {
        Error_ArraySubscriptLessThanZero(RD_ROOT->child[4]->token->Lineshow, RD_ROOT->child[4]->token->Sem);//数组声明时下标小于0
        delete temp;
        return NULL;
    }
    else if (str2_n < str1_n) {
        Error_ArraySubscriptOutBounds(RD_ROOT->child[2]->token->Lineshow, RD_ROOT->child[2]->token->Sem, RD_ROOT->child[4]->token->Sem);//数组下标越界
        delete temp;
        return NULL;
    }
    else {
        tempTypeIR->size = (str2_n - str1_n + 1) * tempTypeIR->More.elemTy->size;//数组类型size = 数组元素个数 * 数组元素尺寸
    }
    tempTypeIR->serial = TypeList.size();
    TypeList.push_back(tempTypeIR);


    //END 新建一个TypeIR，并插入TypeList------------------------------------------------------------------------------------
    temp->idtype = TypeList[TypeList.size() - 1];
    return temp;
}

AttributeIR* structureTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"StructureType"，对应RD中的"structureType函数"
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

AttributeIR* baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"BaseType"，对应RD中的"baseType函数"
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

void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeDecMore"，对应RD中的"typeDecMore函数"
    if (RD_ROOT == NULL) { return; }
    typeDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
}


AttributeIR* typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeDef"，对应RD中的"typeDef函数"
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
        if (Sym == NULL) {//如果未找到此标识符
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
void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeID"，对应RD中的"typeID函数"
    if (RD_ROOT == NULL) { return; }
}
*/


void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeDecList"，对应RD中的"typeDecList函数"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;//需要插入的新标识符
    //RD_ROOT->child[0]: ID, 标识符名称复制
    strcpy(temp.idname, RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: EQ,无操作
    //RD_ROOT->child[2]:typeDef
    AttributeIR* tempAttr = typeDefParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    if (tempAttr != NULL) {//如果为NULL的话，则会在typeDefParsing函数或typeDefParsing中的函数中报错，所以这里不需要报错
        temp.attrIR = *tempAttr;
        delete tempAttr;
        //RD_ROOT->child[3]: SEMI, 特殊符号，无操作
        temp.attrIR.kind = TYPEKIND;//冗余操作
        bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region);//插入标识符
        if (flag == false) {//插入标识符失败（标识符重复定义）
            Error_IdentifierDuplicateDec(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        }
    }
    //RD_ROOT->child[4]: typeDecMore
    typeDecMoreParsing(RD_ROOT->child[4], scope, exit_region, TypeList);
    return;
}


void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeDec"，对应RD中的"typeDec函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: type，无操作
    //RD_ROOT->child[1]: typeDecList
    typeDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    return;
}


void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProcDecPart"，对应RD中的"procDecpart函数"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//procDecpart,新建一个空符号表
    procDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecPart"，对应RD中的"varDecPart函数"
    if (RD_ROOT == NULL) { return; }
    varDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"
    if (RD_ROOT == NULL) { return; }
    typeDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    return;
}

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProgramBody"，对应RD中的"programBody函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: Begin,无操作
    //RD_ROOT->child[1]: stmList
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    DestroyTable(exit_region, ValidTableCount);//RD_ROOT->child[2]: End，废除第一个符号表
    return;
}

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"DeclarePart"，对应RD中的"declarePart函数"
    if (RD_ROOT == NULL) { return; }
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"ProgramHead"，对应RD中的"programHead函数"
    if (RD_ROOT == NULL) { return; }
    //do nothing
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//解析语法树,根节点名称为"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,新建一个空符号表
    programHeadParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token中只有ID, INTC, CHARC含有语义信息，分别为标识符名称、整数、字符，且都是字符数组形式

void semantic_analysis(treenode* RD_ROOT) {
    //定义变量
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出
    vector<struct TypeIR*> TypeList;//存放类型定义，前三个固定，分别为[0]:int, [1]:char, [2]bool,可以使用 TypeList[INTTY - '0'] 来访问int
    int ValidTableCount = 0;

    //初始化函数
    InitFile(ERROR_FILE);//清空存储语义错误信息的文件
    InitFile(TABLE_FILE);//清空存储类型表和符号表的文件
    initialize(TypeList);////初始化三种基本类型

    //解析语法树
    RDTreeParsing(RD_ROOT, scope, exit_region, TypeList, ValidTableCount);

    //打印类型表和符号表到文件
    PrintTable(scope, TypeList);
    return;
}
