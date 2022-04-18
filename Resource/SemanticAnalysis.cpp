#include"SemanticAnalysis.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream> 
#include<unordered_map>
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

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind, const int ProcLevel) {//查找标识符是否存在与对应表中,kind表示所需标识符的种类('*'表示全部匹配),不考虑过程标识符的Level的话需要令ProcLevel小于零
    int position = level - 1;
    SymbTable* temp = NULL;
    for (int ix = scope[position].size() - 1; ix >= 0; ix--) {
        
        
        if (scope[position][ix].attrIR.kind != PROCKIND) {//1. id 等于"*"或者idname; 2. kind等于'*'或者attrIR.kind; 3.不是过程标识符
            if ((0 == strcmp("*", id) || 0 == strcmp(scope[position][ix].idname, id)) && (scope[position][ix].attrIR.kind == kind || kind == '*')) {
                temp = &(scope[position][ix]);
                return temp;//返回这个标识符的地址
            }
        }
        else {//1. 是过程标识符; 2. id 等于"*"或者idname; 3. kind等于'*'或者attrIR.kind;  4.  Level相同或者ProcLevel<0 
            if ((0 == strcmp("*", id) || 0 == strcmp(scope[position][ix].idname, id)) && (scope[position][ix].attrIR.kind == kind || kind == '*') &&  (scope[position][ix].attrIR.More.ProcAttr.level == ProcLevel || ProcLevel < 0)) {
                temp = &(scope[position][ix]);
                return temp;//返回这个标识符的地址
            }
        }
        
    }
    return temp;//NULL: 不存在， 非空: 对应标识符地址信息
}

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind, const int ProcLevel) {//flag == false:在当前符号表中查找； flag == true:在整个scope栈中查找
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
    temp = SearchSingleTable(id, scope, level, kind, ProcLevel);//查找当前符号表
    if (flag == true && temp == NULL) {//如果flag == true，且当前符号表内未查找到所需标识符id，则继续查找整个scope栈
        level--;
        while (level > 0) {
            if (exit_region[level - 1] == false || (kind == PROCKIND && ProcLevel>=0)) {//找到未退出的局部化区的层数；如果是查找过程标识符的重定义情况（kind == PROCKIND && ProcLevel>=0），则一直允许查找
                temp = SearchSingleTable(id, scope, level, kind, ProcLevel);//查找此符号表
                if (temp != NULL) {
                    break;
                }
            }
            level--;
        }
    }
    return temp;//NULL: 不存在， 非空: 对应标识符地址信息
}

//FindProc()用于body中寻找正确的过程标识符，返回符合条件的过程标识符在scope栈的位置，使用scope[level][0]表示此过程标识符
int FindProc(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const int ValidTableCount){
    //ProcLevel只能递减的寻找，例如查找了第n层的，就不能再查找大于n层的Proc标识符
    /*例：定义如下过程标识符
    * p1()        --- level 1
    *   p2()      --- level 2
    *      p3()   --- level 3
    * p4()        --- level 1
    *   p5()      --- level 2
    * 从p5开始向上寻找，只能按照p5 -> p4 -> p1的顺序寻找，因为p5不能调用p3或p2
    */
    int level = exit_region.size() - 1;//表示第几层，level == 0相当于scope栈中无元素。使用 n = level-1 访问scope[n][x]或者exit_region[n]，
    int level_last = ValidTableCount;
    for (; level >= 1; level--) {//第一个符号表内区内没有定义Proc标识符
        //level小于等于1时，scope[level][0]表示此标志符为此符号表的过程标识符
        if (scope[level].size() >= 1) {
            if (scope[level][0].attrIR.More.ProcAttr.level <= level_last) {//不能大于上一次的level
                if (0 == strcmp(scope[level][0].idname, id)) {
                    return level;
                }
                level_last = scope[level][0].attrIR.More.ProcAttr.level;
            }
        }
    }
    return 0;
}

