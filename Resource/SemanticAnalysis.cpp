#include"SemanticAnalysis.h"
/*
    ���ű���������ת��,��exitջ��ʾ��Ӧ�ֲ������Ƿ��Ѿ��˳�
*/

void PrintFile(string message, string file_path)//�ַ�����ӡ����Ӧ�ļ���
{
	ofstream output(file_path);
	output << message;
	output.close();
	exit(0);
}

void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &level){//�������ű�
    level ++;
    vector<SymbTable> temp_Sym;//�����ձ�
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//��ʾδ�˳��ֲ�����
    return;
}
void DestroyTable(int &level, vector<bool> &exit_region){//�ϳ����ű�
    level--;
    if(level<0){//������߼�©�����ܻᵼ���������
        cout <<"DestroyTable_ERROR (level<0)\n";
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
        if(scope[position][ix].idname == id){
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
    present = SearchSingleTable(id, scope, level);//���ҵ�ǰ���ű�
    if(flag == true && present == false){//���flag == true���ҵ�ǰ���ű���δ���ҵ������ʶ��id���������������scopeջ
        while(level > 0){
            if(exit_region[level - 1] == false){//�ҵ�δ�˳��ľֲ������Ĳ���
                present = SearchSingleTable(id, scope, level);//���Ҵ˷��ű�
                if(present == true){
                    break;
                }
            }else{
                level--;
            }
        }
    }
    if(level<=0){//������߼�©�����ܻᵼ���������
        cout <<"FindEntry_ERROR (level<=0)\n";
        exit(0);
    }
    return present;
}

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�

    int level = 0;//��ʾδ���ϳ��ķ��ű�����

    //�˴�ǰ�������﷨��
}
