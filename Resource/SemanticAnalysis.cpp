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
void PrintFile(string message, string file_path)//字符串打印到相应文件中
{
	ofstream output(file_path);
	output << message + "\n";
	output.close();
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

SymbTable* SearchSingleTable(const char *id, vector< vector<SymbTable> > &scope, int level){//查找标识符是否存在与对应表中
    int position = level - 1;
    SymbTable* temp = NULL;
    for(int ix=scope[position].size() - 1; ix>=0; ix--){
        if(0 == strcmp(scope[position][ix].idname, id)/*scope[position][ix].idname == id*/){
            temp = &(scope[position][ix]);
            return temp;//返回这个标识符的地址
        }
    }
    return temp;//NULL: 不存在， 非空: 对应标识符地址信息
}

SymbTable* FindEntry(const char *id, bool flag, vector< vector<SymbTable> > &scope, vector<bool> exit_region){//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找
    int level = exit_region.size();//表示第几层，level == 0相当于scope栈中无元素。使用 n = level-1 访问scope[n][x]或者exit_region[n]，
    SymbTable* temp = NULL;
    //先在当前符号表中查找
    while(level > 0){
        if(exit_region[level - 1] == false){//找到第一个未退出的局部化区的层数
            break;
        }else{
            level--;
        }
    }
    if (level <= 0) {//不存在一个有效的局部化区
        return NULL;
    }
    temp = SearchSingleTable(id, scope, level);//查找当前符号表
    if(flag == true && temp == NULL){//如果flag == true，且当前符号表内未查找到所需标识符id，则继续查找整个scope栈
        while(level > 0){
            if(exit_region[level - 1] == false){//找到未退出的局部化区的层数
                temp = SearchSingleTable(id, scope, level);//查找此符号表
                if(temp != NULL){
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
    if (FindEntry(id, false, scope, exit_region) == NULL) {
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

/******语义错误信息输出相关函数******/
void Error_IdentifierDuplicateDec(int line, string sem){//（1）标识符的重复定义
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"重复定义\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {//（2）无声明的标识符
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"未声明就使用\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/******解析语法树需要的相关函数******/

void initialize(vector<struct TypeIR>& TypeList) {//初始化三种基本类型
    //初始化整数类型
    struct TypeIR int_temp;
    int_temp.size = 2;
    int_temp.Typekind = INTTY;
    TypeList.push_back(int_temp);

    //初始化字符类型
    struct TypeIR char_temp;
    char_temp.size = 1;
    char_temp.Typekind = CHARTY;
    TypeList.push_back(char_temp);

    //初始化布尔类型
    struct TypeIR bool_temp;
    bool_temp.size = 1;
    bool_temp.Typekind = BOOLTY;
    TypeList.push_back(bool_temp);

    return;
}


void fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"
    if (RD_ROOT == NULL) { return; }
}

void fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FieldVar"，对应RD中的"fieldVar函数"
    if (RD_ROOT == NULL) { return; }
}

void variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Variable"，对应RD中的"variable函数"
    if (RD_ROOT == NULL) { return; }
}

void multOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"MultOp"，对应RD中的"multOp函数"
    if (RD_ROOT == NULL) { return; }
}

void otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"
    if (RD_ROOT == NULL) { return; }
}

void factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Factor"，对应RD中的"factor函数"
    if (RD_ROOT == NULL) { return; }
}

void addOpParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"AddOp"，对应RD中的"addOp函数"
    if (RD_ROOT == NULL) { return; }
}

void otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"
    if (RD_ROOT == NULL) { return; }
}

void termParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Term"，对应RD中的"term函数"
    if (RD_ROOT == NULL) { return; }
}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"
    if (RD_ROOT == NULL) { return; }
}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ActParamList"，对应RD中的"actparamList函数"
    if (RD_ROOT == NULL) { return; }
}

void expParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Exp"，对应RD中的"exp函数"
    if (RD_ROOT == NULL) { return; }
}

void variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VariMore"，对应RD中的"variMore函数"
    if (RD_ROOT == NULL) { return; }
}

void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"
    if (RD_ROOT == NULL) { return; }
}

void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"
    if (RD_ROOT == NULL) { return; }
}

void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"AssCall"，对应RD中的"assCall函数"
    if (RD_ROOT == NULL) { return; }
}

void returnStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ReturnStm"，对应RD中的"returnStm函数"
    if (RD_ROOT == NULL) { return; }
}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"OutputStm"，对应RD中的"outputStm函数"
    if (RD_ROOT == NULL) { return; }
}

void inputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"InputStm"，对应RD中的"inputStm函数"
    if (RD_ROOT == NULL) { return; }
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为""，对应RD中的"loopStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为""，对应RD中的"conditionalStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"StmMore"，对应RD中的"stmMore函数"
    if (RD_ROOT == NULL) { return; }
}

void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Stm"，对应RD中的"stm函数"
    if (RD_ROOT == NULL) { return; }
}

void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR> TypeList, const int ValidTableCount) {//根节点名称为"StmList"，对应RD中的"stmList函数"
    if (RD_ROOT == NULL) { return; }
}

void fidMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FidMore"，对应RD中的"fidMore函数"
    if (RD_ROOT == NULL) { return; }
}

void formListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FormList"，对应RD中的"formList函数"
    if (RD_ROOT == NULL) { return; }
}

void paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ParamMore"，对应RD中的"paramMore函数"
    if (RD_ROOT == NULL) { return; }
}

void paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"Param"，对应RD中的"param函数"
    if (RD_ROOT == NULL) { return; }
}

void paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"
    if (RD_ROOT == NULL) { return; }
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ProcBody"，对应RD中的"procBody函数"
    if (RD_ROOT == NULL) { return; }
}

void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"
    if (RD_ROOT == NULL) { return; }
}

void paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ParamList"，对应RD中的"paramList函数"
    if (RD_ROOT == NULL) { return; }
}

void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ProcDec"，对应RD中的"procDec函数"
    if (RD_ROOT == NULL) { return; }
}

void varIDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"
    if (RD_ROOT == NULL) { return; }
}

void varIDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarIDList"，对应RD中的"varIDList函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecList"，对应RD中的"varDecList函数"
    if (RD_ROOT == NULL) { return; }
}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarDec"，对应RD中的"varDec函数"
    if (RD_ROOT == NULL) { return; }
}

void IDMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"IDMore"，对应RD中的"IDMore函数"
    if (RD_ROOT == NULL) { return; }
}

void fieldDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"
    if (RD_ROOT == NULL) { return; }
}

void IDListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"IDList"，对应RD中的"IDList函数"
    if (RD_ROOT == NULL) { return; }
}

void fieldDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"
    if (RD_ROOT == NULL) { return; }
}

void recTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"RecType"，对应RD中的"recType函数"
    if (RD_ROOT == NULL) { return; }
}

void arrayTypeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ArrayType"，对应RD中的"arrayType函数"
    if (RD_ROOT == NULL) { return; }
}
//--------------------------------------
AttributeIR structureTypeParsing(treenode* RD_ROOT, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"StructureType"，对应RD中的"structureType函数"
    AttributeIR temp{};

    return temp;
}

AttributeIR baseTypeParsing(treenode* RD_ROOT, vector<struct TypeIR> TypeList, const int ValidTableCount) {//根节点名称为"BaseType"，对应RD中的"baseType函数"
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
void typeDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"TypeDecMore"，对应RD中的"typeDecMore函数"
    if (RD_ROOT == NULL) { return; }
}


AttributeIR typeDefParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR>& TypeList,const int ValidTableCount) {//根节点名称为"TypeDef"，对应RD中的"typeDef函数"
    AttributeIR temp;
    if (RD_ROOT->child[0]->str == "BaseType") {
        temp = baseTypeParsing(RD_ROOT->child[0], TypeList, ValidTableCount);
    }
    else if (RD_ROOT->child[0]->str == "StructureType") {
        temp = structureTypeParsing(RD_ROOT->child[0], TypeList, ValidTableCount);
    }
    else {
        SymbTable* Sym = FindEntry(RD_ROOT->child[0]->token->Sem, false, scope, exit_region);
        if (Sym == NULL) {//如果未找到此标识符
            Error_IdentifierUndeclared(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        }
        else {
            temp = Sym->attrIR;
        }
    }
    return temp;
}
/*
void typeIDParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int &ValidTableCount) {//根节点名称为"TypeID"，对应RD中的"typeID函数"
    if (RD_ROOT == NULL) { return; }
}
*/


void typeDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList,const int ValidTableCount) {//根节点名称为"TypeDecList"，对应RD中的"typeDecList函数"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;//需要插入的新标识符
    //RD_ROOT->child[0]: ID, 标识符名称复制
    strcpy(temp.idname, RD_ROOT->child[0]->token->Sem);
    //RD_ROOT->child[1]: EQ,无操作
    //RD_ROOT->child[2]:typeDef
    temp.attrIR = typeDefParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[3]: SEMI, 特殊符号，无操作
    temp.attrIR.kind = TYPEKIND;//冗余操作
    bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region);//插入标识符
    if (flag == false) {//插入标识符失败（标识符重复定义）
        Error_IdentifierDuplicateDec(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
    }
    //RD_ROOT->child[4]: typeDecMore
    typeDecMoreParsing(RD_ROOT->child[4], scope, exit_region, TypeList, ValidTableCount);
    return;
}


void typeDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"TypeDec"，对应RD中的"typeDec函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: type，无操作
    //RD_ROOT->child[1]: typeDecList
    typeDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void procDecpartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//根节点名称为"ProcDecPart"，对应RD中的"procDecpart函数"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//procDecpart,新建一个空符号表
    procDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void varDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecPart"，对应RD中的"varDecPart函数"
    if (RD_ROOT == NULL) { return; }
    varDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"
    if (RD_ROOT == NULL) { return; }
    typeDecParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//根节点名称为"ProgramBody"，对应RD中的"programBody函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: Begin,无操作
    //RD_ROOT->child[1]: stmList
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    DestroyTable(exit_region, ValidTableCount);//RD_ROOT->child[2]: End，废除第一个符号表
    return;
}

void declarePartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//根节点名称为"DeclarePart"，对应RD中的"declarePart函数"
    if (RD_ROOT == NULL) { return; }
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, const int ValidTableCount) {//根节点名称为"ProgramHead"，对应RD中的"programHead函数"
    if (RD_ROOT == NULL) { return; }
    //do nothing
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR>& TypeList, int& ValidTableCount) {//解析语法树,根节点名称为"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,新建一个空符号表
    programHeadParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token中只有ID, INTC, CHARC含有语义信息，分别为标识符名称、整数、字符，且都是字符数组形式

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出

    vector<struct TypeIR> TypeList;//存放类型定义，前三个固定，分别为[0]:int, [1]:char, [2]bool,可以使用 TypeList[INTTY - '0'] 来访问int
    initialize(TypeList);////初始化三种基本类型
    int ValidTableCount = 0;
    RDTreeParsing(NULL, scope, exit_region, TypeList, ValidTableCount);



    //此处前根遍历语法树
}
