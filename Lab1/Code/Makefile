# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

# test 中的 .output文件
OUTPUTS = $(shell find ../Test/ -name "*.output")

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	./parser ../Test/test1.cmm > ../Test/test1.output
	./parser ../Test/test2.cmm > ../Test/test2.output
	./parser ../Test/test3.cmm > ../Test/test3.output
	./parser ../Test/test4.cmm > ../Test/test4.output
	./parser ../Test/test5.cmm > ../Test/test5.output
	./parser ../Test/test6.cmm > ../Test/test6.output
	diff ../Test/test1.output ../Test/test1.answer
	diff ../Test/test2.output ../Test/test2.answer
	diff ../Test/test3.output ../Test/test3.answer
	diff ../Test/test4.output ../Test/test4.answer
	diff ../Test/test5.output ../Test/test5.answer
	diff ../Test/test6.output ../Test/test6.answer
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~

	rm -f $(OUTPUTS)
