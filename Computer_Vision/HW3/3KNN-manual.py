import numpy as np
import json
from collections import Counter


def knn(train_hists, responses, test_hist, k):
    # the input is not the path, its the array of histograms or the histogram for the test image
    distances = np.sum((train_hists - test_hist) ** 2, axis=1)
    retrieved_ids = np.argsort(distances)
    # now i have the indexes showing the least to the highest distance
    # and i want the labels of the items for the k first indexes
    retrieved_labels = []
    for indx in range(k):
        retrieved_labels.append(int(responses[retrieved_ids[indx]]))

    # gives me a dict like structure that shows the item and the number of times it appears in the list
    results = Counter(retrieved_labels)
    # gives me a sorted dict from the most frequent to the least frequent item and the number of times it
    # appeared and I choose from the first pair of label:value and the label
    res = results.most_common()[0][0]
    return res


def knn_train_validate(train_path, responses, test_path, labels):
    hist_train = np.load(train_path)

    hist_tests = np.load(test_path)
    # the actual output of test images
    with open('index_test/test_img_names.txt') as f:
        hist_names = json.load(f)

    # different counts of nearest neighbors
    nearest_neigh = [5, 10, 50, 100, 200, 500]
    validation_per_knn = []
    for neigh_count in nearest_neigh:
        val_list = []
        for indx, hist in enumerate(hist_tests):

            # important in order for knn to work manual can work without it tho
            hist = np.reshape(hist, (1, len(hist)))

            # creating a different validation list for each of the nearest neighbors
            # doing it this way for depiction purposes

            # results will contain the label of the predicted item that acts as an index to the labelnames list
            results = knn(hist_train, responses, hist, neigh_count)

            # printing everything. Not recommended
            # print('Knn prediction {} \t\t {} \n'
            #       'real life {} \n\n'.format(labels[int(results)],
            #                                  (labels[int(results)] == hist_names[indx]), hist_names[indx]))

            # keeping the True False values
            val_list.append(labels[int(results)] == hist_names[indx])

        # appending the True False values to a list that will contain the results for every K
        validation_per_knn.append(val_list)

    # summing up all the True values of validation and dividing with the length of it for the accuracy percentage
    accuracy = [np.float(sum(valknn)/len(valknn)) for valknn in validation_per_knn]

    # printing the results of different K options for a given Bag Of Words
    print('\nBag Of Words: {}\n'.format(hist_train.shape[1]))
    for knear, accuracy in zip(nearest_neigh, accuracy):
        print('nearest neighbors: {}, '
              'accuracy: {:.3f}'.format(knear, accuracy))


# Creating the labels of the histograms = output
with open('index_train/train_img_paths.txt', mode='r') as file:
    img_paths = json.load(file)

# the label names should exist in the path of each photo and should be a unique label for each object
labelnames = ['fighter-jet', 'motorbike', 'school-bus', 'touring-bike', 'airplane', 'car']
labelid = np.zeros((len(img_paths), 1), dtype='float32')

for index, path in enumerate(img_paths):
    for lind, label in enumerate(labelnames):
        if label in path:
            labelid[index] = lind
            break

# labeling will remain the same for each input with different bag of words (= different histogram = different columns)

train_paths = ['index_train/hist10.npy', 'index_train/hist50.npy',
               'index_train/hist100.npy', 'index_train/hist200.npy',
               'index_train/hist500.npy']
test_paths = ['index_test/hist10test.npy', 'index_test/hist50test.npy',
              'index_test/hist100test.npy', 'index_test/hist200test.npy',
              'index_test/hist500test.npy']


for train, test in zip(train_paths, test_paths):
    knn_train_validate(train, labelid, test, labelnames)




