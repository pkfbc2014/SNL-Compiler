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

## 其他说明

1. 文件目录：

   Resource：源代码.cpp和头文件.h

   Sample：较为全面地包含了各种错误检测样例，答辩时需成员各自再创建对应的错误检测样例

   Data：需自行创建，存放token序列，first、follow、predict集，符号表

   Error_Message：需自行创建，存放语义分析错误信息

2. 关于脚本作图：

   需要安装GraphViz才可运行自动作图命令，详见printTree.cpp内介绍

3. 参考资料：

   《编译程序的设计与实现》刘磊 金英 张晶 张荷花 单郸；高等教育出版社，ISBN 978-7-04-014620-7.

   《编译原理课程设计》课程PPT

4. 勘误：

   教材第 10 页，产生式 (43) 应为：`ProcDecMore -> ProcDec`. (而不是 ProcDeclare)

   教材第 63 页，产生式 (67) Predict 集应为 `[` (左中括号), `.` (句点), `:=` (赋值号).

   教材第 64 页，产生式 (93) Predict 集应包含 `]` (右中括号).

   教材第 55 页，例子中 `v2:='a';` 只能通过词法分析，而**不能通过语法分析**，因为按照给定的产生式(`Exp`)不能推出字符型。

   教材第 10 页，产生式 (42), (43) 完全多余。可以去掉，并且把产生式 (41) 右部的 `ProcDecMore` 改为 `ProcDecpart`.

   教材第 10 页，产生式 (48), 预测符集应为 `)` (右括号)，而不是左括号。(第 62 页)

   PPT中，第74条产生式RETURN按照书上来，RETURN语句不能携带运算式Exp
