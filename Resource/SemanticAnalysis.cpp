#include"SemanticAnalysis.h"
/*
    符号表管理采用跳转法,以exit栈表示对应局部化区是否已经退出
*/

void PrintFile(string message, string file_path)//字符串打印到相应文件中
{
	ofstream output(file_path);
	output << message;
	output.close();
	exit(0);
}

void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, int &level){//创建符号表
    level ++;
    vector<SymbTable> temp_Sym;//创建空表
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//表示未退出局部化区
    return;
}
void DestroyTable(int &level, vector<bool> &exit_region){//废除符号表
    level--;
    if(level<0){//程序的逻辑漏洞可能会导致这个问题
        cout <<"DestroyTable_ERROR (level<0)\n";
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
        if(scope[position][ix].idname == id){
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
    present = SearchSingleTable(id, scope, level);//查找当前符号表
    if(flag == true && present == false){//如果flag == true，且当前符号表内未查找到所需标识符id，则继续查找整个scope栈
        while(level > 0){
            if(exit_region[level - 1] == false){//找到未退出的局部化区的层数
                present = SearchSingleTable(id, scope, level);//查找此符号表
                if(present == true){
                    break;
                }
            }else{
                level--;
            }
        }
    }
    if(level<=0){//程序的逻辑漏洞可能会导致这个问题
        cout <<"FindEntry_ERROR (level<=0)\n";
        exit(0);
    }
    return present;
}

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出

    int level = 0;//表示未被废除的符号表总数

    //此处前根遍历语法树
}
