import os

def canonical_path(fpath, extension = ".cpp"):
    if fpath[-len(extension):] != extension:
        fpath += extension
    if fpath[0] == '~':
        fpath = os.path.expanduser(fpath)
    return os.path.abspath(fpath)
