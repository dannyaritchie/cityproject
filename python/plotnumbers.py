import numpy as np
import scipy.stats as stats
import json
import pandas as pd
import csv

def load_tester(path):
    with open(path) as f:
        data = json.load(f)
    return np.asarray(data)

data = load_tester("../c++/data/newdata/numbers/numbers.txt")
a=[]
with open("../c++/data/newdata/numbers/labels.txt", newline ='') as cvsfile:
        reader=csv.reader(cvsfile, delimiter = ',')
        for row in reader:
            a.append(row)

        number = a[0]
        velocity = a[1]
        frames = a[2]
        post = a[3]
print(frames)
#number,velocity,frames,post = np.genfromtxt("../c++/data/newdata/numbers/labels.txt", delimiter = ",",unpack = 'True')
#data = load_tester("poo.txt")

maj_dim = 1
for dim in data.shape:
            maj_dim = maj_dim*dim
new_dims = (maj_dim)
data = data.reshape(new_dims)

midx=pd.MultiIndex.from_product([number, velocity, frames, post], names = ['Number','Velocity','Frame','Post'])
df = pd.DataFrame(data = data, index = midx, columns = ['number'])
print(df)
print(df.xs(('2','1.4','25'), level=('Number','Velocity','Post'),axis=0))
df.xs(('2','1.4','25'), level=('Number','Velocity','Post'),axis=0).plot();
