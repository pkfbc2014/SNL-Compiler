1. # SNL-Compiler
2019级吉林大学计算机科学与技术专业编译原理课程设计——SNL语言编译器

## 词法分析

## 语法分析
![语法树图示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语法树图示.png "语法树图示")
## 语义分析
### SNL标识符

1. 标识符共有三种
![语义分析标识符种类](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/语义分析标识符种类.png "语义分析标识符种类")
1. 类型标识符内部表示
 ![类型内部表示](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/类型内部表示.png "类型内部表示")
1. 类型的种类共有5种
```C++
TypeKind = {intTy, charTy, arrayTy, recordTy, boolTy}
```
## 实例
![词法分析和语法分析实例](https://github.com/pkfbc2014/SNL-Compiler/blob/main/Image/%E8%AF%8D%E6%B3%95%E5%88%86%E6%9E%90%E5%92%8C%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90%E5%AE%9E%E4%BE%8B.png "词法分析和语法分析实例")