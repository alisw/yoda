import yoda
import numpy as np
import sys


class NumpyHist(object):

    def __init__(self, ao):
        if not isinstance(ao, yoda.AnalysisObject):
            raise Exception("ao argument must be a YODA AnalysisObject; this is a %s" % type(ao))
        ## Get annotations
        self.path = ao.path
        #self.annotations = {aname : ao.annotation(aname) for aname in ao.annotations}
        self.annotations = {}
        for aname in ao.annotations:
            self.annotations[aname] = ao.annotation(aname)
        ## Convert to Scatter and set dimensionality & recarray column names
        s = ao.mkScatter()
        names = ['x', 'y', 'exminus', 'explus', 'eyminus', 'eyplus']
        if type(s) is yoda.Scatter2D:
            self.dim = 2
        elif type(s) is yoda.Scatter3D:
            self.dim = 3
            names.insert(2, "z")
            names += ['ezminus', 'ezplus']
        else:
            raise RuntimeError("Whoa! If ao doesn't convert to a 2D or 3D scatter, what is it?!")
        ## Put data points into numpy structure
        dtype = {"names": names, "formats": ["f4" for _ in names]}
        self.data = np.zeros(len(s.points), dtype).view(np.recarray)
        for i, p in enumerate(s.points):
            self.data.x[i] = p.x
            self.data.exminus[i] = p.xErrs[0]
            self.data.explus[i]  = p.xErrs[1]
            self.data.y[i] = p.y
            self.data.eyminus[i] = p.yErrs[0]
            self.data.eyplus[i]  = p.yErrs[1]
            if self.dim > 2:
                self.data.z[i] = p.z
                self.data.ezminus[i] = p.zErrs[0]
                self.data.ezplus[i]  = p.zErrs[1]


    def __len__(self):
        return len(self.x)


    @property
    def xedges_sgl(self):
        return np.append(self.xmin, self.xmax[-1])

    @property
    def xedges_dbl(self):
        edges = np.empty((2*len(self.x),), dtype=self.x.dtype)
        edges[0::2] = self.xmin
        edges[1::2] = self.xmax
        return edges


    @property
    def xmin(self):
        return self.x - self.exminus

    @property
    def xmax(self):
        return self.x + self.explus


    @property
    def ymin(self):
        return self.y - self.eyminus

    @property
    def ymax(self):
        return self.y + self.eyplus


    # TODO: automate more with __get/setattr__?

    @property
    def x(self):
        return self.data.x

    @property
    def exminus(self):
        return self.data.exminus

    @property
    def explus(self):
        return self.data.explus


    @property
    def y(self):
        return self.data.y

    @property
    def eyminus(self):
        return self.data.eyminus

    @property
    def eyplus(self):
        return self.data.eyplus


    # TODO: don't provide these / throw helpful errors if only 2D

    @property
    def z(self):
        return self.data.z

    @property
    def ezminus(self):
        return self.data.ezminus

    @property
    def ezplus(self):
        return self.data.ezplus


    # def __getattr__(self, attr):
    #     "Fall back to the data array for attributes not defined on NumpyHist"
    #     return getattr(self.data, attr)


    def same_binning_as(self, other):
        if self.dim != other.dim:
            return False
        if not (other.x == self.x).all() and \
               (other.exminus == self.exminus).all() and \
               (other.explus == self.explus).all():
            return False
        if self.dim == 2:
            return True
        return (other.y == self.y).all() and \
               (other.eyminus == self.eyminus).all() and \
               (other.eyplus == self.eyplus).all()



def mk_numpyhist(h):
    return h if type(h) is NumpyHist else NumpyHist(h)


##########

def read_plot_keys(datfile):
    import re
    re_begin = re.compile("#*\s*BEGIN\s+PLOT\s*(\w*)")
    re_comment = re.compile("#.*")
    re_attr = re.compile("(\w+)\s*=\s*(.*)")
    re_end = re.compile("#*\s*END\s+PLOT\s+\w*")
    plotkeys = {}
    with open(datfile) as f:
        inplot = False
        name = None
        for line in f:
            l = line.strip()
            if re_begin.match(l):
                inplot = True
                name = re_begin.match(l).group(1)
            elif re_end.match(l):
                inplot = False
                name = None
            elif re_comment.match(l):
                continue
            elif inplot:
                m = re_attr.match(l)
                if m is None: continue
                plotkeys.setdefault(name, {})[m.group(1)] = m.group(2)
    return plotkeys


