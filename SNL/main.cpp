#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp;
    char input_filename[20] = {"test.txt"};

    if ((fp = fopen(input_filename, "r")) == NULL)
    {
        printf("cannot open the file\n"); // can not open the input file
        exit(0); // shutdown the process
    }

    //handle(fp); // lexical analysis, firstly

    fclose(fp);
	return 0;
} 