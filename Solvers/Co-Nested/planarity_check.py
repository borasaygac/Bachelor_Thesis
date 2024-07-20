import networkx as nx

def check_planarity(cnf):
    # Define the graph
    G = nx.Graph()

    unique_variables = set()

    # Get the number of clauses and variables
    for clause in cnf:
        for variable in clause:
            unique_variables.add(abs(variable))

    num_clauses = len(cnf) - 1
    num_variables = len(unique_variables)

    #print("Number of clauses:", num_clauses)
    #print("Number of variables:", num_variables)

    # Add clause-to-clause edges
    for i in range(1, num_clauses):
        print("Adding edge between", i, "and", i+1)
        G.add_edge(i, i+1)

    # Add clause-to-variable edges
    clause_edges = [
        (0, 7), (1, 7),        # C1: 1 -2
        (1, 8), (2, 8), (3, 8),# C2: 2 3 4
        (3, 9), (4, 9),        # C3: 4 5
        (4, 10), (5, 10),      # C4: 5 -6
        (5, 11), (6, 11),      # C5: 6 -7
        (0, 12), (1, 12), (3, 12), # C6: 1 2 -4
        (4, 13), (6, 13),      # C7: -5 7
        (0, 14), (3, 14), (4, 14)  # C8: -1 4 5
    ]
    G.add_edges_from(clause_edges)

    # Check for planarity
    is_planar, _ = nx.check_planarity(G)
    return is_planar

if __name__ == "__main__":
    result = check_planarity()
    print("The graph is planar." if result else "The graph is not planar.")
