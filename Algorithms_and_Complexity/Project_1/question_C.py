import networkx as nx
import matplotlib.pyplot as plt
import hellas_cities as hc

# question 1
print('\nΕρώτημα 1ο:\n')
G = hc.get_cities_distances_80_graph()
print('Αρχικα, δημιουργούμε το γράφο των πόλεων που απέχουν το πολυ \n'
      '80km μεταξύ τους και εξετάζουμε εάν είναι συνεκτικός.')

connected = nx.is_connected(G)

print('Βλέπουμε ότι δεν είναι, (connected = {}) άρα το επόμενο βήμα είναι \n'
      'να βρούμε τον αριθμό των συνεκτικών συνιστωσών(connected components).'.format(connected))

con_comp = nx.number_connected_components(G)

print('Ο αριθμός των συνεκτικών συνιστοσών είναι: {}\n'
      'αυτό σημαίνει ότι ο ποδηλάτης θα πρέπει να πάρει \n'
      'διαφορετικό μέσο μεταφοράς κάθε φορά που ταξιδεύει απο \n'
      'ένα connected component σε ένα άλλο. Αφού αρχίζει το ταξίδι\n'
      'του απο μία υπάρχον συνιστώσα του γράφου θα χρειαστεί \n'
      '{} - 1 = {} συνολικά \'άλματα\' απο συνιστώσα σε συνιστώσα ώστε να \n'
      'επισκεφθεί όλες τις πόλεις της Ελλάδας(η τουλάχιστον αυτές τις οποίες εξετάζουμε).\n'
      'Αρα η απάντηση είναι οτι θα χρειαστεί {} φορές να χρησιμοποιήσει διαφορετικό μέσο μεταφοράς.'.format(con_comp, con_comp, con_comp-1, con_comp-1))


# question 2
print('\n\nΕρώτημα 2ο:\n')


def get_distance(G, path):
    final_distance = 0
    cities_num = len(path)
    total_hops = cities_num - 1
    for hop in range(total_hops):
        final_distance += G.edges[path[hop], path[hop+1]]['weight']
    return final_distance


print('Αρχικά φορτώνουμε τον γράφο με της πόλεις όπου οι μέγιστες αποστάσεις είναι ίσες με το πολύ 120km.')
G120 = hc.get_cities_distances_120_graph()
print('Έπειτα ελέγχουμε εάν ο γράφος είναι συνεκτικός(connected).')
conntected_120 = nx.is_connected(G120)
print('Εφόσον είναι συνεκτικός (connected = {}), βρίσκουμε τις πόλεις που συμμετέχουν \n'
      'στο ελάχιστο μονοπάτι ανάμεσα στην Ξάνθη και την Σπάρτη'.format(conntected_120))
city_paths = nx.shortest_path(G120, 'Xanthi', 'Sparti', 'weight')
print('και στην συνέχεια μέσω της συνάρτησης get_distance υπολογίζουμε την συνολική απόσταση \n'
      'βάση της απόστασης που έχουν γειτονικές πόλεις.')
final_dist = get_distance(G120, city_paths)
print('Συνεπως η απάντηση στο ερώτημα είναι {}km ελάχιστη απόσταση Ξάνθης-Σπαρτης.'.format(final_dist))

# question 3
print('\n\nΕρώτημα 3ο:\n')
print('Για να πάει απο την ΠΑΘΕ αρκεί να σιγουρευτούμε ότι θα παει απο την ελάχιστη διαδρομή \n'
      'στην Αθήνα και απο εκεί στον τελικό προορισμό, την Πάτρα.')
pathe1 = nx.shortest_path(G120, 'Xanthi', 'Athina', 'weight')
pathe2 = nx.shortest_path(G120, 'Athina', 'Patra', 'weight')

pathe_full = pathe1.copy()
pathe_full.extend(pathe2[1:])
print('Διαδρομή που θα ακολουθίσει αποτελείται, με την σειρά, απο τις πόλεις: ')
for city in pathe_full:
    print(city)

pathe_distance = get_distance(G120, pathe1) + get_distance(G120, pathe2)
print('\nΗ συνολική απόσταση που θα διανύσει έιναι το άθροισμα των επιμέρους \n'
      'διαδρομών και ισούτε με: {}km.'.format(pathe_distance))

print('\nΓια να πάει απο την Εγνατία αρκεί να σιγουρευτούμε ότι θα παει απο την ελάχιστη διαδρομή \n'
      'στα Ιωάννινα και απο εκεί στον τελικό προορισμό, την Πάτρα')
egnatia1 = nx.shortest_path(G120, 'Xanthi', 'Ioannina', 'weight')
egnatia2 = nx.shortest_path(G120, 'Ioannina', 'Patra', 'weight')

egnatia_full = egnatia1.copy()
egnatia_full.extend(egnatia2[1:])
print('Διαδρομή που θα ακολουθίσει αποτελείται, με την σειρά, απο τις πόλεις: ')
for city in egnatia_full:
    print(city)

egnatia_distance = get_distance(G120, egnatia1) + get_distance(G120, egnatia2)
print('\nΗ συνολική απόσταση που θα διανύσει έιναι το άθροισμα των επιμέρους \n'
      'διαδρομών και ισούτε με: {}km.'.format(egnatia_distance))

if egnatia_distance < pathe_distance:
    print('Παρατηρούμε ότι η απόσταση της διαδρομής απο Εγνατία οδό είναι μικρότερη.')
else:
    print('Παρατηρούμε ότι η απόσταση της διαδρομής απο ΠΑΘΕ είναι μικρότερη.')

# question 4
print('\n\nΕρώτημα 4ο:\n')
print('Ο ποδηλάτης διανύει 120km την ημέρα, όμως μπορεί να μεταβεί μόνο σε μία πόλη ανα ημέρα.\n'
      'Συνεπώς θα χρειαστεί τόσες μέρες όσες και οι μετακινήσεις του απο πόλη σε πόλη,\n'
      'δηλαδή το άθροισμα των πόλεων της διαδρομής μείον 1, αφού η αρχική πόλη δεν συμβάλει σε μετακίνηση.')

days_pathe = len(pathe_full) - 1
days_egnatia = len(egnatia_full) - 1

print('Αρα, συνολικά θα χρειαστεί {} μερες απο ΠΑΘΕ και {} μέρες απο Εγνατία οδό.'.format(days_pathe, days_egnatia))