## Plotting helper functions

import matplotlib as mpl

def setup_mpl(engine="PGF", font="TeX Gyre Pagella", fontsize=17, mfont=None, textfigs=True):
    """One-liner matplotlib (mpl) setup.

    By default mpl will be configured with the TeX PGF rendering backend, and a
    Palatino-like font for both text and math contexts, using 'lower-case
    numerals' if supported. Setting the engine to 'TEX' will use standard mpl
    rendering, with calls to LaTeX for axis labels and other text; setting it to
    'MPL' will use the built-in mpl MathText renderer. MPL mode only supports a
    limited set of LaTeX macros and does not render as well as TeX, but it is
    faster and can be used to render to an interactive window.

    The font and mfont optional arguments can be used to choose a different text
    font and math font respectively; if mfont is None, it defaults to the same
    as the text font. The textfigs boolean argument can be set false to disable
    the lower-case/text/old-style numerals and use 'upper-case' numerals
    everywhere. These options do not currently apply to the MPL rendering engine.
    """
    # import matplotlib as mpl
    mpl.rcParams.update({
        "text.usetex" : (engine != "MPL"),
        "font.size"   : int(fontsize),
        "font.family" : "serif", #< TODO: make configurable? auto-detect?
        })

    texpreamble = [r"\usepackage{amsmath,amssymb}", r"\usepackage{mathspec}"]
    mfont = mfont if mfont else font
    fontopts = "[Numbers=OldStyle]" if textfigs else ""
    mfontopts = fontopts.replace("]", ",") + "Scale=MatchUppercase" + "]"
    texpreamble.append( r"\setmainfont{fopts}{{{font}}}".format(fopts=fontopts, font=font) )
    texpreamble.append( r"\setmathsfont(Digits,Latin){fopts}{{{font}}}".format(fopts=mfontopts, font=mfont) )

    if engine == "PGF":
        mpl.use("pgf")
        mpl.rcParams["pgf.preamble"] = texpreamble
    elif engine == "TEX":
        mpl.rcParams["tex.preamble"] = texpreamble

    # TODO: do we need plt?
    from matplotlib import pyplot as plt
    return mpl, plt


def mk_figaxes_1d(ratio=True, title=None, figsize=(8,6)):
    "Make figure and subplot grid layout"

    if "plt" not in dir():
        mpl, plt = setup_mpl()

    # TODO: Eliminate plt? Requires manual work to set up the backend-specific
    # canvas, but would be better for 'more local' memory management
    fig = plt.figure(figsize=figsize)
    # fig = mpl.figure.Figure(figsize=figsize, tight_layout=True)

    if title:
        fig.suptitle(title, horizontalalignment="left", x=0.13)


    ## Make axes. GridSpec may not be available, in which case fall back ~gracefully
    axmain, axratio = None, None
    if ratio:
        try:
            gs = mpl.gridspec.GridSpec(2, 1, height_ratios=[3,1], hspace=0)
            axmain = fig.add_subplot(gs[0])
            axmain.hold(True)
            axratio = fig.add_subplot(gs[1], sharex=axmain)
            axratio.hold(True)
            axratio.axhline(1.0, color="gray") #< Ratio = 1 marker line
        except:
            sys.stderr.write("matplotlib.gridspec not available: falling back to plotting without a ratio\n")
            ratio = False
    if not ratio:
        axmain = fig.add_subplot(1,1,1)
        axmain.hold(True)

    return fig, axmain, axratio


def set_axis_labels_1d(axmain, axratio, xlabel=None, ylabel=None, ratioylabel=None):
    axmain.set_ylabel(ylabel, y=1, ha="right", labelpad=None)
    if axratio:
        axmain.xaxis.set_major_locator(mpl.ticker.NullLocator())
        axratio.set_xlabel(xlabel, x=1, ha="right", labelpad=None)
        axratio.set_ylabel(ratioylabel)
    else:
        axmain.set_xlabel(xlabel, x=1, ha="right", labelpad=None)


