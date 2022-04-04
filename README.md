1. # SNL-Compiler
2019级吉林大学计算机科学与技术专业编译原理课程设计——SNL语言编译器

## 词法分析

## 语法分析
![语法树图示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语法树图示.png "语法树图示")
## 语义分析
### SNL符号表信息项

1. 标识符共有3种
```C++
Kind = {typeKind, varKind, procKind}
```
![语义分析标识符种类](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语义分析标识符种类.png "语义分析标识符种类")
2. 类型标识符内部表示
 ![类型内部表示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/类型内部表示.png "类型内部表示")
 3. 类型的种类共有5种
```C++
TypeKind = {intTy, charTy, arrayTy, recordTy, boolTy}
```
 4. 标识符信息项的数据结构C语言定义
```C++
typedef struct{
    struct typeIR *idtype;/*指向标识符的类型内部表示，共有5种(intTy, charTy, arrayTy, recordTy, boolTy)*/
    IdKind kind;/*标识符的类型，共有3种(typeKind, varKind, procKind)*/
    union{
        struct{
            AccessKind access;
            int level;
            int off;
        }VarAttr;/*变量标识符的属性*/
        
        struct{
            int level;
            ParamTable *param;/*参数表*/
            int code;
            int size;
        }ProcAttr;/*过程名标识符的属性*/
    }More; /*标识符的不同类型有不同的属性*/
}AttributeIR;
```
## 实例
![词法分析和语法分析实例](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/%E8%AF%8D%E6%B3%95%E5%88%86%E6%9E%90%E5%92%8C%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90%E5%AE%9E%E4%BE%8B.png "词法分析和语法分析实例")