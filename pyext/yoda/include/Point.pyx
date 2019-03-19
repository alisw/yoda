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


    def errs(self, i, source=""):
        """int -> float
        Errors on axis i"""
        if source==None: source=""
        return util.read_error_pair(self.pptr().errs(i,source))

    def setErr(self, i, e, source=""):
        """(int, float) -> None
        Set symmetric errors on axis i"""
        if source==None: source=""
        print "LC DEBUG setErr ", e, source
        self.pptr().setErr(i, e, source)

    def setErrs(self, i, *es):
        """(int, float) -> None
           (int, [float, float]) -> None
           (int, float, float) -> None
        Set asymmetric errors on axis i"""
        source=None
        es=list(es)
        if type(es[-1]) is str:
          source=es[-1]
          es=es[:-1]
        else:
          pass
        errs = es
        if source==None: source=""
        if len(errs) == 1:
            if not hasattr(errs[0], "__iter__"):
                self.setErr(i,errs[0], source)
                return
            errs=errs[0]
        # assert len(errs) == 2:
        self.pptr().setErrs(i, tuple(errs), source)


    def errMinus(self, i, source=""):
        """int -> float
        Minus error on axis i"""
        if source==None: source=""
        return self.pptr().errMinus(i ,source)

    def setErrMinus(self, i, e, source=""):
        """(int, float) -> None
        Set minus error on axis i"""
        if source==None: source=""
        self.pptr().setErrMinus(i, e, source)


    def errPlus(self, i, source=""):
        """int -> float
        Plus error on axis i"""
        if source==None: source=""
        return self.pptr().errPlus(i, source)

    def setErrPlus(self, i, e, source=""):
        """(int, float) -> None
        Set plus error on axis i"""
        if source==None: source=""
        self.pptr().setErrPlus(i, e, source)


    def errAvg(self, i, source=""):
        """int -> float
        Average error on axis i"""
        if source==None: source=""
        return self.pptr().errAvg(i, source)


    def set(self, i, val, *es, source=""):
        """(int, float, float) -> None
           (int, float, [float, float]) -> None
           (int, float, float, float) -> None
        Set value and errors on axis i"""
        errs = es
        if source==None: source=""
        if len(es) == 1:
            if hasattr(es[0], "__iter__"):
                errs = [es[0], es[0]]
            else:
                errs = es[0]
        # assert len(errs) == 2:
        self.pptr().set(i, val, errs, source)
    
    def errMap(self):
        """None -> {string: [float,float]} 
        error map of this point"""
        return self.pptr().errMap()


    # def __repr__(self):
    #     return '<Point(x=%g)>' % self.x
