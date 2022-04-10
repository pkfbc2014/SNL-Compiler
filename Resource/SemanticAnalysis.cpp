#include"SemanticAnalysis.h"
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
void semantic_analysis() {
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出
    int ValidTableCount = 0;//表示未被废除的符号表总数




    //此处前根遍历语法树
}
