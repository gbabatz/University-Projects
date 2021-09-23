import networkx as nx
import matplotlib.pyplot as plt
# needs scipy library installed


# question 1
print('1) Loading florentine families graph.\n')
G = nx.florentine_families_graph()

# question 2
print('2) Saving graph as gexf format.\n')
nx.write_gexf(G, "florentine.gexf")

# question 3
print('3) Saving graph as edgelist format.\n')
nx.write_edgelist(G, 'florentine.edgelist')

# question 4
print('4) Default plotting. . .')
nx.draw(G, with_labels=True)
plt.show()

# question 5
print('5) Planar layout plotting. . .')
nx.draw_planar(G, with_labels=True)
plt.show()

# question 6
print('6) Kamada kawai layout plotting. . .')
nx.draw_kamada_kawai(G, with_labels=True)
plt.show()

