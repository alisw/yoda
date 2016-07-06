def mkScatter(ao, usefocus=False, usestddev=False):
    """AnalysisObject -> Scatter{1,2,3}D
    Convert an AnalysisObject to a Scatter, using the logic of the bound mkScatter methods.

    @todo This falls back on use of optional args until we find one that works: is there a nicer way?
    """
    try:
        return ao.mkScatter(usefocus, usestddev)
    except:
        try:
            return ao.mkScatter(usefocus)
        except:
            return ao.mkScatter()


def divide(ao1, ao2):
    """(AnalysisObject, AnalysisObject) -> Scatter{1,2,3}D
    Divide one AnalysisObject by another, producing a Scatter of appropriate dimension by using the logic of the bound divideBy methods."""
    return ao1.divideBy(ao2)


# def divide(ao1, ao2, efficiency=False):
#     """(AO, AO, [bool]) -> Scatter{1,2,3}D
#     Convert two AnalysisObjects (either two HistoNDs or two ProfileNDs) to a Scatter(N+1)D.

#     The efficiency boolean is used to enable correlated division for Histos, using binomial
#     statistics as appropriate when the numerator contains a subset of the fills in the denominator.

#     TODO: Implement efficiency division mapping.
#     TODO: Implement 2D/2D division mapping.
#     TODO: Implement scatter division.
#     """
#     cdef c.Scatter2D s2
#     if type(ao1) is not type(ao2):
#         raise ValueError("Histograms must be of the same type to be divided")
#     if type(ao1) is Histo1D:
#         s2 = c.Histo1D_div_Histo1D(deref(ao1._Histo1D()), deref(ao2._Histo1D()))
#         return cutil.new_owned_cls(Scatter2D, s2.newclone())
#     elif type(ao1) is Profile1D:
#         s2 = c.Profile1D_div_Profile1D(deref(ao1._Profile1D()), deref(ao2._Profile1D()))
#         return cutil.new_owned_cls(Scatter2D, s2.newclone())
#     raise ValueError("TODO: Only division of Histo1D and Profile1D supported so far... please contact the developers!")


def linspace(nbins, start, end):
    """(int, float, float) -> list[float]
    Make a list of n+1 bin edges linearly spaced between start and end, with the first and
    last edges on those boundaries."""
    return c.linspace(nbins, start, end)


def logspace(nbins, start, end):
    """(int, float, float) -> list[float]
    Make a list of n+1 bin edges linearly spaced on the interval log(start..end), with
    the first and last edges on those boundaries."""
    return c.logspace(nbins, start, end)


def index_between(x, binedges):
    """(float, list[float]) -> int
    Return the index of the bin which would contain x, or -1 if there is no enclosing
    bin in the given set of n+1 bin edges."""
    return c.index_between(x, binedges)


def mean(sample):
    """(list[float]) -> float
    Return the unweighted mean of the entries in the provided sample list."""
    return c.mean(sample)


def covariance(sample1, sample2):
    """(list[float], list[float]) -> float
    Return the unweighted covariance of the two provided sample lists."""
    return c.covariance(sample1, sample2)


def correlation(sample1, sample2):
    """(float, list[float]) -> int
    Return the unweighted correlation of the two provided sample lists."""
    return c.correlation(sample1, sample2)
