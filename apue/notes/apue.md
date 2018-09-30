[TOC]

# APUE学习笔记

## 一、 限制
### 1. sysconf、pathconf和fpathconf [2.5.4]
```c
	#include <unistd.h>
	long sysconf(int name);
	long pathconf(const char* pathname, int name);	//路径名
	long fpathconf(int fd, int name);	//文件描述符
	
	所有函数返回值，若成功，返回相应值；若出错，返回-1
```


