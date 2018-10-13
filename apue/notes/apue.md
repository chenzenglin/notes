## 目录
- [APUE学习笔记](#apue学习笔记)
    - [1. 限制](#1-限制)
        - [1.1. sysconf、pathconf和fpathconf](#11-sysconfpathconf和fpathconf)
    - [2. 选项](#2-选项)
        - [2.1. POSIX.1 3种处理选项的方法(可移植程序依赖这些可选的支持功能)](#21-posix1-3种处理选项的方法可移植程序依赖这些可选的支持功能)
        - [2.2. 选项平台支持情况](#22-选项平台支持情况)

# APUE学习笔记

## 1. 限制

### 1.1. sysconf、pathconf和fpathconf
章节2.5.4
```c
	#include <unistd.h>
	long sysconf(int name);
	long pathconf(const char* pathname, int name);	//路径名
	long fpathconf(int fd, int name);	//文件描述符
	
	所有函数返回值，若成功，返回相应值；若出错，返回-1
```

## 2. 选项

### 2.1. POSIX.1 3种处理选项的方法(可移植程序依赖这些可选的支持功能)
1. 编译时选项定义在<unistd.h>
2. 与文件或目录无关的运行时选项用sysconf函数来判断
3. 与文件或目录有关的运行时选项通过调用pathconf或fpathconf函数来判断

### 2.2. 选项平台支持情况
1. 如果符号常量没有定义或者定义值为-1，那么该平台在编译时并不支持相应选项。但是有一种可能，即在已支持该选项的新系统上运行老的应用时，即使该选项在应用编译时未被支持，但如今新系统运行时检查会显示该选项已被支持。
2. 如果符号常量的定义值大于0，那么该平台支持相应选项。
3. 如果符号常量的定义值为0，那必须调用sysconf、pathconf或fpathconf来判断相应选项是否收到支持。

## 3. I/O

### 3.1. 常用函数
open、read、write、lseed以及close

### 3.2. 文件描述符
文件描述符变化范围 **0 ~ OPEN_MAX - 1**
0 标准输入	**STDIN_FILENO**
1 标准输出	**STDOUT_FILENO**
2 标准错误	**STDERR_FILENO**

### 3.3. open和openat
```c
	#include <fcntl.h>

	/**
	 * @param path	要打开或创建文件的名字
	 * @param oflag	打开文件时的参数选项
	 * @return fd	文件描述符
	 * @return -1	出错
	 */
	int open(const char *path, int oflag, ... /* mode_t mode */);
	int openat(int fd, const char *path, int oflag, ... /* mode_t mode */);
```

**oflag参数**
```c
	O_RDONLY		只读打开
	O_WRONLY		只写打开
	O_RDWR			读写打开
		-- 大多数实现将O_RDONLY定义为0，O_WRONLY定义为1，O_RDWR定义为2
	O_EXEC			只执行打开
	O_SEARCH		只搜索打开(应用于目录)
		-- O_SEARCH用来验证在目录打开时它的搜索权限
	
	*以上5个常量必须制定一个且只能指定一个，以下是可选的

	O_APPEND		每次写是都追加到文件尾
	O_CLOEXEC		把FD_CLOEXEC常量设置为文件描述符标志
	O_CREAT			若此文件不存在则创建它。在使用此选项时，open函数需同时说明第三个参数mode
				(openat函数需要说明第4个参数mode)，用mode指定该文件的访问权限
	O_DIRECTORY
	O_EXCL
	O_NOFOLLOW
	O_NONBLOCK
	O_SYNC
	O_TRUNC
	O_TTY_INIT
	O_DSYNC
```