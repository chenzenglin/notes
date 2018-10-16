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
        - [3.11. 原子操作](#311-原子操作)
        - [3.12. dup和dup2](#312-dup和dup2)
        - [3.13. sync、fsync和fdatasync](#313-syncfsync和fdatasync)
        - [3.14. fcntl](#314-fcntl)
        - [3.15. ioctl](#315-ioctl)
        - [3.16. /dev/fd](#316-devfd)
    - [4. 文件和目录](#4-文件和目录)
        - [4.1. stat、fstat、fstatat和lstat](#41-statfstatfstatat和lstat)
        - [4.2. 文件类型](#42-文件类型)
        - [4.3. 设置用户ID和设置组ID](#43-设置用户id和设置组id)
        - [4.4. 文件访问权限](#44-文件访问权限)
        - [4.5. 新文件和目录的所有权](#45-新文件和目录的所有权)
        - [4.6. 函数access和faccessat](#46-函数access和faccessat)
    - [5. 标准I/O库](#5-标准io库)
    - [6. 系统数据文件和信息](#6-系统数据文件和信息)
    - [7. 进程环境](#7-进程环境)
    - [8. 进程控制](#8-进程控制)
    - [9. 进程关系](#9-进程关系)
    - [10. 信号](#10-信号)
    - [11. 线程](#11-线程)
    - [12. 线程控制](#12-线程控制)
    - [13. 守护进程](#13-守护进程)
    - [14. 高级I/O](#14-高级io)
    - [15. 进程间通信](#15-进程间通信)
    - [16. 网络IPC：套接字](#16-网络ipc套接字)
    - [17. 高级进程间通信](#17-高级进程间通信)
    - [18. 终端I/O](#18-终端io)
    - [19. 伪终端](#19-伪终端)
    - [20. 数据库函数库](#20-数据库函数库)
    - [21. 与网络打印机通信](#21-与网络打印机通信)

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

Linux没有使用v节点，而是使用了通用i节点结构。虽然两种实现有所不同，但在概念上，v节点和i节点是一样的。两者都指向文件系统特有的i节点结构。
Linux没有将相关的数据结构分为i节点和v节点，而是采用了一个与文件系统相关的i节点和一个与文件系统无关的i节点。

打开文件的内核数据结构:
![内核数据结构][1]

两个独立进程各自打开同一个文件
![同一文件结构图][2]

每个进程都有自己的文件表项，是因为这可以使每个进程都有它自己的对该文件的当前偏移量。

**注意**，文件描述符标志和文件状态标志在作用范围的区别，前者只用于一个进程的一个描述符，而后者则应用与指向该给定文件表项的任何进程中的所有描述符。

### 3.11. 原子操作

1. 追加到一个文件
考虑一个进程，它要将数据追加到一个文件尾端。早期的UNIX系统版本并不支持open的O_APPEND选项，所以程序被编写成下列形式：
```c
if(lseek(fd, OL, 2) < 0) {		/* position to EOF */
	err_sys("lseek error");
}
if(100 != write(fd, buf, 100)) {	/* and write */
	err_sys("write error");
}
```
2. 函数pread和pwirte
```c
#include <unistd.h>

/**
 * @param	fd	要读取的文件描述符
 * @param	buf	读取数据存放的缓冲区
 * @param	nbytes	读取字节数
 * @param	offset	读文件前文件偏移量
 * @return	读到的字节数，若已读到文件尾，返回0
 * @return	-1	出错
 */
ssize_t pread(int fd, void *buf, size_t nbytes, off_t offset);

/**
 * @param	fd	要写入的文件描述符
 * @param	buf	写入数据存放的缓冲区
 * @param	nbytes	写入字节数
 * @param	offset	写文件前文件偏移量
 * @return	返回已写的字节数
 * @return	-1	出错
 */
ssize_t pwrite(int fd, const void *buf, size_t nbytes, off_t offset);
```
调用pread相当于调用lseek后调用read
- 调用pread时，无法终端其定位和读操作。
- 不更新当前文件偏移量

调用pwrite相当于调用lseek后调用write

3. 创建一个文件
```c
if((fd = open(pathname, O_WRONLY)) < 0) {
	if(ENOENT == errno) {
		if((fd = creat(path, mode)) < 0) {
			err_sys("creat error");
		}
	}
}
```

一般而言，**原子操作**指的是由多步组成的一个操作。

### 3.12. dup和dup2

下面两个函数都可用来复制一个现有的文件描述符
```c
#include <unistd.h>

/**
 * @param	fd	要复制的文件描述符
 * @return	新的文件描述符
 * @return	-1	出错
 */
int dup(int fd);

/**
 * @param	fd	要复制的文件描述符
 * @param	fd2	新的文件描述符
 * @return	新的文件描述符
 * @return	-1	出错
 */
int dup2(int fd, int fd2);
```
dup返回的新文件描述符一定是当前可用文件描述符中的最小数值。
dup2可以用fd2指定新的描述符的值。如果fd2已经打开，则先将其关闭。若fd等于fd2，则dup2返回fd2，而不关闭它。否则，fd2的FD_CLOEXEC文件描述符标志就被清楚，这样fd2在进程调用exec时是打开状态。
```c
dup(fd);

//等价于：
fcntl(fd, F_DUPFD, 0);
```
```c
dup2(fd, fd2);

//等价于：
fcntl(fd, F_DUPFD, fd2);
```
在后一种情况中，dup2并不完全等同于close加上fcntl。它们之间的区别具体如下：
- dup2是一个原子操作，而close和fcntl包括两个函数调用。有可能在close和fcntl之间调用了信号捕捉函数，它可能修改文件描述符。如果不同的线程改变了文件描述符的话也会出现相同的问题。
- dup2和fcntl有一些不同的errno。

### 3.13. sync、fsync和fdatasync

当内核需要重用缓冲区来存放其他磁盘块数据时，它会把所有延迟写数据块写入磁盘。为了保证磁盘上实际文件系统与缓冲区中内容的一致性，UNIX系统提供了sync、fsync和fdatasync三个函数。
```c
#include <unistd.h>

/**
 * @param	fd	同步文件描述符
 * @return	0	成功
 * @return	-1	出错
 */ 
int fsync(int fd);

/**
 * @param	fd	同步文件描述符
 * @return	0	成功
 * @return	-1	出错
 */ 
int fdatasync(int fd);

//sync只是将所有修改过的块缓冲区排入队列，然后就返回，他并不等待实际写磁盘操作结束。
void sync(void);
```
通常，成为update的系统守护进程周期性调用(一般每隔30秒)sync函数，这就保证了定期冲洗(flush)内核块缓冲区。
fsync函数只对由文件描述符fd指定的一个文件起作用，并且等待写磁盘操作结束才返回。fsync可用于数据库这样的应用程序，这种应用程序需要确保修改过的块立即写到磁盘上。
fdatasync函数类似于fsync，但它只影响文件的数据部分，而除数据外，fsync还会永不更新文件的属性。

### 3.14. fcntl

fcntl可以改变已经打开文件的属性。
```c
#include <fcntl.c>

/**
 * @param	fd	要修改文件的文件描述符
 * @param	cmd	命令
 * @param	arg	参数
 * @return	依赖cmd
 * @return	-1	出错
 */
int fcntl(int fd, int cmd, ... /* int arg */);
```
fcntl函数有以下5种功能：
1. 复制一个已有的文件描述符(cmd=F_DUPFD或F_DUPFD_CLOEXEC)。
2. 获取/设置文件描述符标志(cmd=F_GETFD或F_SETFD)。
3. 获取/设置文件状态标志(cmd=F_GETFL或F_SETFL)。
4. 获取/设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN)。
5. 获取/设置记录锁(cmd=F_GETLK、F_SETLK或F_SETLKW)。

*F_DUPFD*
- 复制文件描述符fd。新文件描述符作为函数值返回。它是尚未打开的各描述符中大于等于第3个参数值(取为整数值)中各值的最小值。新描述符与fd共享同一文件表项。但是，新描述符有它自己的一套描述符标志，其FD_CLOEXEC文件描述符标志被消除(这表示该描述符在exec时仍保持有效)。

*F_DUPFD_CLOEXEC*
- 复制文件描述符，设置与新描述符关联的FD_CLOEXEC文件描述符标志的值，返回新文件描述符。

*F_GETFD*
- 对应与fd的文件描述符标志作为函数值返回。当前只定义了一份文件描述符标志FD_CLOEXEC。

*F_SETFD*
- 对于fd设置文件描述符标志。新标志值按第3个参数(取为整数值)设置。

*F_GETFL*
- 对应于fd的文件状态标志作为函数值返回。我们在说明open函数时，已描述了文件状态标志。遗憾的是，5个访问方式标志(O_RDONLY、O_WRONLY、O_RDWR、O_EXEC以及O_SEARCH)并不各占1位(如前所述，由于历史原因，前3个标志的值分别是0、1和2.这5个值互斥、一个文件的访问方式只能取这5个值之一)。因此首先必须用屏蔽字O_ACCMODE取得访问方式位，然后将结果与这5个值中的每一个相比较。

*F_SETFL*
- 将文件状态标志设置为第3个参数的值(取为整数值)。可以更改的几个标志是：O_APPEND、O_NONBLOCK、O_SYNC、O_DSYNC、O_RSYNC、O_FSYNC和O_ASYNC。

*F_GETOWN*
- 获取当前接受SIGIO和SIGURG信号的进程ID或者进程组ID。

*F_SETOWN*
- 设置接受SIGIO和SIGURG信号的进程ID或进程组ID。正的arg指定一个进程ID，负的arg表示等于arg绝对值的一个进程组ID。

### 3.15. ioctl

```c
#include <unistd.h>     /* System V */
#include <sys/ioctl.h>  /* BSD and Linux */

/**
 * @param   fd
 * @param   request
 * @return  其他值      成功
 * @return  -1          出错
 */
int ioctl(int fd, int request, ...);    /* POSIX.1 */
int ioctl(int fd, unsigned long request, ...);
```
在此原型中，我们表示的只是ioctl函数本身所要求的头文件。通常，还要求另外的设备专用头文件。例如，除POSIX.1所说明的基本操作之外，终端I/O的ioctl命令都需要头文件\<termios.h>。

| 类别      | 变量名  | 头文件              | ioctl数 |
| -------- | ------- | ------------------ | :-----: |
| 盘标号    | DTOxxx  | \<sys/disklabel.h> | 4       |
| 文件I/O   | FIOxxx  | \<sys/filio.h>     | 14      |
| 磁盘I/O   | MTIOxxx | \<sys/mtio.h>      | 11      |
| 套接字I/O | SIOxxx  | \<sys/sockio.h>    | 73      |
| 终端I/O   | TIOxxx  | \<sys/ttycom.h>    | 43      |

FreeBSD中通用的ioctl操作

### 3.16. /dev/fd

较新的系统都提供名为/dev/fd的目录，其目录项是名为0、1、2等的文件。打开文件/dev/fd/n等效于复制描述符n(假定描述符n是打开的)。
```c
fd = open("/dev/fd/0", mode);
```
等效于：
```c
fd = dup(0);
```
所以描述符0和fd共享同一文件表项。例如，若描述符0先前被打开为只读，那么我们只能对fd进行读操作。即使系统忽略打开模式，而且下列调用是成功的：
```c
fd = open("/dev/fd/0", O_RDWR);
```
我们仍然不能对fd进行写操作。

/dev/fd文件主要由shell使用，它允许使用路径名作为调用参数的程序，能用处理其他路径名的相同方式处理标准输入和输出。例如，**cat**命令对其命令行参数采用了一种特殊处理，它将单独的一个字符“-”解释为标准输入。
```shell
filter file2 | cat file1 - file3 | lpr
```
```shell
filter file2 | cat file1 /dev/fd/0 file3 | lpr
```

## 4. 文件和目录

### 4.1. stat、fstat、fstatat和lstat

```c
#include <sys/stat.h>

/**
 * @return 0    成功
 * @return -1   失败
 */

//返回pathname文件相关的信息结构
int stat(const char *restrict pathname, struct stat *restrict buf);

//获得已在描述符fd上打开文件的有关信息
int fstat(int fd, struct stat *buf);

//与stat类似，但是当命名的文件是一个符号链接时，返回该符号链接的信息，而不是由该符号链接引用的文件的信息
int lstat(const char *restrict pathname, struct stat *restrict buf);

//为一个相对于当前打开目录(由fd参数指向)的路径名返回文件统计信息
int fstatat(int fd, const char *restrict pathname, struct stat *restrict buf, int flag);
```
fstatat中flag参数控制着是否跟随着一个符号链接。当AT_SYMLINK_NOFOLLOW标志被设置时，fstatat不会跟随符号链接，而是返回符号链接本身的信息。否则，在默认情况下，返回的是符号链接所指向的实际文件的信息。如果fd参数的值是AT_FDCWD，并且pathname参数是一个相对路径名，fstatat会计算相对于当前目录的pathname参数。如果pathname是一个绝对路径，fd参数就会被忽略。这两种情况下，根据flag的取值，fstatat的作用就跟stat和lstat一样。
struct stat结构
```c
struct stat {
    mode_t              st_mode;    /* file type & mode (permissions) */
    ino_t               st_ino;     /* i-node number (serial number) */
    dev_t               st_dev;     /* device number (file system) */
    dev_t               st_rdev;    /* device number for special files */
    nlink_t             st_nlink;   /* numer of links */
    uid_t               st_uid;     /* user ID of owner */
    gid_t               st_gid;     /* group ID for owner */
    off_t               st_size;    /* size in bytes, for regular files */
    struct timespec     st_atime;   /* time of last access */
    struct timespec     st_mtime;   /* time of last modification */
    struct timespec     st_ctime;   /* time of last file status change */
    blksize_t           st_blksize; /* best I/O block size */
    blkcnt_t            st_blocks;  /* number of disk blocks allocated */
}
```
timespec结构类型按照秒和纳秒定义了时间，至少包括下面两个字段：
```c
time_t tv_sec;
long tv_nsec;
```

### 4.2. 文件类型

文件类型：
1. 普通文件
2. 目标文件
3. 块特殊文件
    这种类型的文件提供对设备（如磁盘）带缓冲的访问，每次访问以固定长度为单位进行。
4. 字符特殊文件
    这种类型的文件提供对设备不带缓冲区的访问，每次访问长度可变。系统中所有设备要么是字符特殊文件，要么是块特殊文件。
5. FIFO
    这这种类型的文件用于进程间通信，有时也称为命名管道。
6. 套接字(socket)
    这种类型的文件用于进程间的网络通信。套接字也可用于在一台宿主机上进程之间的非网络通信。
7. 符号链接(symbolic link)
    这种类型的文件指向另一个文件。

文件类型信息包含在stat结构的st_mode成员中。

| 宏         | 文件类型     |
| ---------- | ----------- |
| S_ISREG()  | 普通文件     |
| S_ISDIR()  | 目录文件     |
| S_ISCHR()  | 字符特殊文件 |
| S_ISBLK()  | 块特殊文件   |
| S_ISFIFO() | 管道或FIFO   |
| S_ISLNK()  | 符号链接     |
| S_ISSOCK() | 套接字      |

\<sys/stat.h>中的文件类型宏

| 宏            | 对象的类型   |
| ------------- | ----------- |
| S_TYPEUSMQ()  | 消息队列     |
| S_TYPEISSEM() | 信号量      |
| S_TYPEISSHM() | 共享存储对象 |

\<sys/stat.h>中IPC类型宏
它们的参数并非st_mode，而是指向stat结构的指针。

早期的UNIX版本并不提供S_ISxxx宏，于是需要就st_mode与屏蔽字S_IFMT进行逻辑“与”运算，然后与名为S_IFxxx的常量相比较。大多数系统在文件\<sys/stat.h>中定义了此屏蔽字和相关的常量。
```c
#define S_ISDIR (mode) (((mode) & S_IFMT) == S_IFDIR)
```

### 4.3. 设置用户ID和设置组ID

与一个进程相关联的ID有6个或更多
- **实际用户ID和实际组ID**标识我们究竟是谁。这两个字段在登录时取自口令文件中的登录项。
- **有效用户ID、有效组ID以及附属组ID**决定了我们的文件访问权限。
- **保存的设置用户ID和保存的设置组ID**在执行一个程序时包含了有效用户ID和有效组ID的副本。

通常有效用户ID等于实际用户ID，有效组ID等于实际组ID。

文件模式字中**设置用户ID**(set-user-ID)位和**设置组ID**(set-group-ID)位。

### 4.4. 文件访问权限

| st_mode屏蔽                   | 含义                        |
| ----------------------------- | -------------------------- |
| S_IRUSR<br>S_IWUSR<br>S_IXUSR | 用户读<br>用户写<br>用户执行 |
| S_IRGRP<br>S_IWGRP<br>S_IXGRP | 组读<br>组写<br>组执行       |
| S_IROTH<br>S_IWOTH<br>S_IXOTH | 其他读<br>其他写<br>其他执行 |

9个访问权限位，\<sys/stat.h>
术语用户指的是文件所有者（owner）。**chmod**命令用户修改这9个权限位。该命令允许我们用u表示用户（所有者），用g表示组，用o表示其他。
这3类访问权限(即读、写及执行)以各种方式由不同的函数使用。使用方式汇总如下：
-   第一个规则是，我们用名字打开任意类型的文件时，对该名字中包含的每一个目录，包括它可能隐含的当前工作目录都应具有执行权限。这就是为什么对于目录其执行权限位常被称为搜索位的原因。
    例如，为了打开文件/usr/include/stdio.h，需要对目录/，/usr和/usr/include具有执行权限。然后，需要具有对文件本身的适当权限，这取决与以何种模式打开它(只读、读-写等)。
    如果当前目录是/usr/include，那么为了打开文件stdio.h，需要对当前目录有执行权限。这是隐含当前目录的一个示例。打开stdio.h文件与打开./stdio.h作用相同。
    **注意**，目录的读权限和执行权限的意义是不相同的。读权限允许我们读目录，获得在该目录中所有文件名的列表。当一个目录是我们要访问文件的路径名的一个组成部分时，对该目录的执行权限使我们可以通过该目录（也就是搜索该目录，寻找一个特定的文件名）。
-   对于一个文件的读权限决定了我们是否能够打开现有文件进行读操作。这与open函数的O_RDONLY和O_RDWR标志相关。
-   对于一个文件的写权限决定了我们是否能够打开现有文件进行写操作。这与open函数的O_WRONLY和O_RDWR标志相关。
-   为了在open函数中对一个文件指定O_TRUNC标志，必须对该文件具有写权限。
-   为了在一个目录中创建一个新文件，必须对该目录具有写权限和执行权限。
-   为了删除一个现有文件，必须对包含该文件的目录具有写权限和执行权限。对该文件本身则不需要有读、写权限。
-   如果用7个exec函数中的任何一个执行某个文件，那必须对该文件具有执行权限。该文件还必须是一个普通文件。

进程每次打开、创建或删除一个文件时，内核就进行文件访问权限测试，而这种测试可能涉及文件的所有者（st_uid和st_gid）、进程的有效ID（有效用户ID和有效组ID）以及进程的附属组ID（若支持的话）。两个所有者ID是文件的性质，而两个有效ID和附属组ID则是进程的性质。内核进行的测试具体如下。
1.  若进程的有效用户ID是0（超级用户），则允许访问。这给予了超级用户对整个文件系统进行处理的最充分的自由。
2.  若进程的有效用户ID等于文件的所有者ID（也就是进程拥有此文件），那么如果所有者适当的访问权限位被设置，则允许访问；否则拒绝访问。*适当的访问权限位*指的是，若进程为读而打开该文件，则用户读位应为1；若进程为写而打开该文件，则用户写位应为1，；若进程将执行该文件，则用户执行位应为1。
3.  若进程的有效用户组ID或进程的附属组ID之一等于文件的组ID，那么如果组适当的访问权限位被设置，则允许访问；否则拒绝访问。
4.  若其他用户适当的访问权限位被设置，则允许访问；否则拒绝访问。

### 4.5. 新文件和目录的所有权

open和creat创建新文件时，新文件的用户ID设置为进程的有效用户ID。关于组ID，POSIX.1允许事先选择下列之一作为新文件的组ID。
1.  新文件的组ID可以是进程的有效组ID。
2.  新文件的组ID可以是它所在目录的组ID。

### 4.6. 函数access和faccessat



## 5. 标准I/O库
## 6. 系统数据文件和信息
## 7. 进程环境
## 8. 进程控制
## 9. 进程关系
## 10. 信号
## 11. 线程
## 12. 线程控制
## 13. 守护进程
## 14. 高级I/O
## 15. 进程间通信
## 16. 网络IPC：套接字
## 17. 高级进程间通信
## 18. 终端I/O
## 19. 伪终端
## 20. 数据库函数库
## 21. 与网络打印机通信

[1]: https://github.com/stanleyguo0207/notes/blob/master/apue/res/icon1.png
[2]: https://github.com/stanleyguo0207/notes/blob/master/apue/res/icon2.png