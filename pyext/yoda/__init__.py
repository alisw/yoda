## Pull in core YODA C++/Python extension functionality
from yoda.core import *

__version__ = core.version()


## Pull in useful tools from submodules
from yoda.search import match_aos


## Try to pull in plotting tools in a non-dependency-inducing way
def plot(*args, **kwargs):
    from yoda.plotting import plot as p
    return p(*args, **kwargs)


## Try to pull in optional ROOT compatibility
try:
    import yoda.root
    HAS_ROOT_SUPPORT = True
    # TODO: remove in v2
    def to_root(ao):
        print "yoda.to_root() is deprecated: use yoda.root.to_root()"
        return yoda.root.to_root(ao)
except:
    HAS_ROOT_SUPPORT = False
