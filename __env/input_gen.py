import sys
import re
import math
import random
import operator
from collections import defaultdict

class colors:
    HEADER = '\033[95m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def err(message):
    print colors.BOLD + colors.FAIL + "ERROR" + colors.ENDC + ": " + message
    exit(1)

NUMBER = "-?[0-9]+([.][0-9]+)?(e[0-9]+)?"
VAR_NAME = "[a-zA-Z0-9_]+"

class Scope:
    def __init__(self):
        self.vars = {}

    def is_number(self, expr):
        return re.match(NUMBER, expr)

    def dependencies(self, expr):
        return [x for x in re.findall(VAR_NAME, expr) if not self.is_number(x)]

    def evaluate(self, expr):
        deps = self.dependencies(expr)
        deps.sort(lambda x, y : len(x) > len(y))

        for vname in deps:
            expr = expr.replace(vname, str(self.vars[vname].value()))

        try:
            return eval(expr)
        except Exception:
            err("Could not evaluate " + expr)

    def toposort(self):
        degree = defaultdict(int)
        rev_deps = defaultdict(list)

        for vname, var in self.vars.iteritems():
            for dep in var.dependencies():
                if not dep in self.vars:
                    err("Variable {} has undefined dependency {}".format(vname, dep))
                degree[vname] += 1
                rev_deps[dep].append(vname)

        order = []
        for vname in self.vars:
            if degree[vname] == 0:
                order.append(vname)

        for vname in order:
            for dep in rev_deps[vname]:
                degree[dep] -= 1
                if not degree[dep]:
                    order.append(dep)

        if len(order) != len(self.vars):
            err("Variable dependencies are cyclic")
        return order

class Number:
    TYPES = [ "int", "ll", "double" ]

    def __init__(self, scope, name, num_type, lower_bound, upper_bound):
        assert num_type in self.TYPES

        self.name = name
        self.scope = scope
        self.num_type = num_type
        self.deps = []

        self.lb_type = lower_bound[0]
        self.lb_value = lower_bound[1:]

        self.ub_type = upper_bound[-1]
        self.ub_value = upper_bound[:-1]

    def dependencies(self):
        return self.scope.dependencies(self.lb_value) + self.scope.dependencies(self.ub_value)

    def assign(self):
        lb = self.scope.evaluate(self.lb_value)
        ub = self.scope.evaluate(self.ub_value)

        if self.num_type == "double":
            self.assigned_value = random.uniform(lb, ub)
        else:
            if self.lb_type == '(':
                lb += 1
            if self.ub_type == ')':
                ub -= 1
            self.assigned_value = random.randint(math.ceil(lb), math.floor(ub))

    def value(self):
        return self.assigned_value


INTERVAL = "\s*([\[\(][^,]+),([^,]+[\]\)])\s*";
NUMBER_SPEC = "^#([\w\s]+)(int|ll|double)" + INTERVAL + "$"

def main():
    if len(sys.argv) < 2:
        err("No input spec provided.")

    scope = Scope()
    layout = []

    spec = open(sys.argv[1]).readlines()
    for line in spec:
        if line[0] != "#":
            layout.append(line)
            continue

        ns = re.search(NUMBER_SPEC, line)
        if ns:
            names, vtype, lower, upper = ns.groups()
            for vname in filter(None, names.split(" ")):
                if vname in scope.vars:
                    err("Variable \"{}\" was already defined.".format(vname))
                scope.vars[vname] = Number(scope, vname, vtype, lower, upper)

    eval_order = scope.toposort()
    for vname in eval_order:
        scope.vars[vname].assign()
        print vname, scope.vars[vname].value()

"""
    tmp = scope.vars["x"] = Number(scope, "x", "double", "[2", "4]")
    tmp.assign()
    tmp2 = scope.vars["y"] = Number(scope, "y", "int", "[x", "x^3]")
    print tmp2.dependencies()
    tmp2.assign()

    print scope.evaluate("-42.0123 + 22     + 101.3 * x^2")
    print scope.evaluate("-42.0123 + 22     + 101.3 * y^2")
    print scope.evaluate("x")
    print scope.evaluate("-42.0123 + 22     + 101.3 * y^2")
    print scope.evaluate("y")
    print scope.evaluate("y^2^3")
    print scope.evaluate("6^2")
    print scope.evaluate("2^2^3")
    print scope.evaluate("6^2 + 2^3^2")
"""

if __name__ == "__main__": main()
