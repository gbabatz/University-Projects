# Description

Tools used:
  - Python
  - PyCharm
  - Library OpenCV

### HW1
  Using an image of cells, the code performs:
  1. Counting of cells.
  2. Computing of cell size (in pixels).
  3. Computing of mean pixel value contained inside specific bounding boxes in a way that 
computation speed is invariant of cell size.

### HW2
  Algorithm that creates a panorama effect using 4 input images with the help of SIFT and SURF methods.
  Some of the functions (e.g. crosschecking) are custom implementations of opencv library functions.
  The project required to act on provided images but also on personal images.
  To reduce the size of the file I left only the final panorama using SIFT, for both the default and my images, and the minimum (4) initial images needed to generate a panorama.

### HW3
  Having a database of images (not included), the code performs multi-class classification.
  1. Generation of visual vocabulary based on Bag of Visual Words model (BOVW), using K-Means algorithm.
  2. Extraction of a descriptor for every train image based on BOVW, using the generated vocabulary of step 1.
  3. Classification of images, using step 2, using k-NN and SVM (one-vs-all) algorithms.
  4. Evaluation of the accuracy of the system.


*Images are based on this dataset: http://www.vision.caltech.edu/Image_Datasets/Caltech256/*
