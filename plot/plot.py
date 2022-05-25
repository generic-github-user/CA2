import numpy as np
import matplotlib.pyplot as plt
import sys
import time

# TODO
def writelog(content):
    with open("pylog.txt", "a") as log:
        log.write(content+"\n")
writelog('\n'*3)
path = sys.argv[1]
#sys.stdout.write(path)
writelog(path)
with open(path, "r") as f:
    text = f.readlines()
    print(text)
#    sys.stdout.write(text)
    writelog(text[0])
    data = np.fromstring(text[1], dtype=int, sep=",")
    axes = text[0].split(",")
    shape = tuple(int(a.split(":")[1]) for a in axes)
    print(shape)
    
    writelog("Generating plot")
    plt.style.use("ggplot")
    fig, ax = plt.subplots()
    plt_content = data.reshape(shape, order="C")
    if len(shape) == 1:
        plt.plot(plt_content)
    elif len(shape) == 2:
        ax.imshow(plt_content, cmap="GnBu")
    fig.savefig("./ca_plot.png")
    plt.close(fig)

