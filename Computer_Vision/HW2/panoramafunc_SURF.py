import numpy as np
import cv2 as cv
import os
import timeit


def show(img, winname):
    cv.namedWindow(winname, cv.WINDOW_NORMAL)
    cv.imshow(winname, img)
    cv.waitKey(0)


def find_transform(matches, kp1, kp2):
    img_pt1 = np.array([kp1[x.queryIdx].pt for x in matches])
    img_pt2 = np.array([kp2[x.trainIdx].pt for x in matches])

    m, mask = cv.findHomography(img_pt2, img_pt1, cv.RANSAC)
    return m


def crop(img):
    # checking if the image is gray or not
    # because the way i used is for grayscale only
    if len(img.shape) == 3:
        gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    else:
        gray = np.copy(img)

    _, binmage = cv.threshold(gray, 0, 255, cv.THRESH_BINARY)
    _, contours, _ = cv.findContours(binmage, cv.RETR_LIST, cv.CHAIN_APPROX_NONE)

    # finding the far right edge to cut
    maxr = 0
    maxc = 0
    for pair in contours[0]:
        col = pair[0][0]  # x
        row = pair[0][1]  # y
        if col > maxc:
            maxc = col
        if row > maxr:
            maxr = row
    img = img[0:maxr, 0:maxc]

    return img


def cropping_with_margins(img):
    # flag in order to return the same image format
    if len(img.shape) == 3:
        gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    else:
        gray = np.copy(img)

    points = find_corner_points(img)
    points = np.array(points)
    xmin, ymin = np.amin(points, axis=0)
    xmax, ymax = np.amax(points, axis=0)
    newimg = img[ymin:ymax, xmin:xmax]

    return newimg


def find_corner_points(img):
    # warning do not crop down the photo
    # the actual image should not touch with the boundaries of the image array
    # so that its corners can be detectable
    # to find the corners we ll use the cornerHarris function
    # first we'll check if our image is coloured or not and then
    # we 'll transform our image to binary to do the processing

    if len(img.shape) == 3:
        gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    else:
        gray = np.copy(img)

    _, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY)

    # I found that with the parameters (binary, 5, 5, 0.04)
    # cornerHarris gives me exactly one point on the corners
    # which is what I actually want
    # however its not the exact corner point at some cases
    # it may be one col and row towards the image

    dst = cv.cornerHarris(binary, 5, 5, 0.04)
    validPoints = [dst > 0.85 * dst.max()]

    # validPoints is like [array[[]]] I don't know why so I need to do the following
    validPoints = validPoints[0]

    # a list that will contain the coordinates
    pairs = []
    # filling the pairs list with points that we have a good corner score
    for inrow, row in enumerate(validPoints):
        for incol, item in enumerate(row):
            if item:
                # I need x,y coords so col,row
                pairs.append([incol, inrow])

    # checking if the points are as they are supposed to be
    print('found {} points with cornerHarris method.'.format(len(pairs)))
    return pairs


def crosscheck_match(desc1, desc2):
    # simple matcher
    bf = cv.BFMatcher(cv.NORM_L2)

    # finding the matches of the first image according to second
    # and of the second image according to first
    matches1 = bf.match(desc1, desc2)
    matches2 = bf.match(desc2, desc1)

    crosschecked = []
    for match1 in matches1:
        kp1_indx_match1 = match1.queryIdx
        kp2_indx_match1 = match1.trainIdx

        for match2 in matches2:
            kp2_indx_match2 = match2.queryIdx
            kp1_indx_match2 = match2.trainIdx

            if kp1_indx_match1 == kp1_indx_match2 and kp2_indx_match1 == kp2_indx_match2:
                crosschecked.append(match1)

    return crosschecked


def panorama_ltr4to2_surf(img1, img2, img3, img4):
    surf = cv.xfeatures2d_SURF.create(10000)

    kp1, desc1 = surf.detectAndCompute(img1, None)
    kp2, desc2 = surf.detectAndCompute(img2, None)
    kp3, desc3 = surf.detectAndCompute(img3, None)
    kp4, desc4 = surf.detectAndCompute(img4, None)

    start_time = timeit.default_timer()
    # takes 30 sec (doesnt really crossckeck tho)
    # bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
    # match12 = bf.match(desc1, desc2)
    # match34 = bf.match(desc3, desc4)

    # takes 180 sec for the same input
    match12 = crosscheck_match(desc1, desc2)
    match34 = crosscheck_match(desc3, desc4)

    elapsed = timeit.default_timer() - start_time
    print('time using my matcher', elapsed)

    img12matches = cv.drawMatches(img1, kp1, img2, kp2, match12, None)
    img34matches = cv.drawMatches(img3, kp3, img4, kp4, match34, None)

    show(img12matches, 'img1 and img2 matches')
    show(img34matches, 'img3 and img4 matches')

    M12 = find_transform(match12, kp1, kp2)
    M34 = find_transform(match34, kp3, kp4)

    img12 = cv.warpPerspective(img2, M12, (int(img2.shape[1] * 1.5), int(img2.shape[0] * 1.5)))
    img12[0: img1.shape[0], 0: img1.shape[1]] = img1

    img34 = cv.warpPerspective(img4, M34, (int(img4.shape[1] * 1.5), int(img4.shape[0] * 1.5)))
    img34[0: img3.shape[0], 0: img3.shape[1]] = img3

    return img12, img34