bool Enter(const char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region, char kind, const int ProcLevel) {//登记标识符和其属性到符号表,注：如果不是过程标识符的话, ProcLevel无意义，可以直接传ProcLevel = -1
    bool present = false;//记录登记是否成功
    bool flag = false;//FindEntry的传参，表示只查询当前符号表还是全部符号表
    if (kind == PROCKIND && ProcLevel >= 0) {
        flag = true;
    }
    else {
        flag = false;
    }
    if (FindEntry(id, flag, scope, exit_region, kind, ProcLevel) == NULL || 0 == strcmp(id, WRONGID) ) {//如果是WrongID插入符号表，表示标识符命名非法，但是也插入符号表凑数，只用于过程参数标识符的重复声明
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
        if (kind == PROCKIND) {//如果是过程标识符，则将其插到最新的符号表的最前面，即此过程标识符应该为此符号表的第一个
            scope[level - 1].insert(scope[level - 1].begin(), temp);
        }
        else {
            scope[level - 1].push_back(temp);
        }
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
string KindToString(char TypeKind) {//Kind种类编号转字符串
    if (TypeKind == '0') { return "TypeKind"; }
    else if (TypeKind == '1') { return "VarKind "; }
    else if (TypeKind == '2') { return "ProcKind"; }
    return "";
}
string AccessToString(char access) {//Kind种类编号转字符串
    if (access == '0') { return "dir"; }
    else if (access == '1') { return "indir "; }
    return "";
}

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList, string FilePath) {//打印类型表和符号表到文件
    InitFile(FilePath);
    string TabKey = "";
    if (FilePath == TABLE_FILE_XLS) {
        TabKey = TAB_XLS;
    }
    else if (FilePath == TABLE_FILE_CSV) {
        TabKey = TAB_CSV;
    }
    else {
        TabKey = "\t";
    }
    ofstream FileOut;
    FileOut.open(FilePath, ios::out | ios::left);
    FileOut << "类型表" << "\n";
    FileOut << "序号" << TabKey << "Size" << TabKey << "kind" << TabKey << "indexTy" << TabKey << "elemTy" << TabKey << "Low" << TabKey << "body{类型序号; 变量名; 偏移}" << "\n";
    const int TypeList_size = TypeList.size();
    for (int ix = 0; ix < TypeList_size; ix++) {//打印类型表
        FileOut << to_string(TypeList[ix]->serial + 1) << TabKey << to_string(TypeList[ix]->size) << TabKey << TypeIRToString(TypeList[ix]->Typekind) << TabKey;
        if (TypeList[ix]->Typekind == ARRAYTY) {
            FileOut << to_string(TypeList[ix]->More.indexTy->serial + 1) << TabKey << to_string(TypeList[ix]->More.elemTy->serial + 1) << TabKey << to_string(TypeList[ix]->More.Low) << TabKey;
            FileOut << EMPTYSPACE;//Record_body
        }
        else if (TypeList[ix]->Typekind == RECORDTY) {
            FileOut << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey;//Array_indexTy\t Array_elemTy\t Array_Low
            FieldChain* head = TypeList[ix]->More.body;
            while (head != NULL) {
                string message = "";
                message += "{";
                message += to_string(head->unitType->serial + 1);
                message += "; ";
                message += head->idname;
                message += "; ";
                message += to_string(head->offset);
                message += "}  ";
                FileOut << message;
                head = head->next;
            }
        }
        else {//basetype: int\tchar\tbool
            FileOut << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey;//Array_indexTy\t Array_elemTy\t Array_Low\t Record_body
        }
        FileOut << "\n";
    }

    //打印scope栈（符号表）
    FileOut << "\n符号表" << "\n";
    FileOut << "IDname" << TabKey << "TypePtr" << TabKey << "Kind" << TabKey << "Level" << TabKey << "Parm" << TabKey << "Code" << TabKey << "Size" << TabKey << "Access" << TabKey << "Off" << "\n";
    const int scope_size = scope.size();
    for (int ix = 0; ix < scope_size; ix++) {
        const int scope_size_2 = scope[ix].size();
        for (int jx = 0; jx < scope_size_2; jx++) {
            FileOut << scope[ix][jx].idname << TabKey;//IDname
            if (scope[ix][jx].attrIR.kind == TYPEKIND) {
                FileOut << to_string(scope[ix][jx].attrIR.idtype->serial + 1) << TabKey;//TypePtr
                FileOut << "TypeKind" << TabKey;//Kind
                FileOut << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE;
            }
            else if (scope[ix][jx].attrIR.kind == VARKIND) {
                if (scope[ix][jx].attrIR.idtype != NULL) {//TypePtr
                    FileOut << to_string(scope[ix][jx].attrIR.idtype->serial + 1) << TabKey;
                }
                else {//Kind为VarKind且TypePtr为空的情况，即过程的参数的类型错误的情况，仍算进符号表，目的是凑数。此编译器只有当参数标识符重定义时才不将此参数算进符号表
                    FileOut << "Unknown" << TabKey;
                }
                FileOut << "VarKind" << TabKey;//Kind
                FileOut << to_string(scope[ix][jx].attrIR.More.VarAttr.level) << TabKey;
                FileOut << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey << EMPTYSPACE << TabKey;//Level
                FileOut << AccessToString(scope[ix][jx].attrIR.More.VarAttr.access) << TabKey;//Access
                FileOut << to_string(scope[ix][jx].attrIR.More.VarAttr.off);//Off
            }
            else if (scope[ix][jx].attrIR.kind == PROCKIND) {
                FileOut << EMPTYSPACE << TabKey;//TypePtr
                FileOut << "ProcKind" << TabKey;//Kind
                FileOut << to_string(scope[ix][jx].attrIR.More.ProcAttr.level) << TabKey;//Level
                struct ParamTable* tempptr = scope[ix][jx].attrIR.More.ProcAttr.param;
                string param = "{ ";
                if (tempptr != NULL) {
                    param += to_string(tempptr->entry);
                    tempptr = tempptr->next;
                    while (tempptr != NULL) {
                        param += "; ";
                        param += to_string(tempptr->entry);
                        tempptr = tempptr->next;
                    }
                }
                param += " }";
                FileOut << param << TabKey;//Parm
                if (scope[ix][jx].attrIR.More.ProcAttr.code == 0) {//Code
                    FileOut << EMPTYSPACE << TabKey;//如果code等于初始化的0，即空地址，则返回空
                }
                else {//否则返回地址
                    FileOut << to_string( scope[ix][jx].attrIR.More.ProcAttr.code ) << TabKey;
                }
                FileOut << to_string(scope[ix][jx].attrIR.More.ProcAttr.size) << TabKey;//Size
                FileOut << EMPTYSPACE << TabKey << EMPTYSPACE;
            }
            FileOut << "\n";
        }
    }
    FileOut.close();
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

void Error_IdentifierDuplicateDecRecord(int line, string sem) {// 2.Record标识符中的标识符的重复定义，对应书中语义错误(1)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   在record标识符定义中，标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"重复定义\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {// 3.无声明的标识符，对应书中语义错误(2)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"未声明\n";
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

void Error_ArrayNotArrayType(int line, string sem) {// 3.body内使用的标识符不是数组标识符，对于书中语义错误(3)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   变量\"";
    ErrorMessage += sem;
    ErrorMessage += "\"不是数组类型\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

void Error_ArrayGroupInvalid(int line, string sem) {// 3.数组成员变量的引用不合法，对于书中语义错误(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   数组\"";
    ErrorMessage += sem;
    ErrorMessage += "\"的引用不合法\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

/***记录***/
void Error_RecordFieldInvalid(int line, string sem1, string sem2) {// 1.记录的域成员变量的引用不合法，对于书中语义错误(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   记录\"";
    ErrorMessage += sem1;
    ErrorMessage += "\"中不存在域变量\"";
    ErrorMessage += sem2;
    ErrorMessage += "\"\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordFieldArrayInvalid(int line, string Rsem1, string Asem2) {// 2.记录的数组类型域成员变量的引用不合法，对于书中语义错误(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   记录\"";
    ErrorMessage += Rsem1;
    ErrorMessage += "\"中的数组类型域成员\"";
    ErrorMessage += Asem2;
    ErrorMessage += "\"的成员变量引用不合法\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordFieldNotArrayType(int line, string Rsem1, string Asem2) {// 3.body内使用的域成员变量不是数组标识符，对于书中语义错误(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   记录\"";
    ErrorMessage += Rsem1;
    ErrorMessage += "\"中的域成员\"";
    ErrorMessage += Asem2;
    ErrorMessage += "\"的不是数组类型，引用不合法\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordNotRecordType(int line, string sem) {// 4.body内使用的标识符不是记录标识符，对于书中语义错误(3)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   变量\"";
    ErrorMessage += sem;
    ErrorMessage += "\"不是记录类型\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

/***过程调用***/
void Error_ProcParamType(int line, int ParamSerial, string ProcName) {// 1.过程调用语句中，形实参种类不匹配，对应书中语义错误(8)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   过程调用\"";
    ErrorMessage += ProcName;
    ErrorMessage += "\"中, 第 ";
    ErrorMessage += to_string(ParamSerial);
    ErrorMessage += " 个参数类型不匹配";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_ProcParamAmount(int line, string sem) {// 2.过程调用语句中，形实参个数不相同，对应书中语义错误(9)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   过程调用\"";
    ErrorMessage += sem;
    ErrorMessage += "\"形实参个数不匹配\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_ProcNotProcIdentifier(int line, string sem) {// 3.过程调用语句中，标识符不是过程标识符，对应书中语义错误(10)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"不是过程标识符\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***赋值***/
void Error_AssignContentIncompatible(int line, string sem) {// 1.赋值语句中，左右两边类型不相容，对应书中语义错误(6)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"与赋值内容类型不相容\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_AssignNotVarIdentifier(int line, string sem) {// 2.赋值语句中，标识符不是变量标识符，对应书中语义错误(7)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   标识符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"不是变量标识符\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***表达式***/
void Error_StmOpComponentIncompatibility(int line, string sem) {// 1.计算或赋值参表达式中运算符的分量的类型不相容，对应书中语义错误(12)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",  运算符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"的分量的类型不相容\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***条件语句***/
void Error_StmCompareComponentIncompatibility(int line, string sem) {// 1.条件表达式中运算符的分量的类型不相容，对应书中语义错误(11)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",  比较运算符\"";
    ErrorMessage += sem;
    ErrorMessage += "\"的比较结果不合法\n";
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
//在类型表里，参数Type类型是否已经存在存在，存在的话返回其地址，否则返回空
//Typekind是待测标识符的类型（数组类型或者记录类型）
struct TypeIR* WhetherTypeDuplicate(vector<struct TypeIR*>& TypeList, struct TypeIR* Type, char Typekind) {
    if (Typekind == ARRAYTY) {
        for (int ix = TypeList.size() - 1; ix >= 0; ix--) {
            //数组类型，测试属性否全部相同
            if (TypeList[ix]->size == Type->size && TypeList[ix]->Typekind == Type->Typekind) {
                if (TypeList[ix]->More.elemTy == Type->More.elemTy && TypeList[ix]->More.indexTy == Type->More.indexTy && TypeList[ix]->More.Low == Type->More.Low) {
                    return TypeList[ix];
                }
            }
        }
        return NULL;
    }
    else if (Typekind == RECORDTY) {
        for (int ix = TypeList.size() - 1; ix >= 0; ix--) {
            //记录类型，测试属性否全部相同
            if (TypeList[ix]->size == Type->size && TypeList[ix]->Typekind == Type->Typekind) {
                fieldChain* body1 = TypeList[ix]->More.body;
                fieldChain* body2 = Type->More.body;
                bool flag = true;
                //测试body部分是否全部相同
                while (body1 != NULL && body2 != NULL && flag == true) {
                    if (0 != strcmp(body1->idname, body2->idname) || body1->offset != body2->offset || body1->unitType != body2->unitType) {
                        flag = false;
                    }
                    body1 = body1->next;
                    body2 = body2->next;
                }
                if (body1 == NULL && body2 == NULL && flag == true) {
                    return TypeList[ix];
                }
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}
//tok是运算符的token信息
struct TypeIR* WhetherResaultValid(struct TypeIR* TypeP1, struct TypeIR* TypeP2 ,token* tok) {//在计算或赋值表达式中，返回TypeP1与TypeP2是否匹配，并根据情况返回匹配的类型或者NULL(不匹配)
    /*什么情况下返回NULL(表示结果不合法)
    * 1. TypeP1==NULL || TypeP2==NULL
    * 2. 当TypeP1->Typekind != TypeP2->Typekind 时，一定不合法
    * 3. 当TypeP1 == TypeP2 时，TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY（数组类型不能和数组类型直接计算，记录类型也是一样）
    * */
    bool flag = true;
    if (TypeP1 == NULL || TypeP2 == NULL) {
        flag = false;
    }
    else {
        if (TypeP1->Typekind != TypeP2->Typekind) {
            flag = false;
        }
        else if (TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY) {//且TypeP1 == TypeP2
            flag = false;
        }
        else {
            flag = true;
        }
    }
    if (flag == true) {
        return TypeP1;
    }
    else {
        return NULL;
    }
}
//tok是运算符的token信息
void WhetherCompareValid(struct TypeIR* TypeP1, struct TypeIR* TypeP2, token* tok) {//在比较表达式中，检查TypeP1与TypeP2是否匹配，并根据情况返回匹配的类型或者NULL(不匹配)
    /*什么情况下返回NULL(表示结果不合法)
    * 1. TypeP1==NULL || TypeP2==NULL
    * 2. 当TypeP1->Typekind != TypeP2->Typekind 时，一定不合法
    * 3. 当TypeP1 == TypeP2 时，TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY（数组类型不能和数组类型直接计算，记录类型也是一样）
    * */
    bool flag = true;
    if (TypeP1 == NULL || TypeP2 == NULL) {
        flag = false;
    }
    else {
        if (TypeP1->Typekind != TypeP2->Typekind) {
            flag = false;
        }
        else if (TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY) {//且TypeP1 == TypeP2
            flag = false;
        }
        else {
            flag = true;
        }
    }
    if (flag == false) {
        Error_StmCompareComponentIncompatibility(tok->Lineshow, tok->Sem);
    }
}

//判断记录类型标识符内的数组类型域成员变量的成员引用是否合法，合法就返回数组元素的类型
struct TypeIR* fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, token* Arraytok, struct TypeIR* ArrayTy) {//根节点名称为"FieldVarMore"，对应RD中的"fieldVarMore函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: LMIDPAREN，无操作
    //RD_ROOT->child[1]: exp()，判断记录类型标识符内的数组类型域成员变量的成员引用是否合法
    if (expParsing(RD_ROOT->child[1], scope, exit_region, TypeList) != TypeList[INTTY - '0']) {
        Error_RecordFieldArrayInvalid(Arraytok->Lineshow, sym->idname, Arraytok->Sem);
        return NULL;
    }
    else {
        return ArrayTy->More.elemTy;//数组元素的类型
    }
    //RD_ROOT->child[2]: RMIDPAREN，无操作
}

//返回记录类型的域成员类型,tok是Record类型变量的token信息，目的是传递此变量的行数
struct TypeIR* fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, token* tok) {//根节点名称为"FieldVar"，对应RD中的"fieldVar函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: ID，此ID是记录类型的域成员ID
    struct FieldChain* body = sym->attrIR.idtype->More.body;
    bool flag = false;//在此记录标识符内存不存在对应的域成员
    while (body != NULL) {
        if (0 == strcmp(body->idname, RD_ROOT->child[0]->token->Sem)) {//找到了对应的域变量
            flag = true;
            break;
        }
        body = body->next;
    }
    if (flag == false) {
        Error_RecordFieldInvalid(RD_ROOT->child[0]->token->Lineshow, tok->Sem, RD_ROOT->child[0]->token->Sem);
        return NULL;
    }
    else {
        if (RD_ROOT->child[1] == NULL) {//域成员是BaseType
            return body->unitType;
        }
        else {//域成员是AyyarType
            //RD_ROOT->child[1]: fieldVarMore()
            if (body->unitType->Typekind == ARRAYTY) {
                return fieldVarMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, RD_ROOT->child[0]->token, body->unitType);
            }
            else {//如果不是数组类型，则相当于访问了非法区域
                Error_RecordFieldNotArrayType(RD_ROOT->child[0]->token->Lineshow, tok->Sem, RD_ROOT->child[0]->token->Sem);
                return NULL;
            }
            
        }
    }
    

}

//返回数组或者记录的类型，如果二号孩子节点不为空，则返回数组或记录的成员类型
struct TypeIR* variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Variable"，对应RD中的"variable函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: ID，变量的ID
    SymbTable* sym = FindEntry(RD_ROOT->child[0]->token->Sem, true, scope, exit_region, VARKIND, -1);
    if (sym == NULL) {//未找到此标识符
        Error_IdentifierUndeclared(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        return NULL;
    }
    else {
        //RD_ROOT->child[1]: variMore());
        if (RD_ROOT->child[1] == NULL) {//ID后面无'[' 或者 '.'
            return sym->attrIR.idtype;
        }
        else{
            return variMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, RD_ROOT->child[0]->token);
        }
    }
}

void multOpParsing() {//根节点名称为"MultOp"，对应RD中的"multOp函数"
    //此节点下是符号，无操作
    return;
}

//tok作用在于得到otherFactor内的符号的token信息
struct TypeIR* otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token*& tok) {//根节点名称为"OtherFactor"，对应RD中的"otherFactor函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: multOp，无操作
    multOpParsing();
    tok = RD_ROOT->child[0]->child[0]->token;
    //RD_ROOT->child[1]: term()
    return termParsing(RD_ROOT->child[1], scope, exit_region, TypeList);

}

struct TypeIR* factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Factor"，对应RD中的"factor函数"
    if (RD_ROOT == NULL) { return NULL; }

    if (RD_ROOT->child[0]->token == NULL) {//variable()
        //RD_ROOT->child[0]: variable()
        return variableParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    }
    else if (RD_ROOT->child[0]->token->Lex == INTC) {
        //RD_ROOT->child[0]: INTC
        return TypeList[INTTY - '0'];//返回整数类型的内部表示
    }
    else if (RD_ROOT->child[0]->token->Lex == LPAREN) {
        //RD_ROOT->child[0]: LPAREN
        //RD_ROOT->child[1]: exp()
        return expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
        //RD_ROOT->child[2]: RPAREN
    }
}

void addOpParsing() {//根节点名称为"AddOp"，对应RD中的"addOp函数"
    //此节点下是符号，无操作
    return; 
}
//返回此表达式的otherTerm部分的计算结果的类型，tok作用在于得到otherTermParsing内的符号的token信息
struct TypeIR* otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token* &tok) {//根节点名称为"OtherTerm"，对应RD中的"otherTerm函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: addOp(), 只是决定了符号而已，并不影响类型的返回值，但是需要将其赋值给tok
    addOpParsing();
    tok = RD_ROOT->child[0]->child[0]->token;
    //RD_ROOT->child[1]: exp());
    return expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);

}

//返回此表达式的term部分的计算结果的类型
struct TypeIR* termParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Term"，对应RD中的"term函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: factor()
    struct TypeIR* TypeP1 = factorParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    //RD_ROOT->child[1]: otherFactor()
    if (RD_ROOT->child[1] == NULL) {//不存在otherFactorParsing()部分，则直接返回factorParsing()部分
        return TypeP1;
    }
    else {
        token* tok = new token;
        struct TypeIR* TypeP2 = otherFactorParsing(RD_ROOT->child[1], scope, exit_region, TypeList, tok);//tok作用在于得到otherFactor内的符号的token信息
        struct TypeIR* CalculatValidity = WhetherResaultValid(TypeP1, TypeP2, tok); //返回TypeP1与TypeP2是否匹配
        if (CalculatValidity == NULL) {//如果为NULL，则不匹配
            Error_StmOpComponentIncompatibility(tok->Lineshow, tok->Sem);
        }
        return CalculatValidity;//返回TypeP1与TypeP2是否匹配
    }

}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec) {//根节点名称为"ActParamMore"，对应RD中的"actparamMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA，无操作
    //RD_ROOT->child[1]: actparamList()
    actparamListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ProcPosition, TypeVec);//得到调用过程时的参数信息

}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec) {//根节点名称为"ActParamList"，对应RD中的"actparamList函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: exp());
    //RD_ROOT->child[1]: actparamMore()
    TypeVec.push_back(expParsing(RD_ROOT->child[0], scope, exit_region, TypeList));//将参数的类型接入参数信息链

    actparamMoreParsing(RD_ROOT->child[1], scope,exit_region, TypeList, ProcPosition, TypeVec);//得到调用过程时的参数信息

}


//返回此表达式的计算结果的类型
struct TypeIR* expParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"Exp"，对应RD中的"exp函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: term()
    struct TypeIR* TypeP1 = termParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    //RD_ROOT->child[1]: otherTerm()
    if (RD_ROOT->child[1] == NULL) {//不存在otherTermParsing()部分，则直接返回termParsing()部分
        return TypeP1;
    }
    else {
        token* tok = new token;
        struct TypeIR* TypeP2 = otherTermParsing(RD_ROOT->child[1], scope, exit_region, TypeList, tok);//tok作用在于得到otherTermParsing内的符号的token信息
        struct TypeIR* CalculatValidity = WhetherResaultValid(TypeP1, TypeP2, tok); //返回TypeP1与TypeP2是否匹配
        if (CalculatValidity == NULL) {//如果为NULL，则不匹配
            Error_StmOpComponentIncompatibility(tok->Lineshow, tok->Sem);
        }
        return CalculatValidity; //返回TypeP1与TypeP2是否匹配
    }
}

//sym是structure类型变量的符号表信息，tok是其token信息，用于传递其行数
struct TypeIR* variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable *sym, token *tok) {//根节点名称为"VariMore"，对应RD中的"variMore函数"
    if (RD_ROOT == NULL) { return NULL; }
    if (RD_ROOT->child[0]->token->Lex == LMIDPAREN) {
        //RD_ROOT->child[0]: ReadmatchToken(LMIDPAREN));
        //RD_ROOT->child[1]: exp());
        //RD_ROOT->child[2]: ReadmatchToken(RMIDPAREN));
        if (sym->attrIR.idtype->Typekind != ARRAYTY) {//如果sym不是数组类型标识符，则返回错误
            Error_ArrayNotArrayType(tok->Lineshow, tok->Sem);
        }
        if (expParsing(RD_ROOT->child[1], scope, exit_region, TypeList) != TypeList[INTTY - '0']) {
            Error_ArrayGroupInvalid(tok->Lineshow, tok->Sem);
        }
        else {
            return sym->attrIR.idtype->More.elemTy;
        }
    }
    else if (RD_ROOT->child[0]->token->Lex == DOT) {
        //RD_ROOT->child[0]: ReadmatchToken(DOT));
        //RD_ROOT->child[1]: fieldVar());
        if (sym->attrIR.idtype->Typekind != RECORDTY) {//如果sym不是记录类型标识符，则返回错误
            Error_RecordNotRecordType(tok->Lineshow, tok->Sem);
            return NULL;
        }
        return fieldVarParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, tok);
    }
    else {
        return NULL;
    }

}

