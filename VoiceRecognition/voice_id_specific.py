import numpy as np
import librosa
import librosa.display
import os
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn import svm

speakers = {"kana":0, "ayana":1, "miku":2, "ayane":3, "inori":4}

# return feature of voice data
def get_feat(file_name):
    a, sr = librosa.load(file_name)
    y = librosa.feature.mfcc(y=a, sr=sr)  # get MFCC
    return y

# return combination of feature and label(speaker)
def get_data(dir_name):
    data_X = []
    data_y = []
    for file_name in sorted(os.listdir(path=dir_name)):
        print(f"read: {file_name}")
        speaker = file_name[0:file_name.index('_')]
        data_X.append(get_feat(os.path.join(dir_name, file_name)))
        data_y.append((speakers[speaker], file_name))
    return (data_X, data_y)

data_X, data_y = get_data("voiceset")

# split data into train data and test data
train_X, test_X, train_y, test_y = train_test_split(data_X, data_y, random_state=0)

# split data at each time into feature
def split_feat(data_X, data_y):
    data_X2 = []
    data_y2 = []
    for X, y in zip(data_X, data_y):
        X2 = X.T
        y2 = np.array([y[0]] * X.shape[1])
        data_X2.append(X2)
        data_y2.append(y2)
    data_X2 = np.concatenate(data_X2)
    data_y2 = np.concatenate(data_y2)
    return (data_X2, data_y2)

train_X2, train_y2 = split_feat(train_X, train_y)

# learning
clf = svm.SVC(gamma=0.0001, C=1)
clf.fit(train_X2, train_y2)

# classification
def my_predict(X):
    result = clf.predict(X.T)
    return np.argmax(np.bincount(result))



# read voice data you want to identify
while True:
    print("input filename = ")
    f = input()
    a, sr = librosa.load("voiceset/" + f + ".wav")

    # get MFCC
    test_data = librosa.feature.mfcc(y=a, sr=sr)

    # judge by AI
    actual = my_predict(test_data)

    # output result
    for key, value in speakers.items():
        if actual == value:
            print("speaker = " + key)

    # continue?
    print("Do you want to continue speaker identification? [y/n]")
    choice = input()
    if choice == "n":
        break
