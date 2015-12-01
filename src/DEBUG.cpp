#include <iostream>
#include <fstream>
#include "DEBUG.h"

using std::ofstream;
using std::cout;
using std::endl;

#ifdef DEBUG_SAMPLES_PER_PIXEL_MAP

const string SPPDebuger::sppFN("spp.dat");
vector< vector<size_t> > SPPDebuger::sppMap;

void SPPDebuger::DEBUG_DumpSPP() {
    ofstream spp(sppFN);
    if (!spp.is_open()) {
        cout << "Failed open " << sppFN << endl;
        return;
    }

    for (int row = 0; row < sppMap.size(); ++row) {
        for (int col = 0; col < sppMap[row].size(); ++col) {
            spp << sppMap[row][col] << " ";
        }
        spp << endl;
    }
    spp.close();
}

#endif