//参数 ProcPosition 是过程标识符所在的符号表在scope栈的位置
void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, token* IDTok) {//根节点名称为"CallStmRest"，对应RD中的"callStmRest函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: LPAREN, 无操作
    //RD_ROOT->child[1]: actparamList()
    vector<struct TypeIR*> TypeVec;
    actparamListParsing(RD_ROOT->child[1], scope,exit_region, TypeList, ProcPosition, TypeVec);//得到调用过程时的参数信息

    //使用scope[ProcPosition][0]表示此过程标识符
    struct ParamTable* param = scope[ProcPosition][0].attrIR.More.ProcAttr.param;//得到声明过程时的参数表
    struct ParamTable* paramP = param;

    int paramSize = 0;
    while (paramP != NULL) {
        paramSize++;
        paramP = paramP->next;
    }
    if ( paramSize != TypeVec.size() ) {
        //形实参数个数不相同
        Error_ProcParamAmount(IDTok->Lineshow, IDTok->Sem);
    }
    else {
        paramP = param;
        for (int ix = 0; ix < TypeVec.size() && paramP != NULL; ix++) {
            //使用scope[ProcPosition][ix+1]访问此过程声明的第ix+1个参数
            //使用TypeList[ix]访问调用过程的第ix个参数的类型
            if (scope[ProcPosition][ix + 1].attrIR.idtype == NULL || TypeVec[ix] == NULL) {
                //形实参中存在NULL，表示参数类型未定义，则形实参数类型一定不匹配
                Error_ProcParamType(IDTok->Lineshow, ix + 1, IDTok->Sem);
            }
            else if(scope[ProcPosition][ix + 1].attrIR.idtype != TypeVec[ix]){
                //形实参数类型一定不匹配
                Error_ProcParamType(IDTok->Lineshow, ix + 1, IDTok->Sem);
            }
        }
    }
    //RD_ROOT->child[2]: RPAREN, 无操作

}
//VarSym是指被赋值的变量的符号表信息，IDtok是指被赋值的变量的token信息，目的是传递行数
void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* VarSym, token* IDTok) {//根节点名称为"AssignmentRest"，对应RD中的"assignmentRest函数"
    if (RD_ROOT == NULL) { return; }
    struct TypeIR* TypeP1 = NULL;
    struct TypeIR* TypeP2 = NULL;
    if ( 0 == strcmp(RD_ROOT->child[0]->str, "VariMore") ) {//variMore()
        //RD_ROOT->child[0]: variMore()
        TypeP1 = variMoreParsing(RD_ROOT->child[0], scope, exit_region, TypeList, VarSym, IDTok);
        //RD_ROOT->child[1]: ASSIGN，无操作
        //RD_ROOT->child[2]: exp()
        TypeP2 = expParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    }
    else {
        //RD_ROOT->child[0]: ASSIGN，无操作
        TypeP1 = VarSym->attrIR.idtype;
        //RD_ROOT->child[1]: exp()
        TypeP2 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    }
    if (WhetherResaultValid(TypeP1, TypeP2, RD_ROOT->child[0]->token) == NULL) {//说明赋值语句两边类型不相容
        Error_AssignContentIncompatible(IDTok->Lineshow, IDTok->Sem);
    }
}


