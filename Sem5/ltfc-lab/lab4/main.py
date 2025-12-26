import numpy as np


def dfs(adj_mat, states, start, finals):
    productions = []

    for i in range(states):
        for j in range(states):
            symbols = adj_mat[i][j]

            for symbol in symbols:
                productions.append(f"Q{i} -> {symbol} Q{j}")

    for final in finals:
        productions.append(f"Q{final} -> \u03B5")

    with open('rg.out', 'w') as f:
        f.write(f"Start: Q{start}\n")
        f.write("Productions:\n")

        f.write('\n'.join(productions))


with open("fa.in") as f:
    lines = [line.strip() for line in f]

states = int(lines[0][0])
start = int(lines[1][0])
finals = [int(final) for final in lines[2].split()]

transitions = [line.split() for line in lines[3:]]

adj_mat = np.empty((states, states), dtype=object)
nested_lists = [[[] for _ in range(states)] for _ in range(states)]
adj_mat[:] = nested_lists

for tran in transitions:
    adj_mat[int(tran[0])][int(tran[1])].append(tran[2])

dfs(adj_mat, states, start, finals)