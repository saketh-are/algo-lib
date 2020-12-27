import re
import os
import sys

from util import canonical_path

def expand(infile, outfile, clib_root = "~/clib/"):
    deps = {}
    file_contents = {}
    unique_files = [ infile ]

    dependency_re = re.compile("//\s*{{{(.*)}}}\s*")

    inx = 0
    while inx < len(unique_files):
        fpath = unique_files[inx]
        inx += 1

        with open(fpath) as f:
            lines = f.readlines()

            deps[fpath] = []
            file_contents[fpath] = lines

            for line in lines:
                match = dependency_re.match(line)
                if match:
                    dep_fpath = canonical_path(clib_root + match.group(1).strip())
                    deps[fpath].append(dep_fpath)
                    if dep_fpath not in unique_files:
                        unique_files.append(dep_fpath)

    write_order = []

    while len(write_order) < len(unique_files):
        appended = False

        for fpath in unique_files:
            if fpath in write_order:
                continue

            can_write = True
            for dep in deps[fpath]:
                if dep not in write_order:
                    can_write = False

            if can_write:
                write_order.append(fpath)
                appended = True

        if not appended:
            print "Encountered a cyclic dependency!"
            exit(1)

    with open(outfile, 'w') as outf:
        for fpath in write_order:
            outf.writelines(file_contents[fpath])
            outf.write("\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Path to source file expected as argument"
        exit(1)

    if len(sys.argv) < 3:
        print "Output filepath expected as argument"
        exit(1)

    clib_root = "~/clib/"
    if len(sys.argv) >= 4:
        clib_root = sys.argv[3]

    expand(sys.argv[1], sys.argv[2], clib_root)

