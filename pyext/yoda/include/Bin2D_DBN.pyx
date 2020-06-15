cimport util
# TODO: docstrings
cdef class Bin2D_${DBN}(Bin):
    """2D Bin class templated on a ${DBN}"""

    cdef inline c.Bin2D_${DBN}* b2ptr(self) except NULL:
        return <c.Bin2D_${DBN}*> self.ptr()
    # TODO: remove
    cdef inline c.Bin2D_${DBN}* _Bin2D(self) except NULL:
        return <c.Bin2D_${DBN}*> self.ptr()


    def __init__(self, xlow, xhigh, ylow, yhigh):
        cutil.set_owned_ptr(self, new c.Bin2D_${DBN}( pair[double, double](xlow, xhigh),
                                                      pair[double, double](ylow, yhigh) ))

    def __repr__(self):
        return '<%s x=[%g, %g), y=[%g, %g)>' % (self.__class__.__name__, self.xMin, self.xMax, self.yMin, self.yMax)


    # def scaleXY(self, x=1.0, y=1.0):
    #     self.b2ptr().scaleXY(x, y)

    # def scaleW(self, w):
    #     self.b2ptr().scaleW(w)


    #@property
    def xEdges(self):
        """
        The lower and upper x edges.
        """
        cdef pair[double, double] x = self.b2ptr().xEdges()
        return (x.first, x.second)

    #@property
    def yEdges(self):
        """
        The lower and upper y edges.
        """
        cdef pair[double, double] y = self.b2ptr().yEdges()
        return (y.first, y.second)

    #@property
    def xyEdges(self):
        """
        The lower and upper x,y edge pairs.
        """
        return util.XY(self.xEdges, self.yEdges)


    #@property
    def xMin(self):
        """Low edge in x."""
        return self.b2ptr().xMin()

    #@property
    def yMin(self):
        """Low edge in y."""
        return self.b2ptr().yMin()

    #@property
    def xyMin(self):
        """Low edges in x,y."""
        return util.XY(self.xMin, self.yMin)


    #@property
    def xMax(self):
        """High edge in x."""
        return self.b2ptr().xMax()

    #@property
    def yMax(self):
        """High edge in y."""
        return self.b2ptr().yMax()

    #@property
    def xyMax(self):
        """High edges in x,y."""
        return util.XY(self.xMax, self.yMax)


    #@property
    def xMid(self):
        """Geometric centre of the bin in x"""
        return self.b2ptr().xMid()

    #@property
    def yMid(self):
        """Geometric centre of the bin in y"""
        return self.b2ptr().yMid()

    #@property
    def xyMid(self):
        """Geometric centre of the bin"""
        return util.XY(self.xMid, self.yMid)


    #@property
    def xWidth(self):
        """Width of the bin in x"""
        return self.b2ptr().xWidth()

    #@property
    def yWidth(self):
        """Width of the bin in y"""
        return self.b2ptr().yWidth()

    #@property
    def xyWidths(self):
        """The widths of this bin in the x- and y-dimensions."""
        return util.XY(self.xWidth, self.yWidth)


    #@property
    def area(self):
        """The area of this bin in the x-y plane."""
        return self.b2ptr().area()


    #@property
    def xFocus(self):
        """Focus of the bin in x"""
        return self.b2ptr().xFocus()

    #@property
    def yFocus(self):
        """Focus of the bin in y"""
        return self.b2ptr().yFocus()

    #@property
    def xyFocus(self):
        """The focus of the bin in the x- and y-dimensions"""
        return util.XY(self.xFocus, self.yFocus)


    #@property
    def xMean(self):
        return self.b2ptr().xMean()

    #@property
    def yMean(self):
        return self.b2ptr().xMean()

    #@property
    def xyMean(self):
        return util.XY(self.xMean, self.yMean)


    #@property
    def xVariance(self):
        return self.b2ptr().xVariance()

    #@property
    def yVariance(self):
        return self.b2ptr().xVariance()

    #@property
    def xyVariance(self):
        return util.XY(self.xVariance, self.yVariance)


    #@property
    def xStdDev(self):
        return self.b2ptr().xStdDev()

    #@property
    def yStdDev(self):
        return self.b2ptr().yStdDev()

    #@property
    def xyStdDev(self):
        return util.XY(self.xStdDev, self.yStdDev)


    #@property
    def xStdErr(self):
        return self.b2ptr().xStdErr()

    #@property
    def yStdErr(self):
        return self.b2ptr().yStdErr()

    #@property
    def xyStdErr(self):
        return util.XY(self.xStdErr, self.yStdErr)


    #@property
    def xRMS(self):
        return self.b2ptr().xRMS()

    #@property
    def yRMS(self):
        return self.b2ptr().yRMS()

    #@property
    def xyRMS(self):
        return util.XY(self.xRMS, self.yRMS)


    # Raw statistics #
    ##################

    #@property
    def sumWX(self):
        return self.b2ptr().sumWX()

    #@property
    def sumWY(self):
        return self.b2ptr().sumWY()

    #@property
    def sumWXY(self):
        return self.b2ptr().sumWXY()

    #@property
    def sumWX2(self):
        return self.b2ptr().sumWX2()

    #@property
    def sumWY2(self):
        return self.b2ptr().sumWY2()


    #def merge(Bin2D_${DBN} self, Bin2D_${DBN} other):
    #    self.b2ptr().merge(deref(other.b2ptr()))
    #    return self

    def adjacentTo(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return self.b2ptr().adjacentTo(deref(other.b2ptr()))


    def __add__(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return cutil.new_owned_cls(
            Bin2D_${DBN},
            new c.Bin2D_${DBN}(deref(self.b2ptr()) + deref(other.b2ptr())))

    def __sub__(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return cutil.new_owned_cls(
            Bin2D_${DBN},
            new c.Bin2D_${DBN}(deref(self.b2ptr()) - deref(other.b2ptr())))
