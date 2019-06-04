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

VAR_NAME_RE = re.compile("[a-zA-Z0-9_]+")
NUMBER_RE = re.compile("-?[0-9]+([.][0-9]+)?(e[0-9]+)?")
INTERVAL = "\s*([\[\(][^,]+),([^,]+[\]\)])\s*"

class Scope:
    def __init__(self):
        self.vars = {}

    def is_number(self, expr):
        return NUMBER_RE.match(expr)

    def dependencies(self, expr):
        return [x for x in VAR_NAME_RE.findall(expr) if not self.is_number(x)]

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

        inx = 0
        while inx < len(order):
            vname = order[inx]
            for dep in rev_deps[vname]:
                degree[dep] -= 1
                if not degree[dep]:
                    order.append(dep)
            inx += 1

        if len(order) != len(self.vars):
            err("Variable dependencies are cyclic")
        return order

class Number:
    TYPES = [ "int", "ll", "double" ]
    SPEC_RE = re.compile("#([\w\s]+)(int|ll|double)" + INTERVAL)

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
        pretty_bounds = self.lb_type + str(lb) + ", " + str(ub) + self.ub_type

        if self.num_type == "double":
            if lb > ub:
                err("Double \"{}\" bounded by empty range {}".format(self.name, pretty_bounds))
            self.assigned_value = random.uniform(lb, ub)
        else:
            if self.lb_type == '(':
                lb += 1e-9
            lb = math.ceil(lb)
            if self.ub_type == ')':
                ub -= 1e-9
            ub = math.floor(ub)

            if lb > ub:
                err("Integer \"{}\" bounded by empty range {}".format(self.name, pretty_bounds))
            self.assigned_value = random.randint(lb, ub)

        return self.assigned_value

    def value(self):
        return self.assigned_value

class NumberVector:
    TYPES = [ "ints", "lls", "doubles" ]
    SPEC_RE = re.compile("#([\w\s]+)(ints|lls|doubles)(.+)" + INTERVAL)

    def __init__(self, scope, name, vec_type, length, lower_bound, upper_bound):
        assert vec_type in self.TYPES

        self.name = name
        self.scope = scope
        self.vec_type = vec_type
        self.numbers = Number(scope, "__" + name, vec_type[:-1], lower_bound, upper_bound)
        self.length = length

    def dependencies(self):
        return self.numbers.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.numbers.assign() for i in range(0, self.assigned_length)]

    def value(self):
        return self.assigned_value

def main():
    if len(sys.argv) < 2:
        err("No input spec provided.")

    scope = Scope()
    layout = []

    spec = [x.strip() for x in open(sys.argv[1]).readlines() if x.strip()]
    for line in spec:
        if line[0] != "#":
            layout.append(line)
            continue

        ns = Number.SPEC_RE.match(line)
        if ns:
            names, vtype, lower, upper = ns.groups()
            for vname in filter(None, names.split(" ")):
                if vname in scope.vars:
                    err("Variable \"{}\" was already defined".format(vname))
                scope.vars[vname] = Number(scope, vname, vtype, lower, upper)
            continue

        nv = NumberVector.SPEC_RE.match(line)
        if nv:
            names, vtype, length, lower, upper = nv.groups()
            for vname in filter(None, names.split(" ")):
                if vname in scope.vars:
                    err("Variable \"{}\" was already defined".format(vname))
                scope.vars[vname] = NumberVector(scope, vname, vtype, length, lower, upper)
            continue

        err("Could not parse variable spec \"{}\"".format(line))

    eval_order = scope.toposort()
    for vname in eval_order:
        scope.vars[vname].assign()

    output = []
    for line in layout:
        vnames = VAR_NAME_RE.findall(line)

        nospec = filter(lambda vn: vn not in scope.vars, vnames)
        if nospec:
            err("No spec provided for variables {}".format(nospec))

        elts = [scope.vars[vn] for vn in vnames]
        vectors = filter(lambda e : isinstance(e, NumberVector), elts)
        if vectors:
            if vectors != elts:
                err("Line \"{}\" mixes vectors and primitives".format(line))

            lengths = set([e.assigned_length for e in elts])
            if len(lengths) > 1:
                err("Line \"{}\" contains columns with differing lengths".format(line))
            length = next(iter(lengths))

            rows = [" ".join([str(v.value()[i]) for v in vectors]) for i in range(0, length)]
            delim = "\n" if len(vectors) > 1 else " "
            output.append(delim.join(rows))
        else:
            output.append(" ".join([str(scope.vars[vn].value()) for vn in vnames]))

    for line in output:
        print line

if __name__ == "__main__": main()
