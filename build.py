import glob
import os
import re

def expand(m):
    print(f"Searching match {m[0]}")
    args = re.search(r"(\w+)\(((?:\w+\:.+,?)+)\)", m[0])
    tname = args[1]
    params = args[2].split(",")
    with open(f"./{tname.lower()}.ct", "r") as t:
        template = t.read()
    for p in params:
        a, b = p.split(":")
        template = template.replace(a.upper(), b)
    return template
for path in glob.glob("./*.c0"):
    with open(path, "r") as src:
        content = src.read()
    for t in glob.glob("./*.ct"):
        name = os.path.basename(t).split(".")[0]
        with open(t, "r") as th:
            template = th.read()
            content = content.replace("{{"+name+"}}", template)
    content = re.sub(r"\$(.*)\$", expand, content);
    with open(path.replace("c0", "c"), "w") as dest:
        dest.write(content)


