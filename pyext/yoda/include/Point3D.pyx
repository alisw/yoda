cdef class Point3D(util.Base):
    """
    A 3D point with errors, used by the Scatter3D class.
    """

    cdef c.Point3D *_Point3D(self) except NULL:
        return <c.Point3D *> self.ptr()

    def __dealloc__(self):
        cdef c.Point3D *p = self._Point3D()
        if self._deallocate:
            del p


    def __init__(self, x=0, y=0, xerrs=0, yerrs=0):
        cutil.set_owned_ptr(self, new c.Point3D())
        self.xy = x, y
        self.xerrs = xerrs
        self.yerrs = yerrs

    def copy(self):
        return cutil.new_owned_cls(Point3D, new c.Point3D(deref(self._Point3D())))


    property x:
        """x coordinate"""
        def __get__(self):
            return self._Point3D().x()
        def __set__(self, x):
            self._Point3D().setX(x)

    property y:
        """y coordinate"""
        def __get__(self):
            return self._Point3D().y()
        def __set__(self, y):
            self._Point3D().setY(y)

    property z:
        """y coordinate"""
        def __get__(self):
            return self._Point3D().z()
        def __set__(self, z):
            self._Point3D().setZ(z)

    property xyz:
        def __get__(self):
            return util.XYZ(self.x, self.y, self.z)
        def __set__(self, val):
            self.x, self.y, self.z = val


    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property xErrs:
        def __get__(self):
            return util.read_error_pair(self._Point3D().xErrs())
        def __set__(self, val):
            self._Point3D().setXErr(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property yErrs:
        def __get__(self):
            return util.read_error_pair(self._Point3D().yErrs())
        def __set__(self, val):
            self._Point3D().setYErr(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property zErrs:
        def __get__(self):
            return util.read_error_pair(self._Point3D().zErrs())
        def __set__(self, val):
            self._Point3D().setZErr(util.read_symmetric(val))


    @property
    def xMin(self):
        """The minimum x position, i.e. lowest error"""
        return self._Point3D().xMin()
    @property
    def xMax(self):
        """The maximum x position, i.e. highest error"""
        return self._Point3D().xMax()

    @property
    def yMin(self):
        """The minimum y position, i.e. lowest error"""
        return self._Point3D().yMin()
    @property
    def yMax(self):
        """The maximum y position, i.e. highest error"""
        return self._Point3D().yMax()

    @property
    def zMin(self):
        """The minimum z position, i.e. lowest error"""
        return self._Point3D().zMin()
    @property
    def zMax(self):
        """The maximum z position, i.e. highest error"""
        return self._Point3D().zMax()


    property xErrAvg:
        def __get__(self):
            return self._Point3D().xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self._Point3D().yErrAvg()

    property zErrAvg:
        def __get__(self):
            return self._Point3D().zErrAvg()


    def scaleX(self, ax):
        """
        (float) -> None
        Scale the x point coordinates by the given factor.
        """
        self._Point3D().scaleX(ax)

    def scaleY(self, ay):
        """
        (float) -> None
        Scale the y point coordinates by the given factor.
        """
        self._Point3D().scaleY(ay)

    def scaleZ(self, az):
        """
        (float) -> None
        Scale the z point coordinates by the given factor.
        """
        self._Point3D().scaleZ(az)

    def scaleXYZ(self, ax=1.0, ay=1.0, az=1.0):
        """
        (float=1.0, float=1.0, float=1.0) -> None
        Scale the point coordinates by the given factors.
        """
        self._Point3D().scaleXYZ(ax, ay, az)

    # TODO: remove
    def scaleXYZ(self, ax=1.0, ay=1.0, az=1.0):
        """
        (double=1.0, double=1.0, double=1.0) -> None
        DEPRECATED: USE scaleXYZ
        Scale the point coordinates by the given factors.
        """
        self.scaleXYZ(ax, ay, az)


    # TODO: transformX,Y,Z


    def __repr__(self):
        return '<Point3D(x=%g, y=%g, z=%g)>' % (self.x, self.y, self.z)

    def __richcmp__(Point3D self, Point3D other, int op):
        if op == 0:
            return deref(self._Point3D()) < deref(other._Point3D())
        elif op == 1:
            return deref(self._Point3D()) <= deref(other._Point3D())
        elif op == 2:
            return deref(self._Point3D()) == deref(other._Point3D())
        elif op == 3:
            return deref(self._Point3D()) != deref(other._Point3D())
        elif op == 4:
            return deref(self._Point3D()) > deref(other._Point3D())
        elif op == 5:
            return deref(self._Point3D()) >= deref(other._Point3D())
