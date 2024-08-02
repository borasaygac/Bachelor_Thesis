#include <iostream>
#include "../../globals.hpp"
#include <Python.h>
#include <filesystem>


void callPythonGraphScript(vector<vector<int>> gCNF,int numOfVars, int numOfClauses, char definingChar){

    // Initialize the Python interpreter
    Py_Initialize();

    // Define the Python script to be executed
    const char* scriptName = "draw_graph";
    const char* functionName = "draw_planar_graph";

     // Add the current directory and the Solvers/Co-Nested directory to the Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    PyRun_SimpleString("sys.path.append(\"Solvers/Co-Nested\")");

     // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault(scriptName);
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, functionName);

        if (pFunc && PyCallable_Check(pFunc)) {

            // Convert the CNF to a Python list
            PyObject* pList = PyList_New(gCNF.size());
            for (int i = 0; i < gCNF.size(); i++) {
                PyObject* pInnerList = PyList_New(gCNF[i].size());
                for (int j = 0; j < gCNF[i].size(); j++) {
                    PyList_SetItem(pInnerList, j, PyLong_FromLong(gCNF[i][j]));
                }
                PyList_SetItem(pList, i, pInnerList);
            }

            // Convert the character to a Python string
            PyObject* pChar = PyUnicode_FromString(&definingChar);
            
            // Call the Python function with the list as an argument
            PyObject* pArgs = PyTuple_Pack(4, pList, PyLong_FromLong(numOfVars), PyLong_FromLong(numOfClauses), pChar);
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pList);
            Py_DECREF(pChar);

            if (pValue != nullptr) {
                Py_DECREF(pValue);
            }
            else {
                PyErr_Print();
            }
        }
        else {
            PyErr_Print();
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
    }
}