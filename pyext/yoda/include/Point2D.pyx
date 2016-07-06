cdef class Point2D(util.Base):
    """
    A 2D point with errors, used by the Scatter2D class.
    """

    cdef c.Point2D *_Point2D(self) except NULL:
        return <c.Point2D *> self.ptr()

    def __dealloc__(self):
        cdef c.Point2D *p = self._Point2D()
        if self._deallocate:
            del p


    def __init__(self, x=0, y=0, xerrs=0, yerrs=0):
        cutil.set_owned_ptr(self, new c.Point2D())
        self.x = x
        self.y = y
        #self.coords = x, y
        self.xErrs = xerrs
        self.yErrs = yerrs

    def copy(self):
        return cutil.new_owned_cls(Point2D, new c.Point2D(deref(self._Point2D())))

    # TODO: add clone() as mapping to (not yet existing) C++ newclone()?


    property x:
        """x coordinate"""
        def __get__(self):
            return self._Point2D().x()
        def __set__(self, x):
            self._Point2D().setX(x)

    property y:
        """y coordinate"""
        def __get__(self):
            return self._Point2D().y()
        def __set__(self, y):
            self._Point2D().setY(y)

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
            return util.read_error_pair(self._Point2D().xErrs())
        def __set__(self, val):
            self._Point2D().setXErr(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property yErrs:
        """The y errors"""
        def __get__(self):
            return util.read_error_pair(self._Point2D().yErrs())
        def __set__(self, val):
            self._Point2D().setYErr(util.read_symmetric(val))


    # TODO: add similar to C++, plus xRanges, and xWidth(s) and map
    # property xRange:
    #     """The minimum and maximum points within the x errors"""
    #     def __get__(self):
    #         return util.EdgePair(self._Point2D().xMin(), self._Point2D().xMax())

    # # TODO: remove! or add similar to C++ and map
    # property yRange:
    #     """The minimum and maximum points within the y errors"""
    #     def __get__(self):
    #         return util.EdgePair(self._Point2D().yMin(), self._Point2D().yMax())


    @property
    def xMin(self):
        """The minimum x position, i.e. lowest error"""
        return self._Point2D().xMin()
    @property
    def xMax(self):
        """The maximum x position, i.e. highest error"""
        return self._Point2D().xMax()


    @property
    def yMin(self):
        """The minimum y position, i.e. lowest error"""
        return self._Point2D().yMin()
    @property
    def yMax(self):
        """The maximum y position, i.e. highest error"""
        return self._Point2D().yMax()


    property xErrAvg:
        def __get__(self):
            return self._Point2D().xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self._Point2D().yErrAvg()


    def scaleX(self, a):
        """(float) -> None
        Scale the x values and errors by factor a."""
        self._Point2D().scaleX(a)

    def scaleY(self, a):
        """(float) -> None
        Scale the y values and errors by factor a."""
        self._Point2D().scaleY(a)

    def scaleXY(self, x=1.0, y=1.0):
        """
        (float=1, float=1) -> None
        Scale the point coordinates by the given factors.
        """
        self._Point2D().scaleXY(x, y)

    # TODO: remove
    def scale(self, x=1.0, y=1.0):
        """
        (float=1, float=1) -> None
        DEPRECATED! Use scaleXY
        Scale the point coordinates by the given factors.
        """
        self._Point2D().scaleXY(x, y)


    def __repr__(self):
        return '<Point2D(x=%g, y=%g)>' % (self.x, self.y)

    def __richcmp__(Point2D self, Point2D other, int op):
        if op == 0:
            return deref(self._Point2D()) < deref(other._Point2D())
        elif op == 1:
            return deref(self._Point2D()) <= deref(other._Point2D())
        elif op == 2:
            return deref(self._Point2D()) == deref(other._Point2D())
        elif op == 3:
            return deref(self._Point2D()) != deref(other._Point2D())
        elif op == 4:
            return deref(self._Point2D()) > deref(other._Point2D())
        elif op == 5:
            return deref(self._Point2D()) >= deref(other._Point2D())
