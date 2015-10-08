#include <iostream>
#include <string>
#include "RayTracer.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "ERROR: NO INPUT FILE\n";
        exit(1);
    }

    string fn(argv[1]);
    RayTracer rt(fn);
    rt.generate(8);
    rt.rinse();

    return 0;
}