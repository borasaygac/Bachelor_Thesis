#include <iostream>
#include <stdexcept>
#include <chrono>
#include <string>
#include <thread>
#include <stack>
#include <queue>
#include "parse.hpp"
#include "clause.hpp"

using namespace std;

// General Global Variables
int numOfVars = 0;
int numOfClauses = 0;
vector<Variable> vars;
vector<Clause> clauses;
vector<vector<int>> cnf;
int unitClauseCount = 0;
vector<Clause> unitClauses;

// for 2 SAT and DPLL
int numOfSatClauses = 0;
bool backtrackFlag = false;
int dc = 0;
int curVar = 1;
int btc = 0;
stack<int> assig;
queue<int> toPropagate;

// Nested Global Variables
vector<pair<int,int>> deltaF;

// Co-nested Global Variables for graph drawing 
vector<vector<int>> forGraphCoNestedCNF;

// Flags
bool twosat = false;
bool horn = false;
bool nested = false;
bool conested = false;
bool nonInterlaced = false;

int main (int argc, char *argv[]) {

    if (argc < 2 ){
        cerr << "\nUsage: " << argv[0] << " <filename>\n" << "Please add a file when calling the main.exe!\n" << endl;
        return 1;
    }

    // measure CPU time...
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    // Call to the parser
    string fileName = argv[1];
    printf("Parsing file: %s\n\n", fileName.c_str());
    parseDIMACS(fileName);

    // Check if the formula is Horn 
    horn = isHornFormula(numOfClauses, clauses); 

    printHornClauses(horn); // print the non-horn clauses if formula not horn

    // Check if the formula is 2-SAT
    twosat = isTwoSat(numOfClauses, clauses);

    // Check if the formula is nested

    createOrderedCNF(); // Creates a new CNF with the order introduced in the paper

    nested = isNested();

    // Check if the formula is co-nested

    copyCNF();
    
    conested = callPlanarityPythonScript(coNestedCNF);

    forGraphCoNestedCNF = coNestedCNF; // Save the Co-nested CNF permutation for the graph

    /* Check if the formula is non-interlaced
    createDeltaF(); // create deltaF vector
    isFNonInterlaced(); // check if the formula is non-interlaced */

    // The big if-else block to determine which algorithm to use
    if (horn) {
        hornSolver();
    } else if (twosat) {
        // DPLL two sat
        pthread_t thread;

        if (pthread_create(&thread, NULL, DPLL,NULL)) {
        std::cerr << "Error: Unable to create thread."
                  << "\n";
        std::cout.flush();
        return -1;
        }

        pthread_join(thread, NULL);
        
    } else if (nested) {

        fillLiteralsAndStart(); // Fills the occurrances of the literals and starting clauses for the nested algorithm

        nestedSolver();

        // DPLL call for nested
        pthread_t thread;

        if (pthread_create(&thread, NULL, DPLL,NULL)) {
        std::cerr << "Error: Unable to create thread."
                  << "\n";
        std::cout.flush();
        return -1;
        }

        pthread_join(thread, NULL);

    } else if (conested) {
        /* int M = 0;
        M = conestedAlgorithm(); 
        if (M != numOfClauses) {
            printf("Co-Nested Algorithm: Not Satisfiable.\nMaximum clauses satisfaible simultaneously: %d\n", M);
            return 0;
        } else {
            printf("Co-Nested Algorithm: Satisfiable.\nMaximum clauses satisfaible simultaneously: %d\n", M);
        } */
        
        // DPLL call for co-nested
        pthread_t thread;

        if (pthread_create(&thread, NULL, DPLL,NULL)) {
        std::cerr << "Error: Unable to create thread."
                  << "\n";
        std::cout.flush();
        return -1;
        }

        pthread_join(thread, NULL);

    } else {
        // Does not fit any of the above therefore a universal DPLL call

        pthread_t thread;

        if (pthread_create(&thread, NULL, DPLL,NULL)) {
        std::cerr << "Error: Unable to create thread."
                  << "\n";
        std::cout.flush();
        return -1;
        }

        pthread_join(thread, NULL);
    }

    bool sat = isFormulaSat();

    if (sat) {
        printf("The formula is SAT\n");
        if (!(horn || twosat || nested || conested || nonInterlaced)) {
            // DPLL
            printf("However, does not fit any of the cases!\n");
        }
    } else {
        if (horn || twosat || nested || conested || nonInterlaced) {
            printf("The formula is UNSAT\n");
        } else {
            // DPLL
            printf("And does not fit any of the cases!\n");
        }
    }

    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Calculate total the CPU time used
    chrono::duration<double> duration = chrono::duration_cast<std::chrono::duration<double>>(end - start);

    printModel(sat);

    printf("\nCPU time used: %.6f seconds\n\n", duration.count());

    std::cout.flush();

    printf("-------------------------------------\n\n");

    // Call the python script to generate the graph for nested and co-nested
    // These are after CPU time calculation to not interfere with the time
    // Two seperate calls for nested and co-nested ensures that we get a graph for both
    if (nested) {
        printf("Calling Python script for nested graph...\n");
        callPythonGraphScript(nestedCNF, numOfVars, numOfClauses, 'N');
    }

    if (conested) {
        printf("Calling Python script for co-nested graph...\n");
        callPythonGraphScript(forGraphCoNestedCNF, numOfVars, numOfClauses, 'C');
    }

    return 0;
}