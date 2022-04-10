#include"SemanticAnalysis.h"
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
void semantic_analysis() {
    vector< vector<SymbTable> > scope;//ʹ��vector�����ʾscopeջ
    vector<bool> exit_region;//��Ӧ�ֲ������Ƿ��Ѿ��˳���true:�Ѿ��˳���false:δ�˳�
    int ValidTableCount = 0;//��ʾδ���ϳ��ķ��ű�����




    //�˴�ǰ�������﷨��
}