//赋值或者调用过程，返回ID的情况，如果ID不是过程标识符的话，则还要返回对应的类型(Int, Char, Array, Record)
//参数IDtoke是ID的token信息
void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token* IDtok, const int ValidTableCount) {//根节点名称为"AssCall"，对应RD中的"assCall函数"
    if (RD_ROOT == NULL) { return ; }
    if ( 0 == strcmp(RD_ROOT->child[0]->str, "CallStmRest") ){//过程调用callStmRest()
        int ProcPosition = FindProc(IDtok->Sem, true, scope, exit_region, ValidTableCount);//找此名称的过程标识符，找不到则返回0，后续使用scope[level][0]表示此过程标识符
        if (ProcPosition == 0) {
            Error_ProcNotProcIdentifier(IDtok->Lineshow, IDtok->Sem);//不是过程标识符
            return;
        }
        else {
            callStmRestParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ProcPosition, IDtok);
        }
        
    }
    else if( 0 == strcmp(RD_ROOT->child[0]->str, "AssignmentRest") ){//赋值assignmentRest()
        SymbTable* VarSym = FindEntry(IDtok->Sem, true, scope, exit_region, VARKIND, -1);//找此名称的变量标识符，找不到则返回空
        if (VarSym == NULL) {
            Error_AssignNotVarIdentifier(IDtok->Lineshow, IDtok->Sem);//不是变量标识符
            return;
        }
        else {
            assignmentRestParsing(RD_ROOT->child[0], scope, exit_region, TypeList, VarSym, IDtok);
        }
        
    }
    
}

