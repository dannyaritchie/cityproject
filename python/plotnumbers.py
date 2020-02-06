import numpy as np
import scipy.stats as stats
import json

def load_tester(path):
    with open(path) as f:
        data = json.load(f)
    return np.asarray(data)

data = load_tester("poo.json")
print(data.shape)
