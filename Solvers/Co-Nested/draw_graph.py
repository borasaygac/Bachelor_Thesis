import networkx as nx
import matplotlib.pyplot as plt

def draw_planar_graph(cnf, num_of_vars, num_of_clauses, defining_char):

    G = nx.Graph()

    if defining_char == 'C':
        # Add clause-to-clause edges
        clause_to_clause_edges = []
        for i in range(1, num_of_clauses):
            clause_to_clause_edges.append((i, i+1))
            #print("Adding edge between", i, "and", i+1)
        
        # Add clause-to-variable edges
        clause_to_var_edges = []
        for i, clause in enumerate(cnf[1:],start=0):
            for variable in clause:
                clause_to_var_edges.append((i, abs(variable) + num_of_clauses))
                #print("Adding edge between", i, "and", abs(variable)+ num_of_clauses)

        # Add the edges to the graph
        G.add_edges_from(clause_to_clause_edges)
        G.add_edges_from(clause_to_var_edges)
        # Add a dummy node to check for planarity - if planar then G the clauses of G are outerplanar
        G.add_node(num_of_clauses + num_of_vars + 1)
        outerplanar_edges = []
        for i in range(1, num_of_clauses + 1):
            outerplanar_edges.append((i, num_of_clauses + num_of_vars + 1))

        G.add_edges_from(outerplanar_edges)

    elif defining_char == 'N':
        # Add variable-to-variable edges
        var_to_var_edges = []
        for i in range(1, num_of_vars):
            var_to_var_edges.append((i, i+1))
            #print("Adding edge between", i, "and", i+1)
        
        # Add var-to-clause edges
        var_to_clause_edges = []
        for i, clause in enumerate(cnf[1:],start=1):
            for variable in clause:
                var_to_clause_edges.append((abs(variable), i + num_of_vars))
                #print("Adding edge between", i, "and", abs(variable)+ num_of_clauses)
        

        G.add_edges_from(var_to_var_edges)
        G.add_edges_from(var_to_clause_edges)
        # Add a dummy node to check for planarity - if planar then G the clauses of G are outerplanar
        G.add_node(num_of_clauses + num_of_vars + 1)

        outerplanar_edges = []
        for i in range(1, num_of_vars + 1):
            outerplanar_edges.append((i, num_of_clauses + num_of_vars + 1))

        G.add_edges_from(outerplanar_edges)

    pos = nx.planar_layout(G)
    plt.figure(figsize=(8, 8))

    #draw the graph

    if defining_char == 'C':
        plt.title("Co-nested Planar Graph with outerplanar Clauses")

           
        # Draw the clause to clause edges
        nx.draw_networkx_edges(G, pos, edgelist=clause_to_clause_edges, edge_color='red',
                            label='Clause to Clause Edges')
    
        # Draw the clause to variable edges
        nx.draw_networkx_edges(G, pos, edgelist=clause_to_var_edges, edge_color='blue',
                label='Clause to Variable Edges')

        clause_node_list= [i for i in range(1, num_of_clauses+1)]
        nx.draw_networkx_nodes(G, pos, nodelist= clause_node_list, node_shape="s", node_color='red', node_size=600
                              ,edgecolors='none', label='Clauses')
        
        variable_node_list = [i + num_of_clauses for i in range(1, num_of_vars+1)]

        nx.draw_networkx_nodes(G, pos, nodelist= variable_node_list, node_color='lightblue', node_size=600
                            , label='Variables')
        
        # Draw the outerplanar edges
        nx.draw_networkx_edges(G, pos, edgelist=outerplanar_edges, edge_color='green',
            label='Outerplanar Edges')
        
        labels = {i: f'C.{i}' for i in range(1, num_of_clauses + 1)}
        labels2 = {i + num_of_clauses: f'V.{i}' for i in range(1, num_of_vars + 1)}
        labels3 = {num_of_clauses + num_of_vars + 1: 'D'}

        # Draw the labels
        nx.draw_networkx_labels(G, pos, labels=labels, font_size=10, font_weight='bold')
        nx.draw_networkx_labels(G, pos, labels=labels2, font_size=10, font_weight='bold')
        nx.draw_networkx_labels(G, pos, labels=labels3, font_size=10, font_weight='bold')
    
        red_patch = plt.Line2D([0], [0], color='red', lw=2, label= 'Clause-to-Clause')
        blue_patch = plt.Line2D([0], [0], color='blue', lw=2, label='Clause-to-Variable')
        circle = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='lightblue', markersize=10, label='Literals')
        square = plt.Line2D([0], [0], marker='s', color='w', markerfacecolor='red', markersize=10, label='Clauses')

        dummy_node_id = [num_of_clauses + num_of_vars + 1]
        nx.draw_networkx_nodes(G, pos, nodelist=dummy_node_id, node_shape="p", node_color='green', node_size=600
                               ,label='Dummy Node')
        

    elif defining_char == 'N':
        plt.title("Nested Planar Graph with outerplanar Variables")

        # Draw the clause to clause edges
        nx.draw_networkx_edges(G, pos, edgelist=var_to_var_edges, edge_color='red',
                label='Variable to variable Edges')
    
        # Draw the clause to variable edges
        nx.draw_networkx_edges(G, pos, edgelist=var_to_clause_edges, edge_color='blue',
                label='Variable to Clause Edges')
        
        vars_node_list= [i for i in range(1, num_of_vars+1)]
        nx.draw_networkx_nodes(G, pos, nodelist= vars_node_list, node_shape="s", node_color='red', node_size=500
                            , label='Variables')
        
        clau_node_list = [i + num_of_vars for i in range(1, num_of_clauses+1)]
        nx.draw_networkx_nodes(G, pos, nodelist= clau_node_list, node_color='lightblue', node_size=500
                            , label='Clauses')
        # Draw the outerplanar edges
        nx.draw_networkx_edges(G, pos, edgelist=outerplanar_edges, edge_color='green',
            label='Outerplanar Edges')
    
        nx.draw_networkx_nodes(G, pos, nodelist=[num_of_clauses+num_of_vars + 1], node_shape="p", node_color='green', node_size=500
                            , label='Dummy Node')
        
        labels = {i: f'V.{i}' for i in range(1, num_of_vars + 1)}
        labels2 = {i + num_of_vars: f'C.{i}' for i in range(1, num_of_clauses + 1)}
        labels3 = {num_of_clauses + num_of_vars + 1: 'D'}

        # Draw the labels
        nx.draw_networkx_labels(G, pos, labels=labels, font_size=10, font_weight='bold')
        nx.draw_networkx_labels(G, pos, labels=labels2, font_size=10, font_weight='bold')
        nx.draw_networkx_labels(G, pos, labels=labels3, font_size=10, font_weight='bold')
    
        red_patch = plt.Line2D([0], [0], color='red', lw=2, label= 'Variable-to-Variable')
        blue_patch = plt.Line2D([0], [0], color='blue', lw=2, label='Variable-to-Clause')
        circle = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='lightblue', markersize=10, label='Clauses')
        square = plt.Line2D([0], [0], marker='s', color='w', markerfacecolor='red', markersize=10, label='Literals')
    
    
    green_patch = plt.Line2D([0], [0], color='green', lw=2, label='Outerplanar Edges')
    pentagon = plt.Line2D([0], [0], marker='p', color='w', markerfacecolor='green', markersize=10, label='Dummy Node')
    count_c = plt.Line2D([0], [0], color='w', lw=0, label='Number of Clauses: ' + str(num_of_clauses))
    count_v = plt.Line2D([0], [0], color='w', lw=0, label='Number of Variables: ' + str(num_of_vars))
    plt.legend(handles=[circle, square, pentagon, red_patch, blue_patch,green_patch,count_c,count_v], loc='upper right')

    plt.show()

    return 0