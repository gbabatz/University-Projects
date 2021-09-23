import networkx as nx


G = nx.read_edgelist('florentine.edgelist')
# question 1
bet_cent = nx.betweenness_centrality(G)
max_bet_cent = max(bet_cent.items(), key=lambda k: k[1])
print('\n1) The node with the highest betweenness centrality is '
      'the family of {} with score of {:.3f}'.format(max_bet_cent[0], max_bet_cent[1]))

# question 2
pagerank = nx.pagerank(G)
max_pagerank = max(pagerank.items(), key=lambda k: k[1])
print('2) The node with the highest pagerank centrality is '
      'the family of {} with score of {:.3f}'.format(max_pagerank[0], max_pagerank[1]))

# question 3
clos_cent = nx.closeness_centrality(G)
max_closeness = max(clos_cent.items(), key=lambda k: k[1])
print('3) The node with the highest closeness centrality is '
      'the family of {} with score of {:.3f}'.format(max_closeness[0], max_closeness[1]))

# question 4
avg_clos_cent = sum(clos_cent.values())/len(clos_cent)
print('4) The average closeness centrality is: {:.3f}'.format(avg_clos_cent))
