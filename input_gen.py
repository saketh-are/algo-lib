import sys
import re
import math
import random
import operator
import itertools
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

def warn(message):
    print colors.BOLD + colors.HEADER + "WARN" + colors.ENDC + ": " + message

VAR_NAME_RE = re.compile("[a-zA-Z0-9_]+")
INTERVAL = "([\[\(].*[\]\)])"

class Scope:
    NUMBER_RE = re.compile("-?[0-9]+([.][0-9]+)?(e[0-9]+)?")

    def __init__(self):
        self.vars = {}
        self.locals = {}
        self.assigned = []

    def is_builtin(self, expr):
        return self.NUMBER_RE.match(expr) or expr in ["min", "max", "abs"]

    def dependencies(self, expr):
        return [x for x in VAR_NAME_RE.findall(expr) if not self.is_builtin(x)]

    def evaluate(self, expr):
        try:
            for assignment in self.assigned:
                exec(assignment)
            return eval(expr)
        except Exception as e:
            err("Could not evaluate \"{}\": {}".format(expr, e))

    def toposort(self):
        degree = defaultdict(int)
        rev_deps = defaultdict(list)

        for vname, var in self.vars.iteritems():
            for dep in var.dependencies():
                if not dep in self.vars:
                    err("Variable {} has undefined dependency \"{}\"".format(vname, dep))
                degree[vname] += 1
                rev_deps[dep].append(vname)

        order = []
        for vname in self.vars:
            if degree[vname] == 0:
                order.append(vname)

        inx = 0
        while inx < len(order):
            for dep in rev_deps[order[inx]]:
                degree[dep] -= 1
                if not degree[dep]:
                    order.append(dep)
            inx += 1

        if len(order) != len(self.vars):
            err("Variable dependencies are cyclic")
        return order

    def assign_all(self):
        self.assigned = []
        for vname in self.toposort():
            vobj = self.vars[vname]
            vobj.assign()
            if isinstance(vobj, Number):
                self.assigned += [ "{}={}".format(vname, vobj.value()) ]

SPEC_CLASSES = {}

class Number:
    TYPES = [ "int", "float" ]
    SPEC = re.compile(INTERVAL)

    def __init__(self, scope, name, num_type, spec):
        assert num_type in self.TYPES
        self.scope, self.name, self.num_type = scope, name, num_type
        self.lb_type, self.boundaries, self.ub_type = spec[0], spec[1:-1], spec[-1]

    def dependencies(self):
        return self.scope.dependencies(self.boundaries)

    def range(self):
        lb, ub = self.scope.evaluate(self.boundaries)
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
    SPEC = re.compile("(.+)(\[.*\])")

    def __init__(self, scope, name, str_type, spec):
        assert str_type in self.TYPES
        self.scope, self.name, self.str_type = scope, name, str_type
        self.length, self.alphabet = self.SPEC.match(spec).groups()
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
    SPEC = re.compile("(.*)" + INTERVAL + "(.*)")

    def __init__(self, scope, name, vec_type, spec):
        assert vec_type in self.TYPES
        self.scope, self.name, self.vec_type = scope, name, vec_type
        self.length, self.element_spec, self.flags = self.SPEC.match(spec).groups()

        self.element_type = "float" if vec_type is "floats" else "int"
        self.numbers = Number(scope, "element of " + name, self.element_type, self.element_spec)

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
            lb, ub = self.numbers.range()

            if "distinct" in self.flags and self.element_type is "int" and self.assigned_length * 2 > ub - lb:
                include = [1] * self.assigned_length + [0] * (ub - lb + 1 - self.assigned_length)
                random.shuffle(include)
                self.assigned_value = [x for x in xrange(lb, ub + 1) if include[x - lb]]
                random.shuffle(self.assigned_value)
            else:
                self.assigned_value = [self.numbers.assign() for i in xrange(0, self.assigned_length)]

            if "asc" in self.flags:
                self.assigned_value = sorted(self.assigned_value)
            if "desc" in self.flags:
                self.assigned_value = sorted(self.assigned_value, reverse=True)
        return self.assigned_value

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "ints" ] = NumberVector
SPEC_CLASSES[ "floats" ] = NumberVector
SPEC_CLASSES[ "permutation" ] = NumberVector

class NumberGrid:
    TYPES = [ "intss", "floatss" ]
    SPEC = re.compile("(.+)%(.+)")

    def __init__(self, scope, name, grid_type, spec):
        assert grid_type in self.TYPES
        self.scope, self.name, self.grid_type = scope, name, grid_type
        self.length, self.element_spec = self.SPEC.match(spec).groups()
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
    SPEC = re.compile("(.+)%(.+)")

    def __init__(self, scope, name, vec_type, spec):
        assert vec_type in self.TYPES
        self.scope, self.name, self.vec_type = scope, name, vec_type
        self.length, self.element_spec = self.SPEC.match(spec).groups()
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

