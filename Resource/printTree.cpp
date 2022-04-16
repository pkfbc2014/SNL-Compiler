#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "treenode_var.h"

/*
	将语法树输出到本地，根据树生成dot脚本，再调用 Graphviz 插件根据生成的dot脚本作图
	Graphviz下载链接: http://www.graphviz.org/gallery/
	使用教程: https://www.jianshu.com/p/6d9bbbbf38b1
	dot脚本的简单语法格式如下所示: 
	digraph graph{
		a->b;
		b->d;
		c->d;
	}
*/

FILE* treefp = NULL; // 文件读写指针

void choosePrint(treenode* root, int treetype)
{
	if (treetype == 0) // 打印RD树
	{	
		if ((treefp = fopen("RDtree.dot", "w")) == NULL)
		{
			printf("cannot open the RDtree.dot file\n");
			return;
		}

		fprintf(treefp, "digraph graphRD{\n");
		printRDTree(root);
		fprintf(treefp, "}");
		fclose(treefp);

		//system("dot -Tpng RDtree.dot -o RDtree.png"); // 运行脚本
	}
	else // 打印LL1树
	{
		if ((treefp = fopen("LL1tree.dot", "w")) == NULL)
		{
			printf("cannot open the LL1tree.dot file\n");
			return;
		}

		fprintf(treefp, "digraph graphLL1{\n");
		printLL1Tree(root);
		fprintf(treefp, "}");
		fclose(treefp);

		//system("dot -Tpng LL1tree.dot -o LL1tree.png"); // 运行脚本
	}
}

void printRDTree(treenode* root) // 语法树根节点、树的类型（RD树 - 0，LL1树 - 1）
{

}

void printLL1Tree(treenode* root)
{

}