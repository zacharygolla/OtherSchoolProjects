from sklearn.datasets import load_svmlight_file
from sklearn.model_selection import cross_val_score
from sklearn import linear_model
from sklearn import svm

X_train, y_train = load_svmlight_file("train.dat")
X_test, y_test = load_svmlight_file("test.dat")

clf_linear = svm.SVC(kernel='linear')

for i in range(2, 6):
    scores = cross_val_score(clf_linear, X_train, y_train, cv=i)
    scores_sum = sum(scores)
    print(scores_sum/i)




