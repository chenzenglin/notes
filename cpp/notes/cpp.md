[TOC]

# CPP学习笔记

## 一、知识架构

## 二、易混淆知识点
### 1. 顶层const和底层const

  指针本身是一个对象，它又可以指向另外一个对象。因此，指针本身是不是常量以及指针所指的是不是一个常量就是两个互相独立的问题。
  顶层const(top-level const)表示指针本身是个常量。
  底层const(low-level const)表示指针所指的对象是一个常量。

```cpp
	int i = 0;
	int *const p1 = &i;			//不能改变p1的值，这是一个顶层const
	const int ci = 42;			//不能改变ci的值，这是一个顶层const
	const int *p2 = &ci;		//允许改变p2的值，这是一个底层const
	const int *const p3 = p2;	//靠右的const是顶层const，靠左的是底层const
	const int &r = ci;			//用于生命引用的const都是底层const

	i = ci;				//正确，拷贝ci的值，ci是一个顶层const，对此操作无影响
	p2 = p3;			//正确，p2和p3指向的对象类型相同，p3顶层const的部分不影响
	int *p = p3;		//错误，p3包含底层const的定义，而p没有
	p2 = &i;			//正确，int*能转换成const int*
	int &r = ci;		//错误，普通的int&不能绑定到int常量上
	const int &r2 = i;	//正确，const int&可以绑定到一个普通的int上
```
