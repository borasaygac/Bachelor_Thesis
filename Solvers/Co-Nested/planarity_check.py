import networkx as nx
import matplotlib.pyplot as plt
import itertools

def construct_graph_from_cnf(cnf):
    

    G = nx.Graph()

    unique_variables = set()

    # Get the number of clauses and variables
    for clause in cnf:
        for variable in clause:
            unique_variables.add(abs(variable))

    num_clauses = len(cnf)
    num_variables = len(unique_variables)

    #print("Number of clauses:", num_clauses)
    #print("Number of variables:", num_variables)

    # Add clause-to-clause edges
    clause_to_clause_edges = []
    for i in range(1, num_clauses):
        clause_to_clause_edges.append((i, i+1))
        

    # Add clause-to-variable edges
    clause_to_var_edges = []
    for i, clause in enumerate(cnf):
        for variable in clause:
            clause_to_var_edges.append((i, abs(variable) + num_clauses))
            #print("Adding edge between", i, "and", abs(variable))

    # Add the edges to the graph
    G.add_edges_from(clause_to_clause_edges)
    G.add_edges_from(clause_to_var_edges)
    # Add a dummy node to check for planarity - if planar then G is outerplanar
    G.add_node(num_clauses+num_variables + 1)
    outerplanar_edges = []
    for i in range(1, num_clauses+1):
        outerplanar_edges.append((i, num_clauses+num_variables + 1))

    G.add_edges_from(outerplanar_edges)

    # Check for planarity
    is_planar = nx.check_planarity(G)

    return is_planar[0]

def check_planarity_for_all_permutations(cnf):
    check_counter = 0

    # Pair each clause with its original index
    indexed_clauses = list(enumerate(cnf[1:], start=1))
    
    for permuted_indexed_clauses in itertools.permutations(indexed_clauses):
        # Reconstruct the CNF with permuted clauses while keeping original indices
        permutation = [clause for idx, clause in permuted_indexed_clauses]

        # Construct the graph with the permuted clauses
        is_planar = construct_graph_from_cnf(permutation)
        check_counter += 1

        if is_planar:
            print("Co-Nested Recognition: Y\nReason: There exists a planar permutation.")
            result = [clause for idx, clause in permuted_indexed_clauses]
            result.insert(0, [])  # Insert an empty list at the beginning
            print("Number of planarity checks:", check_counter)
            return result
    
    print("Co-Nested Recognition: N\nReason: There exists no planar permutation.")
    print("Number of planarity checks:", check_counter)
    return False
