import random
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from itertools import count 
from matplotlib.animation import FuncAnimation
import csv

plt.style.use('fivethirtyeight')



index = count()


def liveplotter2d(title,xlabel,ylabel):

    x_vals = []
    y_vals = []
    with open('/home/jetson/Programs/cpp/qvt/build/'+title+'.csv','r') as csv_file:
        csv_reader = csv.reader(csv_file)
        for line in csv_reader:
            x_vals.append(line[0])
            y_vals.append(line[1])

    plt.cla()
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.plot(x_vals,y_vals)
    plt.legend()

    

    
    
    plt.tight_layout()
    plt.show(block=False)
    plt.pause(.001)

