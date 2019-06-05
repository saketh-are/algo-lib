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
INTERVAL = "\s*([\[\(])([^,]+),([^,]+)([\]\)])\s*";
SIMPLE_INTERVAL = "([\[\(].*[\]\)])"

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
        except Exception as e:
            err("Could not evaluate \"{}\": {}".format(expr, e))

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

    def assign_all(self):
        for vname in self.toposort():
            self.vars[vname].assign()

SPEC_CLASSES = {}

class Number:
    TYPES = [ "int", "float" ]
    SPEC = INTERVAL

    def __init__(self, scope, name, num_type, spec):
        assert num_type in self.TYPES
        self.scope, self.name, self.num_type = scope, name, num_type
        self.lb_type, self.lb_value, self.ub_value, self.ub_type = re.match(self.SPEC, spec).groups()

    def dependencies(self):
        return self.scope.dependencies(self.lb_value) + self.scope.dependencies(self.ub_value)

    def range(self):
        lb = self.scope.evaluate(self.lb_value)
        ub = self.scope.evaluate(self.ub_value)
        pretty_bounds = self.lb_type + str(lb) + ", " + str(ub) + self.ub_type

        if self.num_type == "int":
            if self.lb_type == '(':
                lb += 1e-9
            lb = int(math.ceil(lb))

            if self.ub_type == ')':
                ub -= 1e-9
            ub = int(math.floor(ub))

        if lb > ub:
            err("{} \"{}\" bounded by empty range {}".format(self.num_type, self.name, pretty_bounds))

        return (lb, ub)

    def assign(self):
        lb, ub = self.range()
        if self.num_type == "float":
            self.assigned_value = random.uniform(lb, ub)
        else:
            self.assigned_value = random.randint(lb, ub)
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "int" ] = Number
SPEC_CLASSES[ "float" ] = Number

class String:
    TYPES = [ "string" ]
    SPEC = "(.+)(\[.*\])"

    def __init__(self, scope, name, str_type, spec):
        assert str_type in self.TYPES
        self.scope, self.name, self.str_type = scope, name, str_type
        self.length, self.alphabet = re.match(self.SPEC, spec).groups()
        self.charset = [chr(ch) for ch in xrange(0, 256) if re.match(self.alphabet, chr(ch))]

    def dependencies(self):
        return self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = "".join([random.choice(self. charset) for i in xrange(0, self.assigned_length)])
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "string" ] = String

class NumberVector:
    TYPES = [ "ints", "floats", "permutation" ]
    SPEC = "(.*)" + SIMPLE_INTERVAL

    def __init__(self, scope, name, vec_type, spec):
        assert vec_type in self.TYPES
        self.scope, self.name, self.vec_type = scope, name, vec_type
        self.length, self.element_spec = re.match(self.SPEC, spec).groups()

        element_type = "float" if vec_type is "floats" else "int"
        self.numbers = Number(scope, "element of " + name, element_type, self.element_spec)

    def dependencies(self):
        return self.numbers.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        if self.vec_type is "permutation":
            lb, ub = self.numbers.range()
            self.assigned_value = range(lb, ub + 1)
            random.shuffle(self.assigned_value)
            self.assigned_length = len(self.assigned_value)
        else:
            self.assigned_length = self.scope.evaluate(self.length)
            self.assigned_value = [self.numbers.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "ints" ] = NumberVector
SPEC_CLASSES[ "floats" ] = NumberVector
SPEC_CLASSES[ "permutation" ] = NumberVector

class NumberGrid:
    TYPES = [ "intss", "floatss" ]
    SPEC = "(.+)%(.+)"

    def __init__(self, scope, name, grid_type, spec):
        assert grid_type in self.TYPES
        self.scope, self.name, self.grid_type = scope, name, grid_type
        self.length, self.element_spec = re.match(self.SPEC, spec).groups()
        self.row_vectors = NumberVector(scope, "rows of " + name, grid_type[:-1], self.element_spec)

    def dependencies(self):
        return self.row_vectors.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.row_vectors.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "intss" ] = NumberGrid
SPEC_CLASSES[ "floatss" ] = NumberGrid

class StringVector:
    TYPES = [ "strings" ]
    SPEC = "(.+)%(.+)"

    def __init__(self, scope, name, vec_type, spec):
        assert vec_type in self.TYPES
        self.scope, self.name, self.vec_type = scope, name, vec_type
        self.length, self.element_spec = re.match(self.SPEC, spec).groups()
        self.strings = String(scope, "elements of " + name, "string", self.element_spec)

    def dependencies(self):
        return self.strings.dependencies() + self.scope.dependencies(self.length)

    def assign(self):
        self.assigned_length = self.scope.evaluate(self.length)
        self.assigned_value = [self.strings.assign() for i in xrange(0, self.assigned_length)]
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "strings" ] = StringVector


SPEC_TYPES = sorted(SPEC_CLASSES.keys(), key=len, reverse=True)

def parse_spec(scope, spec):
    for spec_type in SPEC_TYPES:
        if spec_type in spec:
            vnames, var_spec = spec.split(spec_type)

            vnames = filter(None, vnames.split(" "))
            if not vnames:
                err("No variables associated with spec \"{}\"".format(spec))

            for vname in vnames:
                if vname in scope.vars:
                    err("Variable \"{}\" was already defined".format(vname))
                try:
                    scope.vars[vname] = SPEC_CLASSES[spec_type](scope, vname, spec_type, var_spec)
                except Exception as e:
                    err("Invalid spec \"{}\" of type {}: {}".format(var_spec, spec_type, e))
            return

    err("Spec \"{}\" does not match any recognized type ({})".format(spec, ", ".join(SPEC_TYPES)))

def print_case(scope, layout):
    scope.assign_all()

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

def main():
    if len(sys.argv) < 2:
        err("No input spec provided.")

    scope = Scope()
    layout = []

    spec = [x.strip() for x in open(sys.argv[1]).readlines() if x.strip()]
    for line in spec:
        if "#" in line:
            parse_spec(scope, line.replace("#", "", 1))
            layout.append(line[0:line.find("#")])
        else:
            layout.append(line)

    print_case(scope, filter(None, layout))


if __name__ == "__main__": main()
