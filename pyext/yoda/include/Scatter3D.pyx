cdef class Scatter3D(AnalysisObject):
    """
    3D scatter plot, i.e. a collection of Point3D objects with positions and errors.

    Constructor calling idioms:

    Scatter3D(path="", title="")
      Create a new empty scatter, with optional path and title.

    Scatter3D(points, path="", title=""):
      Create a new empty scatter from an iterable of points, with optional path
      and title.

    TODO: more documentation!
    """

    cdef inline c.Scatter3D* _Scatter3D(self) except NULL:
        return <c.Scatter3D*> self.ptr()

    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init_2, self.__init_3], *args, **kwargs)

    def __init_2(self, char* path="", char* title=""):
        cutil.set_owned_ptr(self, new c.Scatter3D(string(path), string(title)))

    def __init_3(self, points, char* path="", char* title=""):
        self.__init_2(path, title)
        self.addPoints(points)

    def clone(self):
        """() -> Scatter3D.
        Clone this Scatter3D."""
        return cutil.new_owned_cls(Scatter3D, self._Scatter3D().newclone())

    def __repr__(self):
        return "<%s '%s' %d points>" % (self.__class__.__name__, self.path, len(self.points))


    @property
    def numPoints(self):
        """() -> int
        Number of points in this scatter."""
        return self._Scatter3D().numPoints()

    def __len__(self):
        return self.numPoints


    @property
    def points(self):
        """Access the ordered list of points."""
        return [self.point(i) for i in xrange(self.numPoints)]

    def point(self, size_t i):
        """Access the i'th point."""
        return cutil.new_borrowed_cls(Point3D, &self._Scatter3D().point(i), self)

    # TODO: remove?
    # def __getitem__(self, py_ix):
    #     cdef size_t i = cutil.pythonic_index(py_ix, self._Scatter3D().numPoints())
    #     return cutil.new_borrowed_cls(Point3D, &self._Scatter3D().point(i), self)


    def addPoint(self, *args, **kwargs):
        """Add a new point.

        Provide either a single yoda.Point3D object, or the
        3-6 args: x, y, z, xerrs=0, yerrs=0, zerrs=0.
        """
        try:
            self.__addPoint_point(*args, **kwargs)
        except TypeError:
            self.__addPoint_explicit(*args, **kwargs)

    def __addPoint_explicit(self, x, y, z, xerrs=0, yerrs=0, zerrs=0):
        self.__addPoint_point(Point3D(x, y, z, xerrs, yerrs, zerrs))

    def __addPoint_point(self, Point3D p):
        self._Scatter3D().addPoint(p._Point3D()[0])

    def addPoints(self, iterable):
        """Add several new points."""
        for row in iterable:
            self.addPoint(*row)

    def combineWith(self, others):
        """Try to add points from other Scatter3Ds into this one."""
        cdef Scatter3D other
        try:
            # Can we type it as a Scatter3D?
            other = others
        except TypeError:
            # Could be an iterable...
            for other in others:
                self._Scatter3D().combineWith(deref(other._Scatter3D()))
        else:
            self._Scatter3D().combineWith(deref(other._Scatter3D()))


    def mkScatter(self):
        """None -> Scatter3D.
        Make a new Scatter3D. Exists to allow mkScatter calls on any AnalysisObject,
        even if it already is a scatter."""
        cdef c.Scatter3D s3 = c.mkScatter_Scatter3D(deref(self._Scatter3D()))
        return cutil.new_owned_cls(Scatter3D, s3.newclone())


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors of the points in this scatter by factor a."""
        self._Scatter3D().scaleX(a)

    def scaleY(self, a):
        """(float) -> None
        Scale the y values and errors of the points in this scatter by factor a."""
        self._Scatter3D().scaleY(a)

    def scaleZ(self, a):
        """(float) -> None
        Scale the z values and errors of the points in this scatter by factor a."""
        self._Scatter3D().scaleZ(a)

    def scaleXYZ(self, ax=1, ay=1, az=1):
        """(float=1, float=1, float=1) -> None
        Scale the values and errors of the points in this scatter by factors ax, ay, az."""
        self._Scatter3D().scaleXYZ(ax, ay, az)

    # TODO: remove
    def scale(self, ax=1, ay=1, az=1):
        """(float=1, float=1, float=1) -> None
        DEPRECATED: USE scaleXYZ
        Scale the values and errors of the points in this scatter by factors ax, ay, az."""
        self.scaleXYZ(ax, ay, az)


    def transformX(self, f):
        """(fn) -> None
        Transform the x values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter3D_transformX(deref(self._Scatter3D()), fptr)

    def transformY(self, f):
        """(fn) -> None
        Transform the y values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter3D_transformY(deref(self._Scatter3D()), fptr)

    def transformZ(self, f):
        """(fn) -> None
        Transform the z values and errors of the points in this scatter by function f."""
        import ctypes
        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")
        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]
        c.Scatter3D_transformZ(deref(self._Scatter3D()), fptr)


    # # TODO: remove?
    # def __add__(Scatter3D self, Scatter3D other):
    #     return cutil.new_owned_cls(Scatter3D, c.Scatter3D_add_Scatter3D(self._Scatter3D(), other._Scatter3D()))

    # # TODO: remove?
    # def __sub__(Scatter3D self, Scatter3D other):
    #     return cutil.new_owned_cls(Scatter3D, c.Scatter3D_sub_Scatter3D(self._Scatter3D(), other._Scatter3D()))
