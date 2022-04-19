# SNL-Compiler
2019级吉林大学计算机科学与技术专业编译原理课程设计——SNL语言编译器

## 词法分析

## 语法分析
![语法树图示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语法树图示.png "语法树图示")

在实际编写程序时并未严格按照以上结构创建语法树，事实上可以随便创建，只要语义分析方便就行。

1. 递归下降语法分析（RD）能检测出来的错误：

   |                           语法错误                           | 对应非终结符函数 |
   | :----------------------------------------------------------: | :--------------: |
   |         变量类型不为基本类型（即不为CHAR与INTEGER）          |    baseType()    |
   |  域声明中变量类型不为基本类型（即不为CHAR、INTEGER与ARRAY）  |  fieldDecList()  |
   | 过程声明中的变量类型不为基本类型（即不为INTEGER、CHAR、RECORD、ARRAY、ID与VAR，其中VAR是变参，其余为值参） |     param()      |
   | 条件语句中表达式运算符不匹配（注：并不是操作数类型不匹配，那是语义分析干的事） | conditionalStm() |
   | 循环语句中的表达式运算符不匹配（注：并不是操作数类型不匹配，那是语义分析干的事） |    loopStm()     |
   |                  赋值语句与函数调用语句出错                  |    assCall()     |
   |                      加运算运算符不正确                      |     addOp()      |
   |                      乘运算运算符不正确                      |     multOp()     |
   |                    因子递归函数书写不正确                    |     factor()     |

   在遇到错误时，采用恐慌策略，即：记录下当前错误，并忽略当前错误，继续运行。

   直到语法分析结束，输出错误信息后停机。

2. LL1分析法能检测出来的错误：

   

## 语义分析
### SNL符号表信息项

1. 标识符共有3种
```C++
Kind = {typeKind, varKind, procKind}
```
![语义分析标识符种类](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语义分析标识符种类.png "语义分析标识符种类")
2. 类型标识符内部表示
 ![类型内部表示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/类型内部表示.png "类型内部表示")

```C++
intPtr{size, kind}
charPtr{size, kind}

arrayPtr{size, kind, indexty, elemTy}
recordPte{size, kind, body}

body: 
struct fieldChain{
    idname
    unitType
    offset
    next    
}
```


 3. 类型的种类共有5种
```C++
TypeKind = {intTy, charTy, arrayTy, recordTy, boolTy}
```

## 实例
![词法分析和语法分析实例](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/%E8%AF%8D%E6%B3%95%E5%88%86%E6%9E%90%E5%92%8C%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90%E5%AE%9E%E4%BE%8B.png "词法分析和语法分析实例")
