#include "parse.hpp"
#include "clause.hpp"
#include "dpdl.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib> 

using namespace std;

// This function is dedicated to parse the DIMACS file.

void checkFileStatus(std::ifstream& file, const std::string& filename) {
    if (file.bad()) {
        cerr << "Error reading file, badbit set on " << filename << ". Error no: " << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    } else if (file.fail()) {
        cerr << "Error reading file, failbit set on " << filename << ". Error no: " << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    } else if (file.eof()) {
        std::cerr << "Error reading file, eofbit set on " << filename << ". Error no: " << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

bool parseDIMACS(std::string filename)
{

    ifstream file(filename);
    string line;

    if (file.is_open()) {

        getline(file, line);

        // skip comment lines that start with c
        while (line[0] == 'c' || line.empty()) {
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

        //printf("size of vars: %i\n", vars.size());

        for (int i = 0; i < numOfClauses + 1; i++){ // set the index of the clause (1-indexed to m = numOfClauses)
            clauses[i].setIndex(i);
        }

        vector<int> dummy;
        cnf.push_back(dummy); // ensure 1-indexedness for cnf
        int clauseCount = 1;

        vector<int> clause;
        while (getline(file, line)) {

            istringstream iss(line);
            if (line[0] == 0)
                continue;

            int literal;
            while (iss >> literal && literal != 0) { // if lit > 0 increase pos occ otherwise neg occ. Either way tot occ is updated.
                clauses[clauseCount].literals.push_back(literal);
                (literal > 0) ? vars[abs(literal)].incPosOcc()
                              : vars[abs(literal)].incNegOcc();

                clause.push_back(literal);
            }

            if (literal == 0) {
                if (!clauses[clauseCount].literals.empty()) {
                    
                    // if unit clause, push to unit queue
                    if (clauses[clauseCount].literals.size() == 1) {
                        
                        if (!vars[std::abs(clauses[clauseCount].literals[0])].getEnqueued()) {
                            toPropagate.push(clauses[clauseCount].literals[0]);
                            
                            vars[std::abs(clauses[clauseCount].literals[0])].setEnqueued(true);
                        }
                    }
                }
                cnf.push_back(clause);

                if (clause.size() == 1) unitClauseCount++;
                clause = {};
                clauseCount++;
            }
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

        for (int i = 1; i <= numOfVars; i++) {
            vars[i].copyOccsToDynOccs();
        }

        file.close();
    }
    else {
        //throw runtime_error("Failed to open file " + filename);

        // Check for specific error conditions
        checkFileStatus(file, filename);
        return false;
    }

    // push pure literals to prop queue
    /*for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].getNegativeOccurrances().size() == 0 && vars[i].getPositiveOccurrances().size() > 0) {
            vars[std::abs(i)].setEnqueued(true);
            toPropagate.push(i);
        }
        if (vars[i].getPositiveOccurrances().size() == 0 && vars[i].getNegativeOccurrances().size() > 0) {
            vars[std::abs(i)].setEnqueued(true);
            toPropagate.push(-i);
        }
    }*/
    return true;
}