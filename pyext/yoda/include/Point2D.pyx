cimport util
cdef class Point2D(Point):
    """
    A 2D point with errors, used by the Scatter2D class.
    """

    cdef c.Point2D* p2ptr(self) except NULL:
        return <c.Point2D*> self.ptr()


    def __init__(self, x=0, y=0, xerrs=0, yerrs=0):
        cutil.set_owned_ptr(self, new c.Point2D())
        self.x = x
        self.y = y
        self.xErrs = xerrs
        self.yErrs = yerrs

    def copy(self):
        return cutil.new_owned_cls(Point2D, new c.Point2D(deref(self.p2ptr())))

    # TODO: add clone() as mapping to (not yet existing) C++ newclone()?


    property x:
        """x coordinate"""
        def __get__(self):
            return self.p2ptr().x()
        def __set__(self, x):
            self.p2ptr().setX(x)

    property y:
        """y coordinate"""
        def __get__(self):
            return self.p2ptr().y()
        def __set__(self, y):
            self.p2ptr().setY(y)

    property xy:
        """x and y coordinates as a tuple"""
        def __get__(self):
            return util.XY(self.x, self.y)
        def __set__(self, val):
            self.x, self.y = val


    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property xErrs:
        """The x errors"""
        def __get__(self):
            return util.read_error_pair(self.p2ptr().xErrs())
        def __set__(self, val):
            self.p2ptr().setXErrs(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property yErrs:
        """The y errors"""
        def __get__(self):
            return util.read_error_pair(self.p2ptr().yErrs())
        def __set__(self, val):
            self.p2ptr().setYErrs(util.read_symmetric(val))


    @property
    def xMin(self):
        """The minimum x position, i.e. lowest error"""
        return self.p2ptr().xMin()
    @property
    def xMax(self):
        """The maximum x position, i.e. highest error"""
        return self.p2ptr().xMax()


    @property
    def yMin(self):
        """The minimum y position, i.e. lowest error"""
        return self.p2ptr().yMin()
    @property
    def yMax(self):
        """The maximum y position, i.e. highest error"""
        return self.p2ptr().yMax()


    property xErrAvg:
        def __get__(self):
            return self.p2ptr().xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self.p2ptr().yErrAvg()


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors by factor a."""
        self.p2ptr().scaleX(a)

    def scaleY(self, a):
        """(float) -> None
        Scale the y values and errors by factor a."""
        self.p2ptr().scaleY(a)

    def scaleXY(self, x=1.0, y=1.0):
        """
        (float=1, float=1) -> None
        Scale the point coordinates by the given factors.
        """
        self.p2ptr().scaleXY(x, y)

    # TODO: remove
    def scale(self, x=1.0, y=1.0):
        """
        (float=1, float=1) -> None
        DEPRECATED! Use scaleXY
        Scale the point coordinates by the given factors.
        """
        self.p2ptr().scaleXY(x, y)


    def __repr__(self):
        return '<Point2D(x=%g, y=%g)>' % (self.x, self.y)

    def __richcmp__(Point2D self, Point2D other, int op):
        if op == 0:
            return deref(self.p2ptr()) < deref(other.p2ptr())
        elif op == 1:
            return deref(self.p2ptr()) <= deref(other.p2ptr())
        elif op == 2:
            return deref(self.p2ptr()) == deref(other.p2ptr())
        elif op == 3:
            return deref(self.p2ptr()) != deref(other.p2ptr())
        elif op == 4:
            return deref(self.p2ptr()) > deref(other.p2ptr())
        elif op == 5:
            return deref(self.p2ptr()) >= deref(other.p2ptr())
