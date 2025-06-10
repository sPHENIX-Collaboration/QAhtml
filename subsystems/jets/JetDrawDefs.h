// Derek Anderson <ruse-traveler@github.com>

#ifndef JET_DRAW_DEFS_H
#define JET_DRAW_DEFS_H

#include <jetqa/JetQADefs.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TPad.h>
#include <map>
#include <string>
#include <vector>

// ============================================================================
//! Misc. definitions for the JetDraw QAhtml subsystem
// ============================================================================ 
namespace JetDrawDefs
{

  // ==========================================================================
  //! Tags for jet resolutions
  // ==========================================================================
  /*! Enumerates possible jet resolutions to plot. */
  enum JetRes
  {
    R02,
    R03,
    R04,
    R05
  };

  // ==========================================================================
  //! Map of resolution index onto name
  // ==========================================================================
  inline std::map<uint32_t, std::string> MapResToName()
  {
    static const std::map<uint32_t, std::string> mapResToName = {
      {JetRes::R02, "Res02"},
      {JetRes::R03, "Res03"},
      {JetRes::R04, "Res04"},
      {JetRes::R05, "Res05"}
    };
    return mapResToName;
  }

  // ==========================================================================
  //! Map of resultion index to tag
  // ==========================================================================
  inline std::map<uint32_t, std::string> MapResToTag()
  {
    static const std::map<uint32_t, std::string> mapResToTag = {
      {JetRes::R02, "r02"},
      {JetRes::R03, "r03"},
      {JetRes::R04, "r04"},
      {JetRes::R05, "r05"}
    };
    return mapResToTag;
  }

  // ==========================================================================
  //! Map of trigger index onto name
  // ==========================================================================
  inline std::map<uint32_t, std::string> MapTrigToName()
  {
    static const std::map<uint32_t, std::string> mapTrigToName = {
      {JetQADefs::GL1::MBDNSJet1, "JetCoin6GeV"},
      {JetQADefs::GL1::MBDNSJet2, "JetCoin8GeV"},
      {JetQADefs::GL1::MBDNSJet3, "JetCoin10GeV"},
      {JetQADefs::GL1::MBDNSJet4, "JetCoin12GeV"}
    };
    return mapTrigToName;
  }

  // ==========================================================================
  //! Map of trigger index onto tag (FIXME use those from Jet_QA.C)
  // ==========================================================================
  /*! TODO need to add triggers for AuAu */
  inline std::map<uint32_t, std::string> MapTrigToTag()
  {
    static const std::map<uint32_t, std::string> mapTrigToTag = {
      {JetQADefs::GL1::MBDNS1, "mbdns1"},
      {JetQADefs::GL1::MBDNSJet1, "mbdnsjet1"},
      {JetQADefs::GL1::MBDNSJet2, "mbdnsjet2"},
      {JetQADefs::GL1::MBDNSJet3, "mbdnsjet3"},
      {JetQADefs::GL1::MBDNSJet4, "mbdnsjet4"}
    };
    return mapTrigToTag;
  }

  // ==========================================================================
  //! Helper struct to consolidate pads for drawing
  // ==========================================================================
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

  // ==========================================================================
  //! Helper struct to consolidate histogram and drawing options
  // ==========================================================================
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

  // ==========================================================================
  //! Helper class to work with a "matrix" of PlotPads
  // ==========================================================================
  /*! Plots to be drawn are grouped into a dynamic 2D matrix
   *  of vectors, where the indices of the matrix are
   *
   *      (Jet R, Trigger)
   *
   *  And for each entry of the matrix, there will be a vector
   *  PlotPads, 1 for each page of plots to be drawn. Note
   *  that for components that do NOT depend on Jet R (e.g.
   *  EventWise Rho), there will be only 1 row.
   *
   *  FIXME Eigen would be a better option to implement this,
   *  but that is left for the people to come.
   */
  class PlotPadMatrix
  {
    private:

      ///! The matrix of PlotPads
      VPlotPads3D m_matrix;

    public:

      // ----------------------------------------------------------------------
      //! Add a row (jet R index) to matrix
      // ----------------------------------------------------------------------
      void AddRow()
      {
        m_matrix.push_back( {} );
      }

      // ----------------------------------------------------------------------
      //! Add a column (trigger index) to matrix
      // ----------------------------------------------------------------------
      void AddColumn()
      {
        m_matrix.back().push_back( {} );
      }

      // ----------------------------------------------------------------------
      //! Append a new PlotPads object to tip of matrix
      // ----------------------------------------------------------------------
      void AppendPlotPad(PlotPads plot)
      {
        m_matrix.back().back().push_back( plot );
      }

      // ----------------------------------------------------------------------
      //! Get PlotPad at tip of matrix
      // ----------------------------------------------------------------------
      PlotPads& GetBackPlotPad()
      {
        return m_matrix.back().back().back();
      }

      // ----------------------------------------------------------------------
      //! Get latest PlotPad at a particular trigger index
      // ----------------------------------------------------------------------
      /*! FIXME might not be needed... */
      PlotPads& GetBackPlotPad(const std::size_t trig)
      {
        return m_matrix.back().at(trig).back();
      }

      // ----------------------------------------------------------------------
      //! Get latest PlotPad at a particular (jet R, trigger) index
      // ----------------------------------------------------------------------
      PlotPads& GetBackPlotPad(const std::size_t reso, const std::size_t trig)
      {
        return m_matrix.at(reso).at(trig).back();
      }

      //! Get a particular PlotPad at a particular (jet R, trigger) index
      PlotPads& GetPlotPad(const std::size_t page,
                           const std::size_t reso,
                           const std::size_t trig)
      {
        return m_matrix.at(reso).at(trig).at(page);
      }

      // ----------------------------------------------------------------------
      //! Default ctor/dtor
      // ----------------------------------------------------------------------
      PlotPadMatrix()  {};
      ~PlotPadMatrix() {};

  };  // end PlotPadMatrix

}  // end JetDrawDefs namespace

#endif
