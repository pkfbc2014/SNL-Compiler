#pragma once

//һЩ����������

int getNonIndex(char* s); // ���ط��ս��s�ڷ��ս�������е��±�
int getReIndex(char* s); // �����ս��s���ս�������е��±�
void cal_first(); // ����first��
void cal_follow(); // ����follow��
void cal_predict(); // ����predict��������LL1Ԥ�������
void out_fitstfollow(); //���first����follow��������
void out_predict(); //���LL1Ԥ�����������

void LL1_analysis(); //LL1������
void RD_analysis(); //�ݹ��½�������