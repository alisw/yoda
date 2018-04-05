cimport util
cdef class Point(util.Base):
    """
    A generic point with errors, used by the Scatter classes.
    """

    cdef c.Point* pptr(self) except NULL:
        return <c.Point*> self.ptr()

    def __dealloc__(self):
        cdef c.Point *p = self.pptr()
        if self._deallocate:
            del p

    # def __init__(self):
    #     cutil.set_owned_ptr(self, new c.Point())

    # def copy(self):
    #     return cutil.new_owned_cls(Point, new c.Point(deref(self.pptr())))

    # TODO: add clone() as mapping to (not yet existing) C++ newclone()?


    @property
    def dim(self):
        """None -> int
        Space dimension of the point (should match containing Scatter)"""
        return self.pptr().dim()


    def val(self, i):
        """int -> float
        Value on axis i"""
        return self.pptr().val(i)

    def setVal(self, i, val):
        """(int, float) -> None
        Value on axis i"""
        self.pptr().setVal(i, val)


    def errs(self, i):
        """int -> float
        Errors on axis i"""
        return util.read_error_pair(self.pptr().errs(i))

    def setErr(self, i, e):
        """(int, float) -> None
        Set symmetric errors on axis i"""
        self.pptr().setErr(i, e)

    def setErrs(self, i, *es):
        """(int, float) -> None
           (int, [float, float]) -> None
           (int, float, float) -> None
        Set asymmetric errors on axis i"""
        errs = es
        if len(es) == 1:
            if not hasattr(es[0], "__iter__"):
                self.setErr(es[0])
            errs = es[0]
        # assert len(errs) == 2:
        self.pptr().setErrs(i, errs)


    def errMinus(self, i):
        """int -> float
        Minus error on axis i"""
        return self.pptr().errMinus(i)

    def setErrMinus(self, i, e):
        """(int, float) -> None
        Set minus error on axis i"""
        self.pptr().setErrMinus(i, e)


    def errPlus(self, i):
        """int -> float
        Plus error on axis i"""
        return self.pptr().errPlus(i)

    def setErrPlus(self, i, e):
        """(int, float) -> None
        Set plus error on axis i"""
        self.pptr().setErrPlus(i, e)


    def errAvg(self, i):
        """int -> float
        Average error on axis i"""
        return self.pptr().errAvg(i)


    def set(self, i, val, *es):
        """(int, float, float) -> None
           (int, float, [float, float]) -> None
           (int, float, float, float) -> None
        Set value and errors on axis i"""
        errs = es
        if len(es) == 1:
            if hasattr(es[0], "__iter__"):
                errs = [es[0], es[0]]
            else:
                errs = es[0]
        # assert len(errs) == 2:
        self.pptr().set(i, val, errs)


    # def __repr__(self):
    #     return '<Point(x=%g)>' % self.x
