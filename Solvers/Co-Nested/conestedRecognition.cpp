#include <iostream>
#include "../../globals.hpp"
#include <Python.h>
#include <filesystem>


vector<vector<int>> coNestedCNF;

void copyCNF() {
    coNestedCNF = cnf;
}

bool callPythonScript(vector<vector<int>>& coNestedCNF) {
    // Initialize the Python interpreter
    Py_Initialize();

    // Define the Python script to be executed
    const char* scriptName = "planarity_check";
    const char* functionName = "check_planarity_for_all_permutations";

    //// Add the current directory and the Solvers/Co-Nested directory to the Python path
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
            PyObject* pList = PyList_New(coNestedCNF.size());
            for (int i = 0; i < coNestedCNF.size(); i++) {
                PyObject* pInnerList = PyList_New(coNestedCNF[i].size());
                for (int j = 0; j < coNestedCNF[i].size(); j++) {
                    PyList_SetItem(pInnerList, j, PyLong_FromLong(coNestedCNF[i][j]));
                }
                PyList_SetItem(pList, i, pInnerList);
            }

            // Call the Python function with the list as an argument
            PyObject* pArgs = PyTuple_Pack(1, pList);
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pList);


            if (pValue != nullptr) {
                // Convert the Python lis of list back to C++ vector of vectors
                vector<vector<int>> result;
                if (PyList_Check(pValue)) {

                    if (pValue == Py_False) { // check if a bool is returned
                        std::cout << "The formula is not co-nested." << std::endl;
                        return false;
                    } else { // if a list of lists is returned
                        for (Py_ssize_t i = 0; i < PyList_Size(pValue); ++i) {
                            PyObject* pSubList = PyList_GetItem(pValue, i);
                            if (PyList_Check(pSubList)) {
                                vector<int> subVector;
                                for (Py_ssize_t j = 0; j < PyList_Size(pSubList); ++j) {
                                    subVector.push_back(PyLong_AsLong(PyList_GetItem(pSubList, j)));
                                }
                                result.push_back(subVector);
                            }
                        }
                    }
                }

                coNestedCNF = result;

                Py_DECREF(pValue);

            } else {
                PyErr_Print();
                std::cerr << "Failed to call the Python function." << std::endl;
            }
            Py_XDECREF(pFunc);
        } else {
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
            std::cerr << "Cannot find or call the Python function." << std::endl;
        }
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load the Python script." << std::endl;
    }

    return true;
}



