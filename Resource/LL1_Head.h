#pragma once

#include "LexicalAnalysis.h"
#include "global_var.h"
#include "treenode_var.h"

const int maxlen = 20; // һ���ַ�����󳤶�
token* nowtoken; // ��ǰָ���token�ڵ�

void LL1_analysis(); //LL1������

