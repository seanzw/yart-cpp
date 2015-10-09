
target: file

file: parser lex

all: parser lex cc

parser: yart-cpp.yy
	bison -d yart-cpp.yy
	mv yart-cpp.tab.cc yart-cpp.tab.cpp

lex: yart-cpp.ll
	flex yart-cpp.ll
	mv lex.yy.c lex.yy.cpp

cc: yart-cpp.tab.cpp lex.yy.cpp main.c
	g++ lex.yy.cpp yart-cpp.tab.cpp main.c

clean: rm a.exe