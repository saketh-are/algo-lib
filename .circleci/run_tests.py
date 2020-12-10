import re
import os
import sys

from util import canonical_path
from expand_dependencies import expand
from test_harness import build_and_run

def run_tests(infile, clib_root):
    infile = canonical_path(infile)

    dirname = os.path.dirname(infile)
    basename = os.path.basename(infile)
    expanded_file = dirname + "/." + basename

    expand(infile, expanded_file, clib_root)

    build_and_run(expanded_file)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Path to source file expected as argument"
        exit(1)

    clib_root = "~/clib/"
    if len(sys.argv) >= 3:
        clib_root = sys.argv[2]

    run_tests(sys.argv[1], clib_root)
