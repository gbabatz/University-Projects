import networkx as nx
import matplotlib.pyplot as plt
import hellas_cities


D120 = hellas_cities.get_cities_distances_120_graph()

min_tree = nx.minimum_spanning_tree(D120, 'weight')
print('\nΤο ερώτημα Δ στην ουσία απαντάται με την εύρεση ελαχίστου δέντρου. \n'
      'Χρησιμοποιούμε λοιπόν τον άπληστο αλγόριθμο του Κρασκάλ για τον υπολογισμό\n'
      'του δέντρου στον γράφο των 120km. Το συνολικό μήκος καλωδίωσης ανέρχεται \n'
      'στα {} χιλιόμετρα. Οι συνδέσεις φαίνονται στο σχήμα, αλλά πιο αναλυτικά:'.format(min_tree.size('weight')))


pair_text_size = [len(pair[0])+len(pair[1])+1 for pair in min_tree.edges()]
max_pair_size = max(pair_text_size)

sorted_tree = sorted(min_tree.edges(data=True), reverse=True, key=lambda kv: kv[2]['weight'])
for pair in sorted_tree:
    pair_size = len(pair[0]) + len(pair[1]) + 1
    indent_size = max_pair_size - pair_size
    indent = indent_size*' '
    print('{}-{}{} απόσταση: {}km'.format(pair[0], pair[1], indent, pair[2]['weight']))

nx.draw(min_tree, with_labels=True)
plt.show()