void returnStmParsing() {//根节点名称为"ReturnStm"，对应RD中的"returnStm函数"
    //returnStm中都是token，无函数，只有语法错误，不会出现语义错误，不需要检测错误


}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"OutputStm"，对应RD中的"outputStm函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: WRITE，无操作
    //RD_ROOT->child[1]: LPAREN，无操作
    //RD_ROOT->child[2]: exp()
    expParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    //RD_ROOT->child[3]: RPAREN，无操作
}

void inputStmParsing() {//根节点名称为"InputStm"，对应RD中的"inputStm函数"
    //inputStm中都是token，无函数，只有语法错误，不会出现语义错误，不需要检测错误
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为""，对应RD中的"loopStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: WHILE，无操作
    //RD_ROOT->child[1]: exp()，第一个表达式
    struct TypeIR* TypeP1 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    //RD_ROOT->child[2]: LT或者EQ，比较符号('<', '=')，无操作
    //RD_ROOT->child[3]: exp()，第二个表达式
    struct TypeIR* TypeP2 = expParsing(RD_ROOT->child[3], scope, exit_region, TypeList);
    WhetherCompareValid(TypeP1, TypeP2, RD_ROOT->child[2]->token);
    //RD_ROOT->child[4]: DO，无操作
    //RD_ROOT->child[5]: stmList()
    stmListParsing(RD_ROOT->child[5], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[6]: ENDWH，无操作
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为""，对应RD中的"conditionalStm函数"********************************************
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: IF，无操作
    //RD_ROOT->child[1]: exp()，第一个表达式
    struct TypeIR* TypeP1 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    //RD_ROOT->child[2]: LT或者EQ，比较符号('<', '=')，无操作
    //RD_ROOT->child[3]: exp()，第二个表达式
    struct TypeIR* TypeP2 = expParsing(RD_ROOT->child[3], scope, exit_region, TypeList);
    WhetherCompareValid(TypeP1, TypeP2, RD_ROOT->child[2]->token);
    //RD_ROOT->child[4]: THEN，无操作
    //RD_ROOT->child[5]: stmList()
    stmListParsing(RD_ROOT->child[5], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[6]: ELSE，无操作
    //RD_ROOT->child[7]: stmList()
    stmListParsing(RD_ROOT->child[7], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[8]: FI，无操作

}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"StmMore"，对应RD中的"stmMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: SEMI， 无操作
    //RD_ROOT->child[1]: stmList());
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


//-----------------------------------------------
void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"Stm"，对应RD中的"stm函数"
    if (RD_ROOT == NULL) { return; }
    if(0 == strcmp(RD_ROOT->child[0]->str, "InputStm")){
        inputStmParsing();
    }
    else if (0 == strcmp(RD_ROOT->child[0]->str, "OutputStm")) {
        outputStmParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    }
    else if (0 == strcmp(RD_ROOT->child[0]->str, "ReturnStm")) {
        returnStmParsing();
    }
    else if (0 == strcmp(RD_ROOT->child[0]->str, "ConditionalStm")) {
        conditionalStmParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);//IF
    }
    else if (0 == strcmp(RD_ROOT->child[0]->str, "LoopStm")) {
        loopStmParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);//WHILE
    }
    else {//赋值或者调用过程
        assCallParsing(RD_ROOT->child[1], scope, exit_region, TypeList, RD_ROOT->child[0]->token, ValidTableCount);
    }
    

}

