import numpy as np
import cv2 as cv
import os
import json


def images_histogram(vocabulary, img, kp):

    hist = np.zeros((1, vocabulary.shape[0]), dtype='float32')
    sift = cv.xfeatures2d_SIFT.create()
    bf = cv.BFMatcher(cv.NORM_L2SQR)

    querydesc = sift.compute(img, kp)
    querydesc = querydesc[1]

    matches = bf.match(querydesc, vocabulary)

    for match in matches:
        trainidx = match.trainIdx

        hist[0][trainidx] = hist[0][trainidx] + 1

    hist = hist/(len(kp))
    return hist


def index_creator_test(vocabulary, train_folders, label_names):
    # hist_names will contain the actual output of the image
    # its almost the same as having the image path but now we have the
    # information that counts -> only the name of the item
    hist_names = []
    currlabel = ''
    hist_tests = np.zeros((0, vocabulary.shape[0]))

    sift = cv.xfeatures2d_SIFT.create()
    for folder in train_folders:
        files = os.listdir(folder)

        # find which folder we have for later validation
        for label in label_names:
            if label in folder:
                currlabel = label
                break

        for file in files:
            path = os.path.join(folder, file)

            img_test = cv.imread(path)
            kptest = sift.detect(img_test)
            hist_test = images_histogram(vocabulary, img_test, kptest)
            hist_tests = np.concatenate((hist_tests, hist_test), axis=0)
            hist_names.append(currlabel)

    return hist_tests, hist_names


test_folders = ['../imagedb/imagedb_test/069.fighter-jet', '../imagedb/imagedb_test/145.motorbikes-101',
                '../imagedb/imagedb_test/178.school-bus', '../imagedb/imagedb_test/224.touring-bike',
                '../imagedb/imagedb_test/251.airplanes-101', '../imagedb/imagedb_test/252.car-side-101']


# Load vocabulary
voc10 = np.load('vocabulary/voc10.npy')
voc50 = np.load('vocabulary/voc50.npy')
voc100 = np.load('vocabulary/voc100.npy')
voc200 = np.load('vocabulary/voc200.npy')
voc500 = np.load('vocabulary/voc500.npy')

# now we have our vocabulary
# next step is to create a histogram of each image
# which is how frequent each value of img descriptor appears in a specific cluster
# or 'near a cluster center'
# this histogram will be the identifier of the image

# Create Index
print('Creating index_test...')

labelnames = ['fighter-jet', 'motorbike', 'school-bus', 'touring-bike', 'airplane', 'car']

bow_descs10, _ = index_creator_test(voc10, test_folders, labelnames)
np.save('index_test/hist10test', bow_descs10)

bow_descs50, _ = index_creator_test(voc50, test_folders, labelnames)
np.save('index_test/hist50test', bow_descs50)

bow_descs100, _ = index_creator_test(voc100, test_folders, labelnames)
np.save('index_test/hist100test', bow_descs100)

bow_descs200, _ = index_creator_test(voc200, test_folders, labelnames)
np.save('index_test/hist200test', bow_descs200)

bow_descs500, actual_name = index_creator_test(voc500, test_folders, labelnames)
np.save('index_test/hist500test', bow_descs500)

with open('index_test/test_img_names.txt', mode='w+') as f:
    json.dump(actual_name, f)

# based on the train_img_paths ill extract the target of each image histogram before fitting into the model
