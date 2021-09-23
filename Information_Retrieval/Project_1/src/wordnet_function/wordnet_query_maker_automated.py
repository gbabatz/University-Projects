import xml.etree.ElementTree as et
import string
import nltk
# nltk.download()
from nltk.corpus import wordnet as wn


def remove_doubles(x):
    return list(dict.fromkeys(x))


def normalize(string1):
    string1.lower()
    string1 = string1.replace('_', ' ')
    string1 = string1.replace('/', ' ')
    return string1


def remove_punctuation(str_in, punctuation_list):
    char_list_without_punct = [char for char in str_in if char not in punctuation_list]
    text_without_punct = ''.join(char_list_without_punct)
    return text_without_punct


def indent(elem, level=0):
    i = "\n" + level * "  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level + 1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i


# takes as input a string word and
# returns an enhances string
def find_word_synonyms(word):
    syn_sets = wn.synsets(word)

    # we add the word to the synonyms in case its not
    # in the list , to make sure that the original
    # word would not be lost
    syns = word
    for syn_set in syn_sets:
        lemma_names = syn_set.lemma_names()
        # lemma names are candidates for synonyms
        # ill have a score to see if they match enough
        # with the initial word
        for lemma in lemma_names[:2]:
            # we take the first synset of the given lemma name to compare with
            lemma_syn = wn.synsets(lemma)[0]
            score = syn_set.wup_similarity(lemma_syn)
            if score is not None and score > 0.95:
                syns = syns + ' ' + lemma
    return syns


def enhance_text(text):
    final_text = []
    words = text.split(' ')
    # for each word
    for word in words:
        syns = find_word_synonyms(word)
        final_text.append(syns)
        # final_text now is a list of strings with strings being the
        # enhanced version of each word

    # after we are done with each word its time for some
    # normalization and deleting doubles
    final_text = ' '.join(final_text)
    # final text is now a unified string
    final_text = normalize(final_text)
    final_text = final_text.split(' ')
    # now final text is a list with each word
    # time to delete doubles
    final_text = remove_doubles(final_text)
    final_text = ' '.join(final_text)
    return final_text


def wordnet(mode_option):
    mode_option = int(mode_option)
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    basic_query_paths = ['basic_queries_titles.xml', 'basic_queries_titles_desc.xml', 'basic_queries_titles_desc_narr.xml']

    query_parse_tree = et.parse(base_path + basic_query_paths[mode_option])
    query_parse_root = query_parse_tree.getroot()

    number = 301

    root = et.Element('parameters')
    et.SubElement(root, 'index').text = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/indices/index1'
    et.SubElement(root, 'rule').text = 'method:dirichlet,mu:1000'
    et.SubElement(root, 'count').text = '1000'
    et.SubElement(root, 'trecFormat').text = 'true'

    punct_list = set(string.punctuation)
    punct_list.remove('-')
    for query_text in query_parse_root.iter('text'):

        query = et.SubElement(root, 'query')
        et.SubElement(query, 'type').text = 'indri'
        et.SubElement(query, 'number').text = str(number)
        number += 1

        enhanced_query = enhance_text(query_text.text)
        enhanced_query = remove_punctuation(enhanced_query, punct_list)
        et.SubElement(query, 'text').text = enhanced_query

    indent(root)
    tree = et.ElementTree(root)
    output_files = ['wordnet_titles.xml', 'wordnet_titles_desc.xml', 'wordnet_titles_desc_narr.xml']
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    file_title = base_path + output_files[mode_option]

    tree.write(file_title)


def main():
    for mode in [0, 1, 2]:
        wordnet(mode)


main()

