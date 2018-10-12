# APUE学习笔记

## 一、限制

### 1. sysconf、pathconf和fpathconf [2.5.4]
```c
	#include <unistd.h>
	long sysconf(int name);
	long pathconf(const char* pathname, int name);	//路径名
	long fpathconf(int fd, int name);	//文件描述符
	
	所有函数返回值，若成功，返回相应值；若出错，返回-1
```

## 二、选项

### 1. POSIX.1 3种处理选项的方法(可移植程序依赖这些可选的支持功能)
1. 编译时选项定义在<unistd.h>
2. 与文件或目录无关的运行时选项用sysconf函数来判断
3. 与文件或目录有关的运行时选项通过调用pathconf或fpathconf函数来判断

### 2. 选项平台支持情况
1. 如果符号常量没有定义或者定义值为-1，那么该平台在编译时并不支持相应选项。但是有一种可能，即在已支持该选项的新系统上运行老的应用时，即使该选项在应用编译时未被支持，但如今新系统运行时检查会显示该选项已被支持。
2. 如果符号常量的定义值大于0，那么该平台支持相应选项。
3. 如果符号常量的定义值为0，那必须调用sysconf、pathconf或fpathconf来判断相应选项是否收到支持。