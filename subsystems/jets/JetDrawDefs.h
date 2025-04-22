// Derek Anderson <ruse-traveler@github.com>

#ifndef JET_DRAW_DEFS_H
#define JET_DRAW_DEFS_H

#include <TCanvas.h>
#include <TH1.h>
#include <TPad.h>
#include <string>
#include <vector>

// ============================================================================
//! Misc. definitions for the JetDraw QAhtml subsystem
// ============================================================================ 
namespace JetDrawDefs
{

  // ========================================================================
  //! Helper struct to consolidate pads for drawing
  // ========================================================================
  struct PlotPads
  {
    TCanvas* canvas  {nullptr};  ///< canvas to draw on
    TPad*    histPad {nullptr};  ///< pad for histograms
    TPad*    runPad  {nullptr};  ///< pad for run/build info
  };

  // vector of plot pads
  typedef std::vector<PlotPads> VPlotPads1D;
  typedef std::vector<std::vector<PlotPads>> VPlotPads2D;
  typedef std::vector<std::vector<std::vector<PlotPads>>> VPlotPads3D;

  // ========================================================================
  //! Helper struct to consolidate histogram and drawing options
  // ========================================================================
  struct HistAndOpts
  {
    TH1*        hist   {nullptr}; ///< histogram
    std::string title  {""};      ///< histogram title
    std::string titlex {""};      ///< x-axis title
    std::string titley {""};      ///< y-axis title
    std::string titlez {""};      ///< z-axis title
    float       margin {0.25};    ///< right margin of pad
    bool        logy   {false};   ///< make y axis log
    bool        logz   {false};   ///< make z axis log
  };

  // vector of histogram and options
  typedef std::vector<HistAndOpts> VHistAndOpts1D;

}  // end JetDrawDefs namespace

#endif
