import sys
import subprocess
import re
import os
from os.path import isfile, join

from util import canonical_path

class colors:
    HEADER = '\033[95m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'

    ENDC = '\033[0m'

class icons:
    PASS  = colors.GREEN + 'Y' + colors.ENDC
    FAIL  = colors.RED + 'N' + colors.ENDC
    MAYBE = colors.YELLOW + '?' + colors.ENDC

def err(message):
    print colors.BOLD + colors.RED + "ERROR" + colors.ENDC + ": " + message
    exit(1)

def warn(message):
    print colors.BOLD + colors.HEADER + "WARN" + colors.ENDC + ": " + message

def bold(message):
    print colors.BOLD + message + colors.ENDC

def run(cmd):
    return subprocess.call(['/bin/bash', '-c', cmd])

def build_with_compiler_warnings(filepath, binary_path):
    return run("g++ -std=c++17 -O2 -fconcepts -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align " + filepath + " -o " + binary_path)

def build_with_instrumentation(filepath, binary_path):
    return run("g++ -std=c++17 -O0 -fconcepts -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector -g " + filepath + " -o " + binary_path)

def build_and_run(filepath):
    filepath = canonical_path(filepath)
    binary_path = filepath[:-4]

    bold("Compiling " + filepath + " ...")
    if build_with_compiler_warnings(filepath, binary_path):
        err("Compilation failed")

    if build_with_instrumentation(filepath, binary_path):
        err("Compilation failed")

    dirname = os.path.dirname(filepath)
    test_dir = dirname + "/tests"

    sample_input = re.compile("sample.*[.]in")
    sample_files = [canonical_path(test_dir + "/" + f, "")[:-3] for f in os.listdir(test_dir) if sample_input.match(f)]

    failed = []
    summary = ""
    first_line_has_multiple_tokens = False

    bold("\nRunning samples...")
    for sample in sorted(sample_files):
        first_line = open("{}.in".format(sample), 'r').read().split("\n")[0]
        if (len(first_line.strip().split(" ")) > 1):
            first_line_has_multiple_tokens = True

        if run("{} < {}.in > {}.my".format(binary_path, sample, sample)):
            err("Run-time error on " + sample)

        print "\nExpected output on {}:".format(sample)
        has_output = True
        try:
            out = open("{}.out".format(sample), 'r').read()
            print out
        except IOError:
            print "File \'{}.out\' not found\n".format(sample)
            has_output = False

        my = open("{}.my".format(sample), 'r').read()
        print "Your output on {}:".format(sample)
        print my

        if not has_output:
            warn(sample + " failed (missing \'{}.out\')".format(sample))
            failed.append(sample)
            summary += icons.MAYBE
            continue

        proc = subprocess.Popen("diff {}.out {}.my".format(sample, sample),\
                shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        o,e = proc.communicate()

        if o.decode('ascii') == "":
            bold(sample + " passed")
            summary += icons.PASS
        else:
            proc = subprocess.Popen("diff -b {}.out {}.my".format(sample, sample),\
                    shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            o,e = proc.communicate()
            if o.decode('ascii') == "":
                bold(sample + " passed (ignored whitespace)")
                summary += icons.PASS
            else:
                my_output = open("{}.my".format(sample)).readlines()
                my_tokens = [token for line in my_output for token in line.split()]

                exp_output = open("{}.out".format(sample)).readlines()
                exp_tokens = [token for line in exp_output for token in line.split()]

                match = True
                if len(my_output) != len(exp_output):
                    match = False
                else:
                    for i in xrange(0, len(my_output)):
                        if my_output[i] == exp_output[i]:
                            continue
                        try:
                            my_val  = float( my_output[i])
                            exp_val = float(exp_output[i])
                            if abs(my_val - exp_val) / max(1, abs(exp_val)) > 1e-9:
                                raise ValueError
                        except ValueError:
                            match = False
                            break

                if match:
                    bold(sample + " passed (ignored whitespace, float tolerance 1e-9)")
                    summary += icons.PASS
                else:
                    warn(sample + " failed")
                    failed.append(sample)
                    summary += icons.FAIL

    if failed:
        print
        print summary + ": FAILED samples " + str(failed)
    elif not sample_files:
        print
        warn("No sample inputs found in {}".format(test_dir))
    else:
        print
        print summary + colors.BOLD + ": ALL SAMPLES OK!" + colors.ENDC

    if first_line_has_multiple_tokens:
        warn("First line of input has multiple tokens. Check their order carefully.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        err("Program expected as argument")

    build_and_run(sys.argv[1])
