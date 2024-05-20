#include "parse.hpp"
#include "clause.hpp"

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

        // 4th elem of DIMACS is always the count of clauses,
        numOfClauses = stoi(tokens[3]);

        printf("Number of variables: %i\n", numOfVars);
        printf("Number of clauses: %i\n",  numOfClauses);
        
        // Parse operations for the rest of the formula
        vector<Variable> vars(numOfVars + 1);

        printf("size of vars: %i", vars.size());

        vector<Clause> clauses(numOfClauses + 1);

        vector<int> dummy;
        cnf.push_back(dummy); // ensure 1-indexedness for cnf
        int clauseCount = 1;

        vector<int> clause;
        while(getline(file,line)){
            istringstream iss(line);
            if (line[0] == 0) continue;

            int literal;
            while (iss >> line && literal != 0){  // if lit > 0 increase pos occ otherwise neg occ. Either way tot occ is updated.
                (literal > 0) ? vars[abs(literal)].incPosOcc() 
                : vars[abs(literal)].incNegOcc();

                clause.push_back(literal);
            }

            for (int lit: clause) {
                clauses[clauseCount].clauseAddElem(lit); 
            }

            cnf.push_back(clause);
            clause = {};
        }

        for (int i = 0; i < numOfClauses + 1; i++) {
            for (int j = 0; j < cnf[i].size(); j++) {
                printf("%i ", cnf[i][j]);
            }
            printf("\n");
        }
        
    } else {
        throw runtime_error("Failed to open file " + filename);
        return false;
    }

    return true;
}