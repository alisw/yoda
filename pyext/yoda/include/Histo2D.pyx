cimport util
cdef class Histo2D(AnalysisObject):
    """
    2D histogram.

    Complete histogramming is supported, including uniform/regular binning,
    variable-width bininng, unbinned gaps in the covered range, and outflows
    (under/overflows around all edges and corners).

    Rebinning by integer factors, or by explicit merging of contiguous bins is
    also supported, but in development.

    Rescaling of weights and/or the x axis is permitted in-place: the
    result is still a valid Histo2D. Binning-compatible 2D histograms
    may be divided, resulting in a Scatter3D since further fills would
    not be meaningful.

    Several sets of arguments are tried by the constructor in the
    following order.

    Histo2D(path="", title="").
      Construct a histogram with optional path and title but no bins.

    Histo2D(nxbins, xlow, xhigh, nybins, ylow, yhigh, path="", title="").
      Construct a histogram with nxbins on the x axis and nybins on the y
      axis, distributed linearly between the respective low--high limits.
    """

    cdef inline c.Histo2D* h2ptr(self) except NULL:
        return <c.Histo2D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init4, self.__init8], *args, **kwargs)

    def __init2(Histo2D self, path="", title=""):
        path  = path.encode('utf-8')
        title = title.encode('utf-8')
        cutil.set_owned_ptr(self, new c.Histo2D(<string>path, <string>title))

    def __init4(Histo2D self, xedges,  yedges,  path="", title=""):
        path  = path.encode('utf-8')
        title = title.encode('utf-8')
        # TODO: Do some type-checking and allow iterables of HistoBin2D as well?
        cutil.set_owned_ptr(self, new c.Histo2D(xedges, yedges, <string>path, <string>title))

    def __init8(Histo2D self, nxbins, xlow, xhigh,  nybins, ylow, yhigh,  path="", title=""):
        path  = path.encode('utf-8')
        title = title.encode('utf-8')
        cutil.set_owned_ptr(self, new c.Histo2D(nxbins, xlow, xhigh,  nybins, ylow, yhigh,  <string>path, <string>title))


    def __len__(self):
        "Number of bins"
        return self.numBins

    def __getitem__(self, py_ix):
        "Direct access to bins"
        cdef size_t i = cutil.pythonic_index(py_ix, self.h2ptr().numBins())
        return cutil.new_borrowed_cls(HistoBin2D, & self.h2ptr().bins().at(i), self)


    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%.2g>" % (self.__class__.__name__, self.path, len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self.h2ptr().reset()

    def clone(self):
        """None -> Histo2D.
        Clone this Profile2D."""
        return cutil.new_owned_cls(Histo2D, self.h2ptr().newclone())


    def fill(self, double x, double y, weight=1.0, fraction=1.0):
        """(x,y,[w]) -> None.
        Fill with given x,y values and optional weight."""
        self.h2ptr().fill(x, y, weight, fraction)

    def fillBin(self, size_t i, weight=1.0, fraction=1.0):
        """(i,[w]) -> None.
        Fill bin i and optional weight."""
        self.h2ptr().fillBin(i, weight, fraction)


    @property
    def totalDbn(self):
        """() -> Dbn2D
        The Dbn2D representing the total distribution."""
        return cutil.new_borrowed_cls(Dbn2D, &self.h2ptr().totalDbn(), self)

    # TODO: reinstate
    # def outflow(self, ix, iy):
    #     """(ix,iy) -> Dbn2D
    #     The Dbn2D representing the ix,iy outflow distribution."""
    #     return cutil.new_borrowed_cls(Dbn2D, &self.h2ptr().outflow(ix, iy), self)


    def integral(self, includeoverflows=True):
        """([bool]) -> float
        Histogram integral, optionally excluding the overflows."""
        return self.h2ptr().integral(includeoverflows)


    def numEntries(self, includeoverflows=True):
        """([bool]) -> float
        Number of times this histogram was filled, optionally excluding overflows."""
        return self.h2ptr().numEntries(includeoverflows)

    def effNumEntries(self, includeoverflows=True):
        """([bool]) -> float
        Effective number of times this histogram was filled, computed from weights and optionally excluding overflows."""
        return self.h2ptr().effNumEntries(includeoverflows)

    def sumW(self, includeoverflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self.h2ptr().sumW(includeoverflows)

    def sumW2(self, includeoverflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self.h2ptr().sumW2(includeoverflows)


    def xMean(self, includeoverflows=True):
        """([bool]) -> float
        Mean x of the histogram, optionally excluding the overflows."""
        return self.h2ptr().xMean(includeoverflows)

    def yMean(self, includeoverflows=True):
        """([bool]) -> float
        Mean y of the histogram, optionally excluding the overflows."""
        return self.h2ptr().yMean(includeoverflows)

    def xyMean(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xMean(includeoverflows), self.yMean(includeoverflows))


    def xVariance(self, includeoverflows=True):
        """([bool]) -> float
        Variance in x of the histogram, optionally excluding the overflows."""
        return self.h2ptr().xVariance(includeoverflows)

    def yVariance(self, includeoverflows=True):
        """([bool]) -> float
        Variance in y of the histogram, optionally excluding the overflows."""
        return self.h2ptr().yVariance(includeoverflows)

    def xyVariance(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Variances in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xVariance(includeoverflows), self.yVariance(includeoverflows))


    def xStdDev(self, includeoverflows=True):
        """([bool]) -> float
        Standard deviation in x of the histogram, optionally excluding the overflows."""
        return self.h2ptr().xStdDev(includeoverflows)

    def yStdDev(self, includeoverflows=True):
        """([bool]) -> float
        Standard deviation in y of the histogram, optionally excluding the overflows."""
        return self.h2ptr().yStdDev(includeoverflows)

    def xyStdDev(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Standard deviations in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xStdDev(includeoverflows), self.yStdDev(includeoverflows))


    def xStdErr(self, includeoverflows=True):
        """([bool]) -> float
        Standard error on the mean x of the histogram, optionally excluding the overflows."""
        return self.h2ptr().xStdErr(includeoverflows)

    def yStdErr(self, includeoverflows=True):
        """([bool]) -> float
        Standard error on the mean y of the histogram, optionally excluding the overflows."""
        return self.h2ptr().yStdErr(includeoverflows)

    def xyStdErr(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Standard errors on the mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xStdErr(includeoverflows), self.yStdErr(includeoverflows))


    def xRMS(self, includeoverflows=True):
        """([bool]) -> float
        RMS in x of the histogram, optionally excluding the overflows."""
        return self.h2ptr().xRMS(includeoverflows)

    def yRMS(self, includeoverflows=True):
        """([bool]) -> float
        RMS in y of the histogram, optionally excluding the overflows."""
        return self.h2ptr().yRMS(includeoverflows)

    def xyRMS(self, includeoverflows=True):
        """([bool]) -> (float,float)
        RMS in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xRMS(includeoverflows), self.yRMS(includeoverflows))


    def scaleW(self, w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self.h2ptr().scaleW(w)

    def normalize(self, double normto=1.0, bint includeoverflows=True):
        """(float, bool) -> None.
        Normalize the histogram."""
        self.h2ptr().normalize(normto, includeoverflows)


    @property
    def xMin(self):
        """Low x edge of the histo."""
        return self.h2ptr().xMin()

    @property
    def xMax(self):
        """High x edge of the histo."""
        return self.h2ptr().xMax()

    @property
    def yMin(self):
        """Low y edge of the histo."""
        return self.h2ptr().yMin()

    @property
    def yMax(self):
        """High y edge of the histo."""
        return self.h2ptr().yMax()


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self.h2ptr().numBins()

    @property
    def numBinsX(self):
        """() -> int
        Number of bins (edges) along the x axis."""
        return self.h2ptr().numBinsX()

    @property
    def numBinsY(self):
        """() -> int
        Number of bins (edges) along the y axis."""
        return self.h2ptr().numBinsY()


    @property
    def bins(self):
        """Access the ordered bins list."""
        return [self.bin(i) for i in xrange( self.h2ptr().numBins())]

    def bin(self, i):
        """Get the i'th bin"""
        # cdef size_t ii = cutil.pythonic_index(i, self.h2ptr().numBins())
        return cutil.new_borrowed_cls(HistoBin2D, & self.h2ptr().bin(i), self)

    # TODO: it's more intuitive to have an index for each axis
    # def bin(self, i, j):
    #     """Get the (i,j)'th bin"""
    #     # cdef size_t ii = cutil.pythonic_index(i, self.h2ptr().numBins())
    #     # cdef size_t jj = cutil.pythonic_index(j, self.h2ptr().numBins())
    #     return cutil.new_borrowed_cls(HistoBin2D, & self.h2ptr().bin(i,j), self)

    def binIndexAt(self, x, y):
        """Get the bin index pair containing position (x,y)"""
        return self.h2ptr().binIndexAt(x, y)

    def binAt(self, x, y):
        """Get the bin containing position (x,y)"""
        # TODO: what's the problem with this direct mapping? Produces compile error re. no default constructor...
        #return cutil.new_borrowed_cls(HistoBin2D, & self.h2ptr().binAt(x,y), self)
        # TODO: need out-of-range check to return None?
        return self.bin(self.binIndexAt(x,y))


    def addBin(self, xlow, xhigh, ylow, yhigh):
        """Add a bin."""
        self.h2ptr().addBin(pair[double, double](xlow, xhigh),
                               pair[double, double](ylow, yhigh))
        return self

    def addBins(self, bounds):
        """Add several bins."""
        # TODO: simplify / make consistent
        for xlow, xhigh, ylow, yhigh in bounds:
            self.h2ptr().addBin(pair[double, double](xlow, xhigh),
                                pair[double, double](ylow, yhigh))

    # def mergeBins(self, size_t a, size_t b):
    #     self.h2ptr().mergeBins(a, b)

    # def rebin(self, int n):
    #     self.h2ptr().rebin(n)


    def mkScatter(self, usefocus=False):
        """None -> Scatter3D.
        Convert this Histo2D to a Scatter3D, with y representing bin heights
        (not sumW) and height errors."""
        cdef c.Scatter3D s3 = c.mkScatter_Histo2D(deref(self.h2ptr()), usefocus)
        return cutil.new_owned_cls(Scatter3D, s3.newclone())

    def divideBy(self, Histo2D h, efficiency=False):
        """Histo2D -> Scatter3D

        Divide this histogram by Histo2D h, returning a Scatter3D. The optional 'efficiency'
        argument, if set True, will use a binomial efficiency treatment of the errors.
        """
        # if type(h) is not Histo2D:
        #     raise ValueError("Histograms must be of the same type to be divided")
        # TODO: allow dividing profiles by histos, etc.? (But then what do the errors mean? Add in quad?)
        cdef c.Scatter3D s
        if not efficiency:
            s = c.Histo2D_div_Histo2D(deref(self.h2ptr()), deref(h.h2ptr()))
        else:
            s = c.Histo2D_eff_Histo2D(deref(self.h2ptr()), deref(h.h2ptr()))
        return cutil.new_owned_cls(Scatter3D, s.newclone())



    def __iadd__(Histo2D self, Histo2D other):
        c.Histo2D_iadd_Histo2D(self.h2ptr(), other.h2ptr())
        return self
    def __isub__(Histo2D self, Histo2D other):
        c.Histo2D_isub_Histo2D(self.h2ptr(), other.h2ptr())
        return self

    def __add__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_add_Histo2D(self.h2ptr(), other.h2ptr()))
        return h
    def __sub__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_sub_Histo2D(self.h2ptr(), other.h2ptr()))
        return h

    def __div__(Histo2D self, Histo2D other):
        return self.divideBy(other)

    def __truediv__(Histo2D self, Histo2D other):
        return self.divideBy(other)

## Convenience alias
H2D = Histo2D
