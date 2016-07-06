cdef class Scatter1D(AnalysisObject):
    """
    1D scatter plot, i.e. a collection of Point1D objects with positions and errors.

    Constructor calling idioms:

    Scatter1D(path="", title="")
      Create a new empty scatter, with optional path and title.

    Scatter1D(points, path="", title=""):
      Create a new empty scatter from an iterable of points, with optional path
      and title.

    TODO: more documentation!
    """

    cdef inline c.Scatter1D* _s1ptr(self) except NULL:
        return <c.Scatter1D*> self.ptr()

    # TODO: remove
    cdef inline c.Scatter1D* _Scatter1D(self) except NULL:
        return <c.Scatter1D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init_2, self.__init_3], *args, **kwargs)

    def __init_2(self, char* path="", char* title=""):
        cutil.set_owned_ptr(self, new c.Scatter1D(string(path), string(title)))

    def __init_3(self, points, char* path="", char* title=""):
        self.__init_2(path, title)
        self.addPoints(points)


    def clone(self):
        """() -> Scatter1D.
        Clone this Scatter1D."""
        return cutil.new_owned_cls(Scatter1D, self._Scatter1D().newclone())

    def __repr__(self):
        return "<%s '%s' %d points>" % (self.__class__.__name__, self.path, len(self.points))


    @property
    def numPoints(self):
        """() -> int
        Number of points in this scatter."""
        return self._Scatter1D().numPoints()

    def __len__(self):
        return self.numPoints


    @property
    def points(self):
        """Access the ordered list of points."""
        return [self.point(i) for i in xrange(self.numPoints)]

    def point(self, size_t i):
        """Access the i'th point."""
        return cutil.new_borrowed_cls(Point1D, &self._Scatter1D().point(i), self)

    # TODO: remove?
    # def __getitem__(self, py_ix):
    #     cdef size_t i = cutil.pythonic_index(py_ix, self._Scatter1D().numPoints())
    #     return cutil.new_borrowed_cls(Point1D, &self._Scatter1D().point(i), self)


    def addPoint(self, *args, **kwargs):
        """Add a new point.

        Provide either a single yoda.Point1D object, or the
        two args: x, xerrs=0.
        """
        try:
            self.__addPoint_point(*args, **kwargs)
        except TypeError:
            self.__addPoint_explicit(*args, **kwargs)

    def __addPoint_explicit(self, x, xerrs=0):
        self.__addPoint_point(Point1D(x, xerrs))

    def __addPoint_point(self, Point1D p):
        self._Scatter1D().addPoint(p._Point1D()[0])

    def addPoints(self, iterable):
        """Add several new points."""
        for row in iterable:
            self.addPoint(*row)

    # def combineWith(self, others):
    #     """Try to add points from other Scatter1Ds into this one."""
    #     cdef Scatter1D other
    #     try:
    #         # Can we type it as a Scatter1D?
    #         other = others
    #     except TypeError:
    #         # Could be an iterable...
    #         for other in others:
    #             self._Scatter1D().combineWith(deref(other._Scatter1D()))
    #     else:
    #         self._Scatter1D().combineWith(deref(other._Scatter1D()))


    def mkScatter(self):
        """None -> Scatter1D.
        Make a new Scatter1D. Exists to allow mkScatter calls on any AnalysisObject,
        even if it already is a scatter."""
        cdef c.Scatter1D s2 = c.mkScatter_Scatter1D(deref(self._Scatter1D()))
        return cutil.new_owned_cls(Scatter1D, s2.newclone())


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors of the points in this scatter by factor a."""
        self._Scatter1D().scaleX(a)


    def transformX(self, f):
        """(fn) -> None
        Transform the x values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter1D_transformX(deref(self._Scatter1D()), fptr)


    # # TODO: remove?
    # def __add__(Scatter1D self, Scatter1D other):
    #     return cutil.new_owned_cls(Scatter1D, c.Scatter1D_add_Scatter1D(self._Scatter1D(), other._Scatter1D()))

    # # TODO: remove?
    # def __sub__(Scatter1D self, Scatter1D other):
    #     return cutil.new_owned_cls(Scatter1D, c.Scatter1D_sub_Scatter1D(self._Scatter1D(), other._Scatter1D()))
