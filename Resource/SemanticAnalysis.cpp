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
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &ValidTableCount){//创建一个符号表，并插入scope栈
    ValidTableCount++;
    vector<SymbTable> temp_Sym;//创建空表
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//表示未退出局部化区
    return;
}

void DestroyTable(vector<bool> &exit_region, int &ValidTableCount){//废除最新的一个有效符号表
    ValidTableCount--;
    if(ValidTableCount<0){//程序的逻辑漏洞可能会导致这个问题
        cout <<"DestroyTable_ERROR (ValidTableCount<0)\n";
        exit(0);
    }
    for(int ix=exit_region.size() - 1; ix>=0; ix--){
        if(exit_region[ix] == false){//废除符号表从下往上数的第一个未退出的局部化区
            exit_region[ix] = true;
            break;
        }
    }
}

bool SearchSingleTable(char *id, vector< vector<SymbTable> > scope, int level){//查找标识符是否存在与对应表中
    int position = level - 1;
    bool present = false;//所查找标识符是否存在与对应表中
    for(int ix=scope[position].size() - 1; ix>=0; ix--){
        if(0 == strcmp(scope[position][ix].idname, id)/*scope[position][ix].idname == id*/){
            present = true;
            return present;
        }
    }
    return present;
}

bool FindEntry(char *id, bool flag, vector< vector<SymbTable> > scope, vector<bool> exit_region){//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找
    int level = exit_region.size();//表示第几层，level == 0相当于scope栈中无元素。使用 n = level-1 访问scope[n][x]或者exit_region[n]，
    bool present = false;//是否存在
    //先在当前符号表中查找
    while(level > 0){
        if(exit_region[level - 1] == false){//找到第一个未退出的局部化区的层数
            break;
        }else{
            level--;
        }
    }
    if (level <= 0) {//不存在一个有效的局部化区
        return false;
    }
    present = SearchSingleTable(id, scope, level);//查找当前符号表
    if(flag == true && present == false){//如果flag == true，且当前符号表内未查找到所需标识符id，则继续查找整个scope栈
        while(level > 0){
            if(exit_region[level - 1] == false){//找到未退出的局部化区的层数
                present = SearchSingleTable(id, scope, level);//查找此符号表
                if(present == true){
                    break;
                }
            }
            level--;
        }
    }
    return present;//true:存在， false:不存在
}

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region) {//登记标识符和其属性到符号表
    bool present = false;//记录登记是否成功
    if (FindEntry(id, false, scope, exit_region) == false) {
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
        PrintFile(SemanticERROR1, ERROR_FILE);
    }
    return present;//true:成功， false:不成功
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

/*语法树解析函数模板
void Parsing() {//根节点名称为""，对应RD中的"函数"

}
*/
void fieldVarMoreParsing() {//根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"

}

void fieldVarParsing() {//根节点名称为"FieldVar"，对应RD中的"fieldVar函数"

}

void variableParsing() {//根节点名称为"Variable"，对应RD中的"variable函数"

}

void multOpParsing() {//根节点名称为"MultOp"，对应RD中的"multOp函数"

}

void otherFactorParsing() {//根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"

}

void factorParsing() {//根节点名称为"Factor"，对应RD中的"factor函数"

}

void addOpParsing() {//根节点名称为"AddOp"，对应RD中的"addOp函数"

}

void otherTermParsing() {//根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"

}

void termParsing() {//根节点名称为"Term"，对应RD中的"term函数"

}

void actparamMoreParsing() {//根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"

}

void actparamListParsing() {//根节点名称为"ActParamList"，对应RD中的"actparamList函数"

}

void expParsing() {//根节点名称为"Exp"，对应RD中的"exp函数"

}

void variMoreParsing() {//根节点名称为"VariMore"，对应RD中的"variMore函数"

}

void callStmRestParsing() {//根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"

}

void assignmentRestParsing() {//根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"

}

void assCallParsing() {//根节点名称为"AssCall"，对应RD中的"assCall函数"

}

void returnStmParsing() {//根节点名称为"ReturnStm"，对应RD中的"returnStm函数"

}

void outputStmParsing() {//根节点名称为"OutputStm"，对应RD中的"outputStm函数"

}

void inputStmParsing() {//根节点名称为"InputStm"，对应RD中的"inputStm函数"

}

void loopStmParsing() {//根节点名称为""，对应RD中的"loopStm函数"********************************************

}

void conditionalStmParsing() {//根节点名称为""，对应RD中的"conditionalStm函数"********************************************

}

void stmMoreParsing() {//根节点名称为"StmMore"，对应RD中的"stmMore函数"

}

void stmParsing() {//根节点名称为"Stm"，对应RD中的"stm函数"

}

void stmListParsing() {//根节点名称为"StmList"，对应RD中的"stmList函数"

}

void fidMoreParsing() {//根节点名称为"FidMore"，对应RD中的"fidMore函数"

}

void formListParsing() {//根节点名称为"FormList"，对应RD中的"formList函数"

}

void paramMoreParsing() {//根节点名称为"ParamMore"，对应RD中的"paramMore函数"

}

void paramParsing() {//根节点名称为"Param"，对应RD中的"param函数"

}

void paramDecListParsing() {//根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"

}

void procBodyParsing() {//根节点名称为"ProcBody"，对应RD中的"procBody函数"

}

void procDecPartParsing() {//根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"

}

void paramListParsing() {//根节点名称为"ParamList"，对应RD中的"paramList函数"

}

void procDecParsing() {//根节点名称为"ProcDec"，对应RD中的"procDec函数"

}

void varIDMoreParsing() {//根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"

}

void varDecMoreParsing() {//根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"

}

void varIDListParsing() {//根节点名称为"VarIDList"，对应RD中的"varIDList函数"

}

void varDecListParsing() {//根节点名称为"VarDecList"，对应RD中的"varDecList函数"

}

void varDecParsing() {//根节点名称为"VarDec"，对应RD中的"varDec函数"

}

void IDMoreParsing() {//根节点名称为"IDMore"，对应RD中的"IDMore函数"

}

void fieldDecMoreParsing() {//根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"

}

void IDListParsing() {//根节点名称为"IDList"，对应RD中的"IDList函数"

}

void fieldDecListParsing() {//根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"

}

void recTypeParsing() {//根节点名称为"RecType"，对应RD中的"recType函数"

}

void arrayTypeParsing() {//根节点名称为"ArrayType"，对应RD中的"arrayType函数"

}

void structureTypeParsing() {//根节点名称为"StructureType"，对应RD中的"structureType函数"

}

void baseTypeParsing() {//根节点名称为"BaseType"，对应RD中的"baseType函数"

}

void typeDecMoreParsing() {//根节点名称为"TypeDecMore"，对应RD中的"typeDecMore函数"

}

void typeDefParsing() {//根节点名称为"TypeDef"，对应RD中的"typeDef函数"

}

void typeIDParsing() {//根节点名称为"TypeID"，对应RD中的"typeID函数"

}

void typeDecListParsing() {//根节点名称为"TypeDecList"，对应RD中的"typeDecList函数"

}

void typeDecParsing() {//根节点名称为"TypeDec"，对应RD中的"typeDec函数"

}

void procDecpartParsing() {//根节点名称为"ProcDecPart"，对应RD中的"procDecpart函数"

}

void varDecPartParsing() {//根节点名称为"VarDecPart"，对应RD中的"varDecPart函数"

}

void typeDecPartParsing() {//根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"

}

void programBodyParsing() {//根节点名称为"ProgramBody"，对应RD中的"programBody函数"

}

void declarePartParsing() {//根节点名称为"DeclarePart"，对应RD中的"declarePart函数"

}

void programHeadParsing() {//根节点名称为"ProgramHead"，对应RD中的"programHead函数"

}

void RDTreeParsing(treenode *RD_ROOT, vector< vector<SymbTable> > &scope, vector<bool> &exit_region, vector<struct TypeIR> &TypeList) {//解析语法树,根节点名称为"Program"

}


//token中只有ID, INTC, CHARC含有语义信息，分别为标识符名称、整数、字符，且都是字符数组形式

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出
    int ValidTableCount = 0;//表示未被废除的符号表总数

    vector<struct TypeIR> TypeList;//存放类型定义，前三个固定，分别为[0]:int, [1]:char, [2]bool,可以使用 TypeList[INTTY - '0'] 来访问int
    initialize(TypeList);////初始化三种基本类型

    RDTreeParsing(NULL, scope, exit_region, TypeList);



    //此处前根遍历语法树
}
