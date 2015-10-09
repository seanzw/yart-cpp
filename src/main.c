#include <cstdio>

int main(int argc, char *argv[]) {

    extern FILE *yyin;
    extern int yyparse(void);

    yyin = fopen(argv[1], "r");
    yyparse();

    return 0;
}