# TODO: Needs generalisation for 2D marginal axes)
def setup_axes_1d(axmain, axratio, plotkeys):

    ## Axis labels first
    xlabel = plotkeys.get("XLabel", "")
    ylabel = plotkeys.get("YLabel", "")
    ratioylabel = plotkeys.get("RatioYLabel", "Ratio")
    set_axis_labels_1d(axmain, axratio, xlabel, ylabel, ratioylabel)

    ## log/lin measures
    # TODO: Dynamic default based on data ranges?
    # TODO: take log axes and preference for round numbers into account in setting default axis limits
    xmeasure = "log" if yoda.util.as_bool(plotkeys.get("LogX", False)) else "linear"
    ymeasure = "log" if yoda.util.as_bool(plotkeys.get("LogY", False)) else "linear"
    ratioymeasure = "log" if yoda.util.as_bool(plotkeys.get("RatioLogY", False)) else "linear"
    axmain.set_xscale(xmeasure)
    axmain.set_yscale(ymeasure)
    if axratio:
        axratio.set_xscale(xmeasure)
        axratio.set_yscale(ratioymeasure)

    ## Plot range limits
    if plotkeys.has_key("YMin"):
        axmain.set_ylim(bottom=float(plotkeys.get("YMin")))
    if plotkeys.has_key("YMax"):
        axmain.set_ylim(top=float(plotkeys.get("YMin")))
    #
    if plotkeys.has_key("XMin"):
        axmain.set_xlim(left=float(plotkeys.get("XMin")))
    if plotkeys.has_key("XMax"):
        axmain.set_xlim(right=float(plotkeys.get("XMin")))
    #
    if axratio:
        # TODO: RatioSymmRange option
        # axratio.set_xlim([xmin-0.001*xdiff, xmax+0.001*xdiff]) # <- TODO: bad on a log scale!
        if plotkeys.has_key("XMin"):
            axratio.set_xlim(left=float(plotkeys.get("XMin")))
        if plotkeys.has_key("XMax"):
            axratio.set_xlim(right=float(plotkeys.get("XMin")))
        if plotkeys.has_key("RatioYMin"):
            axratio.set_ylim(bottom=float(plotkeys.get("RatioYMin")))
        if plotkeys.has_key("RatioYMax"):
            axratio.set_ylim(top=float(plotkeys.get("RatioYMax")))

    # TODO: Ratio plot manual ticks


# TODO: rename to be more obviously an ~internal helper
def plot_hist_on_axes_1d(axmain, axratio, h, href=None):
    if "plt" not in dir():
        mpl, plt = setup_mpl()

    h = mk_numpyhist(h)

    # TODO: Split into different plot styles: line/filled/range, step/diag/smooth, ...?

    ## Styles
    default_color = h.annotations.get("Color", "black")
    marker = h.annotations.get("Marker", h.annotations.get("PolyMarker", None)) # <- make-plots translation
    marker = {"*":"o"}.get(marker, marker) # <- make-plots translation
    mcolor = h.annotations.get("LineColor", default_color)
    errbar = h.annotations.get("ErrorBars", None)
    ecolor = h.annotations.get("ErrorBarsColor", default_color)
    line = h.annotations.get("Line", None)
    lcolor = h.annotations.get("LineColor", default_color)
    lstyle = h.annotations.get("LineStyle", "-")
    lstyle = {"solid":"-", "dashed":"--", "dotdashed":"-.", "dashdotted":"-.", "dotted":":"}.get(lstyle, lstyle) # <- make-plots translation
    lwidth = 1.4
    msize = 7

    ## If no drawing is enabled, default to a step line
    if not any(h.annotations.get(a) for a in ("Marker", "Line", "ErrorBars")):
        line = "step"

    ## Plotting
    # TODO: Split this into different functions for each kind of data preparation (and smoothing as an extra function?)
    artists = None
    if errbar:
        artists = axmain.errorbar(h.x, h.y, xerr=h.exminus, yerr=h.eyminus, color=ecolor, linestyle="none", linewidth=lwidth, capthick=lwidth) # linestyle="-", marker="o",
    if line == "step":
        artists = axmain.step(np.append(h.xmin, h.xmax[-1]), np.append(h.y, h.y[-1]), where="post", color=lcolor, linestyle=lstyle, linewidth=lwidth)
    elif line == "diag":
        artists = axmain.plot(h.x, h.y, color=lcolor, linestyle=lstyle, linewidth=lwidth)
    elif line == "smooth":
        from scipy.interpolate import spline
        xnew = np.linspace(h.x.min(), h.x.max(), 3*len(h))
        ynew = spline(h.x, h.y, xnew)
        artists = axmain.plot(xnew, ynew, color=lcolor, linestyle=lstyle, linewidth=lwidth)
    if marker:
        artists = axmain.plot(h.x, h.y, marker=marker, markersize=msize, linestyle="none", color=mcolor, markeredgecolor=mcolor)

    ## Legend entry
    label = h.annotations.get("Title", None)
    if label and artists:
        artists[0].set_label(label)

    ## Ratio
    ratioartists = None
    if href and h is not href:
        # TODO: exclude and specify order via RatioIndex
        assert h.same_binning_as(href)
        # TODO: log ratio or #sigma deviation
        yratios = h.y/href.y
        # TODO: Same styling control as for main plot (with Ratio prefix, default to main plot style)
        ## Stepped plot
        ratioartists = axratio.step(href.xedges_sgl, np.append(yratios, yratios[-1]), where="post", color=lcolor, linestyle=lstyle, linewidth=lwidth)
        # TODO: Diag plot
        # axratio.plot(href["x"], yratios, color="r", linestyle="--")
        # TODO: Smoothed plot

    return artists