def margin_top_left(img, d=(300, 300)): # (x,y) ==>> (col,row)
    M = np.array([[1, 0, d[0]], [0, 1, d[1]], [0, 0, 1]], dtype=np.float32)
    # returning an image with the right dimensions to fit the initial image
    # *2 because i want the image to be in the center(not touching the boundaries)
    img2 = cv.warpPerspective(img, M, (int(img.shape[1]+2*d[0]+150), int(img.shape[0]+2*d[1]+150)))
    return img2


# each time i need to call this function I have to place a margined image at img1
# and the UNMARGINED version of it
# on the bareimage
def panorama_ltr2to1_surf(img1, img2, bareimage=None):
    surf = cv.xfeatures2d_SURF.create()

    kp1, desc1 = surf.detectAndCompute(img1, None)
    kp2, desc2 = surf.detectAndCompute(img2, None)

    bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)

    match12 = bf.match(desc1, desc2)

    # img12matches = cv.drawMatches(img1, kp1, img2, kp2, match12, None)
    # show(img12matches, 'img1 and img2 matches')

    M12 = find_transform(match12, kp1, kp2)

    img12 = cv.warpPerspective(img2, M12, (int(img2.shape[1] * 2), int(img2.shape[0] * 2)))

    if bareimage is None:
        img12[0: img1.shape[0], 0: img1.shape[1]] = img1
    else:
        img12[300: bareimage.shape[0]+300, 300: bareimage.shape[1]+300] = bareimage

    return img12


def folder_list(f_name):
    folders = [f_name]
    path = []
    for folder in folders:
        files = os.listdir(folder)
        for file in files:
            path.append(os.path.join(folder, file))
    return path


def panorama_method1(img1, img2, img3, img4):

    # step1 images12
    # flipping the images to reverse panorama
    img1f = cv.flip(img1, 1)
    img2f = cv.flip(img2, 1)
    show(img2f, 'img2f')
    # now the valid matching is 21, 34
    # or the image scenery from left to like is 4 3 2 1
    # adding some margin to img2 in order when the panorama is made not to lose details
    # as well as to img3

    #step2 images12

    img2f_margin = margin_top_left(img2f)
    show(img2f_margin, 'img2f_margin')
    # creating the first level of panorama
    pano21 = panorama_ltr2to1_surf(img2f_margin, img1f, img2f)
    show(pano21, 'pano21')
    pano21 = cropping_with_margins(pano21)
    show(pano21, 'pano21cropped')

    #step1 images34
    img3_margin = margin_top_left(img3)
    pano34 = panorama_ltr2to1_surf(img3_margin, img4, img3)
    show(pano34, 'pano34')
    pano34 = cropping_with_margins(pano34)
    show(pano34, 'pano34cropped')

    #step2 images34
    pano43 = cv.flip(pano34, 1)
    show(pano43, 'pano34flipped')

    # in order to have the exact margin we have to ALWAYS margin with the default d
    # if we want to use the 3rd parameter in panorama_ltr2to1_surf

    # step3
    pano43_margin = margin_top_left(pano43)
    show(pano43_margin, 'pano43margin')
    panofinal_flipped = panorama_ltr2to1_surf(pano43_margin, pano21, pano43)
    show(panofinal_flipped, 'panofinal_flipped')

    # margin to make sure that the crop ( finding corners) will actually find all the corners
    panofinal_flipped = margin_top_left(panofinal_flipped)
    panofinal_flipped = cropping_with_margins(panofinal_flipped)
    show(panofinal_flipped, 'panofinal_flipped_cropped')

    # step4
    panofinal = cv.flip(panofinal_flipped, 1)
    show(panofinal, 'final')
    return panofinal


def panorama_method_basic(img1, img2, img3, img4):
    img5, img6 = panorama_ltr4to2_surf(img1, img2, img3, img4)
    img5 = crop(img5)
    img6 = crop(img6)
    img7 = panorama_ltr2to1_surf(img5, img6)
    return img7


pathrtl = folder_list('database')
pathltr = np.flip(pathrtl)

path = folder_list('river_bridge/2')  # path of my photos

# created 2 path lists to avoid confusion
# ill mainly use the left to right path list
# the right to left need different approach as for the panorama


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UNCOMMENT TO RUN FOR MY PHOTOS ~~~~~~~~~~~~~~~~~~~~~~
img1 = cv.imread(path[0])
img2 = cv.imread(path[1])
img3 = cv.imread(path[2])
img4 = cv.imread(path[3])

# downsizing the images by a factor of 2.5
img1 = cv.resize(img1, (int(img1.shape[1]/2.5), int(img1.shape[0]/2.5)))
img2 = cv.resize(img2, (int(img2.shape[1]/2.5), int(img2.shape[0]/2.5)))
img3 = cv.resize(img3, (int(img3.shape[1]/2.5), int(img3.shape[0]/2.5)))
img4 = cv.resize(img4, (int(img4.shape[1]/2.5), int(img4.shape[0]/2.5)))

panorama_final = panorama_method1(img1, img2, img3, img4)
show(panorama_final, 'finalpanorama')
cv.imwrite('final_panorama_bridge_surf.png', panorama_final)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UNCOMMENT TO RUN FOR DEFAULT PHOTOS ~~~~~~~~~~~~~~~~~~~~~~~~
# img1 = cv.imread(pathltr[0])
# img2 = cv.imread(pathltr[1])
# img3 = cv.imread(pathltr[2])
# img4 = cv.imread(pathltr[3])
#
# # basic_pano = panorama_method_basic(img1, img2, img3, img4)
# # show(basic_pano,'basic pano')
#
#
# panorama_final = panorama_method1(img1, img2, img3, img4)
# show(panorama_final, 'finalpanorama')
# # cv.imwrite('final_panorama_default_surf.png', panorama_final)

