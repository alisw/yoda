cdef class Scatter2D(AnalysisObject):
    """
    2D scatter plot, i.e. a collection of Point2D objects with positions and errors.

    Constructor calling idioms:

    Scatter2D(path="", title="")
      Create a new empty scatter, with optional path and title.

    Scatter2D(points, path="", title=""):
      Create a new empty scatter from an iterable of points, with optional path
      and title.

    TODO: more documentation!
    """

    cdef inline c.Scatter2D* s2ptr(self) except NULL:
        return <c.Scatter2D*> self.ptr()

    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init_2, self.__init_3], *args, **kwargs)

    def __init_2(self, char* path="", char* title=""):
        cutil.set_owned_ptr(self, new c.Scatter2D(string(path), string(title)))

    def __init_3(self, points, char* path="", char* title=""):
        self.__init_2(path, title)
        self.addPoints(points)

    def clone(self):
        """() -> Scatter2D.
        Clone this Scatter2D."""
        return cutil.new_owned_cls(Scatter2D, self.s2ptr().newclone())

    def __repr__(self):
        return "<%s '%s' %d points>" % (self.__class__.__name__, self.path, len(self.points))


    @property
    def numPoints(self):
        """() -> int
        Number of points in this scatter."""
        return self.s2ptr().numPoints()

    def __len__(self):
        return self.numPoints


    @property
    def points(self):
        """Access the ordered list of points."""
        return [self.point(i) for i in xrange(self.numPoints)]

    def point(self, size_t i):
        """Access the i'th point."""
        return cutil.new_borrowed_cls(Point2D, &self.s2ptr().point(i), self)

    def __getitem__(self, py_ix):
        cdef size_t i = cutil.pythonic_index(py_ix, self.s2ptr().numPoints())
        return cutil.new_borrowed_cls(Point2D, &self.s2ptr().point(i), self)


    def addPoint(self, *args, **kwargs):
        """Add a new point.

        Provide either a single yoda.Point2D object, or the
        four args: x, y, xerrs=0, yerrs=0.
        """
        try:
            self.__addPoint_point(*args, **kwargs)
        except TypeError:
            self.__addPoint_explicit(*args, **kwargs)

    def __addPoint_explicit(self, x, y, xerrs=0, yerrs=0):
        self.__addPoint_point(Point2D(x, y, xerrs, yerrs))

    def __addPoint_point(self, Point2D p):
        self.s2ptr().addPoint(p.p2ptr()[0])

    def addPoints(self, iterable):
        """Add several new points."""
        for row in iterable:
            self.addPoint(*row)

    def combineWith(self, others):
        """Try to add points from other Scatter2Ds into this one."""
        cdef Scatter2D other
        try:
            # Can we type it as a Scatter2D?
            other = others
        except TypeError:
            # Could be an iterable...
            for other in others:
                self.s2ptr().combineWith(deref(other.s2ptr()))
        else:
            self.s2ptr().combineWith(deref(other.s2ptr()))


    def mkScatter(self):
        """None -> Scatter2D.
        Make a new Scatter2D. Exists to allow mkScatter calls on any AnalysisObject,
        even if it already is a scatter."""
        cdef c.Scatter2D s2 = c.mkScatter_Scatter2D(deref(self.s2ptr()))
        return cutil.new_owned_cls(Scatter2D, s2.newclone())


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors of the points in this scatter by factor a."""
        self.s2ptr().scaleX(a)

    def scaleY(self, a):
        """(float) -> None
        Scale the y values and errors of the points in this scatter by factor a."""
        self.s2ptr().scaleY(a)

    def scaleXY(self, ax=1.0, ay=1.0):
        """(float=1, float=1) -> None
        Scale the values and errors of the points in this scatter by factors ax, ay."""
        self.s2ptr().scaleXY(ax, ay)

    # TODO: remove
    def scale(self, ax=1.0, ay=1.0):
        """(float=1, float=1) -> None
        DEPRECATED: USE scaleXY
        Scale the values and errors of the points in this scatter by factors ax, ay."""
        self.scaleXY(ax, ay)


    def transformX(self, f):
        """(fn) -> None
        Transform the x values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter2D_transformX(deref(self.s2ptr()), fptr)

    def transformY(self, f):
        """(fn) -> None
        Transform the y values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter2D_transformY(deref(self.s2ptr()), fptr)


    # # TODO: remove?
    # def __add__(Scatter2D self, Scatter2D other):
    #     return cutil.new_owned_cls(Scatter2D, c.Scatter2D_add_Scatter2D(self.s2ptr(), other.s2ptr()))

    # # TODO: remove?
    # def __sub__(Scatter2D self, Scatter2D other):
    #     return cutil.new_owned_cls(Scatter2D, c.Scatter2D_sub_Scatter2D(self.s2ptr(), other.s2ptr()))
