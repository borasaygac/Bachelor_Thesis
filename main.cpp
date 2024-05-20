#include <iostream>
#include <stdexcept>
#include "parse.hpp"

using namespace std;

int numOfVars = 0;
int numOfClauses = 0;
vector<Variable> vars;
vector<vector<int>> cnf;

int main () {

    string fileName = "001_count4_2_s.cnf";
    try {
        parseDIMACS(fileName);
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        // handle error 
    }

    return 0;
}