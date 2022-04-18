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

SymbTable* SearchSingleTable(const char* id, vector< vector<SymbTable> >& scope, int level, const char kind, const int ProcLevel) {//���ұ�ʶ���Ƿ�������Ӧ����,kind��ʾ�����ʶ��������('*'��ʾȫ��ƥ��),�����ǹ��̱�ʶ����Level�Ļ���Ҫ��ProcLevelС����
    int position = level - 1;
    SymbTable* temp = NULL;
    for (int ix = scope[position].size() - 1; ix >= 0; ix--) {
        
        
        if (scope[position][ix].attrIR.kind != PROCKIND) {//1. id ����"*"����idname; 2. kind����'*'����attrIR.kind; 3.���ǹ��̱�ʶ��
            if ((0 == strcmp("*", id) || 0 == strcmp(scope[position][ix].idname, id)) && (scope[position][ix].attrIR.kind == kind || kind == '*')) {
                temp = &(scope[position][ix]);
                return temp;//���������ʶ���ĵ�ַ
            }
        }
        else {//1. �ǹ��̱�ʶ��; 2. id ����"*"����idname; 3. kind����'*'����attrIR.kind;  4.  Level��ͬ����ProcLevel<0 
            if ((0 == strcmp("*", id) || 0 == strcmp(scope[position][ix].idname, id)) && (scope[position][ix].attrIR.kind == kind || kind == '*') &&  (scope[position][ix].attrIR.More.ProcAttr.level == ProcLevel || ProcLevel < 0)) {
                temp = &(scope[position][ix]);
                return temp;//���������ʶ���ĵ�ַ
            }
        }
        
    }
    return temp;//NULL: �����ڣ� �ǿ�: ��Ӧ��ʶ����ַ��Ϣ
}

SymbTable* FindEntry(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const char kind, const int ProcLevel) {//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���
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
    temp = SearchSingleTable(id, scope, level, kind, ProcLevel);//���ҵ�ǰ���ű�
    if (flag == true && temp == NULL) {//���flag == true���ҵ�ǰ���ű���δ���ҵ������ʶ��id���������������scopeջ
        level--;
        while (level > 0) {
            if (exit_region[level - 1] == false || (kind == PROCKIND && ProcLevel>=0)) {//�ҵ�δ�˳��ľֲ������Ĳ���������ǲ��ҹ��̱�ʶ�����ض��������kind == PROCKIND && ProcLevel>=0������һֱ�������
                temp = SearchSingleTable(id, scope, level, kind, ProcLevel);//���Ҵ˷��ű�
                if (temp != NULL) {
                    break;
                }
            }
            level--;
        }
    }
    return temp;//NULL: �����ڣ� �ǿ�: ��Ӧ��ʶ����ַ��Ϣ
}

//FindProc()����body��Ѱ����ȷ�Ĺ��̱�ʶ�������ط��������Ĺ��̱�ʶ����scopeջ��λ�ã�ʹ��scope[level][0]��ʾ�˹��̱�ʶ��
int FindProc(const char* id, bool flag, vector< vector<SymbTable> >& scope, vector<bool> exit_region, const int ValidTableCount){
    //ProcLevelֻ�ܵݼ���Ѱ�ң���������˵�n��ģ��Ͳ����ٲ��Ҵ���n���Proc��ʶ��
    /*�����������¹��̱�ʶ��
    * p1()        --- level 1
    *   p2()      --- level 2
    *      p3()   --- level 3
    * p4()        --- level 1
    *   p5()      --- level 2
    * ��p5��ʼ����Ѱ�ң�ֻ�ܰ���p5 -> p4 -> p1��˳��Ѱ�ң���Ϊp5���ܵ���p3��p2
    */
    int level = exit_region.size() - 1;//��ʾ�ڼ��㣬level == 0�൱��scopeջ����Ԫ�ء�ʹ�� n = level-1 ����scope[n][x]����exit_region[n]��
    int level_last = ValidTableCount;
    for (; level >= 1; level--) {//��һ�����ű�������û�ж���Proc��ʶ��
        //levelС�ڵ���1ʱ��scope[level][0]��ʾ�˱�־��Ϊ�˷��ű�Ĺ��̱�ʶ��
        if (scope[level].size() >= 1) {
            if (scope[level][0].attrIR.More.ProcAttr.level <= level_last) {//���ܴ�����һ�ε�level
                if (0 == strcmp(scope[level][0].idname, id)) {
                    return level;
                }
                level_last = scope[level][0].attrIR.More.ProcAttr.level;
            }
        }
    }
    return 0;
}

