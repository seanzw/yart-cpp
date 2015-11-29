#define DEBUG_SAMPLES_PER_PIXEL_MAP

#ifdef DEBUG_SAMPLES_PER_PIXEL_MAP

vector< vector<size_t> > sppMap;
string sppFN("spp.dat");

void DEBUG_DumpSPP() {
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