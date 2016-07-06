// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ROOTCnv_h
#define YODA_ROOTCnv_h

#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TVectorF.h"

namespace YODA {


  /// @name Conversion functions from ROOT to YODA data types
  //@{

  /// @todo Check that direct Scatter filling gives the same result at mkScatter(h) for ROOT -> YODA

  /// @todo toProfile1D: TProfile -> Profile1D

  /// @todo toScatter2D: TGraph(AsymmErrs) -> Scatter2D


  // /// @brief Convert a ROOT 1D histogram to a YODA Histo1D
  // ///
  // /// Note that ROOT's histograms do not contain enough information to properly rebuild
  // /// @a x distributions within bins, in underflow and overflow bins, or across the whole histogram.
  // inline Histo1D toHisto1D(const TH1& th1) {
  //   std::vector<HistoBin1D> bins;
  //   TArrayD sumw2s = *th1.GetSumw2();
  //   Dbn1D dbn_uflow, dbn_oflow;
  //   double sumWtot(0), sumW2tot(0)
  //   for (int i = 0; i =< th1.GetNbinsX()+1; ++i) {
  //     Dbn1D dbn(static_cast<unsigned long>(th1.GetBinContent(i)), th1.GetBinContent(i), sumw2s[i], 0, 0);
  //               // th1.GetBinContent(i)*th1.GetBinCenter(i), th1.GetBinContent(i)*sqr(th1.GetBinCenter(i)));
  //     if (i == 0) dbn_uflow = dbn;
  //     else if (i == th1.GetNbinsX()+1) dbn_oflow = dbn;
  //     else bins.push_back(HistoBin1D(std::make_pair(th1.GetBinLowEdge(i), th1.GetBinLowEdge(i+1)), dbn));
  //     sumWtot += th1.GetBinContent(i);
  //     sumW2tot += sumw2s[i];
  //   }
  //   Dbn1D dbn_tot(static_cast<unsigned long>(th1.GetEntries()), sumWtot, sumW2tot, 0, 0);

  //   Histo1D rtn(bins, dbn_tot, dbn_uflow, const Dbn1D& dbn_oflow, th1.GetName(), th1.GetTitle());
  //   rtn.addAnnotation("XLabel", th1.GetXaxis->GetTitle());
  //   rtn.addAnnotation("YLabel", th1.GetYaxis->GetTitle());
  //   return rtn;
  // }


  // /// @brief Convert a ROOT 1D histogram to a YODA Histo1D
  // ///
  // /// Note that ROOT's histograms do not contain enough information to properly rebuild
  // /// @a x distributions within bins, in underflow and overflow bins, or across the whole histogram.
  // inline Histo1D toHisto1D(const TH1* th1) {
  //   return toHisto1D(*th1);
  // }


  /////////////////////


  /// Convert a ROOT 1D histogram (including TProfile) to a YODA Scatter2D
  ///
  /// @todo Add a bool flag for whether or not to divide y vals/errs by bin width.
  inline Scatter2D toScatter2D(const TH1& th1) {
    Scatter2D rtn;
    for (int i = 1; i <= th1.GetNbinsX(); ++i) {
      const double x = th1.GetBinCenter(i);
      const double exminus = x - th1.GetBinLowEdge(i);
      const double explus = th1.GetBinLowEdge(i+1) - x;
      const double width = exminus + explus;
      rtn.addPoint(x, th1.GetBinContent(i)/width,
                   exminus, explus,
                   th1.GetBinErrorLow(i)/width, th1.GetBinErrorUp(i)/width);
    }
    rtn.addAnnotation("XLabel", th1.GetXaxis()->GetTitle());
    rtn.addAnnotation("YLabel", th1.GetYaxis()->GetTitle());
    return rtn;
  }


  /// Convert a ROOT 1D histogram (including TProfile) to a YODA Scatter2D
  inline Scatter2D toScatter2D(const TH1* th1) {
    return toScatter2D(*th1);
  }

  //@}



  /////////////////////



  /// @name Conversion functions from YODA to ROOT data types
  //@{

