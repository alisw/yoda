cdef class Counter(AnalysisObject):
    """
    Weight counter. Like a histogram without any axis (and hence only one bin).

    Call fill() like with a histogram. Sums of weights can be returned, with val()
    and err() being shorthand for the sum of weights and its binomial error.

    Counter(path="", title="").
      Construct a counter with optional path and title but no bins.
    """

    cdef inline c.Counter* cptr(self) except NULL:
        return <c.Counter*> self.ptr()
    # TODO: remove
    cdef inline c.Counter* _Counter(self) except NULL:
        return <c.Counter*> self.ptr()


    def __init__(self, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Counter(string(path), string(title)))


    def __repr__(self):
        return "<%s '%s' sumw=%0.2g, err=%s>" % (self.__class__.__name__, self.path, self.val, self.err)


    def reset(self):
        """None -> None.
        Reset the counter."""
        self.cptr().reset()


    def clone(self):
        """None -> Couner.
        Clone this Counter."""
        return cutil.new_owned_cls(Counter, self.cptr().newclone())


    def fill(self, weight=1.0):
        """([w]) -> None.
        Fill with given optional weight."""
        self.cptr().fill(weight)


    #@property
    def numEntries(self):
        """None -> int
        Number of times this counter was filled."""
        return int(self.cptr().numEntries())

    #@property
    def effNumEntries(self):
        """None -> float
        Effective number of times this counter was filled, computed from weights."""
        return self.cptr().effNumEntries()

    #@property
    def sumW(self):
        """() -> float
        Sum of weights filled into this counter."""
        return self.h1ptr().sumW()

    #@property
    def sumW2(self):
        """() -> float
        Sum of weights filled into this counter."""
        return self.cptr().sumW2()

    @property
    def val(self):
        """() -> float
        Sum of weights filled into this counter."""
        return self.cptr().val()

    @property
    def err(self):
        """() -> float
        Binomial uncertainty on the sum of weights filled into this counter."""
        return self.cptr().err()

    @property
    def relErr(self):
        """() -> float
        Relative binomial uncertainty on the sum of weights filled into this counter."""
        return self.cptr().relErr()


    def scaleW(self, w):
        """ (float) -> None.
        Rescale the weights in this counter by the factor w."""
        self.cptr().scaleW(w)


    # def mkScatter(self):
    #     """None -> Scatter1D.
    #     Convert this Counter to a Scatter1D, with x representing the value and error."""
    #     cdef c.Scatter1D s1 = c.mkScatter_Counter(deref(self.cptr()))
    #     return cutil.new_owned_cls(Scatter1D, s1.newclone())

    # def divide(self, Counter c, efficiency=False):
    #     cdef c.Scatter1D s1
    #     if not efficiency:
    #         s1 = c.Histo1D_div_Histo1D(deref(self.cptr()), deref(c.cptr()))
    #     else:
    #         s1 = c.Histo1D_eff_Histo1D(deref(self.cptr()), deref(c.cptr()))
    #     return cutil.new_owned_cls(Scatter1D, s1.newclone())


    ## In-place special methods

    # def __iadd__(Histo1D self, Histo1D other):
    #     c.Histo1D_iadd_Histo1D(self.h1ptr(), other.h1ptr())
    #     return self

    # def __isub__(Histo1D self, Histo1D other):
    #     c.Histo1D_isub_Histo1D(self.h1ptr(), other.h1ptr())
    #     return self


    ## Unbound special methods

    # def __add__(Histo1D self, Histo1D other):
    #     h = Histo1D()
    #     cutil.set_owned_ptr(h, c.Histo1D_add_Histo1D(self.h1ptr(), other.h1ptr()))
    #     return h

    # def __sub__(Histo1D self, Histo1D other):
    #     h = Histo1D()
    #     cutil.set_owned_ptr(h, c.Histo1D_sub_Histo1D(self.h1ptr(), other.h1ptr()))
    #     return h

    # def __div__(Histo1D self, Histo1D other):
    #     h = Histo1D()
    #     cutil.set_owned_ptr(h, c.Histo1D_div_Histo1D(self.h1ptr(), other.h1ptr()))
    #     return h
