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
	extern string current_file;
	extern int yyparse(void);

	current_file = string(argv[1]);

	string fn(argv[1]);
	RayTracer rt(fn);

	/* For the parser. */
	yart = &rt;

	yyin = fopen(argv[1], "r");
	yyparse();

#ifdef _DEBUG
    rt.generate(1);
#else
    rt.generate(8);
#endif
    rt.rinse();

    return 0;
}