# TODO: Add arg for MPL setup?
def plot_hists_1d(hs, outfile=None, ratio=None, plotkeys={}):
    """Plot the given histograms on a single figure, returning the 3-tuple of
    (fig, main_axis, ratio_axis), and saving to outfile if it is given."""

    if "plt" not in dir():
        mpl, plt = setup_mpl()

    #print hs
    hs = [mk_numpyhist(h) for h in hs]

    ## Get data ranges (calculated or forced)
    # TODO: Round up calc'd ymax to nearest round number within 10% of ydiff, to create a top tick label... sensitive to log/lin measure
    xmin = float(plotkeys.get("XMin", min(min(h.xmin) for h in hs)))
    xmax = float(plotkeys.get("XMax", max(max(h.xmax) for h in hs)))
    xdiff = xmax - xmin
    # print xmin, xmax, xdiff
    ymin = float(plotkeys.get("YMin", min(min(h.ymin) for h in hs)))
    ymax = float(plotkeys.get("YMax", max(max(h.ymax) for h in hs)))
    ydiff = ymax - ymin
    # print ymin, ymax, ydiff

    ## Identify reference histo by annotation (unless explicitly disabled)
    href = None
    # TODO: Use ratio to setdefault RatioPlot in plotkeys, then use that to decide whether to look for href
    if ratio is not False:
        for h in hs:
            if yoda.util.as_bool(h.annotations.get("RatioRef", False)):
                if href is None:
                    href = h
                else:
                    print "Multiple ratio references set: using first value = {}".format(href.path)

    ## Make figure and subplot grid layout
    title = plotkeys.get("Title", "")
    fig, axmain, axratio = mk_figaxes_1d(href, title)

    ## Setup axes appearances
    axmain.set_xlim([xmin, xmax])
    axmain.set_ylim([ymin, ymax])
    if axratio:
        axratio.set_xlim([xmin, xmax])
        axratio.set_ylim(auto=True)
    setup_axes_1d(axmain, axratio, plotkeys)

    # TODO: specify ratio display in log/lin, abs, or #sigma, and as x/r or (x-r)/r

    ## Draw ratio error band (do this before looping over cmp lines)
    # TODO: Actually we can call this when we hit the href, and force the zorder into groups: bands, lines, dots, legend, text, frame
    if axratio:
        ref_ymax_ratios = href.ymax/href.y
        ref_ymin_ratios = href.ymin/href.y
        # TODO: Diag: (needs -> limit handling at ends)
        # axratio.fill_between(href.x, ref_ymin_ratios, ref_ymax_ratios, edgecolor="none", facecolor=ratioerrcolor, interpolate=False)
        # Stepped:
        def dbl_array(arr):
            return sum(([x,x] for x in arr), [])
        ratioerrcolor = plotkeys.get("RatioErrColor", "yellow")
        axratio.fill_between(href.xedges_dbl, dbl_array(ref_ymin_ratios), dbl_array(ref_ymax_ratios),
                             edgecolor="none", facecolor=ratioerrcolor)
        # TODO: Smoothed: (needs -> limit handling at ends)
        # Redraw ratio = 1 marker line:
        axratio.axhline(1.0, color="gray")

    ## Dataset plotting
    for ih, h in enumerate(hs):
        #print ih, h.path
        plot_hist_on_axes_1d(axmain, axratio, h, href)

    ## Legend
    # TODO: allow excluding and specify order via LegendIndex
    axmain.legend(loc=plotkeys.get("LegendPos", "best"), fontsize=plotkeys.get("LegendFontSize", "x-small"), frameon=False)

    ## Tweak layout now that everything is in place
    # TODO: merge tight_layout() into the Figure constructor, and maybe the ratio ticker when retrospectively drawing the zorder'ed err band
    if axratio:
        axratio.yaxis.set_major_locator(mpl.ticker.MaxNLocator(4, prune="upper"))
    fig.tight_layout()

    ## Save to an image file if we were asked to
    if outfile:
        #print "Saving to " + outfile
        fig.savefig(outfile)

    ## Return the figure objects
    return fig, axmain, axratio


