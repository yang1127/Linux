#src=$(wildcard ./*.c)
#test:$(src)
#
#	gcc $^ -o $@

src=$(wildcard ./*.c) #test.c test1.c
obj=$(patsubst %.c, %.o, $(src)) #test.o test.o
#$(patsubst %.c,%o,$(src))
#patsubst 关键字将src中的内容进行字符串替换，将.c替换为.o

test:$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $^ -o $@

.PHONY：clear
clear:
	rm -f $(obj)

