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
        - [3.7. read](#37-read)
        - [3.8. write](#38-write)
        - [3.9. I/O的效率](#39-io的效率)
        - [3.10 文件共享](#310-文件共享)

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

文件偏移量可以大于文件的当前长度，在这种情况下，对该文件的下次写将加长该文件，并在文件中构成一个空洞，这一点是允许的。位于文件中但没有写过的字节都被读为0.
文件中的空洞并不要求在磁盘上占用存储区。具体处理方式与文件系统的实现有关，当定位到超出文件尾端之后写时，对于新写的数据需要分配磁盘块，但是对于原文件尾部和新开始写位置之间的部分则不需要分配磁盘块。
```shell
od -c file.hole
```
使用od命令观察该文件的实际内容。命令行中的-c标志表示以字符方式打印文件内容。

因为lseek使用的偏移量是off_t类型表示的，所以允许具体实现根据各自特定的平台自行选择大小合适的数据类型。现今大多数平台提供两组接口处理文件偏移量，一组使用32位文件偏移量，另一组使用64位文件偏移量。

| 选项名称                | 说明                                      | name参数             |
| ---------------------- | ----------------------------------------- | ------------------- |
| _POSIX_V7_ILP32_OFF32  | int、long、指针和off_t类型是32位            | _SC_V7_ILP32_OFF32  |
| _POSIX_V7_ILP32_OFFBIG | int、long、指针类型是32位，off_t类型至少64位 | _SC_V7_ILP32_OFFBIG |
| _POSIX_V7_LP64_OFF64   | int类型32位，long、指针和off_t类型是64位     | _SC_V7_LP64_OFF64   |
| _POSIX_V7_LP64_OFFBIG  | int类型32位，long、指针和off_t类型至少是64位 | _SC_V7_LP64_OFFBIG  |

### 3.7. read

```c
#include <unistd.h>

/**
 * @param	fd	要读取的文件描述符
 * @param	buf	读取数据存放的缓冲区
 * @param	nbytes	读取字节数
 * @return	读到的字节数，若已读到文件尾，返回0
 * @return	-1	出错
 */
ssize_t read(int fd, void *buf, size_t nbytes);
```
read函数原型
```c
int read(int fd, char *buf, unsigned nbytes);
```
- 为了与ISO C一致，第2个参数由char *改为void *。在ISO C中，类型void *用于表示通用指针。
- 其次，返回值必须是一个带符号的整型(ssize_t)，以保证能够返回正整数字节数、0或-1。
- 最后，第3个参数在历史上是一个无符号整型，这允许一个16位的实现一次读或写的数据。新的基本系统数据类型ssize_t以提供带符号的返回值。

### 3.8. write

```c
#include <unistd.h>

/**
 * @param	fd	要写入的文件描述符
 * @param	buf	写入数据存放的缓冲区
 * @param	nbytes	写入字节数
 * @return	返回已写的字节数
 * @return	-1	出错
 */
ssize_t write(int fd, const void *buf, size_t nbytes);
```
其返回值通常与参数nbytes的值相同，否则表示出错。

### 3.9. I/O的效率

### 3.10 文件共享

内核使用3种数据结构表示打开文件，它们之间的关系决定了在文件共享方面一个进程对另一个进程可能产生的影响。
(1) 每个进程在进程中都有一个记录项，记录项中包含一张打开文件描述符表，可将其视为一个矢量，每个描述符占用一项。与每个文件描述符相关联的是：
	a. 文件描述符标志(close_on_exec)。
	b. 指向一个文件表项的指针。
(2) 内核为所有打开文件维持一张文件表。每个文件表包含：
	a. 文件状态标志(读、写、添写、同步和非阻塞等)。
	b. 当前文件偏移量。
	c. 指向该文件v节点表项的指针。
(3) 每个打开文件(或设备)都有一个v节点(v-node)结构。v节点包含了文件类型和对此文件进行各种操作函数的指针。对于大多数文件，v节点还包含了该文件的i节点(i-node，索引节点)。这些信息是在打开文件时从磁盘上读入内存的，所有，文件的所有相关信息都是随时可用的。例如，i节点包含了文件的所有者、文件长度、指向文件实际数据块在磁盘上所在位置的指针等。

![][1]

[1]: https://github.com/stanleyguo0207/notes/tree/master/apue/res/3-10-1.png