class UndirectedGraph:
    TYPES = [ "graph", "connected_graph", "tree" ]
    SPEC = re.compile("(.*)%(.*)")

    def __init__(self, scope, gname, uname, vname, graph_type, spec):
        assert graph_type in self.TYPES
        self.scope, self.gname, self.uname, self.vname, self.graph_type = scope, gname, uname, vname, graph_type

        if self.graph_type is "tree":
            self.graph_type = "connected_graph"
            spec += "%" + spec + " -1"
        elif self.graph_type is "graph":
            warn("Graph on \"{} {}\" isn't specified as connected".format(uname, vname))

        self.vertices, self.edges = self.SPEC.match(spec).groups()
        scope.vars[uname] = DerivedVector(scope, uname, [gname],\
                lambda s : [edge[0] for edge in s.vars[gname].assigned_edgelist])
        scope.vars[vname] = DerivedVector(scope, vname, [gname],\
                lambda s : [edge[1] for edge in s.vars[gname].assigned_edgelist])

    def dependencies(self):
        return self.scope.dependencies(self.vertices) + self.scope.dependencies(self.edges)

    def assign(self):
        self.assigned_vertices = self.scope.evaluate(self.vertices)
        self.assigned_edges = self.scope.evaluate(self.edges)

        V, E = self.assigned_vertices, self.assigned_edges
        self.assigned_edgelist = []

        if "connected" in self.graph_type:
            if E < V - 1:
                err("Connected graph \"{} {}\" on {} vertices cannot contain {} edges".format(self.uname, self.vname, V, E))
            for i in xrange(2, V + 1):
                self.assigned_edgelist.append((random.randint(1, i - 1), i))

        EMAX = V * (V - 1) / 2
        if E > EMAX:
            err("Simple graph \"{} {}\" on {} vertices cannot contain {} edges".format(self.uname, self.vname, V, E))

        used = set(self.assigned_edgelist)
        if 2 * E > EMAX:
            unused = []
            for j in xrange(2, V + 1):
                unused += [(i, j) for i in xrange(1, j) if (i, j) not in used]
            random.shuffle(unused)
            self.assigned_edgelist += unused[0:E-V+1]
        while len(self.assigned_edgelist) < E:
            i, j = random.randint(1, V), random.randint(1, V)
            if i == j:
                continue
            if i > j:
                i,j = j,i
            if (i, j) not in used:
                used.add((i, j))
                self.assigned_edgelist.append((i, j))

        permutation = range(1, V + 1)
        random.shuffle(permutation)
        relabeled_edges = []
        for (i, j) in self.assigned_edgelist:
            relabeled_edges.append((permutation[i-1]+1, permutation[j-1]+1))
        self.assigned_edgelist = relabeled_edges

    def value(self):
        assert False

class DerivedVector:
    def __init__(self, scope, name, dependencies, construct):
        self.scope, self.name, self.deps, self.construct = scope, name, dependencies, construct

    def dependencies(self):
        return self.deps

    def assign(self):
        self.assigned_value = self.construct(self.scope)
        self.assigned_length = len(self.assigned_value)

    def value(self):
        return self.assigned_value

SPEC_CLASSES[ "graph" ] = UndirectedGraph
SPEC_CLASSES[ "connected_graph" ] = UndirectedGraph
SPEC_CLASSES[ "tree" ] = UndirectedGraph


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

            SpecClass = SPEC_CLASSES[spec_type]

            if SpecClass is UndirectedGraph:
                if len(vnames) is not 2:
                    err("Graph spec \"{}\" should be associated with exactly two variables".format(spec))

                u_name, v_name = vnames
                graph_name = u_name + " " + v_name
                scope.vars[graph_name] = SpecClass(scope, graph_name, u_name, v_name, spec_type, var_spec.strip())
                return

            for vname in vnames:
                try:
                    scope.vars[vname] = SpecClass(scope, vname, spec_type, var_spec.strip())
                except Exception as e:
                    err("Invalid spec \"{}\" of type {}: {}".format(var_spec, spec_type, e))
            return

    err("Spec \"{}\" does not match any recognized type ({})".format(spec, ", ".join(SPEC_TYPES)))

def print_case(scope, layout):
    scope.assign_all()

    output = []
    for line in layout:
        vnames = VAR_NAME_RE.findall(line)

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

        vectors = filter(lambda e : isinstance(e, (NumberVector, StringVector, DerivedVector)), elts)
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

    if "cases" in spec[0]:
        interval = spec[0].split("cases")[-1].strip()
        num_cases = Number(scope, "cases", "int", interval).assign()
        spec = spec[1:]
    else:
        num_cases = None

    for line in spec:
        if "#" in line:
            parse_spec(scope, line.replace("#", "", 1))
            layout.append(line[0:line.find("#")])
        else:
            layout.append(line)

    if num_cases:
        print num_cases
        for i in xrange(0, num_cases):
            print_case(scope, filter(None, layout))
    else:
        print_case(scope, filter(None, layout))


if __name__ == "__main__": main()