bool Enter(const char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region, char kind, const int ProcLevel) {//�ǼǱ�ʶ���������Ե����ű�,ע��������ǹ��̱�ʶ���Ļ�, ProcLevel�����壬����ֱ�Ӵ�ProcLevel = -1
    bool present = false;//��¼�Ǽ��Ƿ�ɹ�
    bool flag = false;//FindEntry�Ĵ��Σ���ʾֻ��ѯ��ǰ���ű���ȫ�����ű�
    if (kind == PROCKIND && ProcLevel >= 0) {
        flag = true;
    }
    else {
        flag = false;
    }
    if (FindEntry(id, flag, scope, exit_region, kind, ProcLevel) == NULL || 0 == strcmp(id, WRONGID) ) {//�����WrongID������ű���ʾ��ʶ�������Ƿ�������Ҳ������ű������ֻ���ڹ��̲�����ʶ�����ظ�����
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
        if (kind == PROCKIND) {//����ǹ��̱�ʶ��������嵽���µķ��ű����ǰ�棬���˹��̱�ʶ��Ӧ��Ϊ�˷��ű�ĵ�һ��
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
string KindToString(char TypeKind) {//Kind������ת�ַ���
    if (TypeKind == '0') { return "TypeKind"; }
    else if (TypeKind == '1') { return "VarKind "; }
    else if (TypeKind == '2') { return "ProcKind"; }
    return "";
}
string AccessToString(char access) {//Kind������ת�ַ���
    if (access == '0') { return "dir"; }
    else if (access == '1') { return "indir "; }
    return "";
}

void PrintTable(vector< vector<SymbTable> > scope, vector<struct TypeIR*> TypeList, string FilePath) {//��ӡ���ͱ�ͷ��ű��ļ�
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
    FileOut << "���ͱ�" << "\n";
    FileOut << "���" << TabKey << "Size" << TabKey << "kind" << TabKey << "indexTy" << TabKey << "elemTy" << TabKey << "Low" << TabKey << "body{�������; ������; ƫ��}" << "\n";
    const int TypeList_size = TypeList.size();
    for (int ix = 0; ix < TypeList_size; ix++) {//��ӡ���ͱ�
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

    //��ӡscopeջ�����ű�
    FileOut << "\n���ű�" << "\n";
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
                else {//KindΪVarKind��TypePtrΪ�յ�����������̵Ĳ��������ʹ�����������������ű�Ŀ���Ǵ������˱�����ֻ�е�������ʶ���ض���ʱ�Ų����˲���������ű�
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
                    FileOut << EMPTYSPACE << TabKey;//���code���ڳ�ʼ����0�����յ�ַ���򷵻ؿ�
                }
                else {//���򷵻ص�ַ
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

void Error_IdentifierDuplicateDecRecord(int line, string sem) {// 2.Record��ʶ���еı�ʶ�����ظ����壬��Ӧ�����������(1)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��record��ʶ�������У���ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�ظ�����\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_IdentifierUndeclared(int line, string sem) {// 3.�������ı�ʶ������Ӧ�����������(2)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"δ����\n";
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

void Error_ArrayNotArrayType(int line, string sem) {// 3.body��ʹ�õı�ʶ�����������ʶ�������������������(3)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ����\"";
    ErrorMessage += sem;
    ErrorMessage += "\"������������\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

void Error_ArrayGroupInvalid(int line, string sem) {// 3.�����Ա���������ò��Ϸ������������������(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ����\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�����ò��Ϸ�\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

/***��¼***/
void Error_RecordFieldInvalid(int line, string sem1, string sem2) {// 1.��¼�����Ա���������ò��Ϸ������������������(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��¼\"";
    ErrorMessage += sem1;
    ErrorMessage += "\"�в����������\"";
    ErrorMessage += sem2;
    ErrorMessage += "\"\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordFieldArrayInvalid(int line, string Rsem1, string Asem2) {// 2.��¼�������������Ա���������ò��Ϸ������������������(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��¼\"";
    ErrorMessage += Rsem1;
    ErrorMessage += "\"�е������������Ա\"";
    ErrorMessage += Asem2;
    ErrorMessage += "\"�ĳ�Ա�������ò��Ϸ�\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordFieldNotArrayType(int line, string Rsem1, string Asem2) {// 3.body��ʹ�õ����Ա�������������ʶ�������������������(5)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��¼\"";
    ErrorMessage += Rsem1;
    ErrorMessage += "\"�е����Ա\"";
    ErrorMessage += Asem2;
    ErrorMessage += "\"�Ĳ����������ͣ����ò��Ϸ�\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_RecordNotRecordType(int line, string sem) {// 4.body��ʹ�õı�ʶ�����Ǽ�¼��ʶ�������������������(3)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ����\"";
    ErrorMessage += sem;
    ErrorMessage += "\"���Ǽ�¼����\n";
    PrintFile(ErrorMessage, ERROR_FILE);

}

/***���̵���***/
void Error_ProcParamType(int line, int ParamSerial, string ProcName) {// 1.���̵�������У���ʵ�����಻ƥ�䣬��Ӧ�����������(8)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ���̵���\"";
    ErrorMessage += ProcName;
    ErrorMessage += "\"��, �� ";
    ErrorMessage += to_string(ParamSerial);
    ErrorMessage += " ���������Ͳ�ƥ��";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_ProcParamAmount(int line, string sem) {// 2.���̵�������У���ʵ�θ�������ͬ����Ӧ�����������(9)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ���̵���\"";
    ErrorMessage += sem;
    ErrorMessage += "\"��ʵ�θ�����ƥ��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_ProcNotProcIdentifier(int line, string sem) {// 3.���̵�������У���ʶ�����ǹ��̱�ʶ������Ӧ�����������(10)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"���ǹ��̱�ʶ��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***��ֵ***/
void Error_AssignContentIncompatible(int line, string sem) {// 1.��ֵ����У������������Ͳ����ݣ���Ӧ�����������(6)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�븳ֵ�������Ͳ�����\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

void Error_AssignNotVarIdentifier(int line, string sem) {// 2.��ֵ����У���ʶ�����Ǳ�����ʶ������Ӧ�����������(7)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",   ��ʶ��\"";
    ErrorMessage += sem;
    ErrorMessage += "\"���Ǳ�����ʶ��\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***���ʽ***/
void Error_StmOpComponentIncompatibility(int line, string sem) {// 1.�����ֵ�α��ʽ��������ķ��������Ͳ����ݣ���Ӧ�����������(12)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",  �����\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�ķ��������Ͳ�����\n";
    PrintFile(ErrorMessage, ERROR_FILE);
}

/***�������***/
void Error_StmCompareComponentIncompatibility(int line, string sem) {// 1.�������ʽ��������ķ��������Ͳ����ݣ���Ӧ�����������(11)
    string ErrorMessage = "Line:";
    ErrorMessage += to_string(line);
    ErrorMessage += ",  �Ƚ������\"";
    ErrorMessage += sem;
    ErrorMessage += "\"�ıȽϽ�����Ϸ�\n";
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
//�����ͱ������Type�����Ƿ��Ѿ����ڴ��ڣ����ڵĻ��������ַ�����򷵻ؿ�
//Typekind�Ǵ����ʶ�������ͣ��������ͻ��߼�¼���ͣ�
struct TypeIR* WhetherTypeDuplicate(vector<struct TypeIR*>& TypeList, struct TypeIR* Type, char Typekind) {
    if (Typekind == ARRAYTY) {
        for (int ix = TypeList.size() - 1; ix >= 0; ix--) {
            //�������ͣ��������Է�ȫ����ͬ
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
            //��¼���ͣ��������Է�ȫ����ͬ
            if (TypeList[ix]->size == Type->size && TypeList[ix]->Typekind == Type->Typekind) {
                fieldChain* body1 = TypeList[ix]->More.body;
                fieldChain* body2 = Type->More.body;
                bool flag = true;
                //����body�����Ƿ�ȫ����ͬ
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
//tok���������token��Ϣ
struct TypeIR* WhetherResaultValid(struct TypeIR* TypeP1, struct TypeIR* TypeP2 ,token* tok) {//�ڼ����ֵ���ʽ�У�����TypeP1��TypeP2�Ƿ�ƥ�䣬�������������ƥ������ͻ���NULL(��ƥ��)
    /*ʲô����·���NULL(��ʾ������Ϸ�)
    * 1. TypeP1==NULL || TypeP2==NULL
    * 2. ��TypeP1->Typekind != TypeP2->Typekind ʱ��һ�����Ϸ�
    * 3. ��TypeP1 == TypeP2 ʱ��TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY���������Ͳ��ܺ���������ֱ�Ӽ��㣬��¼����Ҳ��һ����
    * */
    bool flag = true;
    if (TypeP1 == NULL || TypeP2 == NULL) {
        flag = false;
    }
    else {
        if (TypeP1->Typekind != TypeP2->Typekind) {
            flag = false;
        }
        else if (TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY) {//��TypeP1 == TypeP2
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
//tok���������token��Ϣ
void WhetherCompareValid(struct TypeIR* TypeP1, struct TypeIR* TypeP2, token* tok) {//�ڱȽϱ��ʽ�У����TypeP1��TypeP2�Ƿ�ƥ�䣬�������������ƥ������ͻ���NULL(��ƥ��)
    /*ʲô����·���NULL(��ʾ������Ϸ�)
    * 1. TypeP1==NULL || TypeP2==NULL
    * 2. ��TypeP1->Typekind != TypeP2->Typekind ʱ��һ�����Ϸ�
    * 3. ��TypeP1 == TypeP2 ʱ��TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY���������Ͳ��ܺ���������ֱ�Ӽ��㣬��¼����Ҳ��һ����
    * */
    bool flag = true;
    if (TypeP1 == NULL || TypeP2 == NULL) {
        flag = false;
    }
    else {
        if (TypeP1->Typekind != TypeP2->Typekind) {
            flag = false;
        }
        else if (TypeP1->Typekind == ARRAYTY || TypeP1->Typekind == RECORDTY) {//��TypeP1 == TypeP2
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

//�жϼ�¼���ͱ�ʶ���ڵ������������Ա�����ĳ�Ա�����Ƿ�Ϸ����Ϸ��ͷ�������Ԫ�ص�����
struct TypeIR* fieldVarMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, token* Arraytok, struct TypeIR* ArrayTy) {//���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: LMIDPAREN���޲���
    //RD_ROOT->child[1]: exp()���жϼ�¼���ͱ�ʶ���ڵ������������Ա�����ĳ�Ա�����Ƿ�Ϸ�
    if (expParsing(RD_ROOT->child[1], scope, exit_region, TypeList) != TypeList[INTTY - '0']) {
        Error_RecordFieldArrayInvalid(Arraytok->Lineshow, sym->idname, Arraytok->Sem);
        return NULL;
    }
    else {
        return ArrayTy->More.elemTy;//����Ԫ�ص�����
    }
    //RD_ROOT->child[2]: RMIDPAREN���޲���
}

//���ؼ�¼���͵����Ա����,tok��Record���ͱ�����token��Ϣ��Ŀ���Ǵ��ݴ˱���������
struct TypeIR* fieldVarParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* sym, token* tok) {//���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: ID����ID�Ǽ�¼���͵����ԱID
    struct FieldChain* body = sym->attrIR.idtype->More.body;
    bool flag = false;//�ڴ˼�¼��ʶ���ڴ治���ڶ�Ӧ�����Ա
    while (body != NULL) {
        if (0 == strcmp(body->idname, RD_ROOT->child[0]->token->Sem)) {//�ҵ��˶�Ӧ�������
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
        if (RD_ROOT->child[1] == NULL) {//���Ա��BaseType
            return body->unitType;
        }
        else {//���Ա��AyyarType
            //RD_ROOT->child[1]: fieldVarMore()
            if (body->unitType->Typekind == ARRAYTY) {
                return fieldVarMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, RD_ROOT->child[0]->token, body->unitType);
            }
            else {//��������������ͣ����൱�ڷ����˷Ƿ�����
                Error_RecordFieldNotArrayType(RD_ROOT->child[0]->token->Lineshow, tok->Sem, RD_ROOT->child[0]->token->Sem);
                return NULL;
            }
            
        }
    }
    

}

//����������߼�¼�����ͣ�������ź��ӽڵ㲻Ϊ�գ��򷵻�������¼�ĳ�Ա����
struct TypeIR* variableParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: ID��������ID
    SymbTable* sym = FindEntry(RD_ROOT->child[0]->token->Sem, true, scope, exit_region, VARKIND, -1);
    if (sym == NULL) {//δ�ҵ��˱�ʶ��
        Error_IdentifierUndeclared(RD_ROOT->child[0]->token->Lineshow, RD_ROOT->child[0]->token->Sem);
        return NULL;
    }
    else {
        //RD_ROOT->child[1]: variMore());
        if (RD_ROOT->child[1] == NULL) {//ID������'[' ���� '.'
            return sym->attrIR.idtype;
        }
        else{
            return variMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, RD_ROOT->child[0]->token);
        }
    }
}

void multOpParsing() {//���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"
    //�˽ڵ����Ƿ��ţ��޲���
    return;
}

//tok�������ڵõ�otherFactor�ڵķ��ŵ�token��Ϣ
struct TypeIR* otherFactorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token*& tok) {//���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: multOp���޲���
    multOpParsing();
    tok = RD_ROOT->child[0]->child[0]->token;
    //RD_ROOT->child[1]: term()
    return termParsing(RD_ROOT->child[1], scope, exit_region, TypeList);

}

struct TypeIR* factorParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"
    if (RD_ROOT == NULL) { return NULL; }

    if (RD_ROOT->child[0]->token == NULL) {//variable()
        //RD_ROOT->child[0]: variable()
        return variableParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    }
    else if (RD_ROOT->child[0]->token->Lex == INTC) {
        //RD_ROOT->child[0]: INTC
        return TypeList[INTTY - '0'];//�����������͵��ڲ���ʾ
    }
    else if (RD_ROOT->child[0]->token->Lex == LPAREN) {
        //RD_ROOT->child[0]: LPAREN
        //RD_ROOT->child[1]: exp()
        return expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
        //RD_ROOT->child[2]: RPAREN
    }
}

void addOpParsing() {//���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"
    //�˽ڵ����Ƿ��ţ��޲���
    return; 
}
//���ش˱��ʽ��otherTerm���ֵļ����������ͣ�tok�������ڵõ�otherTermParsing�ڵķ��ŵ�token��Ϣ
struct TypeIR* otherTermParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token* &tok) {//���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: addOp(), ֻ�Ǿ����˷��Ŷ��ѣ�����Ӱ�����͵ķ���ֵ��������Ҫ���丳ֵ��tok
    addOpParsing();
    tok = RD_ROOT->child[0]->child[0]->token;
    //RD_ROOT->child[1]: exp());
    return expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);

}

//���ش˱��ʽ��term���ֵļ�����������
struct TypeIR* termParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Term"����ӦRD�е�"term����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: factor()
    struct TypeIR* TypeP1 = factorParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    //RD_ROOT->child[1]: otherFactor()
    if (RD_ROOT->child[1] == NULL) {//������otherFactorParsing()���֣���ֱ�ӷ���factorParsing()����
        return TypeP1;
    }
    else {
        token* tok = new token;
        struct TypeIR* TypeP2 = otherFactorParsing(RD_ROOT->child[1], scope, exit_region, TypeList, tok);//tok�������ڵõ�otherFactor�ڵķ��ŵ�token��Ϣ
        struct TypeIR* CalculatValidity = WhetherResaultValid(TypeP1, TypeP2, tok); //����TypeP1��TypeP2�Ƿ�ƥ��
        if (CalculatValidity == NULL) {//���ΪNULL����ƥ��
            Error_StmOpComponentIncompatibility(tok->Lineshow, tok->Sem);
        }
        return CalculatValidity;//����TypeP1��TypeP2�Ƿ�ƥ��
    }

}

void actparamMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec) {//���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA���޲���
    //RD_ROOT->child[1]: actparamList()
    actparamListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ProcPosition, TypeVec);//�õ����ù���ʱ�Ĳ�����Ϣ

}

void actparamListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, vector<struct TypeIR*>& TypeVec) {//���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: exp());
    //RD_ROOT->child[1]: actparamMore()
    TypeVec.push_back(expParsing(RD_ROOT->child[0], scope, exit_region, TypeList));//�����������ͽ��������Ϣ��

    actparamMoreParsing(RD_ROOT->child[1], scope,exit_region, TypeList, ProcPosition, TypeVec);//�õ����ù���ʱ�Ĳ�����Ϣ

}


//���ش˱��ʽ�ļ�����������
struct TypeIR* expParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: term()
    struct TypeIR* TypeP1 = termParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    //RD_ROOT->child[1]: otherTerm()
    if (RD_ROOT->child[1] == NULL) {//������otherTermParsing()���֣���ֱ�ӷ���termParsing()����
        return TypeP1;
    }
    else {
        token* tok = new token;
        struct TypeIR* TypeP2 = otherTermParsing(RD_ROOT->child[1], scope, exit_region, TypeList, tok);//tok�������ڵõ�otherTermParsing�ڵķ��ŵ�token��Ϣ
        struct TypeIR* CalculatValidity = WhetherResaultValid(TypeP1, TypeP2, tok); //����TypeP1��TypeP2�Ƿ�ƥ��
        if (CalculatValidity == NULL) {//���ΪNULL����ƥ��
            Error_StmOpComponentIncompatibility(tok->Lineshow, tok->Sem);
        }
        return CalculatValidity; //����TypeP1��TypeP2�Ƿ�ƥ��
    }
}

//sym��structure���ͱ����ķ��ű���Ϣ��tok����token��Ϣ�����ڴ���������
struct TypeIR* variMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable *sym, token *tok) {//���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"
    if (RD_ROOT == NULL) { return NULL; }
    if (RD_ROOT->child[0]->token->Lex == LMIDPAREN) {
        //RD_ROOT->child[0]: ReadmatchToken(LMIDPAREN));
        //RD_ROOT->child[1]: exp());
        //RD_ROOT->child[2]: ReadmatchToken(RMIDPAREN));
        if (sym->attrIR.idtype->Typekind != ARRAYTY) {//���sym�����������ͱ�ʶ�����򷵻ش���
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
        if (sym->attrIR.idtype->Typekind != RECORDTY) {//���sym���Ǽ�¼���ͱ�ʶ�����򷵻ش���
            Error_RecordNotRecordType(tok->Lineshow, tok->Sem);
            return NULL;
        }
        return fieldVarParsing(RD_ROOT->child[1], scope, exit_region, TypeList, sym, tok);
    }
    else {
        return NULL;
    }

}

//���� ProcPosition �ǹ��̱�ʶ�����ڵķ��ű���scopeջ��λ��
void callStmRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ProcPosition, token* IDTok) {//���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: LPAREN, �޲���
    //RD_ROOT->child[1]: actparamList()
    vector<struct TypeIR*> TypeVec;
    actparamListParsing(RD_ROOT->child[1], scope,exit_region, TypeList, ProcPosition, TypeVec);//�õ����ù���ʱ�Ĳ�����Ϣ

    //ʹ��scope[ProcPosition][0]��ʾ�˹��̱�ʶ��
    struct ParamTable* param = scope[ProcPosition][0].attrIR.More.ProcAttr.param;//�õ���������ʱ�Ĳ�����
    struct ParamTable* paramP = param;

    int paramSize = 0;
    while (paramP != NULL) {
        paramSize++;
        paramP = paramP->next;
    }
    if ( paramSize != TypeVec.size() ) {
        //��ʵ������������ͬ
        Error_ProcParamAmount(IDTok->Lineshow, IDTok->Sem);
    }
    else {
        paramP = param;
        for (int ix = 0; ix < TypeVec.size() && paramP != NULL; ix++) {
            //ʹ��scope[ProcPosition][ix+1]���ʴ˹��������ĵ�ix+1������
            //ʹ��TypeList[ix]���ʵ��ù��̵ĵ�ix������������
            if (scope[ProcPosition][ix + 1].attrIR.idtype == NULL || TypeVec[ix] == NULL) {
                //��ʵ���д���NULL����ʾ��������δ���壬����ʵ��������һ����ƥ��
                Error_ProcParamType(IDTok->Lineshow, ix + 1, IDTok->Sem);
            }
            else if(scope[ProcPosition][ix + 1].attrIR.idtype != TypeVec[ix]){
                //��ʵ��������һ����ƥ��
                Error_ProcParamType(IDTok->Lineshow, ix + 1, IDTok->Sem);
            }
        }
    }
    //RD_ROOT->child[2]: RPAREN, �޲���

}
//VarSym��ָ����ֵ�ı����ķ��ű���Ϣ��IDtok��ָ����ֵ�ı�����token��Ϣ��Ŀ���Ǵ�������
void assignmentRestParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, SymbTable* VarSym, token* IDTok) {//���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"
    if (RD_ROOT == NULL) { return; }
    struct TypeIR* TypeP1 = NULL;
    struct TypeIR* TypeP2 = NULL;
    if ( 0 == strcmp(RD_ROOT->child[0]->str, "VariMore") ) {//variMore()
        //RD_ROOT->child[0]: variMore()
        TypeP1 = variMoreParsing(RD_ROOT->child[0], scope, exit_region, TypeList, VarSym, IDTok);
        //RD_ROOT->child[1]: ASSIGN���޲���
        //RD_ROOT->child[2]: exp()
        TypeP2 = expParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    }
    else {
        //RD_ROOT->child[0]: ASSIGN���޲���
        TypeP1 = VarSym->attrIR.idtype;
        //RD_ROOT->child[1]: exp()
        TypeP2 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    }
    if (WhetherResaultValid(TypeP1, TypeP2, RD_ROOT->child[0]->token) == NULL) {//˵����ֵ����������Ͳ�����
        Error_AssignContentIncompatible(IDTok->Lineshow, IDTok->Sem);
    }
}


//��ֵ���ߵ��ù��̣�����ID����������ID���ǹ��̱�ʶ���Ļ�����Ҫ���ض�Ӧ������(Int, Char, Array, Record)
//����IDtoke��ID��token��Ϣ
void assCallParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, token* IDtok, const int ValidTableCount) {//���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"
    if (RD_ROOT == NULL) { return ; }
    if ( 0 == strcmp(RD_ROOT->child[0]->str, "CallStmRest") ){//���̵���callStmRest()
        int ProcPosition = FindProc(IDtok->Sem, true, scope, exit_region, ValidTableCount);//�Ҵ����ƵĹ��̱�ʶ�����Ҳ����򷵻�0������ʹ��scope[level][0]��ʾ�˹��̱�ʶ��
        if (ProcPosition == 0) {
            Error_ProcNotProcIdentifier(IDtok->Lineshow, IDtok->Sem);//���ǹ��̱�ʶ��
            return;
        }
        else {
            callStmRestParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ProcPosition, IDtok);
        }
        
    }
    else if( 0 == strcmp(RD_ROOT->child[0]->str, "AssignmentRest") ){//��ֵassignmentRest()
        SymbTable* VarSym = FindEntry(IDtok->Sem, true, scope, exit_region, VARKIND, -1);//�Ҵ����Ƶı�����ʶ�����Ҳ����򷵻ؿ�
        if (VarSym == NULL) {
            Error_AssignNotVarIdentifier(IDtok->Lineshow, IDtok->Sem);//���Ǳ�����ʶ��
            return;
        }
        else {
            assignmentRestParsing(RD_ROOT->child[0], scope, exit_region, TypeList, VarSym, IDtok);
        }
        
    }
    
}

void returnStmParsing() {//���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"
    //returnStm�ж���token���޺�����ֻ���﷨���󣬲������������󣬲���Ҫ������


}

void outputStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: WRITE���޲���
    //RD_ROOT->child[1]: LPAREN���޲���
    //RD_ROOT->child[2]: exp()
    expParsing(RD_ROOT->child[2], scope, exit_region, TypeList);
    //RD_ROOT->child[3]: RPAREN���޲���
}

void inputStmParsing() {//���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"
    //inputStm�ж���token���޺�����ֻ���﷨���󣬲������������󣬲���Ҫ������
}

void loopStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: WHILE���޲���
    //RD_ROOT->child[1]: exp()����һ�����ʽ
    struct TypeIR* TypeP1 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    //RD_ROOT->child[2]: LT����EQ���ȽϷ���('<', '=')���޲���
    //RD_ROOT->child[3]: exp()���ڶ������ʽ
    struct TypeIR* TypeP2 = expParsing(RD_ROOT->child[3], scope, exit_region, TypeList);
    WhetherCompareValid(TypeP1, TypeP2, RD_ROOT->child[2]->token);
    //RD_ROOT->child[4]: DO���޲���
    //RD_ROOT->child[5]: stmList()
    stmListParsing(RD_ROOT->child[5], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[6]: ENDWH���޲���
}

void conditionalStmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: IF���޲���
    //RD_ROOT->child[1]: exp()����һ�����ʽ
    struct TypeIR* TypeP1 = expParsing(RD_ROOT->child[1], scope, exit_region, TypeList);
    //RD_ROOT->child[2]: LT����EQ���ȽϷ���('<', '=')���޲���
    //RD_ROOT->child[3]: exp()���ڶ������ʽ
    struct TypeIR* TypeP2 = expParsing(RD_ROOT->child[3], scope, exit_region, TypeList);
    WhetherCompareValid(TypeP1, TypeP2, RD_ROOT->child[2]->token);
    //RD_ROOT->child[4]: THEN���޲���
    //RD_ROOT->child[5]: stmList()
    stmListParsing(RD_ROOT->child[5], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[6]: ELSE���޲���
    //RD_ROOT->child[7]: stmList()
    stmListParsing(RD_ROOT->child[7], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[8]: FI���޲���

}

void stmMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: SEMI�� �޲���
    //RD_ROOT->child[1]: stmList());
    stmListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


//-----------------------------------------------
void stmParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"
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
    else {//��ֵ���ߵ��ù���
        assCallParsing(RD_ROOT->child[1], scope, exit_region, TypeList, RD_ROOT->child[0]->token, ValidTableCount);
    }
    

}

//body�ڣ����ʽList(List and ListMore)
void stmListParsing(treenode* RD_ROOT, vector< vector<SymbTable> > scope, vector<bool> exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]:newnode, stm());
    stmParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    //RD_ROOT->child[1]:newnode, stmMore());
    stmMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


void fidMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, �޲���;
    //RD_ROOT->child[1]: formList(),������ȡID��token
    formListParsing(RD_ROOT->child[1], token);
}

//���̲�����ID���õ�token
void formListParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: fidMore());
    fidMoreParsing(RD_ROOT->child[1], token);

}

struct ParamTable* paramMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: SEM, �޲���;
    //RD_ROOT->child[1]: paramDecList()
    return paramDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);

}

