#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#define DEBUG_SAMPLES_PER_PIXEL_MAP

#include <vector>
#include <string>

using std::vector;
using std::string;

class SPPDebuger {
public:
    static vector< vector<size_t> > sppMap;
    const static string sppFN;
    static void DEBUG_DumpSPP();
};


#endif