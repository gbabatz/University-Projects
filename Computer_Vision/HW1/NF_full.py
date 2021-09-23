import cv2 as cv
import numpy as np
import sys
sys.path.append('myfunctions.py') #not sure if it works this way but i tried it
import myfunctions as myf
import math


def show(im, winname):
    cv.namedWindow(winname)
    cv.imshow(winname, im)
    cv.waitKey()


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 1.filtering/blur ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


img = cv.imread('NF1.png')                          # loading the image (colored)
imgray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)        # turning the image to grayscale
img_blur = cv.blur(imgray, (13, 13))                # blurring for a smoother threshold
# show(imgray, 'imgray')
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2.thresholding ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

res, thres = cv.threshold(img_blur, 60, 255, cv.THRESH_BINARY)   # creating the binary image
# show(thres, 'threshold')
strel = cv.getStructuringElement(cv.MORPH_ELLIPSE, (25, 25))    # appliying opening in order to detach objects that may touch
opening = cv.morphologyEx(thres, cv.MORPH_OPEN, strel)
# show(opening, 'opening')
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 3. bounding boxes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# findContours finds the contours of each object and outputs the contour's coordinates
# important! coordinates are x, y format where x is the col number and y the row number
_, all_contours, _ = cv.findContours(opening, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)

# ~~~~~~~~~~~~~~~checking if the objects that I found touch on the edges of the image~~~~~~~~~~~~~~~~~~~~~~~~
obj_on_edge = []  # contains the indexes of all_contours that are invalid

for indx, cnt in enumerate(all_contours):
    for pair in cnt:   # a pair is a [[x y]] value  BE CAREFUL x = column , y = row
        # if x value(col) is equal to first or last column index Or if y value(row) is equal to first or last row index
        # shape[0]-1 gives the last row and shape[1]-1 gives the last column (last meaning the last valid index)
        if pair[0][0] == 0 or pair[0][0] == imgray.shape[1]-1 or pair[0][1] == 0 or pair[0][1] == imgray.shape[0]-1:
            obj_on_edge.append(indx)
            break                   # if at least one point has these coords then append it as obj on edge and analyze next obj


valid_contours = np.copy(all_contours)

mask = np.ones(len(all_contours), dtype=bool)
mask[obj_on_edge] = False
valid_contours = valid_contours[mask]

img3 = cv.imread('NF1.png')
cv.drawContours(img3, valid_contours, -1, (0, 255, 0), 2)
show(img3, 'valids')
cv.imwrite('NF_valids.png',img3)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 1. OBJECT COUNTING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

print('We have {} objects in total. {} of them are touching the boundaries of the image so they are taged\nas invalid'
      ' and thus we are left with {} valid objects.'.format(len(all_contours), len(obj_on_edge), len(valid_contours)))

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2. OBJECT AREA COUNTING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

area = [cv.contourArea(i) for i in valid_contours]
for ind, item in enumerate(area):
    print('the item numbered {} has {} total pixel area'.format(ind, math.ceil(item)))


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 3. median gray value of each obj bounding box ~~~~~~~~~~~~~~~~~~~~~~~~~~


boundRect = [cv.boundingRect(c) for c in all_contours]  # create bounding boxes
boundRect_valid = np.copy(boundRect)
boundRect_valid = boundRect_valid[mask]


int_table = myf.integral_image(imgray)
print('table creted')
# demonstration that the calculation is independent of time using user input
# the index of the object in the contours list that that we want the median value
try:   # cant type something else other than a number two times in a row , exception will occur
    indx = int(input('give me a num between 0, {} or -1 to exit  '.format(len(valid_contours)-1)))
except:
    print('type a number')
    indx = int(input('give me a num between 0, {} or -1 to exit  '.format(len(valid_contours)-1)))
while indx != -1:

    if indx > len(valid_contours)-1:
        print("wrong input,out of bounds")
    else:
        col, row, xlen, ylen = boundRect_valid[indx]   # getting the desired box values
        point_gray_sum = myf.median_value(int_table, row, col, xlen, ylen)  # getting the median value
        print('the median gray pixel value of object {} is {}'.format(indx,point_gray_sum))

    # the index of the object in the contours list that that we want the median value
    try:
        indx = int(input('give me a num between 0, {} or -1 to exit  '.format(len(valid_contours)-1)))
    except:
        print('type a number')
        indx = int(input('give me a num between 0, {} or -1 to exit  '.format(len(valid_contours)-1)))

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   EXTRAS  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


for i in range(len(all_contours)):   # appliying the bounding boxes on image so we can visually see the seperated objects

    if i in obj_on_edge:        # red boundary on objects on edge
        color = (0, 0, 255)
        cv.rectangle(img, (int(boundRect[i][0]), int(boundRect[i][1])),
                     (int(boundRect[i][0] + boundRect[i][2]), int(boundRect[i][1] + boundRect[i][3])), color, 2)
    else:
        color = (255, 0, 0)     # blue boundary on valid objects
        cv.rectangle(img, (int(boundRect[i][0]), int(boundRect[i][1])),
                     (int(boundRect[i][0] + boundRect[i][2]), int(boundRect[i][1] + boundRect[i][3])), color, 2)


color = (109, 50, 255)                        # random color
cv.drawContours(img, all_contours, -1, color)     # draw the bounding boxes on the original image
show(img, 'result')


myf.find_show_contours('NF1.png')
