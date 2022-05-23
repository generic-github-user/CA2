import glob
import os

for path in glob.glob("./*.c0"):
    with open(path, "r") as src:
        content = src.read()
    for t in glob.glob("./*.ct"):
        name = os.path.basename(t).split(".")[0]
        with open(t, "r") as th:
            template = th.read()
            content = content.replace("{{"+name+"}}", template)
    with open(path.replace("c0", "c"), "w") as dest:
        dest.write(content)



