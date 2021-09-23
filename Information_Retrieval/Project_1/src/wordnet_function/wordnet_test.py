from nltk.corpus import wordnet

# kathe leksi vgazei ena synset kai kathe synset exei kapoia lemmanames
# thelw na filtrarw ta lemmas, den ta thelw ola gt mporei na exei akyra
# to point einai na eksetasw kata poso ta lemas enos dedomenou synset
# tairiazoyn me to synset kai to kanw eksetazontas to similarity toy arxikou synset
# apo to opoio parigaga kapoia lemas (merika ap ta opoia mporei na mi thelw) me to \
# 1o synset tis leksis lemma . H synartisi wup_similarity dexete san orisma synset
# g afto to mperdema... eprepe dld na dw kata poso to 1o synset toy lemma kathorizei
# tin ennoia tou ... kapws etsi .


word = 'law-breaking'
print('the initial word is : {} \n'.format(word))
syns = wordnet.synsets(word)

print(syns)
print('\n')

for synset in syns:
    lemmanames = synset.lemma_names()
    print('  for synset: {}'.format(synset))
    print('  lemma names: {}\n'.format(lemmanames))

    for name in lemmanames:
        newsynset = wordnet.synsets(name)
        print('    synset of lemma {} is: {}'.format(name, newsynset))


synsetfortest = wordnet.synsets('offense')[0]
print(synsetfortest)
score = syns[0].wup_similarity(synsetfortest)
print(score)
