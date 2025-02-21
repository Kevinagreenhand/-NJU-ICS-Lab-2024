# 这个Makefile没有声明环境变量。和剩下三个文件里的Makefile一样，这里也是要声明环境变量的。不过不管怎么说，代码是没错的。
NAME := $(shell basename $(PWD))
all: $(NAME)-64 $(NAME)-32
export MODULE := Lab1
include ../Makefile
