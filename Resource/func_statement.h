#pragma once
#include "treenode_var.h"

// һЩ���ú���������

int getNonIndex(const char* s); // ���ط��ս��s�ڷ��ս�������е��±�
int getReIndex(const char* s); // �����ս��s���ս�������е��±�
void cal_first(const char* s); // ����first��
void cal_follow(const char* s, bool* flag); // ����follow��
void cal_predict(); // ����predict��������LL1Ԥ�������
int out_fitstfollow(); //���first����follow��������
void out_predict(); //���LL1Ԥ�����������
void freetree(treenode* root); // �ݹ��ͷ��� root Ϊ���ڵ����