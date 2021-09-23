import xml.etree.ElementTree as et
import string


def normalize(string1):
    string1 = string1.replace('_', ' ')
    string1 = string1.replace('/', ' ')
    return string1


def remove_punctuation(str_in, punctuation_list):
    char_list_without_punct = [char for char in str_in if char not in punctuation_list]
    text_without_punct = ''.join(char_list_without_punct)
    return text_without_punct


def query_maker(mode_option):
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    mode_option = int(mode_option)
    index_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/indices/index1'
    basic_structure = '<parameters>\n' \
                      '<index>' + index_path + '</index>\n' \
                      '<rule>method:dirichlet,mu:1000</rule>\n' \
                      '<count>1000</count>\n<trecFormat>true</trecFormat>\n'

    # loop from here
    # parse the topics
    topics_file_name = 'topics_all_reformated_readable.xml'
    full_topics_path = base_path + topics_file_name
    topics_tree = et.parse(full_topics_path)
    topics_root = topics_tree.getroot()

    modes = ['titles', 'titles_desc', 'titles_desc_narr']

    number = 301
    result = basic_structure
    punct_list = set(string.punctuation)
    punct_list.remove('-')
    for title, desc, narr in zip(topics_root.iter('title'), topics_root.iter('desc'), topics_root.iter('narr')):

        # got to take the 'Description: ' off the text
        # we know that the word takes a standard size on the beginning of the string
        desc.text = desc.text[14:]
        # same for Narrative:
        narr.text = narr.text[12:]

        if mode_option == 0:
            query_txt = title.text
            query_txt = normalize(query_txt)
            query_txt = remove_punctuation(query_txt, punct_list)
        elif mode_option == 1:
            query_txt = title.text + desc.text
            query_txt = normalize(query_txt)
            query_txt = remove_punctuation(query_txt, punct_list)
        elif mode_option == 2:
            query_txt = title.text + desc.text + narr.text
            query_txt = normalize(query_txt)
            query_txt = remove_punctuation(query_txt, punct_list)

        query_structure = '  <query> <type>indri</type><number>' + str(number) + '</number><text> ' + query_txt + ' </text></query>\n'
        number += 1
        result = result + query_structure

    result = result + '</parameters>'

    # yes it processes all even though some data may not need to be processed
    # for example when I need only titles however the application is low cost
    # and it looks cleaner like this

    file_title = base_path + 'basic_queries_' + modes[mode_option] + '.xml'
    with open(file_title, 'w') as f:
        f.write(result)


def main():
    for mode in [0, 1, 2]:
        query_maker(mode)


main()
