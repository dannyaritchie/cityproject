import numpy as np
import scipy.stats as stats
import json
import pandas as pd
import csv
import matplotlib.pyplot as plt
#a function to load nd array from json file
def load_tester(path):
    with open(path) as f:
        data = json.load(f)
    return np.asarray(data)
#use abovefunction
data = load_tester("../c++/data/newdata/numbers/numbers.txt")
#this expects a text file with each row a list of the values of the parameters used corresonding to the n-d data in above
a=[]
with open("../c++/data/newdata/numbers/labels.txt", newline ='') as cvsfile:
        reader=csv.reader(cvsfile, delimiter = ',')
        for row in reader:
            a.append(row)

number = a[0]
velocity = a[1]
frames = a[2]
post = a[3]
#to do- names for parameters as above- could be loaded from file
phaseType = ["No Possession", "Possession", "Frame Jump"]

#this reshapes the ndarray to a 2-d array with final dimension things we might plot against
maj_dim = 1
for dim in data.shape[:-1]:
            maj_dim = maj_dim*dim
new_dims = (maj_dim)
data = data.reshape(new_dims, 3)#3 corresponds to fact we had 3 input dimensions

print(data)

ratio = []
for i in data:
    ratio.append([(i[1])])
print(ratio)
u = np.array(ratio)
print(u)
midx=pd.MultiIndex.from_product([number, velocity, frames, post], names = ['Number','Velocity','Frame','Post'])
df = pd.DataFrame(data = ratio, index = midx, columns = ['aFrequancy'])
print(df)
print(df.xs(('2','1.4','25'), level=('Number','Velocity','Post'),axis=0))
for i in number:
    ax=plt.gca()
    myLegend=[]
    for j in velocity:
        df.xs((i,j,'25'), level=('Number','Velocity','Post'),axis=0).plot(kind = 'line',ax=ax);
        myLegend.append(str(i) + ' Defenders, above ' + str(j) + 'ms-1.')
        ax.legend(myLegend)
    #plt.show();
    plt.savefig('data/posnum'+str(i) + 'j25frames.png');
    plt.close()
