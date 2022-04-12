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
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &ValidTableCount){//����һ�����ű�������scopeջ
    ValidTableCount++;
    vector<SymbTable> temp_Sym;//�����ձ�
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//��ʾδ�˳��ֲ�����
    return;
}

void DestroyTable(vector<bool> &exit_region, int &ValidTableCount){//�ϳ����µ�һ����Ч���ű�
    ValidTableCount--;
    if(ValidTableCount<0){//������߼�©�����ܻᵼ���������
        cout <<"DestroyTable_ERROR (ValidTableCount<0)\n";
        exit(0);
    }
    for(int ix=exit_region.size() - 1; ix>=0; ix--){
        if(exit_region[ix] == false){//�ϳ����ű�����������ĵ�һ��δ�˳��ľֲ�����
            exit_region[ix] = true;
            break;
        }
    }
}

bool SearchSingleTable(char *id, vector< vector<SymbTable> > scope, int level){//���ұ�ʶ���Ƿ�������Ӧ����
    int position = level - 1;
    bool present = false;//�����ұ�ʶ���Ƿ�������Ӧ����
    for(int ix=scope[position].size() - 1; ix>=0; ix--){
        if(0 == strcmp(scope[position][ix].idname, id)/*scope[position][ix].idname == id*/){
            present = true;
            return present;
        }
    }
    return present;
}

bool FindEntry(char *id, bool flag, vector< vector<SymbTable> > scope, vector<bool> exit_region){//flag == false:�ڵ�ǰ���ű��в��ң� flag == true:������scopeջ�в���
    int level = exit_region.size();//��ʾ�ڼ��㣬level == 0�൱��scopeջ����Ԫ�ء�ʹ�� n = level-1 ����scope[n][x]����exit_region[n]��
    bool present = false;//�Ƿ����
    //���ڵ�ǰ���ű��в���
    while(level > 0){
        if(exit_region[level - 1] == false){//�ҵ���һ��δ�˳��ľֲ������Ĳ���
            break;
        }else{
            level--;
        }
    }
    if (level <= 0) {//������һ����Ч�ľֲ�����
        return false;
    }
    present = SearchSingleTable(id, scope, level);//���ҵ�ǰ���ű�
    if(flag == true && present == false){//���flag == true���ҵ�ǰ���ű���δ���ҵ������ʶ��id���������������scopeջ
        while(level > 0){
            if(exit_region[level - 1] == false){//�ҵ�δ�˳��ľֲ������Ĳ���
                present = SearchSingleTable(id, scope, level);//���Ҵ˷��ű�
                if(present == true){
                    break;
                }
            }
            level--;
        }
    }
    return present;//true:���ڣ� false:������
}