# TODO: Add arg for MPL setup?
def plot_hist_1d(h, outfile=None, plotkeys={}):
    "Plot the given histogram on a single figure without a ratio plot, returning the 2-tuple of (fig, main_axis)."
    f, ax, _ = plot_hists_1d([h,], outfile=outfile, ratio=False, plotkeys=plotkeys)
    return f, ax


def plot(hs, outfile=None, plotkeys={}, ratio=None):
    """Plot the given histogram(s) on a single figure, maybe with a ratio plot,
    and return the 2-tuple of (fig, (main_axis,ratio_axis)). If an outfile is
    given, it will be saved to before returning the figure objects."""
    # TODO: Handle 2D plots, too.
    #print hs
    if type(hs) in (list, tuple):
        f, axm, axr = plot_hists_1d(hs, outfile=outfile, ratio=ratio, plotkeys=plotkeys)
    else:
        f, axm, axr = plot_hists_1d([hs,], outfile=outfile, ratio=False, plotkeys=plotkeys)
    return f, (axm, axr)


def _plot1arg(args):
    "Helper function for mplot, until pool.starmap() is available"
    # hs, outfile, ratio, plotkeys = args
    return plot(*args)

def mplot(hs, outfiles=None, plotkeys={}, ratio=None, nproc=None):
    """Plot the given list of histogram(s) using the Python multiprocessing
    module to distribute the work on to multiple parallel processes. This is
    just syntactic sugar for something fairly easily done by the user.

    hs must be an iterable, each entry of which will be the content of a single
    plot: the entries can either be single histograms or lists of histograms,
    i.e. either kind of valid first argument to plot().

    The outfiles, plotkeys, and ratio arguments can either be iterables of valid
    corresponding plot() args, or single instances of such args to be applied to
    all the plots.

    The nproc argument should be the integer number of parallel processes on
    which to distribute the plotting. nproc = None (the default value) will use
    Ncpu-1 or 1 process, whichever is larger. If nproc = 1, multiprocessing will
    not be used -- this may ease debugging.

    The return value is a list of the return tuples from each call to plot(), of
    the same length as the hs arg.
    """

    argslist = []
    for i, hs_arg in enumerate(hs):
        hs_arg = [mk_numpyhist(h) for h in hs_arg] if type(hs_arg) in (list,tuple) else mk_numpyhist(hs_arg)
        outfile_arg = outfiles[i] if outfiles else None
        plotkeys_arg = plotkeys if type(plotkeys) is dict else plotkeys[i]
        ratio_arg = ratio[i] if hasattr(ratio, "__iter__") else ratio
        argslist.append( (hs_arg, outfile_arg, plotkeys_arg, ratio_arg) )
    #print argslist

    import multiprocessing
    nproc = nproc or multiprocessing.cpu_count()-1 or 1
    if nproc > 1:
        pool = multiprocessing.Pool(processes=nproc)
        res = pool.map_async(_plot1arg, argslist)
        rtn = res.get()
    else:
        ## Run this way in the 1 proc case for easier debugging
        rtn = [_plot1arg(args) for args in argslist]

    return rtn
