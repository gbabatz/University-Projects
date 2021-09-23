import cv2 as cv
import numpy as np


def show(im, winname):
    cv.namedWindow(winname)
    cv.imshow(winname, im)
    cv.waitKey()


def find_show_contours(src, blur=13, thr=60, strsize=25, salt=False):

    def contour_callback(val, fname=src):
        img = cv.imread(fname)  # read the image on each callback so the image has not the prev drawings
        if val in obj_on_edge:
            cv.drawContours(img, contours, val, (0, 0, 255), 3)
            cv.imshow(source_window, img)   #dispay the outcome on the same window as the trackbar
        else:
            cv.drawContours(img, contours, val, (0, 255, 0), 3)
            cv.imshow(source_window, img)   #dispay the outcome on the same window as the trackbar

    img = cv.imread(src)  # read the image(colored)
    imgray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)  # turn the  image to gray so you can threshold

    if salt:  #if we have the salt and pepper image
        imgray = cv.medianBlur(imgray, 5)

    img_blur = cv.blur(imgray, (blur, blur)) #blur the image

    _, thresh = cv.threshold(img_blur, thr, 255, cv.THRESH_BINARY)  # threshold

    strel = cv.getStructuringElement(cv.MORPH_ELLIPSE, (strsize, strsize))  #opening for objects that are too close together
    opening = cv.morphologyEx(thresh, cv.MORPH_OPEN, strel)
    # show(opening, 'opening')

    _, contours, _ = cv.findContours(opening, cv.RETR_EXTERNAL,
                                     cv.CHAIN_APPROX_NONE)  # find contours at the 'binary' image

    obj_on_edge = []  # contains the indexes of all_contours that are invalid

    for indx, cnt in enumerate(contours):
        for pair in cnt:  # a pair is a [[x y]] value  BE CAREFUL x = column , y = row
            if pair[0][0] == 0 or pair[0][0] == imgray.shape[1] - 1 \
                    or pair[0][1] == 0 or pair[0][1] == imgray.shape[0] - 1:  # ama i timi tis x ( col ) [0][x] einai isi me 0 (aristeri/prwti stili)
                obj_on_edge.append(indx)  # H ama h timi tou col einai isi me shape[1]-1 (teleftaia stili)
                break

    valid_contours = np.copy(contours)
    mask = np.ones(len(contours), dtype=bool)
    mask[obj_on_edge] = False
    valid_contours = valid_contours[mask]


    # create window
    source_window = 'original'
    cv.namedWindow(source_window)
    cv.imshow(source_window, img)

    number = 0  # trackbar initial point
    max = len(contours) - 1  # trackbar maxvalue
    cv.createTrackbar('obj num', source_window, number, max, contour_callback)  # add trackbar on the window
    contour_callback(number)  # call the function so the the contour at index 0 is drawn
    cv.waitKey()


def median_value(integral_table, rowindex, colindex, xrange, yrange):

    temp = integral_table


    if rowindex == 0 and colindex == 0:     # if the requested box is attached to the upper left corner
        result = temp[rowindex + yrange - 1, colindex + xrange - 1]
    elif rowindex == 0:                     # if the requested box is attached to the upper row
        d = temp[rowindex + yrange - 1, colindex + xrange - 1]
        c = temp[rowindex + yrange - 1, colindex - 1]
        result = d - c
    elif colindex == 0:                     # if the requested box is attached to the far left corner
        d = temp[rowindex + yrange - 1, colindex + xrange - 1]
        b = temp[rowindex - 1, colindex + xrange - 1]
        result = d - b
    else:                                   # if none of the above
        a = temp[rowindex - 1, colindex - 1]
        b = temp[rowindex - 1, colindex + xrange - 1]
        c = temp[rowindex + yrange - 1, colindex - 1]
        d = temp[rowindex + yrange - 1, colindex + xrange - 1]
        result = a + d - b - c

    # for the median value we have to divide the sum of gray shades with the amount of pixels
    result = result / (xrange * yrange)
    return result


def integral_image(img):
    integral = np.zeros((len(img), len(img[0])))
    for i in range(len(img)):
        for j in range(len(img[0])):
            integral[i, j] = np.sum(img[:i + 1, :j + 1])
    return integral






def blur_3x3_full(table):
    table_blur = np.zeros((table.shape[0], table.shape[1]), dtype='uint8')
    for i in range(table.shape[0]):  # row
        for j in range(table.shape[1]):  # column

            if i == 0:  # top row case
                if j == 0:  # upper left corner case
                    kernel_sum = sum([table[i, j], table[i, j + 1], table[i + 1, j], table[i + 1, j + 1]])
                    table_blur[i, j] = kernel_sum / 4
                elif j == table.shape[1] - 1:  # upper right corner case
                    kernel_sum = sum([table[i, j], table[i, j - 1], table[i + 1, j], table[i + 1, j - 1]])
                    table_blur[i, j] = kernel_sum / 4
                else:
                    kernel_sum = sum(
                        [table[i, j], table[i, j + 1], table[i + 1, j - 1], table[i + 1, j], table[i + 1, j + 1]])
                    table_blur[i, j] = kernel_sum / 6
            elif i == table.shape[0] - 1:  # bottom row case
                if j == 0:  # bottom left corner case
                    kernel_sum = sum([table[i, j], table[i, j + 1], table[i - 1, j], table[i - 1, j + 1]])
                    table_blur[i, j] = kernel_sum / 4
                elif j == table.shape[1] - 1:  # bottom right corner case
                    kernel_sum = sum([table[i, j], table[i, j - 1], table[i - 1, j], table[i - 1, j - 1]])
                    table_blur[i, j] = kernel_sum / 4
                else:
                    kernel_sum = sum(
                        [table[i - 1, j - 1], table[i - 1, j], table[i - 1, j + 1], table[i, j - 1], table[i, j],
                         table[i, j + 1]])
                    table_blur[i, j] = kernel_sum / 6
            elif j == 0:  # left column case
                kernel_sum = sum([table[i - 1, j], table[i - 1, j + 1], table[i, j], table[i, j + 1], table[i + 1, j],
                                  table[i + 1, j + 1]])
                table_blur[i, j] = kernel_sum / 6
            elif j == table.shape[1] - 1:  # right column case
                kernel_sum = sum(
                    [table[i - 1, j - 1], table[i - 1, j], table[i, j - 1], table[i, j], table[i + 1, j - 1],
                     table[i + 1, j]])
                table_blur[i, j] = kernel_sum / 6
            else:
                kernel_sum = sum([table[i - 1, j - 1], table[i - 1, j], table[i - 1, j + 1], table[i, j - 1], table[i, j],table[i, j + 1], table[i + 1, j - 1], table[i + 1, j], table[i + 1, j + 1]])
                value = np.uint8(kernel_sum/9)
                table_blur[i, j] = value
    return table_blur






