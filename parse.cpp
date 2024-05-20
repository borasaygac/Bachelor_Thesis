#include "parse.hpp"

using namespace std;

// This function is dedicated to parse the DIMACS file.

bool parseDIMACS(std::string filename) {

    ifstream file("DIMACS/" + filename);
    string line;

    if (file.is_open()) {
        getline(file, line);

        // skip comment lines that start with c 
        while (line[0] == 'c') {getline(file, line);}

        istringstream iss(line);
        string token;
        vector<string> tokens;

        // Move head data of DIMACS into a vector
        while (iss >> token) {
            tokens.push_back(token);
        }

        // 3rd elem of DIMACS is always the count of variables 
        numOfVars = stoi(tokens[2]);

        // 3th elem of DIMACS is always the count of clauses,
        numOfClauses = stoi(tokens[3]);

        printf("Number of variables: %i\n", numOfVars);
        printf("Number of clauses: %i\n",  numOfClauses);
        
        // Parse operations for the rest of the formula
        vector<Variable> vars(numOfVars + 1);

        printf("size of vars: %i", vars.size());

    } else {
        throw runtime_error("Failed to open file " + filename);
        return false;
    }

    return true;
}