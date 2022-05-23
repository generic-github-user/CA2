import numpy as np
import matplotlib.pyplot as plt
import sys
import time

# TODO
time.sleep(1)
def writelog(content):
    with open("pylog.txt", "a") as log:
        log.write(content+"\n")
writelog('\n'*3)
path = sys.argv[1]
#sys.stdout.write(path)
writelog(path)
with open(path, "r") as f:
    text = f.read()
#    print(text)
#    sys.stdout.write(text)
    writelog(text)
    data = np.fromstring(text, dtype=int, sep=",")
    
    writelog("Generating plot")
    plt.style.use("ggplot")
    fig, ax = plt.subplots()
    ax.imshow(data.reshape(30, 30, order="C"), cmap="GnBu")
    fig.savefig("./ca_plot.png")
    plt.close(fig)

