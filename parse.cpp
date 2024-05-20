#include "parse.hpp"

using namespace std;

// This function is dedicated to parse the DIMACS file.

bool parseDIMACS(std::string filename) {

    ifstream file(filename);
    string line;

    if (file.is_open()) {

    } else {
        throw runtime_error("Failed to open file " + filename);
        return false;
    }

    return true;
}