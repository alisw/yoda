
cdef class AnalysisObject(util.Base):
    """
    AnalysisObject is the base class of the main user-facing objects, such as
    the Histo, Profile and Scatter classes.
    """

    # Pointer upcasting mechanism
    cdef inline c.AnalysisObject* aoptr(self) except NULL:
        return <c.AnalysisObject*> self.ptr()

    # Pointer upcasting mechanism
    # DEPRECATED
    cdef inline c.AnalysisObject* _AnalysisObject(self) except NULL:
        return <c.AnalysisObject*> self.ptr()

    # Deallocator (only needed as a base class)
    def __dealloc__(self):
        p = self.aoptr()
        if self._deallocate:
            del p


    @property
    def type(self):
        "String identifier for this type"
        return self.aoptr().type()

    @property
    def dim(self):
        "Fill dimension or plot dimension of this object, for fillables and scatters respectively"
        return self.aoptr().dim()

    @property
    def annotations(self):
        """() -> list[str]
        A list of all annotation/metadata keys."""
        return self.aoptr().annotations()

    def annotation(self, string k, default=None):
        """Get annotation k from this object (falling back to default if not set)."""
        try:
            return util._autotype(self.aoptr().annotation(string(k)))
        except:
            return default

    def setAnnotation(self, string k, v):
        """Set annotation k on this object."""
        self.aoptr().setAnnotation(k, util._autostr(v))

    def hasAnnotation(self, string k):
        """Check if this object has annotation k."""
        return self.aoptr().hasAnnotation(string(k))

    def rmAnnotation(self, string k):
        """Remove annotation k from this object."""
        self.aoptr().rmAnnotation(string(k))

    def clearAnnotations(self):
        """Clear the annotations dictionary."""
        self.aoptr().clearAnnotations()


    def dump(self):
        """A human readable representation of this object."""
        from cStringIO import StringIO
        f = StringIO()
        writeFLAT([self], f)
        f.seek(0)
        return f.read().strip()


    @property
    def name(self):
        """
        Return the histogram name, i.e. the last part of the path (which may be empty).
        """
        return self.aoptr().name().c_str()


    property path:
        """
        Used for persistence and as a unique identifier. Must begin with
        a '/' if not the empty string.
        """
        def __get__(self):
            return self.aoptr().path().c_str()

        def __set__(self, char *path):
            self.aoptr().setPath(string(path))


    property title:
        """
        Convenient access to the histogram title (optional).
        """
        def __get__(self):
            return self.aoptr().title().c_str()

        def __set__(self, char *title):
            self.aoptr().setTitle(string(title))


    def __repr__(self):
        return "<%s '%s'>" % (self.__class__.__name__, self.path)
