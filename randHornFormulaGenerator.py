import random 

# This function generates a random Horn formula with the given number of variables, clauses and maximum clause length
# A Horn formula is a CNF formula where each clause contains at most one positive literal
# The function returns a string in DIMACS CNF format
# The function uses a weighted random choice to choose the length of each clause. This is especially important for unit clauses.

def gen_rand_horn_formula(num_vars, num_clauses, max_clause_length): 
    variables = [f'{i}' for i in range(1, num_vars + 1)]
    formula = []

    clause_lengths = list(range(1, min(num_vars, max_clause_length) + 1))
    weights = [lenght*2 for lenght in clause_lengths]

    for _ in range(num_clauses):
        clause_length = random.choices(clause_lengths, weights=weights, k=1)[0] # Choose a weighted random clause length
        clause_vars = random.sample(variables, k=clause_length) # at most 3 variables per clause
        if clause_length > 1: # Randomly choose a positive or negative literal
            positive_literal = random.choice(clause_vars)
        else:
            positive_literal = random.choice([random.choice(clause_vars), f'-{random.choice(clause_vars)}']) 
        clause = [f'-{var} ' if var != positive_literal else f'{var} ' for var in clause_vars] + ['0'] # Negate all literals except the positive one
        formula.append(''.join(clause))
    
    return '\n'.join(clause for clause in formula)

num_vars = int(input("Enter the number of variables: "))
num_clauses = int(input("Enter the number of clauses: "))
max_clause_length = int(input("Enter the maximum clause length: "))

print(f'p cnf {num_vars} {num_clauses}')
print(gen_rand_horn_formula(num_vars, num_clauses, max_clause_length))
