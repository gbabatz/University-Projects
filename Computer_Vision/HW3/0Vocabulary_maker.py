import numpy as np
import cv2 as cv
import os


def extract_local_features(p):
    sift = cv.xfeatures2d_SIFT.create()
    img = cv.imread(p)
    kp = sift.detect(img)
    descr = sift.compute(img, kp)
    descr = descr[1]
    return descr


train_folders = ['../imagedb/imagedb_train/069.fighter-jet', '../imagedb/imagedb_train/145.motorbikes-101',
                 '../imagedb/imagedb_train/178.school-bus', '../imagedb/imagedb_train/224.touring-bike',
                 '../imagedb/imagedb_train/251.airplanes-101', '../imagedb/imagedb_train/252.car-side-101']


# creating a list with rows as many as the total keypoints of all the images
# and columns as many as the descriptor value =128 because of sift

print('Extracting features . . .')
train_descs = np.zeros((0, 128))
for folder in train_folders:
    files = os.listdir(folder)
    for file in files:
        path = os.path.join(folder, file)
        desc = extract_local_features(path)
        train_descs = np.concatenate((train_descs, desc), axis=0)

# now we have all the descriptors
# imagine that the descriptors are 3d values
# and thus can place them in a 3d plane
# what we actually have done is to fill this 3d plane with points in space
# and now we ll create our vocabulary which is the center of some clusters
# algorithm finds the clusters we only need to tell the algorithm how many we want

# Create vocabulary
print('Creating vocabulary...')

term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
trainer = cv.BOWKMeansTrainer(10, term_crit, 1, cv.KMEANS_PP_CENTERS)
vocabulary = trainer.cluster(train_descs.astype(np.float32))
np.save('vocabulary/voc10.npy', vocabulary)

term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
trainer = cv.BOWKMeansTrainer(50, term_crit, 1, cv.KMEANS_PP_CENTERS)
vocabulary = trainer.cluster(train_descs.astype(np.float32))
np.save('vocabulary/voc50.npy', vocabulary)

term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
trainer = cv.BOWKMeansTrainer(100, term_crit, 1, cv.KMEANS_PP_CENTERS)
vocabulary = trainer.cluster(train_descs.astype(np.float32))
np.save('vocabulary/voc100.npy', vocabulary)

term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
trainer = cv.BOWKMeansTrainer(200, term_crit, 1, cv.KMEANS_PP_CENTERS)
vocabulary = trainer.cluster(train_descs.astype(np.float32))
np.save('vocabulary/voc200.npy', vocabulary)

term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
trainer = cv.BOWKMeansTrainer(500, term_crit, 1, cv.KMEANS_PP_CENTERS)
vocabulary = trainer.cluster(train_descs.astype(np.float32))
np.save('vocabulary/voc500.npy', vocabulary)

