#pragma once

//һЩ����������

int getNonIndex(char* s); // ���ط��ս��s�ڷ��ս�������е��±�
int getReIndex(char* s); // �����ս��s���ս�������е��±�
void cal_first(const char* s); // ����first��
void cal_follow(const char* s, bool* flag); // ����follow��
void cal_predict(); // ����predict��������LL1Ԥ�������
void out_fitstfollow(); //���first����follow��������
void out_predict(); //���LL1Ԥ�����������