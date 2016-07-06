EXTRA_DIST = \
    __init__.py \
    axis.pyx \
    errors.cpp errors.hh \
    pyroot_helpers.hh merge.hh \
    rootcompat.pxd rootcompat.pyx \
    search.py plotting.py \
    script_helpers.py

CYTHON_SRC = \
    core.pyx \
    declarations.pxd \
    util.pyx \
    util.pxd \
    include/AnalysisObject.pyx \
    include/Counter.pyx \
    include/Axis1D_BIN1D_DBN.pyx \
    include/Axis2D_BIN2D_DBN.pyx \
    include/Bin.pyx \
    include/Bin1D_DBN.pyx \
    include/Bin2D_DBN.pyx \
    include/Dbn0D.pyx \
    include/Dbn1D.pyx \
    include/Dbn2D.pyx \
    include/Dbn3D.pyx \
    include/Errors.pyx \
    include/Functions.pyx \
    include/Histo1D.pyx \
    include/Histo2D.pyx \
    include/HistoBin1D.pyx \
    include/HistoBin2D.pyx \
    include/IO.pyx \
    include/Point1D.pyx \
    include/Point2D.pyx \
    include/Point3D.pyx \
    include/Profile1D.pyx \
    include/Profile2D.pyx \
    include/ProfileBin1D.pyx \
    include/ProfileBin2D.pyx \
    include/Scatter1D.pyx \
    include/Scatter2D.pyx \
    include/Scatter3D.pyx \
    include/Bin1D.pxi \
    include/Bin2D.pxi \
    include/Axis1D.pxi \
    include/Axis2D.pxi \
	include/generated/Bin1D_Dbn1D.pyx \
	include/generated/Bin1D_Dbn2D.pyx \
	include/generated/Bin2D_Dbn2D.pyx \
	include/generated/Bin2D_Dbn3D.pyx

EXTRA_DIST += \
    $(CYTHON_SRC) \
    mktemplates \
    core.cpp core.h util.cpp \
    rootcompat.cpp \
    .made_pyx_templates


if WITH_CYTHON

core.cpp core.h: $(CYTHON_SRC) .made_pyx_templates
	cython $(srcdir)/core.pyx --cplus -I $(srcdir) -I $(srcdir)/include -I $(builddir) -I $(builddir)/include -o $@

util.cpp: util.pxd util.pyx .made_pyx_templates
	cython $(srcdir)/util.pyx --cplus -I $(srcdir) -I $(srcdir)/include -I $(builddir) -I $(builddir)/include -o $@

rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx .made_pyx_templates
	cython $(srcdir)/rootcompat.pyx --cplus -I $(srcdir) -I $(srcdir)/include -I $(builddir) -I $(builddir)/include -o $@

else

core.cpp core.h: $(CYTHON_SRC)
	@echo "Not (re)generating core.cpp since Cython is not installed"

util.cpp: util.pxd util.pyx
	@echo "Not (re)generating util.cpp since Cython is not installed"

rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx
	@echo "Not (re)generating rootcompat.cpp since Cython is not installed"

endif


## Make text-substitution .pyx files from templates -- should only run once
.made_pyx_templates: include/Bin1D_DBN.pyx include/Bin2D_DBN.pyx include/Axis1D_BIN1D_DBN.pyx include/Axis2D_BIN2D_DBN.pyx
	$(srcdir)/mktemplates $^
	touch $@


## fixes for out-of-source builds, especially "make distcheck"

all-local: fix-out-of-source

FIXSOURCES = $(EXTRA_DIST)

fix-out-of-source: $(FIXSOURCES)
	rm -f $@
	mkdir -p include
	for i in $^; do \
	  orig="$$i"; \
	  build="$$(basename $$i)"; \
	  if [[ "$${orig}" == *include* ]]; then build="include/$${build}"; fi; \
	  if [ ! -e $${build} ]; then cp $${orig} $${build}; \
	  echo $${build} >> $@; fi; \
	done
	touch $@

clean-local: fix-out-of-source
	if [ -e $< ]; then if [ -n "$$(cat $<)" ]; then rm -f $$(cat $<); fi; fi
	rm -f $<

distclean-local:
	rm -f core.cpp core.h util.cpp rootcompat.cpp include/generated/* include/*.pxi .made_pyx_templates
