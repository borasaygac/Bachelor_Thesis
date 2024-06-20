#include "parse.hpp"
#include "clause.hpp"
#include <cstring>

using namespace std;

// This function is dedicated to parse the DIMACS file.

bool parseDIMACS(std::string filename)
{

    ifstream file("DIMACS\\" + filename);
    string line;

    if (file.is_open())
    {
        getline(file, line);

        // skip comment lines that start with c
        while (line[0] == 'c')
        {
            getline(file, line);
        }

        istringstream iss(line);
        string token;
        vector<string> tokens;

        // Move head data of DIMACS into a vector
        while (iss >> token)
        {
            tokens.push_back(token);
        }

        // 3rd elem of DIMACS is always the count of variables
        numOfVars = stoi(tokens[2]);

        // 4th elem of DIMACS is always the count of clauses,
        numOfClauses = stoi(tokens[3]);

        printf("Number of variables: %i\n", numOfVars);
        printf("Number of clauses: %i\n", numOfClauses);

        // Parse operations for the rest of the formula

        vars.resize(numOfVars + 1);
        clauses.resize(numOfClauses + 1);

        // for both vars and clauses, we start from 1 to numOfVars and numOfClauses, 0th elem is included as dummy
        // and initialized for consistency's and mem leak's sake.

        for (int i = 0; i < numOfVars + 1; i++){ // set the index of the variable (1-indexed to n= numOfVars) 
            vars[i].setIndex(i);
            vars[i].setValue(FREE);
        }

        printf("size of vars: %i\n", vars.size());

        for (int i = 0; i < numOfClauses + 1; i++){ // set the index of the clause (1-indexed to m = numOfClauses)
            clauses[i].setIndex(i);
        }

        vector<int> dummy;
        cnf.push_back(dummy); // ensure 1-indexedness for cnf
        int clauseCount = 1;

        vector<int> clause;
        while (getline(file, line))
        {
            istringstream iss(line);
            if (line[0] == 0)
                continue;

            int literal;
            while (iss >> literal && literal != 0)
            { // if lit > 0 increase pos occ otherwise neg occ. Either way tot occ is updated.

                (literal > 0) ? vars[abs(literal)].incPosOcc()
                              : vars[abs(literal)].incNegOcc();

                clause.push_back(literal);
            }

            cnf.push_back(clause);

            if (clause.size() == 1) unitClauseCount++;
            clause = {};
            clauseCount++;
        }

        for (int i = 1; i < numOfClauses + 1; i++) {
            //printf("Clause %i: ", i);
            if (cnf[i].size() == 1) unitClauses.push_back(clauses[i]);
            for (int j = 0; j < cnf[i].size(); j++){
                 // add elements to the clause object
                clauses[i].clauseAddElem(cnf[i][j]);
                //printf("%i ", cnf[i][j]);
            }
            //printf("\n");
        } 
    }
    else {
        throw runtime_error("Failed to open file " + filename);

        // Check for specific error conditions
        if (file.bad()) {
            throw runtime_error("Error reading file, badbit set on " + filename + ". Error no: " + strerror(errno));
        } else if (file.fail()) {
            throw runtime_error("Error reading file, failbit set on " + filename);
        } else if (file.eof()) {
            throw runtime_error("Error reading file, eofbit set on " + filename);
        }
        return false;
    }

    return true;
}