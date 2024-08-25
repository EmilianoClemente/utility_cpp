# memo for check include dependency
import os
import re
import sys


def Extension(filename: str):
    pieces = filename.split(".")
    if len(pieces) != 2:
        return None
    extension = pieces[1]
    if extension == "cpp":
        return "cc"
    if extension == "cc":
        return "cc"
    if extension == "h":
        return "h"
    return None


def SearchIncludes(filename: str,
                   includeReference: list,
                   includeList: list = [],
                   systemHeaderList: list = [],
                   depth: int = 1):
    with open(filename, "r") as file:
        includes = re.findall("#include [<\"](.*)[\">]\n", file.read())
        for include in includes:
            in_ref = False
            for ref in includeReference:
                if include in ref:
                    include = ref
                    in_ref = True
                    break
            if not in_ref:
                systemHeaderList.append(include)
                continue
            if include in includeList:
                continue
            includeList.append(include)
            indent = ""
            for i in range(0, depth):
                indent = indent + "    "
            print(indent + include)
            try:
                SearchIncludes(include, includeReference, includeList,
                               systemIncludes, depth + 1)
            except OSError:
                print("Exception OSError opening " + filename +
                      " and searching " + include)
                raise OSError
    if depth == 1:
        print("    external dependency:")
        for i in systemHeaderList:
            print("        " + i)


Target = "./"
Headers = []
CC = []


for option in sys.argv:
    if option[0] == '-' and option[1] == 't':
        Target = option[2:]


for dirpath, dirnames, filenames in os.walk(Target):
    for fn in filenames:
        extension = Extension(fn)
        if extension is None:
            continue
        fn = os.path.join(dirpath, fn)
        if extension == "h":
            Headers.append(fn)
        elif extension == "cc":
            CC.append(fn)


for cc in CC:
    includes = []
    systemIncludes = []
    print(cc)
    SearchIncludes(cc, Headers, [])
