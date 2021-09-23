# place this file inside trec_eval folder
import os


def run_eval(query_num):
    base_path = '/home/gbabatz/workspace/IR/IR-2019-2020-Project-1/'
    qrels_path = base_path + 'qrels.301-450.trec.adhoc'
    basic_file_names = ['basic_queries_titles', 'basic_queries_titles_desc', 'basic_queries_titles_desc_narr']
    query_results_path = base_path + 'results/basic/' + 'results_' + basic_file_names[query_num] + '.txt'
    save_results_path = base_path + 'results/basic/' + 'eval_' + basic_file_names[query_num] + '.txt'

    command = './trec_eval ' + qrels_path + ' ' + query_results_path + ' > ' + save_results_path
    os.system(command)


def main():
    for qnum in [0, 1, 2]:
        run_eval(qnum)


main()
