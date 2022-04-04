// 主程序，读入样例程序文件进行分析
#include "SemanticAnalysis.h"
#include "head.h"

char Non_symbol[67][20]; // 67个非终结符
char Reserved_word[42][20]; // 42个保留字和其他符号
int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"}; // 输入文件为test.txt

    /*===================================================================*/
    if ((fp = fopen("Non_symbol.txt", "r")) == NULL) // 读入非终结符文件
    {
        printf("cannot open the Non_symbol file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }
    int i = 0;
    while (fgets(Non_symbol[i], 20, fp) != NULL)
    {
        Non_symbol[i][strlen(Non_symbol[i]) - 1] = '\0'; // 去掉结尾换行符
        i++;
    }
    fclose(fp);

    /*===================================================================*/
    if ((fp = fopen("Reserved_word.txt", "r")) == NULL) // 读入保留字文件
    {
        printf("cannot open the Reserved_word file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }
    i = 0;
    while (fgets(Reserved_word[i], 20, fp) != NULL)
    {
        Reserved_word[i][strlen(Reserved_word[i]) - 1] = '\0'; // 去掉结尾换行符
        i++;
    }
    fclose(fp);

    /*===================================================================*/
    if ((fp = fopen(input_filename, "r")) == NULL) // 读入test文件
    {
        printf("cannot open the file!\n"); // 文件打开失败
        exit(0); // 结束进程
    }

    //......

    fclose(fp);

    //function1(fp); // 词法分析
    //LL1_analysis(); // 语法分析 - LL1
    //RD_analysis(); //语法分析 - 递归下降
    //semantic_analysis(); //语义分析
    
	return 0;
} 