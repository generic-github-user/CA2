import glob
import os
import re
import datetime
import sys

def ts():
    t = datetime.datetime.now()
#    timestamp = t.strftime("%m/%d/%Y, %H:%M:%S")
    timestamp = t.strftime("%m/%d/%Y")
    return timestamp
print("Running build script")
def expand(m):
    print(" "*8+f"Searching match {m[0]}")
    args = re.search(r"(\w+)\(((?:\w+\:.+,?)+)\)", m[0])
    tname = args[1]
    params = args[2].split(",")
    tpath = glob.glob(f"./**/{tname.lower()}.ct")[0]
    with open(tpath, "r") as t:
        template = t.read()
    for p in params:
        a, b = p.split(":")
        template = template.replace(a.upper(), b)
    t = datetime.datetime.now()
    timestamp = t.strftime("%m/%d/%Y, %H:%M:%S")
    template = f"/* Imported from {tpath} at {timestamp} */ \n" + template
    return template

#for path in glob.glob("./*/*.c0")+glob.glob("./*.c0"):
path = sys.argv[1]
print(" "*4+f"Processing source file {path}")
with open(path, "r") as src:
    content = src.read()
for t in glob.glob("./*/*.ct"):
    name = os.path.basename(t).split(".")[0]
    with open(t, "r") as th:
        template = th.read()
        template = f"/* Imported from {t} at {ts()} */ \n" + template
        content = content.replace("{{"+name+"}}", template)
content = re.sub(r"\$(.*)\$", expand, content);

content = "/* This is a content file generated from a source (.c0) file; you should edit that file instead */ \n" + content
content = f"/* Generated from {path} at {ts()} */ \n" + content

with open(path.replace("c0", "c"), "w") as dest:
    dest.write(content)
print("Done")

