# initialize the number of epochs and batch size and etc.
NUM_EPOCHS = 12
BS = 16
RESIZE_NUM = 256
TRAIN_CSV = 'train.csv'
TEST_CSV = 'test.csv'
#from keras.utils import to_categorical
from keras.models import Sequential
from keras.layers import Dense, Flatten, Activation, BatchNormalization
from keras.layers import Convolution2D, MaxPool2D
import numpy as np
import pandas as pd
#import json
from sklearn.preprocessing import scale
import cv2
from keras.preprocessing.image import ImageDataGenerator
# construct the training image generator for data augmentation
aug = ImageDataGenerator(rotation_range=20, zoom_range=0.15,
    width_shift_range=0.2, height_shift_range=0.2, shear_range=0.15,
    horizontal_flip=True, fill_mode="nearest")

import pandas as pd
input_path = 'label_new1.csv'
with open(input_path,'r') as fff:
    a = pd.read_csv(input_path)

label = a['Finding Labels'].tolist()
files_path = a['Image Index'].tolist()

from sklearn.model_selection import train_test_split
x_train, x_test, y_train, y_test = train_test_split(files_path, label, test_size=0.3, random_state=42)
train_file = pd.DataFrame({'Label':y_train,'Path':x_train})
test_file = pd.DataFrame({'Label':y_test,'Path':x_test})
train_file.to_csv(TRAIN_CSV, header=False)
test_file.to_csv(TEST_CSV, header=False)


from sklearn.preprocessing import LabelBinarizer    
lb = LabelBinarizer()
lb.fit(list(label))

###
key_words = {'Effusion':1, 'Infiltration':0}

import cv2
#
#lb = LabelBinarizer()
def ones_array(image_array):
    return np.array([[[xs[0]/255.] for xs in item] for item in image_array.tolist()])


def csv_image_generator(inputPath, bs, lb, mode="train", aug=None):
    # open the CSV file for reading
    f = open(inputPath, "r")
    while True:
        # initialize our batches of images and labels
        images = []
        labels = []
        # keep looping until we reach our batch size
        while len(images) < bs:
            # attempt to read the next line of the CSV file
            line = f.readline()
            # check to see if the line is empty, indicating we have
            # reached the end of the file
            if line == "":
                # reset the file pointer to the beginning of the file
                # and re-read the line
                f.seek(0)
                line = f.readline()
                # if we are evaluating we should now break from our
                # loop to ensure we don't continue to fill up the
                # batch from samples at the beginning of the file
                if mode == "eval":
                    break
            # extract the label and construct the image
            line = line.strip().split(",")
            label = line[1]
            if label == "Infiltration" and mode == "train":
                randi = np.random.random()
                if randi < 0.5:
                    continue
            image_path = line[-1]
            image = cv2.imread("Moredata/{}".format(image_path))
            image = cv2.resize(image,(RESIZE_NUM, RESIZE_NUM))
            image = ones_array(image)
            # update our corresponding batches lists
            images.append(image)
            labels.append(label)
            # one-hot encode the labels
        labels = lb.transform(np.array(labels))
        #labels = to_categorical(labels)
        # if the data augmentation object is not None, apply it
        #if aug is not None:
        #    (images, labels) = next(aug.flow(np.array(images),
        #        labels, batch_size=bs))
        # yield the batch to the calling function
        yield (np.array(images), labels)


trainGen = csv_image_generator(TRAIN_CSV, BS, lb,
    mode="train", aug=aug)

testGen = csv_image_generator(TEST_CSV, BS, lb,
    mode="test", aug=None)

model = Sequential()
model.add(Convolution2D(filters=64, kernel_size=(5, 5), strides=(1, 1), input_shape=[RESIZE_NUM, RESIZE_NUM, 1]))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(MaxPool2D())
model.add(Convolution2D(filters=128, kernel_size=(5, 5), strides=(1, 1)))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(MaxPool2D())
model.add(Convolution2D(filters=256, kernel_size=(5, 5), strides=(1, 1)))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(MaxPool2D())
model.add(Convolution2D(filters=512, kernel_size=(5, 5), strides=(1, 1)))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(MaxPool2D())
model.add(Convolution2D(filters=1024, kernel_size=(5, 5), strides=(1, 1)))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(Flatten())
model.add(Dense(4096, activation="relu"))
model.add(Dense(1, activation="sigmoid"))

model.compile(optimizer="adam", loss="binary_crossentropy", metrics=["binary_accuracy"])
NUM_TRAIN_IMAGES = len(train_file)
NUM_TEST_IMAGES = len(test_file)
# train the network
#num_t_images = all_img / batch size
from keras.callbacks import ReduceLROnPlateau
reduce_lr = ReduceLROnPlateau(monitor='val_loss', patience=1, mode='auto')
H = model.fit_generator(trainGen,
    steps_per_epoch=NUM_TRAIN_IMAGES // BS,
    validation_data=testGen,
    validation_steps=NUM_TEST_IMAGES // BS,
    callbacks=[reduce_lr],
    epochs=NUM_EPOCHS,
    use_multiprocessing=True)

##evaluate the model
predIdxs = model.predict_generator(testGen,
    steps=(NUM_TEST_IMAGES // BS) + 1,
    use_multiprocessing=True)

from sklearn.metrics import roc_auc_score
def recall_precision(y_test,y_score,threshold = 0.5):
    true_pos = 0
    recall_num = 0
    precision_num = 0
    for i in range(len(y_score)):
        score = y_score[i]
        true = y_test[i]
        if score > threshold:
            precision_num += 1
            if true == 1:
                true_pos += 1
        if true == 1:
            recall_num += 1
    precision = true_pos/float(precision_num)
    recall = true_pos/float(recall_num)
    return recall, precision

y_true = lb.transform(y_test)
y_score = [item[0] for item in predIdxs[:NUM_TEST_IMAGES]]
print('auc is:',roc_auc_score(y_true,y_score))
for rand in range(1,99):
    try:
        recall, precision=recall_precision(y_true,y_score,rand/100)
        f_score = 2*precision*recall/(precision+recall)
        print("P:{:.5f}, R:{:.5f}, F:{:.5f}, threshold:{}".format(precision,recall,f_score,rand/100))
    except:
        continue
