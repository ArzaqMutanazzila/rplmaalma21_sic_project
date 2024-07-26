from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import LabelBinarizer
import pandas as pd

dataset_filename = "dataset.csv"

df = pd.read_csv(dataset_filename)

X = df[["ambient_c", "object_c"]]
y = df[["sakit"]]

lb = LabelBinarizer()
y = lb.fit_transform(y)
y = y.ravel()

model = LogisticRegression()
model.fit(X, y)