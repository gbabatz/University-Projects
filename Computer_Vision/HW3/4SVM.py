import cv2 as cv
import numpy as np
import json


def svm_train_test(train_hist_path, kernel_type, hist_test_path):
    # the order of the below items is taken from the true values that the kernels represent
    # so when I use a kernel with true value eg 0 = linear , kernel_names[0] will show me the name.
    kernel_names = ['Linear', 'Poly', 'RBF', 'Sigmoid']
    # creating the SVM models
    # print('creating SVMs...')
    svmfigher = cv.ml.SVM_create()
    svmfigher.setType(cv.ml.SVM_C_SVC)
    svmfigher.setKernel(kernel_type)
    svmfigher.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    svmmotorbike = cv.ml.SVM_create()
    svmmotorbike.setType(cv.ml.SVM_C_SVC)
    svmmotorbike.setKernel(kernel_type)
    svmmotorbike.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    svmbus = cv.ml.SVM_create()
    svmbus.setType(cv.ml.SVM_C_SVC)
    svmbus.setKernel(kernel_type)
    svmbus.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    svmbike = cv.ml.SVM_create()
    svmbike.setType(cv.ml.SVM_C_SVC)
    svmbike.setKernel(kernel_type)
    svmbike.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    svmairplane = cv.ml.SVM_create()
    svmairplane.setType(cv.ml.SVM_C_SVC)
    svmairplane.setKernel(kernel_type)
    svmairplane.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    svmcar = cv.ml.SVM_create()
    svmcar.setType(cv.ml.SVM_C_SVC)
    svmcar.setKernel(kernel_type)
    svmcar.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))

    # extra param for poly kernel
    if kernel == 1:
        svmfigher.setDegree(3)
        svmmotorbike.setDegree(3)
        svmbus.setDegree(3)
        svmbike.setDegree(3)
        svmairplane.setDegree(3)
        svmcar.setDegree(3)

    train_hist = np.load(train_hist_path)
    # print('Training the SVMs...for clusters = {}'.format(train_hist.shape[1]))

    # care, the labels are named on the outside scope
    svmfigher.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, fighterlabel)
    svmmotorbike.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, motorbikelabel)
    svmbus.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, buslabel)
    svmbike.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, bikelabel)
    svmairplane.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, airplanelabel)
    svmcar.trainAuto(train_hist.astype(np.float32), cv.ml.ROW_SAMPLE, carlabel)

    # now its time to test the SVMs
    # print('Testing the SVM. . .')

    # loading the test indexes
    hist_tests = np.load(hist_test_path)

    # loading the actual labels
    with open('index_test/test_img_names.txt') as file:
        hist_names = json.load(file)

    labelnames = ['fighter-jet', 'motorbike', 'school-bus', 'touring-bike', 'airplane', 'car']
    # at validation I append True or False depending on the prediction of the given item
    validation = []
    # trying each model for each item
    for indexactuall, hist in enumerate(hist_tests):
        hist = np.reshape(hist, (1, len(hist)))  # important in order to work inside a for loop

        _, resultfigher = svmfigher.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        _, resultmotorbike = svmmotorbike.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        _, resultbus = svmbus.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        _, resultbike = svmbike.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        _, resultairplane = svmairplane.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        _, resultcar = svmcar.predict(hist.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)
        # print('hist', indexactuall)

        # I use the strongest score out of all the models (most negative) as my prediction
        # be careful the indexes of the array below should correspond on the label names in order to work
        prindex = np.argmin(np.array([resultfigher, resultmotorbike, resultbus, resultbike, resultairplane, resultcar]))
        # print('prediction: {}\n'
        #       'actual: {} \t\t overall: {}'.format(labelnames[int(prindex)],
        #                                            hist_names[indexactuall],
        #                                            labelnames[int(prindex)] == hist_names[indexactuall]))

        validation.append(labelnames[int(prindex)] == hist_names[indexactuall])

    # summing up all the True values of validation and dividing with the length of it for the accuracy percentage
    accuracy = np.float(sum(validation)/len(validation))

    print("kernel type : {}\n"
          "visual words = {}\n"
          "accuracy {:.3f}%\n\n".format(kernel_names[kernel_type], train_hist.shape[1], accuracy))


# SVM TRAINING
print('Obtaining the labels...\n')

# opening the img paths that contain the path of the histogram for a given index
# can be used for labeling the data before fitting them
with open('index_train/train_img_paths.txt') as f:
    img_paths = json.load(f)

# extracting the right labeling for each classifier
# the labels will contain '1' on the histograms that that correspond on the specific item and '0' on all the others
fighterlabel = np.array(['fighter' in path for path in img_paths], np.int32)
motorbikelabel = np.array(['motorbikes' in path for path in img_paths], np.int32)
buslabel = np.array(['school-bus' in path for path in img_paths], np.int32)
bikelabel = np.array(['touring-bike' in path for path in img_paths], np.int32)
airplanelabel = np.array(['airplanes' in path for path in img_paths], np.int32)
carlabel = np.array(['car-side' in path for path in img_paths], np.int32)

# for training we'll use the same labels (=outputs) but with different histogram input
# the difference is basically on the columns of each histogram
# loading the histograms

train_paths = ['index_train/hist10.npy', 'index_train/hist50.npy',
               'index_train/hist100.npy', 'index_train/hist200.npy',
               'index_train/hist500.npy']
test_paths = ['index_test/hist10test.npy', 'index_test/hist50test.npy',
              'index_test/hist100test.npy', 'index_test/hist200test.npy',
              'index_test/hist500test.npy']
# be careful the two above lists should correspond

kernel_types = [cv.ml.SVM_LINEAR, cv.ml.SVM_POLY, cv.ml.SVM_RBF, cv.ml.SVM_SIGMOID]

for kernel in kernel_types:
    for train, test in zip(train_paths, test_paths):
        print('Computing. . .')
        svm_train_test(train, kernel, test)
