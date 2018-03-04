import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import pandas as pd
import sys

df = pd.read_csv(sys.argv[1])
pdf = PdfPages(sys.argv[1].rstrip('.csv')+'plots.pdf')

plot_num = 1
for col in df:
    data = df[col]

    # Global data plot
    plt.figure(0)
    plt.plot(data)

    # Column data
    plt.figure(plot_num)
    plot_num = plot_num + 1
    plt.plot(data)
    plt.title(data.name)

plt.figure(0)
plt.title("ALL DATA")

for n in range(plot_num):
    plt.figure(n)
    plt.savefig(pdf, format='pdf')

pdf.close()