base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
topics_str = open(base_path + 'topics.301-450.trec').read()


# takes as input a string parameter and returns a list
# of strings so that each tag is seperated from the other
def split_tags(str_in):
    str_splited = str_in.split(' ')
    # making sure that every tag is seperated from the other
    # because you may see items like '<top>\n\n<num>' if you
    # split only on white space, you dont want that
    str_splited2 = []
    for substr in str_splited:
        temp = substr.split('\n')
        for element in temp:
            str_splited2.append(element)
    return str_splited2


def make_end_tag(tag):
    endtag = tag[0] + '/' + tag[1:]
    return endtag


# making xml readable by adding spaces n stuf
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


topics_str_split = split_tags(topics_str)

# when i find an opening tag I keep the closing version of it
# on temptag and append it when i find the next opening tag
result = []
temptag = ''

# adding root tag
result.append('<topics>')
for elem in topics_str_split:

    # escape char..this can be done in next step doing it now
    # because & produces error when parsing xml in next step
    if '&' in elem:
        elem = elem.replace('&', '&amp;')

    if '<num>' in elem:
        result.append(elem)
        temptag = make_end_tag(elem)
    elif '<title>' in elem:
        result.append(temptag)
        result.append(elem)
        temptag = make_end_tag(elem)
    elif '<desc>' in elem:
        result.append(temptag)
        result.append(elem)
        temptag = make_end_tag(elem)
    elif '<narr>' in elem:
        result.append(temptag)
        result.append(elem)
        temptag = make_end_tag(elem)
    elif '</top>' in elem:
        result.append(temptag)
        result.append(elem)
    else:
        result.append(elem)

# ending root tag
result.append('</topics>')
# print(result)
new_topics_str = ' '.join(result)
# print(newstr)

with open(base_path + 'topics_all_reformated.xml', 'w') as newfile:
    newfile.write(new_topics_str)


# making it readable part..does not really matter
import xml.etree.ElementTree as et

# standar way of opening xml files
tree = et.parse(base_path + 'topics_all_reformated.xml')
root = tree.getroot()

# readable with the indent function I found online :D
indent(root)
newtree = et.ElementTree(root)
newtree.write(base_path + 'topics_all_reformated_readable.xml')