//�˺�������һ�����̲���������Ϊ��Ҫ���ǵ�������Ĺ��̵���ʱ����������ƥ�䣬��������˴���TypeΪ�Ƿ���Ҳ��ѹ����ű�Type������ΪNULL��������Щ��ʶ������ʾ�ڷ��ű���
struct ParamTable* paramParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"Param"����ӦRD�е�"param����"
    if (RD_ROOT == NULL) { return NULL; }
    unsigned int VarExist = 0;
    if (0 == strcmp(RD_ROOT->child[0]->str,"TypeDef")) {
        VarExist = 0;
    }
    else {
        VarExist = 1;
    }
    //RD_ROOT->child[0+ VarExist]: typeDef()
    AttributeIR* tempAttr = typeDefParsing(RD_ROOT->child[0 + VarExist], scope, exit_region, TypeList);//�ҵ�����Ҫ������
    if (tempAttr == NULL) {//���û���ҵ���Ӧ��ʶ�����򴴽�һ��TypeΪNULL������
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
    
    SymbTable* Symtemp = FindEntry("*", false, scope, exit_region, VARKIND, -1);//�ҵ���ǰ���ű����µ�һ��������ʶ������ƫ�Ƽ���
    
    int Off = 0;//���㵱ǰ���ű��ڵ���ƫ�ƣ���������ڹ��̵�ƫ��=����size+��ƫ��
    if (Symtemp != NULL) {
        unsigned int size = 0;
        if (Symtemp->attrIR.idtype != NULL) {//���idtypeΪNULL�� �����ΪsizeΪ0
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
    vector<token*> token;//�洢formList������token

    //RD_ROOT->child[1 + VarExist]: formList()
    formListParsing(RD_ROOT->child[1 + VarExist], token);//token.size()һ������0�����򲻿���ͨ���﷨����
    const int size_origion = scope[scope.size() - 1].size();
    const int token_size = token.size();
    int token_Off = 0;//token�����е�ƫ��
    int VarSize = 0;//�����ʶ����size
    if (tempAttr->idtype != NULL) {
        VarSize = tempAttr->idtype->size;
    }
    for (int ix = 0; ix < token_size; ix++) {
        tempAttr->More.VarAttr.off = Off + token_Off;//���ű���ƫ�Ƽ��ϱ�ʶ����token�����е�ƫ��
        token_Off += VarSize;//tokenƫ�Ƽ��ϴ����ʶ���Ĵ�С

        if (Enter(token[ix]->Sem, *tempAttr, scope, exit_region, VARKIND, -1) == false) {//ѹ�����ջ
            Error_IdentifierDuplicateDec(token[ix]->Lineshow, token[ix]->Sem);//��ʶ��������
            Enter(WRONGID, *tempAttr, scope, exit_region, VARKIND, -1);//��WrongID������ű���ʾ��ʶ�������Ƿ�
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
        rear->entry = ix + size_origion;//�˷���ջ��Ԫ�ص�������ΪĿǰ�����ھֲ������е�λ��(��token�е�λ��ix + �ֲ�������ԭ���ı�ʶ��������)
    }
    return temp;
}

struct ParamTable* paramDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]: param()
    struct ParamTable* temp = paramParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
    // RD_ROOT->child[1]: paramMore()
    if (temp != NULL) {//tmep���趨Ϊ������Ϊ�գ�paramParsing��������ֵ��Ϊ���Ϊ�ǿգ����˴������������
        struct ParamTable* rearptr = temp;
        while (rearptr-> next != NULL) {//�ҵ���������һ���ڵ㣬Ȼ��������paramMoreParsing()
            rearptr = rearptr->next;
        }
        rearptr->next = paramMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    }
    else {
        temp = paramMoreParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    }
    
    return temp;
}

void procBodyParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0], programBody()
    programBodyParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//��дD�� procDecPart, ����������еı�ʶ�������ͱ�ʶ����������ʶ�������̱�ʶ����
void procDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0], procDec()
    declarePartParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//�����̲���ѹ��scopeջ�������ع��̲�����
struct ParamTable* paramListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"
    if (RD_ROOT == NULL) { return NULL; }
    return paramDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

//����������
void procDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"
    if (RD_ROOT == NULL) { return; }
    SymbTable temp;
    //RD_ROOT->child[0]: PROCEDURE, �޲���
    //RD_ROOT->child[1]: ID
    strcpy(temp.idname, RD_ROOT->child[1]->token->Sem);//idname
    temp.attrIR.idtype = NULL;//Typeptr
    temp.attrIR.kind = PROCKIND;//kind
    temp.attrIR.More.ProcAttr.level = ValidTableCount - 1;//level��������������һ��

    if (Enter(temp.idname, temp.attrIR, scope, exit_region, PROCKIND, temp.attrIR.More.ProcAttr.level) == false) {//ѹ�����ջ���迼�ǹ��̵�Level
        Error_IdentifierDuplicateDec(RD_ROOT->child[1]->token->Lineshow, RD_ROOT->child[1]->token->Sem);//��ʶ��������
        const int scope_origin_size = scope.size() - 1;//��Ϊ��ǰ�Ѿ������˴˹��̵ķ��ű�����scopeԭ����size�������ڵ�size-1
        //RD_ROOT->child[7]: procBody()
        procBodyParsing(RD_ROOT->child[7], scope, exit_region, TypeList, ValidTableCount);//�ȼ��body�е�����������Ƴ��˾ֲ�����
        //������̱�־���������µľֲ�����Ϊ������̵ı�ʶ�������Դ˹��̵ľֲ������Լ���body�ж����ȫ���ֲ�����Ӧ��ȫ����ɾ��
        const int times = scope.size() - scope_origin_size;//ͨ��scopeԭ����size����scopeջ��ԭ�ؼ���˾ֲ�����ԭ�������ӣ���Ҫ��scopeջ�Ĵ���
        for (int ix = 0; ix < times; ix++) {
            scope.pop_back();
            exit_region.pop_back();
        }
    }
    else {
        //RD_ROOT->child[2]: LPAREN, �޲���
        //RD_ROOT->child[3]: paramList()
        const int scope_size = scope.size();
        scope[scope_size - 1][0].attrIR.More.ProcAttr.param = paramListParsing(RD_ROOT->child[3], scope, exit_region, TypeList, ValidTableCount);
        scope[scope_size - 1][0].attrIR.More.ProcAttr.code = 0;//Code,���̵�Ŀ������ַ���������ɽ׶���д�����ڳ�ʼ��Ϊ0��
        //RD_ROOT->child[4]: RPAREN, �޲���
        //RD_ROOT->child[5]: SEMI, �޲���
        //RD_ROOT->child[6]: procDecPart()
        procDecPartParsing(RD_ROOT->child[6], scope, exit_region, TypeList, ValidTableCount);
        //����Size
        //scope[scope_size - 1][0]��ʾ������ű�Ĺ��̱�ʶ��
        scope[scope_size - 1][0].attrIR.More.ProcAttr.size = 0;
        for (int ix = scope[scope_size - 1].size() - 1; ix >= 1; ix--) {
            if (scope[scope_size - 1][ix].attrIR.kind == VARKIND) {
                int size = 0;//��ʾ������̵����һ��������ʶ���Ĵ�С
                if (scope[scope_size - 1][ix].attrIR.idtype != NULL) {
                    size = scope[scope_size - 1][ix].attrIR.idtype->size;
                }
                scope[scope_size - 1][0].attrIR.More.ProcAttr.size += size;
            }
        }
        ;
        /*SymbTable* SymPtr = FindEntry("*", false, scope, exit_region, VARKIND, -1);//�ҵ���ǰ���ű����µ�һ��������ʶ������size����
        if (SymPtr == NULL) {//�˱�ʶ��Ϊ��ǰ���ű����µ�һ��������ʶ����size����Ϊ0
            scope[scope_size - 1][0].attrIR.More.ProcAttr.size = 0;
        }
        else {//������ڵ�ǰ���ű����µ�һ��������ʶ����size+ƫ��
            unsigned int size = 0;
            if (SymPtr->attrIR.idtype != NULL) {//���idtypeΪNULL�� �����ΪsizeΪ0
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

void varIDMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, �޲���
    //RD_ROOT->child[1]: IDList()
    varIDListParsing(RD_ROOT->child[1], token);
    return;
}

void varDecMoreParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"
    if (RD_ROOT == NULL) { return; }
    varDecListParsing(RD_ROOT->child[0], scope, exit_region, TypeList, ValidTableCount);
}

void varIDListParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: varIDMore()
    varIDMoreParsing(RD_ROOT->child[1], token);
    return;
}
//�����������
void varDecListParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: typeDef()
    AttributeIR* Attr = typeDefParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    if (Attr != NULL) {
        Attr->kind = VARKIND;//������ʶ��
        Attr->More.VarAttr.access = DIR;//ֱ�ӱ���
        Attr->More.VarAttr.level = ValidTableCount;//����
        //RD_ROOT->child[1]: varIDList()
        vector<token*> token;//�洢IDList�ڵ�����token
        varIDListParsing(RD_ROOT->child[1], token);
        int token_size = token.size();
        for (int ix = 0; ix < token_size; ix++) {
            SymbTable* Symtemp = FindEntry("*", false, scope, exit_region, VARKIND, -1);//�ҵ���ǰ���ű����µ�һ��������ʶ������ƫ�Ƽ���
            if (Symtemp == NULL) {//�˱�ʶ��Ϊ��ǰ���ű����µ�һ��������ʶ����ƫ������Ϊ0
                Attr->More.VarAttr.off = 0;
            }
            else {//������ڵ�ǰ���ű����µ�һ��������ʶ����size+ƫ��
                int size = 0;
                if (Symtemp->attrIR.idtype != NULL) {
                    size = Symtemp->attrIR.idtype->size;
                }
                Attr->More.VarAttr.off = Symtemp->attrIR.More.VarAttr.off + size;
            }
            //����ű��ڲ������
            if (Enter(token[ix]->Sem, *Attr, scope, exit_region, VARKIND, -1) == false) {
                Error_IdentifierDuplicateDec(token[ix]->Lineshow, token[ix]->Sem);//��ʶ��������
            }
        }
    }
    
    //RD_ROOT->child[2]: SEMI, �޲���
    //RD_ROOT->child[3]: varDecMore()
    varDecMoreParsing(RD_ROOT->child[3], scope, exit_region, TypeList, ValidTableCount);

}

void varDecParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, const int ValidTableCount) {//���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: VAR, �޲���
    //RD_ROOT->child[1]: varDecList()
    varDecListParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
}


void IDMoreParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: COMMA, �޲���
    //RD_ROOT->child[1]: IDList()
    IDListParsing(RD_ROOT->child[1], token);
    return;
}

fieldChain* fieldDecMoreParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<token*>& token) {//���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"
    if (RD_ROOT == NULL) { return NULL; }
    //RD_ROOT->child[0]:fieldDecList()
    return fieldDecListParsing(RD_ROOT->child[0], TypeList, token);
}

void IDListParsing(treenode* RD_ROOT, vector<token*>& token) {//���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"��StrVec���������Ҫ��ID��
    if (RD_ROOT == NULL) { return; }
    //RD_ROOT->child[0]: ID
    token.push_back(RD_ROOT->child[0]->token);
    //RD_ROOT->child[1]: IDMore()
    IDMoreParsing(RD_ROOT->child[1], token);
    return;
}

fieldChain* fieldDecListParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList, vector<token*>& token) {//���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"
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
    //RD_ROOT->child[1]:IDList(),����idname
    int token_size1 = token.size();//tokenδ����ǰ
    IDListParsing(RD_ROOT->child[1], token);//�õ�IDList�е�����token
    if (token.size() - token_size1 == 0) {//ID����Ϊ0
        delete temp;
        temp = NULL;
    }

    if (temp == NULL) {//ID����Ϊ0
        return fieldDecMoreParsing(RD_ROOT->child[3], TypeList, token);
    }
    else {
        strcpy(temp->idname, token[token_size1 + 0]->Sem);
        fieldChain* rear = temp;
        for (int ix = token_size1 + 1; ix < token.size(); ix++) {//������token������ΪfieldChain������������
            fieldChain* tempID = new fieldChain;
            tempID->unitType = temp->unitType;//����ID������ͬ������
            strcpy(tempID->idname, token[ix]->Sem);
            rear->next = tempID;
            rear = tempID;
        }
        //RD_ROOT->child[2]:SEMI���޲���
        //RD_ROOT->child[3]:fieldDecMore()
        rear->next = fieldDecMoreParsing(RD_ROOT->child[3], TypeList, token);
        return temp;
    }
    
}

AttributeIR* recTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START �½�һ��TypeIR������Ǵ����͵Ļ���������TypeList�Ļ��������TypeList�������TypeIR��ֵ�Ѿ�������TypeList�����͵ĵ�ַ------------------------------------------------------------------------------------
    struct TypeIR* tempTypeIR = new (struct TypeIR);
    tempTypeIR->Typekind = RECORDTY;//��¼����Typeind
    //RD_ROOT->child[0]: RECORD, �޲���
    //RD_ROOT->child[1]: fieldDecList()
    vector<token*> token;
    fieldChain* body = fieldDecListParsing(RD_ROOT->child[1], TypeList, token);
    //RD_ROOT->child[2]: END, �޲���
    if (body == NULL) {
        delete temp;
        temp = NULL;
        return NULL;
    }
    fieldChain* head = body;
    unordered_map <string, int> UnMap;
    bool flag = false;
    for (int ix = 0; head != NULL && ix<token.size(); ix++) {//token��body�ڵ�һ��һһ��Ӧ
        string str = head->idname;//���ַ�����ת����string����
        if (UnMap.find(str) != UnMap.end()) {//ID����Ѿ���map�ڣ����ʾRecord�б�ʶ���ظ�����
            Error_IdentifierDuplicateDecRecord(token[ix]->Lineshow, token[ix]->Sem);//Record�б�ʶ���ظ�����
            flag = true;
        }
        else {
            UnMap[str] = 0;//ID��Map
        }
        head = head->next;
    }
    if (flag == true) {
        delete temp;
        temp = NULL;
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

    struct TypeIR* existTy = WhetherTypeDuplicate(TypeList, tempTypeIR, RECORDTY);
    if (existTy == NULL) {
        TypeList.push_back(tempTypeIR);
        temp->idtype = TypeList[TypeList.size() - 1];
    }
    else {
        temp->idtype = existTy;
    }
    //END �½�һ��TypeIR������Ǵ����͵Ļ���������TypeList�Ļ��������TypeList�������TypeIR��ֵ�Ѿ�������TypeList�����͵ĵ�ַ------------------------------------------------------------------------------------
    return temp;
}

AttributeIR* arrayTypeParsing(treenode* RD_ROOT, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"
    if (RD_ROOT == NULL) { return NULL; }
    AttributeIR* temp = new AttributeIR;
    temp->kind = TYPEKIND;
    //START �½�һ��TypeIR������Ǵ����͵Ļ���������TypeList�Ļ��������TypeList�������TypeIR��ֵ�Ѿ�������TypeList�����͵ĵ�ַ------------------------------------------------------------------------------------
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

    struct TypeIR* existTy = WhetherTypeDuplicate(TypeList, tempTypeIR, ARRAYTY);
    if (existTy == NULL) {
        TypeList.push_back(tempTypeIR);
        temp->idtype = TypeList[TypeList.size() - 1];
    }
    else {
        temp->idtype = existTy;
    }
    //END �½�һ��TypeIR������Ǵ����͵Ļ���������TypeList�Ļ��������TypeList�������TypeIR��ֵ�Ѿ�������TypeList�����͵ĵ�ַ------------------------------------------------------------------------------------
    
    return temp;
}
//-----------------------------------------------------------------
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
        SymbTable* Sym = FindEntry(RD_ROOT->child[0]->token->Sem, true, scope, exit_region, TYPEKIND, -1);
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
        bool flag = Enter(temp.idname, temp.attrIR, scope, exit_region, TYPEKIND, -1);//�����ʶ��
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

void typeDecPartParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList) {//���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"
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
    typeDecPartParsing(RD_ROOT->child[0], scope, exit_region, TypeList);
    varDecPartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    procDecpartParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
    return;
}

void programHeadParsing(treenode* RD_ROOT) {//���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"
    if (RD_ROOT == NULL) { return; }
    //����ͷ�����������ű�Ļ���û���ô������ﲻ���ǳ���ͷ
    return;
}

void RDTreeParsing(treenode* RD_ROOT, vector< vector<SymbTable> >& scope, vector<bool>& exit_region, vector<struct TypeIR*>& TypeList, int& ValidTableCount) {//�����﷨��,���ڵ�����Ϊ"Program"
    if (RD_ROOT == NULL) { return; }
    CreateTable(scope, exit_region, ValidTableCount);//program,�½�һ���շ��ű�
    programHeadParsing(RD_ROOT->child[0]);
    declarePartParsing(RD_ROOT->child[1], scope, exit_region, TypeList, ValidTableCount);
    programBodyParsing(RD_ROOT->child[2], scope, exit_region, TypeList, ValidTableCount);
}



//token��ֻ��ID, INTC, CHARC����������Ϣ���ֱ�Ϊ��ʶ�����ơ��������ַ����Ҷ����ַ�������ʽ

int semantic_analysis(treenode* RD_ROOT) 
{
    //�������
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�
    vector<struct TypeIR*> TypeList;//������Ͷ��壬ǰ�����̶����ֱ�Ϊ[0]:int, [1]:char, [2]bool,����ʹ�� TypeList[INTTY - '0'] ������int
    int ValidTableCount = 0;

    //��ʼ������
    InitFile(ERROR_FILE);//��մ洢���������Ϣ���ļ�

    initialize(TypeList);////��ʼ�����ֻ�������

    //�����﷨��
    RDTreeParsing(RD_ROOT, scope, exit_region, TypeList, ValidTableCount);

    //��ӡ���ͱ�ͷ��ű��ļ�
    //PrintTable(scope, TypeList, TABLE_FILE_XLS);
    PrintTable(scope, TypeList, TABLE_FILE_CSV);
    return 1;
}

//2022_4_17