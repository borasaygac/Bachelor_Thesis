import networkx as nx
import matplotlib.pyplot as plt

def draw_graph(cnf):
    # Draw the graph
    pos = nx.planar_layout(G) if is_planar else nx.spring_layout(G)
    plt.figure(figsize=(8, 8))

    # Draw the clause to clause edges
    nx.draw(G, pos, edgelist=clause_to_clause_edges, edge_color='red',
            label='Clause to Clause Edges')
    
    # Draw the clause to variable edges
    nx.draw(G, pos, edgelist=clause_to_var_edges, edge_color='blue',
            label='Clause to Variable Edges')
    
    # Draw the outerplanar edges
    nx.draw(G, pos, edgelist=outerplanar_edges, edge_color='green',
            label='Outerplanar Edges')
    
    clause_node_list= [i for i in range(1, num_clauses)]
    nx.draw_networkx_nodes(G, pos, nodelist= clause_node_list, node_shape="s", node_color='lightblue', node_size=1000
                           , label='Clauses')
    
    variable_node_list = [i + num_clauses for i in range(0, num_variables+1)]
    nx.draw_networkx_nodes(G, pos, nodelist= variable_node_list, node_color='lightblue', node_size=1000
                           , label='Clauses')
    
    nx.draw_networkx_nodes(G, pos, nodelist=[num_clauses+num_variables + 1], node_shape="p", node_color='lightblue', node_size=1000
                           , label='Dummy Node')
    
    nx.draw_networkx_labels(G, pos, font_size=15, font_weight='bold')

    plt.title("Planarity Check: " + ("Planar" if is_planar else "Not Planar"))
    
    red_patch = plt.Line2D([0], [0], color='red', lw=2, label='Clause-to-Clause')
    blue_patch = plt.Line2D([0], [0], color='blue', lw=2, label='Clause-to-Variable')
    green_patch = plt.Line2D([0], [0], color='green', lw=2, label='Outerplanar Edges')
    circle = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='lightblue', markersize=10, label='Literals')
    square = plt.Line2D([0], [0], marker='s', color='w', markerfacecolor='lightblue', markersize=10, label='Clauses')
    pentagon = plt.Line2D([0], [0], marker='p', color='w', markerfacecolor='lightblue', markersize=10, label='Dummy Node')
    plt.legend(handles=[circle, square, pentagon, red_patch, blue_patch,green_patch], loc='upper right')

    plt.show()

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
    clause_to_clause_edges = []
    for i in range(1, num_clauses):
        clause_to_clause_edges.append((i, i+1))
        print("Adding edge between", i, "and", i+1)

    # Add clause-to-variable edges
    clause_to_var_edges = []
    for i, clause in enumerate(cnf):
        for variable in clause:
            clause_to_var_edges.append((i, abs(variable) + num_clauses))
            print("Adding edge between", i, "and", abs(variable))

    # Add the edges to the graph
    G.add_edges_from(clause_to_clause_edges)
    G.add_edges_from(clause_to_var_edges)
    # Add a dummy node to check for planarity - if planar then G is outerplanar
    G.add_node(num_clauses+num_variables + 1)
    outerplanar_edges = []
    for i in range(1, num_clauses+num_variables):
        outerplanar_edges.append((i, num_clauses+num_variables + 1))

    # Check for planarity
    is_planar, _ = nx.check_planarity(G)

    draw_graph(cnf)
    return is_planar

if __name__ == "__main__":
    result = check_planarity()
    print("The graph is planar." if result else "The graph is not planar.")
