import pandas as pd
import matplotlib.pyplot as plt
import sys

def get_usage():
    return "Usage: python quick_plot <csv-file>"

if len(sys.argv) < 2:
    raise Exception(get_usage())

df = pd.read_csv(sys.argv[1], index_col=0)

df.plot()
plt.show()