bool Enter(char* id, AttributeIR Attrib, vector< vector<SymbTable> >& scope, vector<bool> exit_region) {//�ǼǱ�ʶ���������Ե����ű�
    bool present = false;//��¼�Ǽ��Ƿ�ɹ�
    if (FindEntry(id, false, scope, exit_region) == false) {
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
        PrintFile(SemanticERROR1, ERROR_FILE);
    }
    return present;//true:�ɹ��� false:���ɹ�
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

/*�﷨����������ģ��
void Parsing() {//���ڵ�����Ϊ""����ӦRD�е�"����"

}
*/
void fieldVarMoreParsing() {//���ڵ�����Ϊ"FieldVarMore"����ӦRD�е�"fieldVarMore����"

}

void fieldVarParsing() {//���ڵ�����Ϊ"FieldVar"����ӦRD�е�"fieldVar����"

}

void variableParsing() {//���ڵ�����Ϊ"Variable"����ӦRD�е�"variable����"

}

void multOpParsing() {//���ڵ�����Ϊ"MultOp"����ӦRD�е�"multOp����"

}

void otherFactorParsing() {//���ڵ�����Ϊ"OtherFactor"����ӦRD�е�"otherFactor����"

}

void factorParsing() {//���ڵ�����Ϊ"Factor"����ӦRD�е�"factor����"

}

void addOpParsing() {//���ڵ�����Ϊ"AddOp"����ӦRD�е�"addOp����"

}

void otherTermParsing() {//���ڵ�����Ϊ"OtherTerm"����ӦRD�е�"otherTerm����"

}

void termParsing() {//���ڵ�����Ϊ"Term"����ӦRD�е�"term����"

}

void actparamMoreParsing() {//���ڵ�����Ϊ"ActParamMore"����ӦRD�е�"actparamMore����"

}

void actparamListParsing() {//���ڵ�����Ϊ"ActParamList"����ӦRD�е�"actparamList����"

}

void expParsing() {//���ڵ�����Ϊ"Exp"����ӦRD�е�"exp����"

}

void variMoreParsing() {//���ڵ�����Ϊ"VariMore"����ӦRD�е�"variMore����"

}

void callStmRestParsing() {//���ڵ�����Ϊ"CallStmRest"����ӦRD�е�"callStmRest����"

}

void assignmentRestParsing() {//���ڵ�����Ϊ"AssignmentRest"����ӦRD�е�"assignmentRest����"

}

void assCallParsing() {//���ڵ�����Ϊ"AssCall"����ӦRD�е�"assCall����"

}

void returnStmParsing() {//���ڵ�����Ϊ"ReturnStm"����ӦRD�е�"returnStm����"

}

void outputStmParsing() {//���ڵ�����Ϊ"OutputStm"����ӦRD�е�"outputStm����"

}

void inputStmParsing() {//���ڵ�����Ϊ"InputStm"����ӦRD�е�"inputStm����"

}

void loopStmParsing() {//���ڵ�����Ϊ""����ӦRD�е�"loopStm����"********************************************

}

void conditionalStmParsing() {//���ڵ�����Ϊ""����ӦRD�е�"conditionalStm����"********************************************

}

void stmMoreParsing() {//���ڵ�����Ϊ"StmMore"����ӦRD�е�"stmMore����"

}

void stmParsing() {//���ڵ�����Ϊ"Stm"����ӦRD�е�"stm����"

}

void stmListParsing() {//���ڵ�����Ϊ"StmList"����ӦRD�е�"stmList����"

}

void fidMoreParsing() {//���ڵ�����Ϊ"FidMore"����ӦRD�е�"fidMore����"

}

void formListParsing() {//���ڵ�����Ϊ"FormList"����ӦRD�е�"formList����"

}

void paramMoreParsing() {//���ڵ�����Ϊ"ParamMore"����ӦRD�е�"paramMore����"

}

void paramParsing() {//���ڵ�����Ϊ"Param"����ӦRD�е�"param����"

}

void paramDecListParsing() {//���ڵ�����Ϊ"ParamDecList"����ӦRD�е�"paramDecList����"

}

void procBodyParsing() {//���ڵ�����Ϊ"ProcBody"����ӦRD�е�"procBody����"

}

void procDecPartParsing() {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecPart����"

}

void paramListParsing() {//���ڵ�����Ϊ"ParamList"����ӦRD�е�"paramList����"

}

void procDecParsing() {//���ڵ�����Ϊ"ProcDec"����ӦRD�е�"procDec����"

}

void varIDMoreParsing() {//���ڵ�����Ϊ"VarIDMore"����ӦRD�е�"varIDMore����"

}

void varDecMoreParsing() {//���ڵ�����Ϊ"VarDecMore"����ӦRD�е�"varDecMore����"

}

void varIDListParsing() {//���ڵ�����Ϊ"VarIDList"����ӦRD�е�"varIDList����"

}

void varDecListParsing() {//���ڵ�����Ϊ"VarDecList"����ӦRD�е�"varDecList����"

}

void varDecParsing() {//���ڵ�����Ϊ"VarDec"����ӦRD�е�"varDec����"

}

void IDMoreParsing() {//���ڵ�����Ϊ"IDMore"����ӦRD�е�"IDMore����"

}

void fieldDecMoreParsing() {//���ڵ�����Ϊ"FieldDecMore"����ӦRD�е�"fieldDecMore����"

}

void IDListParsing() {//���ڵ�����Ϊ"IDList"����ӦRD�е�"IDList����"

}

void fieldDecListParsing() {//���ڵ�����Ϊ"FieldDecList"����ӦRD�е�"fieldDecList����"

}

void recTypeParsing() {//���ڵ�����Ϊ"RecType"����ӦRD�е�"recType����"

}

void arrayTypeParsing() {//���ڵ�����Ϊ"ArrayType"����ӦRD�е�"arrayType����"

}

void structureTypeParsing() {//���ڵ�����Ϊ"StructureType"����ӦRD�е�"structureType����"

}

void baseTypeParsing() {//���ڵ�����Ϊ"BaseType"����ӦRD�е�"baseType����"

}

void typeDecMoreParsing() {//���ڵ�����Ϊ"TypeDecMore"����ӦRD�е�"typeDecMore����"

}

void typeDefParsing() {//���ڵ�����Ϊ"TypeDef"����ӦRD�е�"typeDef����"

}

void typeIDParsing() {//���ڵ�����Ϊ"TypeID"����ӦRD�е�"typeID����"

}

void typeDecListParsing() {//���ڵ�����Ϊ"TypeDecList"����ӦRD�е�"typeDecList����"

}

void typeDecParsing() {//���ڵ�����Ϊ"TypeDec"����ӦRD�е�"typeDec����"

}

void procDecpartParsing() {//���ڵ�����Ϊ"ProcDecPart"����ӦRD�е�"procDecpart����"

}

void varDecPartParsing() {//���ڵ�����Ϊ"VarDecPart"����ӦRD�е�"varDecPart����"

}

void typeDecPartParsing() {//���ڵ�����Ϊ"TypeDecPart"����ӦRD�е�"typeDecPart����"

}

void programBodyParsing() {//���ڵ�����Ϊ"ProgramBody"����ӦRD�е�"programBody����"

}

void declarePartParsing() {//���ڵ�����Ϊ"DeclarePart"����ӦRD�е�"declarePart����"

}

void programHeadParsing() {//���ڵ�����Ϊ"ProgramHead"����ӦRD�е�"programHead����"

}

void RDTreeParsing(treenode *RD_ROOT, vector< vector<SymbTable> > &scope, vector<bool> &exit_region, vector<struct TypeIR> &TypeList) {//�����﷨��,���ڵ�����Ϊ"Program"

}


//token��ֻ��ID, INTC, CHARC����������Ϣ���ֱ�Ϊ��ʶ�����ơ��������ַ����Ҷ����ַ�������ʽ

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�
    int ValidTableCount = 0;//��ʾδ���ϳ��ķ��ű�����

    vector<struct TypeIR> TypeList;//������Ͷ��壬ǰ�����̶����ֱ�Ϊ[0]:int, [1]:char, [2]bool,����ʹ�� TypeList[INTTY - '0'] ������int
    initialize(TypeList);////��ʼ�����ֻ�������

    RDTreeParsing(NULL, scope, exit_region, TypeList);



    //�˴�ǰ�������﷨��
}
