// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef JET_KINEMATIC_DRAWER_H
#define JET_KINEMATIC_DRAWER_H

#include "BaseJetDrawer.h"
#include <qahtml/QADrawClient.h>

// ============================================================================
//! Drawer for Jet Kinematics
// ============================================================================
/*! Class to draw histograms produced by the JetKinematicCheck module
 */
class JetKinematicDrawer : public BaseJetDrawer
{
  public:

    // ------------------------------------------------------------------------
    //! Default ctor
    // ------------------------------------------------------------------------
    JetKinematicDrawer(const std::string& name = "JetKinematic",
                 const std::string& modu = "JetDraw",
                 const std::string& type = "towersub1_antikt",
                 const std::string& pref = "h_jetkinematiccheck",
                 const bool debug = false)
      : BaseJetDrawer(name, modu, type, pref, debug) {};

    // ------------------------------------------------------------------------
    //! Default dtor
    // ------------------------------------------------------------------------
    ~JetKinematicDrawer() {};

    // ------------------------------------------------------------------------
    //! Run histogram drawing for each trigger and resolution
    // ------------------------------------------------------------------------
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override
    {
      // emit debugging message
      if (m_do_debug)
      {
        std::cout << "  -- Drawing component " << m_name << std::endl;
      }

      // loop over resolutions
      int nDraw = 0;
      for (const uint32_t resToDraw : vecResToDraw)
      {
        // add a row for resolution
        m_plots.AddRow();

        // now loop over triggers
        for (const uint32_t trigToDraw : vecTrigToDraw)
        {
          // add a column for trigger and draw hists
          m_plots.AddColumn();
          DoDrawing(trigToDraw, resToDraw);
          ++nDraw;
        }
      }
      return nDraw;
    }

    // ------------------------------------------------------------------------
    //! Generate HTML pages for each trigger and resolution
    // ------------------------------------------------------------------------
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override
    {
      // emit debugging messages
      if (m_do_debug)
      {
        std::cout << "  -- Creating HTML pages for " << m_name << std::endl;
      }

      // instantiate draw client
      QADrawClient* cl = QADrawClient::instance();

      // loop over resolutions and triggers
      for (std::size_t iRes = 0; iRes < vecResToDraw.size(); ++iRes)
      {
        // now loop over triggers
        for (std::size_t iTrig = 0; iTrig < vecTrigToDraw.size(); ++iTrig)
        {
          // grab index & name of trigger being drawn
          const uint32_t idTrig = vecTrigToDraw[iTrig];
          const std::string nameTrig = JetDrawDefs::MapTrigToName().at(idTrig);

          // grab index & name of resolution being drawn
          const uint32_t idRes = vecResToDraw[iRes];
          const std::string nameRes = JetDrawDefs::MapResToName().at(idRes);
          const std::string dirRes = nameTrig + "/" + nameRes;
          const std::string fileRes = nameTrig + "_" + nameRes;

          // make html pages
          for (const auto& plot : m_plots.GetVecPlotPads(iRes, iTrig))
          {
            const std::string name = plot.canvas->GetName();
            const std::string dir = dirRes + "/JetKinematics/" + name;
            const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
            cl->CanvasToPng(plot.canvas, png);
          }
        }
      }
      return 0;
    }

  private:

    // ------------------------------------------------------------------------
    //! Draw histograms for a given trigger and resolution
    // ------------------------------------------------------------------------
    void DoDrawing(const uint32_t trig, const uint32_t res) override
    {
      // emit debugging message
      if (m_do_debug)
      {
        switch (res)
        {
          case JetDrawDefs::JetRes::R02:
            std::cout << "  -- Drawing jet kinematic histograms (trig = " << trig << ", R = 0.2)" << std::endl;
            break;
          case JetDrawDefs::JetRes::R03:
            std::cout << "  -- Drawing jet kinematic histograms (trig = " << trig << ", R = 0.3)" << std::endl;
            break;
          case JetDrawDefs::JetRes::R04:
            std::cout << "  -- Drawing jet kinematic histograms (trig = " << trig << ", R = 0.4)" << std::endl;
            break;
          case JetDrawDefs::JetRes::R05:
            std::cout << "  -- Drawing jet kinematic histograms (trig = " << trig << ", R = 0.5)" << std::endl;
            break;
          default:
            std::cerr << "Warning: trying to draw jet kinemtic histograms for Unknown resolution" << std::endl;
            return;
        }
      }

      // for kinematic hist names
      const std::string histPrefix = m_hist_prefix
                                   + "_"
                                   + JetDrawDefs::MapTrigToTag().at(trig)
                                   + "_"
                                   + m_jet_type;
      const std::string profSuffix = JetDrawDefs::MapResToTag().at(res)
                                   + "_pfx";

      // connect to draw client
      QADrawClient* cl = QADrawClient::instance();

      // grab histograms to draw and set options
      JetDrawDefs::VHistAndOpts1D hists = {
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_etavsphi_" + JetDrawDefs::MapResToTag().at(res))),
          "Jet #eta vs. #phi",
          "Jet #eta",
          "Jet #phi",
          "Counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_jetmassvseta_" + JetDrawDefs::MapResToTag().at(res))),
          "Jet Mass vs #eta",
          "Jet #eta",
          "Jet Mass [GeV/c^{2}]",
          "Counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_jetmassvseta_" + profSuffix)),
          "Jet Mass vs #eta",
          "Jet #eta",
          "Jet Mass [GeV/c^{2}]",
          "Counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_jetmassvspt_" + JetDrawDefs::MapResToTag().at(res))),
          "Jet Mass vs p_{T}",
          "Jet p_{T} [GeV/c]",
          "Jet Mass [GeV/c^{2}]",
          "Counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_jetmassvspt_" + profSuffix)),
          "Jet Mass vs p_{T}",
          "Jet p_{T} [GeV/c]",
          "Jet Mass [GeV/c^{2}]",
          "Counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histPrefix + "_spectra_" + JetDrawDefs::MapResToTag().at(res))),
          "Jet Spectra",
          "Jet p_{T} [GeV/c]",
          "Counts",
          "",
          0.25,
          true,
          false
        }
      };

      // draw all kinematic hists on 1 page
      DrawHists("JetKinematics", {0, 1, 3, 5}, hists, trig, res);

      // draw profiles on relevant pads
      DrawHistOnPad(2, 2, hists, m_plots.GetBackPlotPad());
      DrawHistOnPad(4, 3, hists, m_plots.GetBackPlotPad());
      return;
    }

};  // end JetKinematicDrawer

#endif
