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


def index_creator(vocabulary, train_folders):
    img_paths = []
    bow_descs = np.zeros((0, vocabulary.shape[0]))
    sift = cv.xfeatures2d_SIFT.create()
    for folder in train_folders:
        files = os.listdir(folder)
        for file in files:
            path = os.path.join(folder, file)

            img = cv.imread(path)
            kp = sift.detect(img)
            # bow_desc = descriptor_extractor.compute(img, kp)
            bow_desc = images_histogram(vocabulary, img, kp)
            img_paths.append(path)
            bow_descs = np.concatenate((bow_descs, bow_desc), axis=0)

    return bow_descs, img_paths


train_folders = ['../imagedb/imagedb_train/069.fighter-jet', '../imagedb/imagedb_train/145.motorbikes-101',
                 '../imagedb/imagedb_train/178.school-bus', '../imagedb/imagedb_train/224.touring-bike',
                 '../imagedb/imagedb_train/251.airplanes-101', '../imagedb/imagedb_train/252.car-side-101']


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
print('Creating index_train...')


bow_descs10, _ = index_creator(voc10, train_folders)
np.save('index_train/hist10', bow_descs10)

bow_descs50, _ = index_creator(voc50, train_folders)
np.save('index_train/hist50', bow_descs50)

bow_descs100, _ = index_creator(voc100, train_folders)
np.save('index_train/hist100', bow_descs100)

bow_descs200, _ = index_creator(voc200, train_folders)
np.save('index_train/hist200', bow_descs200)

bow_descs500, img_paths = index_creator(voc500, train_folders)
np.save('index_train/hist500', bow_descs500)

with open('index_train/train_img_paths.txt', mode='w+') as f:
    json.dump(img_paths, f)

# based on the train_img_paths ill extract the target of each image histogram before fitting into the model


