cimport util
cdef class Point3D(Point):
    """
    A 3D point with errors, used by the Scatter3D class.
    """

    cdef c.Point3D* p3ptr(self) except NULL:
        return <c.Point3D*> self.ptr()


    def __init__(self, x=0, y=0, z=0, xerrs=0, yerrs=0, zerrs=0):
        cutil.set_owned_ptr(self, new c.Point3D())
        self.xyz = x, y, z
        self.xErrs = xerrs
        self.yErrs = yerrs
        self.zErrs = zerrs

    def copy(self):
        return cutil.new_owned_cls(Point3D, new c.Point3D(deref(self.p3ptr())))


    property x:
        """x coordinate"""
        def __get__(self):
            return self.p3ptr().x()
        def __set__(self, x):
            self.p3ptr().setX(x)

    property y:
        """y coordinate"""
        def __get__(self):
            return self.p3ptr().y()
        def __set__(self, y):
            self.p3ptr().setY(y)

    property z:
        """y coordinate"""
        def __get__(self):
            return self.p3ptr().z()
        def __set__(self, z):
            self.p3ptr().setZ(z)

    property xyz:
        def __get__(self):
            return util.XYZ(self.x, self.y, self.z)
        def __set__(self, val):
            self.x, self.y, self.z = val


    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property xErrs:
        def __get__(self):
            return util.read_error_pair(self.p3ptr().xErrs())
        def __set__(self, val):
            self.p3ptr().setXErrs(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property yErrs:
        def __get__(self):
            return util.read_error_pair(self.p3ptr().yErrs())
        def __set__(self, val):
            self.p3ptr().setYErrs(util.read_symmetric(val))

    # TODO: How does this fit into the multi-error API? Still useful, but just reports first errs... how to get _all_ +- err pairs?
    property zErrs:
        def __get__(self):
            return util.read_error_pair(self.p3ptr().zErrs())
        def __set__(self, val):
            self.p3ptr().setZErrs(util.read_symmetric(val))


    @property
    def xMin(self):
        """The minimum x position, i.e. lowest error"""
        return self.p3ptr().xMin()
    @property
    def xMax(self):
        """The maximum x position, i.e. highest error"""
        return self.p3ptr().xMax()

    @property
    def yMin(self):
        """The minimum y position, i.e. lowest error"""
        return self.p3ptr().yMin()
    @property
    def yMax(self):
        """The maximum y position, i.e. highest error"""
        return self.p3ptr().yMax()

    @property
    def zMin(self):
        """The minimum z position, i.e. lowest error"""
        return self.p3ptr().zMin()
    @property
    def zMax(self):
        """The maximum z position, i.e. highest error"""
        return self.p3ptr().zMax()


    property xErrAvg:
        def __get__(self):
            return self.p3ptr().xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self.p3ptr().yErrAvg()

    property zErrAvg:
        def __get__(self):
            return self.p3ptr().zErrAvg()


    def scaleX(self, ax):
        """
        (float) -> None
        Scale the x point coordinates by the given factor.
        """
        self.p3ptr().scaleX(ax)

    def scaleY(self, ay):
        """
        (float) -> None
        Scale the y point coordinates by the given factor.
        """
        self.p3ptr().scaleY(ay)

    def scaleZ(self, az):
        """
        (float) -> None
        Scale the z point coordinates by the given factor.
        """
        self.p3ptr().scaleZ(az)

    def scaleXYZ(self, ax=1.0, ay=1.0, az=1.0):
        """
        (float=1.0, float=1.0, float=1.0) -> None
        Scale the point coordinates by the given factors.
        """
        self.p3ptr().scaleXYZ(ax, ay, az)

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
            return deref(self.p3ptr()) < deref(other.p3ptr())
        elif op == 1:
            return deref(self.p3ptr()) <= deref(other.p3ptr())
        elif op == 2:
            return deref(self.p3ptr()) == deref(other.p3ptr())
        elif op == 3:
            return deref(self.p3ptr()) != deref(other.p3ptr())
        elif op == 4:
            return deref(self.p3ptr()) > deref(other.p3ptr())
        elif op == 5:
            return deref(self.p3ptr()) >= deref(other.p3ptr())
