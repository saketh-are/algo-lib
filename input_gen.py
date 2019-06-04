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

VAR_NAME = "[a-zA-Z0-9_]+"
NUMBER = "-?[0-9]+([.][0-9]+)?(e[0-9]+)?"
INTERVAL = "\s*([\[\(][^,]+),([^,]+[\]\)])\s*";

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
    TYPES = [ "int", "float" ]
    SPEC = "([\w\s]+)(int|float)" + INTERVAL

    def __init__(self, scope, name, num_type, lower_bound, upper_bound):
        assert num_type in self.TYPES

        self.name = name
        self.scope = scope
        self.num_type = num_type

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

        if self.num_type == "float":
            if lb > ub:
                err("Float \"{}\" bounded by empty range {}".format(self.name, pretty_bounds))
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

class String:
    TYPES = [ "string" ]
    SPEC = "([\w\s]+)(string)(.+)(\[.*\])"

    def __init__(self, scope, name, str_type, length, alphabet):
        assert str_type in self.TYPES

        self.name = name
        self.scope = scope
        self.str_type = str_type
        self.length = length
        self.charset = [chr(ch) for ch in xrange(0, 256) if re.match(alphabet, chr(ch))]

    def dependencies(self):
        return self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = "".join([random.choice(self. charset) for i in xrange(0, self.assigned_length)])
        return self.assigned_value

    def value(self):
        return self.assigned_value

class NumberVector:
    TYPES = [ "ints", "floats" ]
    SPEC = "([\w\s]+)(ints|floats)(.+)" + INTERVAL

    def __init__(self, scope, name, vec_type, length, lower_bound, upper_bound):
        assert vec_type in self.TYPES

        self.name = name
        self.scope = scope
        self.vec_type = vec_type
        self.numbers = Number(scope, "elements of " + name, vec_type[:-1], lower_bound, upper_bound)
        self.length = length

    def dependencies(self):
        return self.numbers.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.numbers.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

class NumberGrid:
    TYPES = [ "intss", "floatss" ]
    SPEC = "([\w\s]+)(intss|floatss)(.+)%(.+)" + INTERVAL

    def __init__(self, scope, name, grid_type, rows, cols, lower_bound, upper_bound):
        assert grid_type in self.TYPES

        self.name = name
        self.scope = scope
        self.grid_type = grid_type
        self.row_vectors = NumberVector(scope, "rows of " + name, grid_type[:-1], cols, lower_bound, upper_bound)
        self.length = rows

    def dependencies(self):
        return self.row_vectors.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.row_vectors.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

class StringVector:
    TYPES = [ "strings" ]
    SPEC = "([\w\s]+)(strings)(.+)%(.+)(\[.*\])"

    def __init__(self, scope, name, vec_type, length, str_length, alphabet):
        assert vec_type in self.TYPES

        self.name = name
        self.scope = scope
        self.vec_type = vec_type
        self.strings = String(scope, "elements of " + name, "string", str_length, alphabet)
        self.length = length

    def dependencies(self):
        return self.strings.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.strings.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

def main():
    if len(sys.argv) < 2:
        err("No input spec provided.")

    scope = Scope()
    layout = []

    def parse_vnames(names):
        vnames = filter(None, names.split(" "))
        for vname in vnames:
            if vname in scope.vars:
                err("Variable \"{}\" was already defined".format(vname))
        return vnames

    spec = [x.strip() for x in open(sys.argv[1]).readlines() if x.strip()]
    for line in spec:
        if "#" not in line:
            layout.append(line)
            continue

        cloc = line.index("#")
        if cloc > 0:
            layout.append(line[0:cloc])
        line = line.replace("#", "", 1)

        ng = re.match(NumberGrid.SPEC, line)
        if ng:
            names, vtype, rows, cols, lower, upper = ng.groups()
            for vname in parse_vnames(names):
                scope.vars[vname] = NumberGrid(scope, vname, vtype, rows, cols, lower, upper)
            continue

        nv = re.match(NumberVector.SPEC, line)
        if nv:
            names, vtype, length, lower, upper = nv.groups()
            for vname in parse_vnames(names):
                scope.vars[vname] = NumberVector(scope, vname, vtype, length, lower, upper)
            continue

        ns = re.match(Number.SPEC, line)
        if ns:
            names, vtype, lower, upper = ns.groups()
            for vname in parse_vnames(names):
                scope.vars[vname] = Number(scope, vname, vtype, lower, upper)
            continue

        sv = re.match(StringVector.SPEC, line)
        if sv:
            names, vtype, length, str_length, alphabet = sv.groups()
            for vname in parse_vnames(names):
                scope.vars[vname] = StringVector(scope, vname, vtype, length, str_length, alphabet)
            continue

        ss = re.match(String.SPEC, line)
        if ss:
            names, vtype, length, alphabet = ss.groups()
            for vname in parse_vnames(names):
                scope.vars[vname] = String(scope, vname, vtype, length, alphabet)
            continue

        err("Could not parse variable spec \"{}\"".format(line))

    eval_order = scope.toposort()
    for vname in eval_order:
        scope.vars[vname].assign()

    output = []
    for line in layout:
        vnames = re.findall(VAR_NAME, line)

        nospec = filter(lambda vn: vn not in scope.vars, vnames)
        if nospec:
            err("No spec provided for variables {}".format(nospec))

        elts = [scope.vars[vn] for vn in vnames]

        grids = filter(lambda e : isinstance(e, NumberGrid), elts)
        if grids:
            if len(elts) > 1:
                err("2-D vectors must appear on their own line: \"{}\"".format(line))
            output.append("\n".join([" ".join([str(v) for v in row]) for row in elts[0].value()]))
            continue

        vectors = filter(lambda e : isinstance(e, NumberVector) or isinstance(e, StringVector), elts)
        if vectors:
            if vectors != elts:
                err("Line \"{}\" mixes vectors and primitives".format(line))

            lengths = set([e.assigned_length for e in elts])
            if len(lengths) > 1:
                err("Line \"{}\" contains columns with differing lengths".format(line))
            length = next(iter(lengths))

            rows = [" ".join([str(v.value()[i]) for v in vectors]) for i in xrange(0, length)]
            delim = "\n" if len(vectors) > 1 else " "
            output.append(delim.join(rows))
        else:
            output.append(" ".join([str(e.value()) for e in elts]))

    for line in output:
        print line

if __name__ == "__main__": main()
