#include"SemanticAnalysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
/*
    ���ű���������ת��,��exitջ��ʾ��Ӧ�ֲ������Ƿ��Ѿ��˳�
*/
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, unsigned int &level){//�������ű�
    level ++;
    vector<SymbTable> temp_Sym;//�����ձ�
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//��ʾδ�˳��ֲ�����
    return;
}
void DestroyTable(unsigned int &level, vector<bool> &exit_region){
    level--;
    if(level<0){//������߼�©�����ܻᵼ���������
        cout <<"DestroyTable_ERROR (level<0)\n";
        exit(0);
    }
    //exit_region[exit_region.size() - 1] = true;//��ʾ�˳���ǰ�ֲ�����

}

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�

    unsigned int level = 0;

    //�˴�ǰ�������﷨��
}
