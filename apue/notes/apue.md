**目录**
---

<!-- TOC -->

- [APUE学习笔记](#apue学习笔记)
    - [1. 限制](#1-限制)
        - [1.1. sysconf、pathconf和fpathconf](#11-sysconfpathconf和fpathconf)
    - [2. 选项](#2-选项)
        - [2.1. POSIX.1 3种处理选项的方法(可移植程序依赖这些可选的支持功能)](#21-posix1-3种处理选项的方法可移植程序依赖这些可选的支持功能)
        - [2.2. 选项平台支持情况](#22-选项平台支持情况)
    - [3. I/O](#3-io)
        - [3.1. 常用函数](#31-常用函数)
        - [3.2. 文件描述符](#32-文件描述符)
        - [3.3. open和openat](#33-open和openat)
        - [3.4. creat](#34-creat)
        - [3.5. close](#35-close)
        - [3.6. lseek](#36-lseek)

<!-- /TOC -->

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
 * @param	path	要打开或创建文件的名字
 * @param	oflag	打开文件时的参数选项
 * @param	mode_t	文件访问权限
 * @return	fd	文件描述符
 * @return	-1	出错
 */
int open(const char *path, int oflag, ... /* mode_t mode */);

/**
 * @param	fd	要打开的文件描述符
 * @param	path	要打开或创建文件的名字
 * @param	oflag	打开文件时的参数选项
 * @param	mode_t	文件访问权限
 * @return	fd	文件描述符
 * @return	-1	出错
 */
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

*以上5个常量必须制定一个且只能指定一个，以下是可选的。

O_APPEND		每次写是都追加到文件尾。
O_CLOEXEC		把FD_CLOEXEC常量设置为文件描述符标志。
O_CREAT			若此文件不存在则创建它。在使用此选项时，open函数需同时说明第三个参数mode
			(openat函数需要说明第4个参数mode)，用mode指定该文件的访问权限。
O_DIRECTORY		如果path引用的不是目录，则出错。
O_EXCL			如果同时制定了O_CREAT，而文件已经存在，则出错。用此可以测试一个文件是否
			存在，如果不存在，则创建此文件，这使测试和创建两者成为一个原子操作。
O_NOCTTY		如果path引用的是终端设备，则不将该设备分配作为此进程的控制终端。
O_NOFOLLOW		如果path引用的是一个符号连接，则出错。
O_NONBLOCK		如果path引用的是一个FIFO，一个块特殊文件或一个字符特殊文件，则此选项为
			文件的本次打开操作和后续的I/O操作设置非阻塞方式
O_SYNC			使每次write等待物理I/O操作完成，包括由该write操作引起的文件属性更新所需的I/O。
O_TRUNC			如果此文件存在，而且为只写或读写成功打开，则将其长度截断为0.
O_TTY_INIT		如果打开一个还未打开的终端设备，设置非标准termios参数值，使其符合Single
			UNIX Specification。
O_DSYNC			使每次write要等待物理I/O操作完成，但是如果该写操作并不影响读取刚写入的
			数据，则不需要等待文件属性被更新。
O_RSYNC			使每一个以文件描述符作为参数进行的read操作等待，直至所有对文件同一部分
			挂起的写操作都完成。
```
由open和openat函数返回的文件描述符一定是最小的未用的描述符数值。

fd参数把open和openat函数区分开，共有3种可能性：
(1) path参数指定的是绝对路径名，在这种情况下，fd参数被忽略，openat函数就相当于open函数
(2) path参数指定的是相对路径名，fd参数指出了相对路径名在文件系统中的开始地址。fd参数是通过打开相对路径名所在的目录来获取。
(3) path参数指定了相对路径名，fd参数具有特殊值AT_FDCWD。在这种情况下吗，路径名在当前工作目录中获取，openat函数在操作上与open函数类似。

### 3.4. creat

```c
#include <fcntl.h>

/**
 * @param	path	要创建文件的名字
 * @param	mode_t	文件访问权限
 * @return	fd	文件描述符
 * @return	-1	出错 
 */
int creat(const char *path, mode_t mode);
```
注意，此函数等效于:
```c
open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
```
creat的一个不足之处是它以只写方式打开所创建的文件，在提供open的新版本之前，如果要创建一个临时文件，并要先写该文件，然后又读该文件，则必须先调用creat，close，然后再调用open，现在则可用下列方式调用open实现：
```c
open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
```

### 3.5. close

```c
#include <unistd.h>

/**
 * @param	fd	要关闭的文件描述符
 * @return	0	成功
 * @return	-1	失败
 */
int close(int fd);
```
关闭一个文件时还会释放该进程加在该文件上的所有记录锁。
当一个进程终止时，内核自动关闭它所有的打开文件。

### 3.6. lseek

```c
#include <unistd.h>

/**
 * @param	fd	要操作的文件描述符
 * @param	offset	偏移量
 * @param	whence	设置
 * @return	新的文件偏移量
 * @return	-1
 */
off_t lseek(int fd, off_t offset, int whence);
```
对参数offset的解释与参数whence的值有关.
- 若whence是SEEK_SET，则该文件的偏移量设置为距文件开始处offset个字节。
- 若whence是SEEK_CUR，则该文件的偏移量设置为其当前值加offset，offset可为正或负。
- 若whence是SEEK_END，则该文件的偏移量设置为文件长度加offset，oggset可为正或负。
```c
off_t currpos;
currpos = lseek(fd, 0, SEEK_CUR);
```
如果文件描述符指向的是一个管道、FIFO或网络套接字，则lseek返回-1，并将errno设置为ESPIPE。