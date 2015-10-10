#include <iostream>
#include <string>
#include "RayTracer.h"

RayTracer *yart;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "ERROR: NO INPUT FILE\n";
        exit(1);
    }

	extern FILE *yyin;
	extern int yyparse(void);

	string fn(argv[1]);
	RayTracer rt(fn);

	/* For the parser. */
	yart = &rt;

	yyin = fopen(argv[1], "r");
	yyparse();

    rt.generate(8);
    rt.rinse();

    return 0;
}