  /// @brief Convert a YODA Histo1D to a ROOT 1D histogram
  ///
  /// @todo Check/improve/extend -- needs SetBinError or not?
  inline TH1D toTH1D(const Histo1D& h) {
    // Work out bin edges first
    std::vector<double> edges;
    edges.reserve(h.numBins()+1);
    edges.push_back(h.bin(0).xMin());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const HistoBin1D& b = h.bin(i);
      if (!fuzzyEquals(edges.back(), b.xMin())) edges.push_back(b.xMin());
      if (!fuzzyEquals(edges.back(), b.xMax())) edges.push_back(b.xMax());
    }
    // Book ROOT histogram
    TH1D rtn(h.path().c_str(), h.title().c_str(), edges.size()-1, &edges[0]);
    rtn.Sumw2();
    TArrayD& sumw2s = *rtn.GetSumw2();
    for (int i = 1; i <= rtn.GetNbinsX(); ++i) {
      try {
        const HistoBin1D& b = h.binAt(rtn.GetBinCenter(i)); // throws if in a gap
        rtn.SetBinContent(i, b.sumW());
        sumw2s[i] = b.sumW2();
      } catch (const Exception& e) {  }
    }
    // Overflows
    rtn.SetBinContent(0, h.underflow().sumW());
    rtn.SetBinContent(rtn.GetNbinsX()+1, h.overflow().sumW());
    sumw2s[0] = h.underflow().sumW2();
    sumw2s[rtn.GetNbinsX()+1] = h.overflow().sumW2();
    // Labels
    if (h.hasAnnotation("XLabel")) rtn.SetXTitle(h.annotation("XLabel").c_str());
    if (h.hasAnnotation("YLabel")) rtn.SetYTitle(h.annotation("YLabel").c_str());
    return rtn;
  }



  /// @brief Convert a YODA Histo2D to a ROOT 2D histogram
  ///
  /// @todo Check/improve/extend -- needs SetBinError or not?
  inline TH2D toTH2D(const Histo2D& h) {
    // Work out bin edges first
    std::vector<double> xedges, yedges;
    xedges.reserve(h.numBins()+1);
    yedges.reserve(h.numBins()+1);
    xedges.push_back(h.bin(0).xMin());
    yedges.push_back(h.bin(0).yMin());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const HistoBin2D& b = h.bin(i);
      xedges.push_back(b.xMin());
      xedges.push_back(b.xMax());
      yedges.push_back(b.yMin());
      yedges.push_back(b.yMax());
    }
    // Sort and remove (fuzzy) duplicate edges
    std::sort(xedges.begin(), xedges.end());
    std::sort(yedges.begin(), yedges.end());
    const std::vector<double>::iterator xlast = std::unique(xedges.begin(), xedges.end());
    const std::vector<double>::iterator ylast = std::unique(yedges.begin(), yedges.end());
    xedges.erase(xlast, xedges.end());
    yedges.erase(ylast, yedges.end());

    // Book ROOT histogram
    TH2D rtn(h.path().c_str(), h.title().c_str(), xedges.size()-1, &xedges[0], yedges.size()-1, &yedges[0]);
    rtn.Sumw2();
    TArrayD& sumw2s = *rtn.GetSumw2();
    for (int ix = 1; ix <= rtn.GetNbinsX(); ++ix) {
      for (int iy = 1; iy <= rtn.GetNbinsY(); ++iy) {
        const int i = rtn.GetBin(ix, iy);
        try {
          const HistoBin2D& b = h.binAt(rtn.GetBinCenter(ix), rtn.GetBinCenter(iy)); // throws if in a gap
          rtn.SetBinContent(i, b.sumW());
          sumw2s[i] = b.sumW2();
        } catch (const Exception& e) {  }
      }
    }
    // Overflows
    /// @todo Connect up when supported in YODA... if 2D overflows are possible in ROOT?!
    // rtn.SetBinContent(0, h.underflow().sumW());
    // rtn.SetBinContent(rtn.GetNbinsX()+1, h.overflow().sumW());
    // sumw2s[0] = h.underflow().sumW2();
    // sumw2s[rtn.GetNbinsX()+1] = h.overflow().sumW2();
    // Labels
    if (h.hasAnnotation("XLabel")) rtn.SetXTitle(h.annotation("XLabel").c_str());
    if (h.hasAnnotation("YLabel")) rtn.SetYTitle(h.annotation("YLabel").c_str());
    if (h.hasAnnotation("ZLabel")) rtn.SetZTitle(h.annotation("ZLabel").c_str());
    return rtn;
  }



  /// @brief Convert a YODA Scatter2D to a ROOT TH1D


  /// @brief Convert a YODA Profile1D to a ROOT TProfile
  ///
  /// @todo Check/improve/extend. How to set all the y-weights in ROOT profiles?
  inline TProfile toTProfile(const Profile1D& p) {
    // Work out bin edges first
    std::vector<double> edges;
    edges.reserve(p.numBins()+1);
    edges.push_back(p.bin(0).xMin());
    for (size_t i = 0; i < p.numBins(); ++i) {
      const ProfileBin1D& b = p.bin(i);
      if (!fuzzyEquals(edges.back(), b.xMin())) edges.push_back(b.xMin());
      if (!fuzzyEquals(edges.back(), b.xMax())) edges.push_back(b.xMax());
    }
    // Book ROOT histogram
    TProfile rtn(p.path().c_str(), p.title().c_str(), edges.size()-1, &edges[0]);
    rtn.Sumw2();
    Double_t* sumwys = rtn.GetArray(); //< YUCK!!!
    TArrayD& sumwy2s = *rtn.GetSumw2(); //< YUCK!!!
    for (int i = 1; i <= rtn.GetNbinsX(); ++i) {
      try {
        const ProfileBin1D& b = p.binAt(rtn.GetBinCenter(i)); // throws if in a gap
        // rtn.SetBinContent(i, b.mean());
        // rtn.SetBinError(i, b.stdErr());
        /// @todo Need to set the following, according to Roman Lysak:
        // - for sum(y*y): TProfile::GetSumw2()
        // - for sum(y):   TProfile::GetArray()
        // - for sum(w):   TProfile::SetBinEntries(bin, w)
        // Clearly, the names of accessors/methods are confusing...
        rtn.SetBinEntries(i, b.sumW());
        sumwys[i] = b.sumWY();
        sumwy2s[i] = b.sumWY2();
      } catch (const Exception& e) {  }
    }
    // Overflows
    rtn.SetBinEntries(0, p.underflow().sumW());
    rtn.SetBinEntries(rtn.GetNbinsX()+1, p.overflow().sumW());
    sumwys[0] = p.underflow().sumWY();
    sumwys[0] = p.underflow().sumWY();
    sumwy2s[rtn.GetNbinsX()+1] = p.overflow().sumWY2();
    sumwy2s[rtn.GetNbinsX()+1] = p.overflow().sumWY2();
    // Labels
    if (p.hasAnnotation("XLabel")) rtn.SetXTitle(p.annotation("XLabel").c_str());
    if (p.hasAnnotation("YLabel")) rtn.SetYTitle(p.annotation("YLabel").c_str());
    return rtn;
  }


  /// @brief Convert a YODA Profile1D to a ROOT TH1D
  inline TH1D toTH1D(const Profile1D& p) {
    // Work out bin edges first
    std::vector<double> edges;
    edges.reserve(p.numBins()+1);
    edges.push_back(p.bin(0).xMin());
    for (size_t i = 0; i < p.numBins(); ++i) {
      const ProfileBin1D& b = p.bin(i);
      if (!fuzzyEquals(edges.back(), b.xMin())) edges.push_back(b.xMin());
      if (!fuzzyEquals(edges.back(), b.xMax())) edges.push_back(b.xMax());
    }
    // Book ROOT histogram
    TH1D rtn(p.path().c_str(), p.title().c_str(), edges.size()-1, &edges[0]);
    for (int i = 1; i <= rtn.GetNbinsX(); ++i) {
      try {
        const ProfileBin1D& b = p.binAt(rtn.GetBinCenter(i)); // throws if in a gap
        rtn.SetBinContent(i, b.mean());
        rtn.SetBinError(i, b.stdErr());
      } catch (const Exception& e) {  }
    }
    // Overflows
    rtn.SetBinContent(0, p.underflow().yMean());
    rtn.SetBinContent(rtn.GetNbinsX()+1, p.overflow().yMean());
    rtn.SetBinError(0, p.underflow().yStdErr());
    rtn.SetBinError(rtn.GetNbinsX()+1, p.overflow().yStdErr());
    // Labels
    if (p.hasAnnotation("XLabel")) rtn.SetXTitle(p.annotation("XLabel").c_str());
    if (p.hasAnnotation("YLabel")) rtn.SetYTitle(p.annotation("YLabel").c_str());
    return rtn;
  }


  /// @todo Convert a YODA Profile2D to a ROOT TProfile2D





  /// @brief Convert a YODA Scatter2D to a ROOT TGraphAsymmErrors
  ///
  /// @todo Check/improve/extend.
  inline TGraphAsymmErrors toTGraph(const Scatter2D& s) {
    TVectorF xs(s.numPoints()), ys(s.numPoints());
    TVectorF exls(s.numPoints()), exhs(s.numPoints());
    TVectorF eyls(s.numPoints()), eyhs(s.numPoints());
    for (size_t i = 0; i < s.numPoints(); ++i) {
      const Point2D& p = s.point(i);
      xs[i] = p.x();
      ys[i] = p.y();
      exls[i] = p.xErrMinus();
      exhs[i] = p.xErrPlus();
      eyls[i] = p.yErrMinus();
      eyhs[i] = p.yErrPlus();
    }
    // Make the ROOT object... mm, the constructors don't take name+title, unlike all this histos!
    TGraphAsymmErrors rtn(xs, ys, exls, exhs, eyls, eyhs);
    rtn.SetName(s.path().c_str());
    rtn.SetTitle(s.title().c_str());
    // Labels
    if (s.hasAnnotation("XLabel")) rtn.GetXaxis()->SetTitle(s.annotation("XLabel").c_str());
    if (s.hasAnnotation("YLabel")) rtn.GetYaxis()->SetTitle(s.annotation("YLabel").c_str());
    return rtn;
  }


  /// @brief Convert a YODA Histo1D to a ROOT TGraphAsymmErrors
  ///
  inline TGraphAsymmErrors toTGraph(const Histo1D& h) {
    return toTGraph(mkScatter(h));
  }


  /// @brief Convert a YODA Profile1D to a ROOT TGraphAsymmErrors
  ///
  inline TGraphAsymmErrors toTGraph(const Profile1D& p) {
    return toTGraph(mkScatter(p));
  }


  /// @todo Convert YODA Histo2D and Profile2D to TGraph colour maps


  //@}


}

#endif
