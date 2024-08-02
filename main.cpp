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

int numOfVars = 0;
int numOfClauses = 0;
vector<Variable> vars;
vector<Clause> clauses;
vector<vector<int>> cnf;
int unitClauseCount = 0;
vector<Clause> unitClauses;
vector<pair<int,int>> deltaF;
vector<vector<int>> forGraphCoNestedCNF;
bool twosat = false;
bool horn = false;
bool nested = false;
bool conested = false;
bool nonInterlaced = false;


// for 2 SAT and DPLL
int numOfSatClauses = 0;
bool backtrackFlag = false;
int dc = 0;
int curVar = 1;
int btc = 0;
stack<int> assig;
queue<int> toPropagate;


int main (int argc, char *argv[]) {

    if (argc < 2 ){
        cerr << "\nUsage: " << argv[0] << " <filename>\n" << "Please add a file when calling the main.exe!\n" << endl;
        return 1;
    }

    // measure CPU time...
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    string fileName = argv[1];
    try {
        printf("Parsing file: %s\n\n", fileName.c_str());
        parseDIMACS(fileName);
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        // handle error 
    }

    // Check if the formula is Horn 
    horn = isHornFormula(numOfClauses, clauses); 

    printHornClauses(horn); // print the non-horn clauses if formula not horn (for DEBUG)

    // Check if the formula is 2-SAT

    twosat = isTwoSat(numOfClauses, clauses);

    // Check if the formula is nested

    createOrderedCNF(); 

    nested = isNested();

    // Check if the formula is co-nested

    copyCNF();
    
    conested = callPlanarityPythonScript(coNestedCNF);

    // Save the Co-nested CNF permutation for the graph
    forGraphCoNestedCNF = coNestedCNF;

    /* Check if the formula is non-interlaced
    createDeltaF(); // create deltaF vector
    isFNonInterlaced(); // check if the formula is non-interlaced */

    // The big if-else block to determine which algorithm to use
    if (horn) {
        if (unitClauses.empty()){
            printf("No unit clauses!\n");
        } else {
            printf("Unit Clause Count: %i\n", unitClauseCount);
            printf("Unit Clauses: ");
            for (Clause tmp_cls : unitClauses) {
                printf("%i, ", tmp_cls.getIndex());
            }
        printf("\n");
        }
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

        fillLiteralsAndStart();

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
        // conested alg
        //conestedAlgorithm();
        
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
        // Does not fit any of the above

        pthread_t thread;

        if (pthread_create(&thread, NULL, DPLL,NULL)) {
        std::cerr << "Error: Unable to create thread."
                  << "\n";
        std::cout.flush();
        return -1;
        }

        pthread_join(thread, NULL);
        // DPLL
        // Note: In this case call minisat
    }
    
    //print var assigs
    /*for (int i = 1; i <= numOfVars; i++) {
        printf("Variable %i: %i\n", i, vars[i].getValue());
    } */

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

    chrono::duration<double> duration = chrono::duration_cast<std::chrono::duration<double>>(end - start);

    printModel(sat);

    printf("\nCPU time used: %.6f seconds\n\n", duration.count());

    std::cout.flush();

    printf("-------------------------------------\n\n");

    // Call the python script to generate the graph for nested and co-nested
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