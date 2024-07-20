import networkx as nx

def check_planarity():
    # Define the graph
    G = nx.Graph()

    # Number of variables and clauses
    num_variables = 7
    num_clauses = 8

    # Add variable-to-variable edges
    variable_edges = [(0, 1), (1, 2), (2, 3), (3, 4), (4, 5), (5, 6)]
    G.add_edges_from(variable_edges)

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
