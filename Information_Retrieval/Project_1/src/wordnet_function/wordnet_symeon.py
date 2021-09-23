from nltk.corpus import wordnet as wn
import string


def remove_duplicates(list):
    newlist = []
    for i in list:
        if i not in newlist:
            newlist.append(i)
    return newlist


def add_syn(old, new):
    output = open(new, 'w')
    qnum = 301
    exclude = set(string.punctuation)
    synset_num = 0
    with open(old, 'r+') as f:
        for line in f:
            if '<query>' not in line:
                output.write(line)
                continue
            title = line.rpartition('<text>')[-1].rpartition('</text>')[0]
            title = title.split(' ')
            syns = ''
            for i in title:
                syn_sets = wn.synsets(i)
                for syn_set in syn_sets:
                    if synset_num > 1:
                        break
                    newsyns = ' '.join(syn_set.lemma_names())
                    newsyns = newsyns.replace('_', ' ')
                    newsyns = ''.join(ch for ch in newsyns if ch not in exclude)
                    syns = syns + ' ' + newsyns
                    synset_num += 1
            synset_num = 0
            title.append(syns)
            title = ' '.join(title)
            title = title.split(' ')
            title = remove_duplicates(title)
            title = ' '.join(title)
            output.write('<query> <type>indri</type> <number>%d</number> <text>%s</text> </query>\n' % (qnum, title))
            qnum += 1


def main():
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    add_syn(base_path + 'basic_queries_titles.xml', base_path + 'symeon_enhc_titles.xml')


main()
