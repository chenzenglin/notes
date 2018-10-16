**目录**
---

**[简介](#简介)**<br>
**[LINUX](#linux)**<br>
**[APUE](#apue)**<br>
**[CPP](#cpp)**<br>
**[MAKE](#make)**<br>
**[GDB](#gdb)**<br>

# Notes

## 简介
```markdown
    author  ||  stanley
    version ||  0.0.3v
    update  ||  2018-10-17 00:36:30
```

## LINUX

### 压缩与解压缩

- 压缩
    - [ ]  tar  -cvf    jpg.tar     *.jpg   //将目录里所有jpg文件打包成tar.jpg 
    - [x]  tar  -czf    jpg.tar.gz  *.jpg   //将目录里所有jpg文件打包成jpg.tar后，并且将其用gzip压缩，生成一个gzip压缩过的包，命名为jpg.tar.gz
    - [x]  tar  -cjf    jpg.tar.bz2 *.jpg   //将目录里所有jpg文件打包成jpg.tar后，并且将其用bzip2压缩，生成一个bzip2压缩过的包，命名为jpg.tar.bz2
    - [ ]  tar  -cZf    jpg.tar.Z   *.jpg   //将目录里所有jpg文件打包成jpg.tar后，并且将其用compress压缩，生成一个umcompress压缩过的包，命名为jpg.tar.Z
    - [ ]  rar  a       jpg.rar     *.jpg   //rar格式的压缩，需要先下载rar for linux
    - [x]  zip          jpg.zip     *.jpg   //zip格式的压缩，需要先下载zip for linux
    
- 解压缩
    - [ ]  tar  -xvf    file.tar        //解压tar
    - [x]  tar  -xzvf   file.tar.gz     //解压tar.gz
    - [x]  tar  -xjvf   file.tar.bz2    //解压tar.bz2
    - [ ]  tar  -xZvf   file.tar.Z      //解压tar.Z
    - [ ]  unrar    e   file.rar        //解压rar
    - [x]  unzip        file.zip        //解压zip

### 时间服务器上的时间同步

1.  安装ntpdate工具
```shell
    yum -y install ntp ntpdate
```
2.  设置系统时间与网络时间同步
```shell
    ntpdate cn.pool.ntp.org
```
3.  将系统时间写入硬件时间
```shell
    hwclock --systohc
```

### Git

1. 修改默认提交编译器
```shell
    git config --global core.editor vim
```
2. 如何提交一个空目录
```shell
    find . -type d -empty -exec touch {}/.gitignore \;
```

## [APUE](https://github.com/stanleyguo0207/notes/tree/master/apue/notes/apue.md)

### 准备工作

- apue.h 缺失
```markdown
    download
    notes/apue/res/src.3e.tar.gz     
    tar -xzvf src.3e.tar.gz

    cp ./apue.3e/include/apue.h /usr/include/
    cp ./apue.3e/lib/error.c /usr/include/

    编辑/usr/include/apue.h文件
    在文件最后#endif前加上包含error.c的代码：
    #include "error.c";
```

## [CPP](https://github.com/stanleyguo0207/notes/tree/master/cpp/notes/cpp.md)

## [MAKE](https://github.com/stanleyguo0207/notes/tree/master/make/notes/make.md)

### 前言

Makefile是大型项目中必备的技能之一，所以花了些时间整理了文档。

## [GDB](https://github.com/stanleyguo0207/notes/tree/master/gdb/notes/gdb.md)

GDB调试是大型项目中必备的技能之一，所以花了些时间整理了文档。