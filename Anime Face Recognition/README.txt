Anime Face Recognition  -- How to use --

1. face_recognition.py
This is a code that reads csv file and anime face images, and turn images to gray scale.
Then, split images and names into training data and testing data.
Finally, AI predicts which character is in the test image using k-neighbors classifier (KNN).
You can see that AI can identify characters with about 75% accuracy.
With more beautiful images, AI would identify them with higher acuuracy.

2. classified.csv
This file includes 4 elements, id, Norman, Emma, and Ray.
The element id is the number of images. For example, the first image is named "0.jpg".
Norman, Emma, and Ray are the main characters in "The Promised Neverland".
I classified which character is in each image by hand.
If the character exists, the value is "1", otherwise "0".

3. anime_img > 0.jpg, 1.jpg, ..., 99.jpg
Norman, Emma, and Ray exist in some images randomly.
Note that each image is 600px * 600px. All images must be the same size.
/* These images' copyright belongs to "The Promised Neverland". I just took them on the Internet. */
