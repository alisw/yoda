cdef class Point1D(Point):
    """
    A 1D point with errors, used by the Scatter1D class.
    """

    cdef c.Point1D* p1ptr(self) except NULL:
        return <c.Point1D*> self.ptr()


    def __init__(self, x=0, xerrs=0):
        cutil.set_owned_ptr(self, new c.Point1D())
        self.x = x
        self.xErrs = xerrs

    def copy(self):
        return cutil.new_owned_cls(Point1D, new c.Point1D(deref(self.p1ptr())))

    # TODO: add clone() as mapping to (not yet existing) C++ newclone()?


    property x:
        """x coordinate"""
        def __get__(self):
            return self.p1ptr().x()
        def __set__(self, x):
            self.p1ptr().setX(x)

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property xErrs:
        """The x errors"""
        def __get__(self):
            return util.read_error_pair(self.p1ptr().xErrs())
        def __set__(self, val):
            self.p1ptr().setXErrs(util.read_symmetric(val))


    @property
    def xMin(self):
        """The minimum x position, i.e. lowest error"""
        return self.p1ptr().xMin()
    @property
    def xMax(self):
        """The maximum x position, i.e. highest error"""
        return self.p1ptr().xMax()


    property xErrAvg:
        def __get__(self):
            return self.p1ptr().xErrAvg()


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors by factor a."""
        self.p1ptr().scaleX(a)


    def __repr__(self):
        return '<Point1D(x=%g)>' % self.x

    def __richcmp__(Point1D self, Point1D other, int op):
        if op == 0:
            return deref(self.p1ptr()) < deref(other.p1ptr())
        elif op == 1:
            return deref(self.p1ptr()) <= deref(other.p1ptr())
        elif op == 2:
            return deref(self.p1ptr()) == deref(other.p1ptr())
        elif op == 3:
            return deref(self.p1ptr()) != deref(other.p1ptr())
        elif op == 4:
            return deref(self.p1ptr()) > deref(other.p1ptr())
        elif op == 5:
            return deref(self.p1ptr()) >= deref(other.p1ptr())
