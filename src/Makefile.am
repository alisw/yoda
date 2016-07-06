SUBDIRS = tinyxml .

lib_LTLIBRARIES = libYODA.la

libYODA_la_SOURCES = \
    Reader.cc \
    ReaderYODA.cc \
    ReaderFLAT.cc \
    ReaderAIDA.cc \
    Writer.cc \
    WriterYODA.cc \
    WriterFLAT.cc \
    WriterAIDA.cc \
    Dbn0D.cc \
	Dbn1D.cc \
    Counter.cc \
    Histo1D.cc \
    Histo2D.cc \
    Profile1D.cc \
    Profile2D.cc \
    Scatter1D.cc \
    Scatter2D.cc \
    Scatter3D.cc


libYODA_la_LDFLAGS = -avoid-version
libYODA_la_LIBADD = tinyxml/libtinyxml.la
libYODA_la_CPPFLAGS = $(AM_CPPFLAGS) -DTIXML_USE_STL