//body内，表达式List(List and ListMore)
void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"StmList"，对应RD中的"stmList函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]:newnode, stm());
    stmParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[1]:newnode, stmMore());
    stmMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


void fidMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"FidMore"，对应RD中的"fidMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, 无操作;
    //RD_ROOT->child[1]: formList(),继续读取ID的token
    formListParsing(RD_ROOT->child[1], token);
}

//过程参数的ID，得到token
void formListParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"FormList"，对应RD中的"formList函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: fidMore());
    fidMoreParsing(RD_ROOT->child[1], token);

}

struct ParamTable* paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"ParamMore"，对应RD中的"paramMore函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: SEM, 无操作;
    //RD_ROOT->child[1]: paramDecList()
    return paramDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);

}

//此函数返回一个过程参数链表，因为需要考虑到程序体的过程调用时参数个数的匹配，所以如果此处的Type为非法，也会压入符号表，Type处设置为NULL，但是这些标识符不显示在符号表中
struct ParamTable* paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"Param"，对应RD中的"param函数"
    if (RD_ROOT == NULL) { return NULL; }
    unsigned int VarExist = 0;
    if (0 == strcmp(RD_ROOT->child[0]->str,"TypeDef")) {
        VarExist = 0;
    }
    else {
        VarExist = 1;
    }
    //RD_ROOT->child[0+ VarExist]: typeDef()
    AttributeIR* tempAttr = typeDefParsing(RD_ROOT->child[0 + VarExist], scope, exit_region, TypeList);//找到所需要的属性
    if (tempAttr == NULL) {//如果没有找到对应标识符，则创建一个Type为NULL的属性
        tempAttr = new AttributeIR;
        tempAttr->idtype = NULL;
    }
    tempAttr->kind = VARKIND;
    tempAttr->More.VarAttr.level = ValidTableCount;
    if (VarExist == 0) {
        tempAttr->More.VarAttr.access = DIR;
    }
    else {
        tempAttr->More.VarAttr.access = INDIR;
    }
    
    SymbTable* Symtemp = FindEntry("*", false, scope, exit_region, VARKIND, -1);//找到当前符号表最新的一个变量标识符用作偏移计算
    
    int Off = 0;//计算当前符号表内的总偏移，参数相对于过程的偏移=自身size+总偏移
    if (Symtemp != NULL) {
        unsigned int size = 0;
        if (Symtemp->attrIR.idtype != NULL) {//如果idtype为NULL， 则理解为size为0
            size = Symtemp->attrIR.idtype->size;
        }
        else {
            size = 0;
        }
        Off = Symtemp->attrIR.More.VarAttr.off + size;
    }
    else {
        Off = 0;
    }
    struct ParamTable* temp = NULL;
    struct ParamTable* rear = NULL;
    vector<token*> token;//存储formList中所有token

    //RD_ROOT->child[1 + VarExist]: formList()
    formListParsing(RD_ROOT->child[1 + VarExist], token);//token.size()一定大于0，否则不可能通过语法分析
    const int size_origion = scope[scope.size() - 1].size();
    const int token_size = token.size();
    int token_Off = 0;//token数组中的偏移
    int VarSize = 0;//此类标识符的size
    if (tempAttr->idtype != NULL) {
        VarSize = tempAttr->idtype->size;
    }
    for (int ix = 0; ix < token_size; ix++) {
        tempAttr->More.VarAttr.off = Off + token_Off;//符号表总偏移加上标识符在token数组中的偏移
        token_Off += VarSize;//token偏移加上此类标识符的大小

        if (Enter(token[ix]->Sem, *tempAttr, scope, exit_region, VARKIND, -1) == false) {//压入符号栈
            Error_IdentifierDuplicateDec(token[ix]->Lineshow, token[ix]->Sem);//标识符重命名
            Enter(WRONGID, *tempAttr, scope, exit_region, VARKIND, -1);//将WrongID插入符号表，表示标识符命名非法
        }

        struct ParamTable* tempptr = new struct ParamTable;
        tempptr->next = NULL;
        if (temp == NULL) {
            temp = tempptr;
        }
        else {
            rear->next = tempptr;
        }
        rear = tempptr;
        rear->entry = ix + size_origion;//此符号栈中元素的数量即为目前参数在局部化区中的位置(在token中的位置ix + 局部化区内原本的标识符的数量)
    }
    return temp;
}

struct ParamTable* paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"ParamDecList"，对应RD中的"paramDecList函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: param()
    struct ParamTable* temp = paramParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    // RD_ROOT->child[1]: paramMore()
    if (temp != NULL) {//tmep被设定为不可能为空（paramParsing函数返回值人为设计为非空），此处属于冗余操作
        struct ParamTable* rearptr = temp;
        while (rearptr-> next != NULL) {//找到链表的最后一个节点，然后连接上paramMoreParsing()
            rearptr = rearptr->next;
        }
        rearptr->next = paramMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    }
    else {
        temp = paramMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    }
    
    return temp;
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProcBody"，对应RD中的"procBody函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0], programBody()
    programBodyParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//大写D， procDecPart, 负责定义过程中的标识符（类型标识符，变量标识符，过程标识符）
