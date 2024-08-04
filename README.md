# Bora Saygaç's Bachelor Thesis: Uniformly presenting some tractable cases
This project was developed for my Bachelor's Thesis. It tries to shed some light on nested and co-nested formulas, however, does not try to be a universal SAT solver. It determines whether a given CNF formula is an element of class C, {2-SAT, Horn, Nested, Co-Nested}, solves them with the respective algorithm and draws the graphs for nested and co-nested formulas.

# Usage
### Required Libraries 
- [ ] G++ Compiler (c++ 17)
- [ ] Python 3.12 (not tested with previous version)
- [ ] Python/C API
- [ ] Python ```networkx``` Library
- [ ] Pyhton ```matplotlib``` Library (```matplotlib.pyplot``` specifically)
- [ ] Pyhton ```itertools``` Library

### Required Changes to Makefile Before Running 
Python/C API requires the full path of the ```libs``` and ```includes``` directories for your Python installation. Therefore the following lines must be changed in the Makefile 

```PYTHON_INCLUDE = <path-to-includes>``` such as ```C:/Users/user/AppData/Local/Programs/Python/Python312/include```
```PYTHON_LIB = <path-to-libs>``` such as ```C:/Users/user/AppData/Local/Programs/Python/Python312/libs```
```PYTHON_VERSION = <xxx>``` such as ```312``` for Python 3.12

## Complining and Running 
Compiling is pretty straightforward. In the base directory give the command:
```make```

For running, after compiling:
```./main <path-to-CNF-file>```

Both relative and full paths are compatible when running.

# Files and What They Do
## Some Definitions 
This project mainly concerns itself with nested, co-nested, and to some extent non-interlaced formulas. 
For nested the definitions and algorithm from this paper [Knuth's Paper](https://arxiv.org/abs/cs/9301111) were implemented.

For co-nested the work of Kratochvil and Krivanek [Co-nested Paper](https://link.springer.com/article/10.1007/BF01209713) was taken as basis. However, the solving algorithm proved challenging and is not reliable in our implemetation's current status. That is because it finds the minimal and maximal elements given the partial orders, which can be non-existant, causing crashes. Therefore a DPLL implementation has been used instead.The full code is still there. 

For non-interlaced, only the recognition algorithm is present. [Burckel's Paper](https://arxiv.org/abs/1803.10574)

Kratochvil and Krivanek defined a graph based approach where nested formulas need to be planar with a variable chain being outerplanar, and for co-nested planar with a clause chain to be outerplanar (see paper for more information). Those graphs are also being drawn if indeed nested or co-nested by ```draw_graph.py```. Warning! As co-nested formulas have a planirity condition, and only one of it's ordering needs to fullfill that. In the worst case O(m!(V+E)) checks need to be done where m is the number of clause, V vertices on the graph, E the number of edges in the graph. M is the defining factor here, since as early as 8 clauses 40 thousand checks need to be done. At 10 clauses that number becomes 3.6 million. So the algorithm does not scale well.

## Base Directory
In the base directory mostly classes of structures can be found, for clauses, variables, parsing in the ```.cpp``` and ```.hpp``` files respectively. The main function is present as well. 

```randHornFormulaGenerator.py``` generates random Horn formulas with weighted choice mechanicm regarding the lenght of the clauses. To run this file: ```python randHornFormulaGenerator.py```. You will be asked to input the number of variables, clauses, and maximum clause lenght. Then a Horn formula is generated and printed into the terminal.

## Under ```/Solvers```
In ```/Solvers/2-SAT``` our implementation of DPLL algorithm. Credit to Lemar Abawi as we have implemented this algorithm together for the class Sat Solvign in WS 23/24.

In ```/Solvers/Co-Nested``` the recognition and solving algorithm for co-nested formulas can be found. This is where the Python/C API comes into play, as both In ```planarity_check.py``` and In ```draw_graph.py``` are called from C++ code. 

In ```/Solvers/Horn``` the recognition and solving algorithm can be found for Horn formulas.

In ```/Solvers/Nested``` the recognition, the pre-ordering, and the solving algorithms can be found for nested formulas. 
 
## Example CNFs
Some example CNFs from SAT 2006 Competition, some created by the random Horn generator, 1 nested (which is also co-nested) example, and some base (relatively small and easy) CNFs.

# Final Remarks

I would like to sincerely thank Dr Jan Johannsen for his help, guidance, and mentorship throughout this project, thesis, and his teaching in the SAT Solving class. 