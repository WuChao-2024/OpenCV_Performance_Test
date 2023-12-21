import numpy as np

def ConfidenceInterval(dataSet, z_score):
    mean = np.mean(dataSet)
    std_dev = np.std(dataSet, ddof=1)
    half_width = z_score * std_dev / np.sqrt(len(dataSet))
    return mean, half_width