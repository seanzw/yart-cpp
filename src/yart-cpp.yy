
%{
 
#include <iostream>
#include <string>
extern int yylex();
void yyerror(const std::string &s);

#define PARSER_VERBOSE

%}

%union {
    char string[1024];
    float num;
}

%token NUMBER
%type <num> NUMBER
%token SIZE

%%

start: yart_stmt_list {

};

yart_stmt_list: yart_stmt_list yart_stmt {

}

| yart_stmt {

};

yart_stmt: SIZE NUMBER NUMBER {
    std::cout << "SIZE " << $2  << $3;
};


%%

void yyerror(const std::string &s) {
    std::cerr << s << std::endl;
}