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
  //! List of resolutions to draw
  // ==========================================================================
  inline std::vector<uint32_t> VecResToDraw()
  {
    static const std::vector<uint32_t> vecResToDraw = {
      JetDrawDefs::JetRes::R02,
      JetDrawDefs::JetRes::R03,
      JetDrawDefs::JetRes::R04,
      JetDrawDefs::JetRes::R05
    };
    return vecResToDraw;
  }

  // ==========================================================================
  //! List of triggers to draw (p+p)
  // ==========================================================================
  /*! FIXME 999 = JetQADefs::GL1::Inclusive */
  inline std::vector<uint32_t> VecTrigToDrawPP()
  {
    static const std::vector<uint32_t> vecTrigToDrawPP = {
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4,
      999  // FIXME swap out for official tag when ready
    };
    return vecTrigToDrawPP;
  }

  // ==========================================================================
  //! List of triggers to draw (Au+Au)
  // ==========================================================================
  /*! FIXME 999 = JetQADefs::GL1::Inclusive */
  inline std::vector<uint32_t> VecTrigToDrawAuAu()
  {
    static const std::vector<uint32_t> vecTrigToDrawAuAu = {
      JetQADefs::GL1::MBDNSVtx10,  // actually 10 cm
      JetQADefs::GL1::MBDNSVtx30,  // actually 60 cm
      JetQADefs::GL1::MBDNSVtx60,   // actually 150 cm
      999  // FIXME swap out for official tag when ready
    };
    return vecTrigToDrawAuAu;
  }

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
      {JetQADefs::GL1::MBDNSJet4, "JetCoin12GeV"},
      {JetQADefs::GL1::MBDNSVtx10, "MBDNSVtx10"},
      {JetQADefs::GL1::MBDNSVtx30, "MBDNSVtx30"},
      {JetQADefs::GL1::MBDNSVtx60, "MBDNSVtx60"},
      {999, "Inclusive"}  // FIXME this should be implemented in core & macros
    };
    return mapTrigToName;
  }

  // ==========================================================================
  //! Map of trigger index onto tag (FIXME use those from Jet_QA.C)
  // ==========================================================================
  inline std::map<uint32_t, std::string> MapTrigToTag()
  {
    static const std::map<uint32_t, std::string> mapTrigToTag = {
      {JetQADefs::GL1::MBDNS1, "mbdns1"},
      {JetQADefs::GL1::MBDNSJet1, "mbdnsjet1"},
      {JetQADefs::GL1::MBDNSJet2, "mbdnsjet2"},
      {JetQADefs::GL1::MBDNSJet3, "mbdnsjet3"},
      {JetQADefs::GL1::MBDNSJet4, "mbdnsjet4"},
      {JetQADefs::GL1::MBDNSVtx10, "mbdnsvtx10"},
      {JetQADefs::GL1::MBDNSVtx30, "mbdnsvtx30"},
      {JetQADefs::GL1::MBDNSVtx60, "mbdnsvtx60"},
      {999, "inclusive"}  // FIXME this should be implmenented in core & macros
    };
    return mapTrigToTag;
  }

  // ==========================================================================
  //! Helper method to heck if a run number is p+p or Au+Au
  // ==========================================================================
  /*! Used to check if a run falls in p+p running:
   *    - Start: Run 30392 (1st recorded run of run 2, 02.26.2024)
   *    - Stop: Run 53880 (end of p+p running, 09.30.2024)
   */
  inline bool IsPP(const int run)
  {
    return (run > 30392) && (run <= 53880);
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
    float       marker {0.8};     ///< marker size
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

      // ----------------------------------------------------------------------
      //! Get a particular PlotPad at a particular (jet R, trigger) index
      // ----------------------------------------------------------------------
      PlotPads& GetPlotPad(const std::size_t page,
                           const std::size_t reso,
                           const std::size_t trig)
      {
        return m_matrix.at(reso).at(trig).at(page);
      }

      // ----------------------------------------------------------------------
      //! Get vector of PlotPads at a particular (jet R, trigger)
      // ----------------------------------------------------------------------
      VPlotPads1D& GetVecPlotPads(const std::size_t reso, const std::size_t trig)
      {
        return m_matrix.at(reso).at(trig);
      }

      // ----------------------------------------------------------------------
      //! Draw and save all plots to current directory
      // ----------------------------------------------------------------------
      std::size_t Save()
      {
        std::size_t nWrite = 0;
        for (auto row : m_matrix)
        {
          for (auto column : row)
          {
            for (auto page : column)
            {
              page.canvas->Draw();
              page.canvas->Write();
              ++nWrite;
            }
          }
        }
        return nWrite;
      }

      // ----------------------------------------------------------------------
      //! Default ctor/dtor
      // ----------------------------------------------------------------------
      PlotPadMatrix()  {};
      ~PlotPadMatrix() {};

  };  // end PlotPadMatrix

}  // end JetDrawDefs namespace

#endif
