#include <iostream>
#include "../../globals.hpp"
#include <Python.h>
#include <filesystem>


vector<vector<int>> coNestedCNF;

void copyCNF() {
    coNestedCNF = cnf;
}

void callPythonScript() {
    // Initialize the Python interpreter
    std::cout << std::filesystem::current_path() << std::endl;
    Py_Initialize();

    // Define the Python script to be executed
    const char* scriptName = "planarity_check";
    const char* functionName = "check_planarity";

    // Add the current directory to the Python path
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
            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, nullptr);
            if (pValue != nullptr) {
                // Print the result
                int result = PyObject_IsTrue(pValue);
                if (result) {
                    std::cout << "The graph is planar." << std::endl;
                } else {
                    std::cout << "The graph is not planar." << std::endl;
                }
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

}



