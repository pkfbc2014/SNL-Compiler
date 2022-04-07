#include"SemanticAnalysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
/*
    符号表管理采用跳转法,以exit栈表示对应局部化区是否已经退出
*/
void CreateTable(vector< vector<SymbTable> > &scope, vector<bool> &exit_region, unsigned int &level){//创建符号表
    level ++;
    vector<SymbTable> temp_Sym;//创建空表
    scope.push_back(temp_Sym);
    exit_region.push_back(false);//表示未退出局部化区
    return;
}
void DestroyTable(unsigned int &level, vector<bool> &exit_region){
    level--;
    if(level<0){//程序的逻辑漏洞可能会导致这个问题
        cout <<"DestroyTable_ERROR (level<0)\n";
        exit(0);
    }
    //exit_region[exit_region.size() - 1] = true;//表示退出当前局部化区

}

void semantic_analysis() {
    vector< vector<SymbTable> > scope;//使用vector数组表示scope栈
    vector<bool> exit_region;//对应局部化区是否已经退出，true:已经退出，false:未退出

    unsigned int level = 0;

    //此处前根遍历语法树
}
