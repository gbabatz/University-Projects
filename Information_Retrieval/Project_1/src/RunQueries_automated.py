import os


def run_q(query_num):
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    basic_file_names = ['basic_queries_titles', 'basic_queries_titles_desc', 'basic_queries_titles_desc_narr']
    query_path = base_path + basic_file_names[query_num] + '.xml'
    save_results_path = base_path + 'results/basic/' + 'results_' + basic_file_names[query_num] + '.txt'

    command = 'IndriRunQuery ' + query_path + ' > ' + save_results_path
    os.system(command)


def main():
    for qnum in [0, 1, 2]:
        run_q(qnum)


main()
