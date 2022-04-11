#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"
#include "treenode_var.h"

const int maxlen = 20; // 一个字符的最大长度
token* nowtoken; // 当前指向的token节点

treenode* LL1_treeROOT = NULL; // LL1分析法语法分析数根节点

void LL1_analysis(); //LL1分析法