void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProcDecPart"，对应RD中的"procDecPart函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0], procDec()
    declarePartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//将过程参数压入scope栈，并返回过程参数表
struct ParamTable* paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"ParamList"，对应RD中的"paramList函数"
    if (RD_ROOT == NULL) { return NULL; }
    return paramDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//整个过程树
void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//根节点名称为"ProcDec"，对应RD中的"procDec函数"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;
    //RD_ROOT->child[0]: PROCEDURE, 无操作
    //RD_ROOT->child[1]: ID
    strcpy(temp.idname, RD_ROOT->child[1]->token->Sem);//idname
    temp.attrIR.idtype = NULL;//Typeptr
    temp.attrIR.kind = PROCKIND;//kind
    temp.attrIR.More.ProcAttr.level = ValidTableCount - 1;//level，过程仍属于上一层

    if (Enter(temp.idname, temp.attrIR, scope, exit_region, PROCKIND, temp.attrIR.More.ProcAttr.level) == false) {//压入符号栈，需考虑过程的Level
        Error_IdentifierDuplicateDec(RD_ROOT->child[1]->token->Lineshow, RD_ROOT->child[1]->token->Sem);//标识符重命名
        const int scope_origin_size = scope.size() - 1;//因为此前已经创建了此过程的符号表，所以scope原本的size等于现在的size-1
        //RD_ROOT->child[7]: procBody()
        procBodyParsing(RD_ROOT->child[7], scope, exit_region, TypeList, ValidTableCount);//先检测body中的语义错误，再移除此局部化区
        //如果过程标志符，而最新的局部化区为这个过程的标识符，所以此过程的局部化区以及其body中定义的全部局部化区应该全部被删除
        const int times = scope.size() - scope_origin_size;//通过scope原本的size来将scope栈还原回加入此局部化区原本的样子，需要弹scope栈的次数
        for (int ix = 0; ix < times; ix++) {
            scope.pop_back();
            exit_region.pop_back();
        }
    }
    else {
        //RD_ROOT->child[2]: LPAREN, 无操作
        //RD_ROOT->child[3]: paramList()
        const int scope_size = scope.size();
        scope[scope_size - 1][0].attrIR.More.ProcAttr.param = paramListParsing(RD_ROOT->child[3], scope, exit_region, TypeList, ValidTableCount);
        scope[scope_size - 1][0].attrIR.More.ProcAttr.code = 0;//Code,过程的目标代码地址（代码生成阶段填写，现在初始化为0）
        //RD_ROOT->child[4]: RPAREN, 无操作
        //RD_ROOT->child[5]: SEMI, 无操作
        //RD_ROOT->child[6]: procDecPart()
        procDecPartParsing(RD_ROOT->child[6], scope, exit_region, TypeList, ValidTableCount);
        //计算Size
        //scope[scope_size - 1][0]表示这个符号表的过程标识符
        scope[scope_size - 1][0].attrIR.More.ProcAttr.size = 0;
        for (int ix = scope[scope_size - 1].size() - 1; ix >= 1; ix--) {
            if (scope[scope_size - 1][ix].attrIR.kind == VARKIND) {
                int size = 0;//表示这个过程的最后一个变量标识符的大小
                if (scope[scope_size - 1][ix].attrIR.idtype != NULL) {
                    size = scope[scope_size - 1][ix].attrIR.idtype->size;
                }
                scope[scope_size - 1][0].attrIR.More.ProcAttr.size += size;
            }
        }
        ;
        /*SymbTable* SymPtr = FindEntry("*", false, scope, exit_region, VARKIND, -1);//找到当前符号表最新的一个变量标识符用作size计算
        if (SymPtr == NULL) {//此标识符为当前符号表最新的一个变量标识符，size设置为0
            scope[scope_size - 1][0].attrIR.More.ProcAttr.size = 0;
        }
        else {//否则等于当前符号表最新的一个变量标识符的size+偏移
            unsigned int size = 0;
            if (SymPtr->attrIR.idtype != NULL) {//如果idtype为NULL， 则理解为size为0
                size = SymPtr->attrIR.idtype->size;
            }
            else {
                size = 0;
            }
            scope[scope_size - 1][0].attrIR.More.ProcAttr.size = SymPtr->attrIR.More.VarAttr.off + size;
        }*/
        //RD_ROOT->child[7]: procBody()
        procBodyParsing(RD_ROOT->child[7], scope, exit_region, TypeList, ValidTableCount);
        //RD_ROOT->child[8]: procDecpart()
        procDecpartParsing(RD_ROOT->child[8], scope, exit_region, TypeList, ValidTableCount);

    }
}

void varIDMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"VarIDMore"，对应RD中的"varIDMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, 无操作
    //RD_ROOT->child[1]: IDList()
    varIDListParsing(RD_ROOT->child[1], token);
    return;
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecMore"，对应RD中的"varDecMore函数"
    if (RD_ROOT == NULL) { return; }
    varDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

void varIDListParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"VarIDList"，对应RD中的"varIDList函数"
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: varIDMore()
    varIDMoreParsing(RD_ROOT->child[1], token);
    return;
}
//处理变量声明
void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"VarDecList"，对应RD中的"varDecList函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: typeDef()
    AttributeIR* Attr = typeDefParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    if (Attr != NULL) {
        Attr->kind = VARKIND;//变量标识符
        Attr->More.VarAttr.access = DIR;//直接变量
        Attr->More.VarAttr.level = ValidTableCount;//层数
        //RD_ROOT->child[1]: varIDList()
        vector<token*> token;//存储IDList内的所有token
        varIDListParsing(RD_ROOT->child[1], token);
        int token_size = token.size();
        for (int ix = 0; ix < token_size; ix++) {
            SymbTable* Symtemp = FindEntry("*", false, scope, exit_region, VARKIND, -1);//找到当前符号表最新的一个变量标识符用作偏移计算
            if (Symtemp == NULL) {//此标识符为当前符号表最新的一个变量标识符，偏移设置为0
                Attr->More.VarAttr.off = 0;
            }
            else {//否则等于当前符号表最新的一个变量标识符的size+偏移
                int size = 0;
                if (Symtemp->attrIR.idtype != NULL) {
                    size = Symtemp->attrIR.idtype->size;
                }
                Attr->More.VarAttr.off = Symtemp->attrIR.More.VarAttr.off + size;
            }
            //向符号表内插入符号
            if (Enter(token[ix]->Sem, *Attr, scope, exit_region, VARKIND, -1) == false) {
                Error_IdentifierDuplicateDec(token[ix]->Lineshow, token[ix]->Sem);//标识符重命名
            }
        }
    }
    
    //RD_ROOT->child[2]: SEMI, 无操作
    //RD_ROOT->child[3]: varDecMore()
    varDecMoreParsing(RD_ROOT->child[3], scope, exit_region, TypeList, ValidTableCount);

}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//根节点名称为"VarDec"，对应RD中的"varDec函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: VAR, 无操作
    //RD_ROOT->child[1]: varDecList()
    varDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


void IDMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"IDMore"，对应RD中的"IDMore函数"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, 无操作
    //RD_ROOT->child[1]: IDList()
    IDListParsing(RD_ROOT->child[1], token);
    return;
}

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<token*>& token) {//根节点名称为"FieldDecMore"，对应RD中的"fieldDecMore函数"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]:fieldDecList()
    return fieldDecListParsing(RD_ROOT->child[0], TypeList, token);
}

void IDListParsing(treenode* RD_ROOT, vector<token*>& token) {//根节点名称为"IDList"，对应RD中的"IDList函数"，StrVec是最后所需要的ID表
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: IDMore()
    IDMoreParsing(RD_ROOT->child[1], token);
    return;
}

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<token*>& token) {//根节点名称为"FieldDecList"，对应RD中的"fieldDecList函数"
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
        AttributeIR* tempAttr = arrayTypeParsing(RD_ROOT->child[0], TypeList);
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
    int token_size1 = token.size();//token未增长前
    IDListParsing(RD_ROOT->child[1], token);//得到IDList中的所有token
    if (token.size() - token_size1 == 0) {//ID数量为0
        delete temp;
        temp = NULL;
    }

    if (temp == NULL) {//ID数量为0
        return fieldDecMoreParsing(RD_ROOT->child[3], TypeList, token);
    }
    else {
        strcpy(temp->idname, token[token_size1 + 0]->Sem);
        fieldChain* rear = temp;
        for (int ix = token_size1 + 1; ix < token.size(); ix++) {//将所有token都生成为fieldChain，并接入链表
            fieldChain* tempID = new fieldChain;
            tempID->unitType = temp->unitType;//所有ID都是相同的类型
            strcpy(tempID->idname, token[ix]->Sem);
            rear->next = tempID;
            rear = tempID;
        }
        //RD_ROOT->child[2]:SEMI，无操作
        //RD_ROOT->child[3]:fieldDecMore()
        rear->next = fieldDecMoreParsing(RD_ROOT->child[3], TypeList, token);
        return temp;
    }
    
}

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"RecType"，对应RD中的"recType函数"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START 新建一个TypeIR，如果是此类型的话不存在于TypeList的话，则插入TypeList，否则给TypeIR赋值已经存在于TypeList的类型的地址------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = RECORDTY;//记录类型Typeind
    //RD_ROOT->child[0]: RECORD, 无操作
    //RD_ROOT->child[1]: fieldDecList()
    vector<token*> token;
    fieldChain* body = fieldDecListParsing(RD_ROOT->child[1], TypeList, token);
    //RD_ROOT->child[2]: END, 无操作
    if (body == NULL) {
        delete temp;
        temp = NULL;
        return NULL;
    }
    fieldChain* head = body;
    unordered_map <string, int> UnMap;
    bool flag = false;
    for (int ix = 0; head != NULL && ix<token.size(); ix++) {//token与body节点一定一一对应
        string str = head->idname;//将字符数组转化成string类型
        if (UnMap.find(str) != UnMap.end()) {//ID如果已经在map内，则表示Record中标识符重复声明
            Error_IdentifierDuplicateDecRecord(token[ix]->Lineshow, token[ix]->Sem);//Record中标识符重复声明
            flag = true;
        }
        else {
            UnMap[str] = 0;//ID入Map
        }
        head = head->next;
    }
    if (flag == true) {
        delete temp;
        temp = NULL;
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

    struct TypeIR* existTy = WhetherTypeDuplicate(TypeList, tempTypeIR, RECORDTY);
    if (existTy == NULL) {
        TypeList.push_back(tempTypeIR);
        temp->idtype = TypeList[TypeList.size() - 1];
    }
    else {
        temp->idtype = existTy;
    }
    //END 新建一个TypeIR，如果是此类型的话不存在于TypeList的话，则插入TypeList，否则给TypeIR赋值已经存在于TypeList的类型的地址------------------------------------------------------------------------------------
    return temp;
}

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//根节点名称为"ArrayType"，对应RD中的"arrayType函数"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START 新建一个TypeIR，如果是此类型的话不存在于TypeList的话，则插入TypeList，否则给TypeIR赋值已经存在于TypeList的类型的地址------------------------------------------------------------------------------------
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

    struct TypeIR* existTy = WhetherTypeDuplicate(TypeList, tempTypeIR, ARRAYTY);
    if (existTy == NULL) {
        TypeList.push_back(tempTypeIR);
        temp->idtype = TypeList[TypeList.size() - 1];
    }
    else {
        temp->idtype = existTy;
    }
    //END 新建一个TypeIR，如果是此类型的话不存在于TypeList的话，则插入TypeList，否则给TypeIR赋值已经存在于TypeList的类型的地址------------------------------------------------------------------------------------
    
    return temp;
}
//-----------------------------------------------------------------
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
        SymbTable* Sym = FindEntry(RD_ROOT->child[0]->token->Sem, true, scope, exit_region, TYPEKIND, -1);
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
        bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region, TYPEKIND, -1);//插入标识符
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

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//根节点名称为"TypeDecPart"，对应RD中的"typeDecPart函数"
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
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT) {//根节点名称为"ProgramHead"，对应RD中的"programHead函数"
    if (RD_ROOT == NULL) { return; }
    //程序头如果不加入符号表的话就没有用处，这里不考虑程序头
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//解析语法树,根节点名称为"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,新建一个空符号表
    programHeadParsing(RD_ROOT->child[0]);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token中只有ID, INTC, CHARC含有语义信息，分别为标识符名称、整数、字符，且都是字符数组形式

int semantic_analysis(treenode* RD_ROOT) 
{
    //定义变量
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出
    vector<struct TypeIR*> TypeList;//存放类型定义，前三个固定，分别为[0]:int, [1]:char, [2]bool,可以使用 TypeList[INTTY - '0'] 来访问int
    int ValidTableCount = 0;

    //初始化函数
    InitFile(ERROR_FILE);//清空存储语义错误信息的文件

    initialize(TypeList);////初始化三种基本类型

    //解析语法树
    RDTreeParsing(RD_ROOT, scope, exit_region, TypeList, ValidTableCount);

    //打印类型表和符号表到文件
    //PrintTable(scope, TypeList, TABLE_FILE_XLS);
    PrintTable(scope, TypeList, TABLE_FILE_CSV);
    return 1;
}

//2022_4_17