// LL1分析法

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_statement.h"

void LL1_analysis()
{
	cal_predict(); // 计算predict集并构造LL1预测分析表
	out_predict(); //输出LL1预测分析表到本地
}