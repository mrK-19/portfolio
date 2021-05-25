import cv2
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sklearn
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier

# make a histogram
targets = pd.read_csv("classified.csv")
images = np.empty((0, 256), int)
for i in range(100):
    jpg = f"anime_img/{i}.jpg"
    img = cv2.imread(jpg, cv2.IMREAD_GRAYSCALE)
    hist = np.histogram(img.ravel(), 256, [0, 256])
    images = np.append(images, np.array([hist[0]]), axis = 0)

names = ["Norman", "Emma", "Ray"]

for name in names:
    # separate data into train data and test data
    X_train, X_test, y_train, y_test = train_test_split(images, targets[name], random_state = 0, train_size = 80)

    # execute machine learning
    knn = KNeighborsClassifier(n_neighbors = 5)
    knn.fit(X_train, y_train)

    # show correct rate
    print("\n" + name)
    y_pred = knn.predict(X_test)
    print(y_pred)
    print(y_test)
    print(np.mean(y_pred == y_test))
