#include "../../globals.hpp"
#include "../../dpdl.hpp"

void propagate() {

    int variableToPropagate;
    while (!toPropagate.empty()) { // While there are variables to propagate in the queue toPropagate -- FIFO
        variableToPropagate = toPropagate.front();
        toPropagate.pop();
        int absoluteProp = std::abs(variableToPropagate); // Get the absolute value of the variable to propagate, needed to be able to address the object
        
        // If the variable to propagate is positive, set it to TRUE, otherwise set it to FALSE
        (variableToPropagate > 0) ? vars[absoluteProp].setValue(TRUE) : vars[absoluteProp].setValue(FALSE); 


        vars[absoluteProp].setEnqueued(false); // the variable is no longer enqueued
        vars[absoluteProp].setForced(true); // the variable is now forced because the assignment happened due to propagation of the chosen variable 

        assig.push(absoluteProp);      // push the variable to the stack of assigned variables -- needed for backtrack
        
        update(absoluteProp);       // update the variables that are affected by the assignment of the variable
    }
}