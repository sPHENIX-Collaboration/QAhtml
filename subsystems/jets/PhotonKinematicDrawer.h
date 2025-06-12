// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef PHOTON_KINEMATIC_DRAWER_H
#define PHOTON_KINEMATIC_DRAWER_H

#include "BaseJetDrawer.h"
#include <qahtml/QADrawClient.h>

// ============================================================================
//! Drawer for Photon Kinematics
// ============================================================================
/*! Class to draw histograms produced by the PhotonJetsKinematics module
 */
class PhotonKinematicDrawer : public BaseJetDrawer
{
  public:

    // ------------------------------------------------------------------------
    //! Default ctor
    // ------------------------------------------------------------------------
    PhotonKinematicDrawer(const std::string& name = "PhotonKinematics",
                          const std::string& modu = "JetDraw",
                          const std::string& type = "emcal_cluster",
                          const std::string& pref = "h_photonjetskinematics",
                          const bool debug = false)
      : BaseJetDrawer(name, modu, type, pref, debug) {};

    // ------------------------------------------------------------------------
    //! Default dtor
    // ------------------------------------------------------------------------
    ~PhotonKinematicDrawer() {};

    // ------------------------------------------------------------------------
    //! Run histogram drawing for each trigger
    // ------------------------------------------------------------------------
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> /*vecResToDraw*/) override
    {
      // emit debugging message
      if (m_do_debug)
      {
        std::cout << "  -- Drawing component " << m_name << std::endl;
      }

      // create 1 row to hold trigger vectors
      m_plots.AddRow();

      // loop over triggers
      int nDraw = 0;
      for (const uint32_t trigToDraw : vecTrigToDraw)
      {
        m_plots.AddColumn();
        DoDrawing(trigToDraw, 0);
        ++nDraw;
      }
      return nDraw;
    }

    // ------------------------------------------------------------------------
    //! Generate HTML pages for each trigger
    // ------------------------------------------------------------------------
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> /*vecResToDraw*/,
                 const QADraw& subsystem) override
    {
      // emit debugging messages
      if (m_do_debug)
      {
        std::cout << "  -- Creating HTML pages for " << m_name << std::endl;
      }

      // instantiate draw client
      QADrawClient* cl = QADrawClient::instance();

      // loop over triggers to draw
      for (std::size_t iTrig = 0; iTrig < vecTrigToDraw.size(); ++iTrig)
      {
        // grab index & name of trigger being drawn
        const uint32_t idTrig = vecTrigToDraw[iTrig];
        const std::string nameTrig = JetDrawDefs::MapTrigToName().at(idTrig);

        // make html pages
        for (const auto& plot : m_plots.GetVecPlotPads(0, iTrig))
        {
          const std::string name = plot.canvas->GetName();
          const std::string dir = nameTrig + "/" + name;
          const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
          cl->CanvasToPng(plot.canvas, png);
        }
      }

      // return w/o error
      return 0;
    }

  private:

    // ------------------------------------------------------------------------
    //! Draw histograms for a given trigger
    // ------------------------------------------------------------------------
    void DoDrawing(const uint32_t trig, const uint32_t /*res*/) override
    {
      // emit debugging message
      if (m_do_debug)
      {
        std::cout << "  -- Drawing rho histograms (trig = " << trig << ")" << std::endl;
      }

      // for histogram names
      const std::string histName = m_hist_prefix
                                 + "_"
                                 + JetDrawDefs::MapTrigToTag().at(trig)
                                 + "_"
                                 + m_jet_type;  // n.b. this should be the cluster type

      // connect to draw client
      QADrawClient* cl = QADrawClient::instance();

      // grab histogram do draw and set options
      JetDrawDefs::VHistAndOpts1D hists = {
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_chi2")),
          "Cluster #chi^{2}",
          "#chi^{2}",
          "counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_chi2_energy")),
          "Cluster #chi^{2} vs. E_{T}",
          "E_{T} [GeV]",
          "#chi^{2}",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_chi2_eta")),
          "Cluster #chi^{2} vs. #eta",
          "#eta",
          "#chi^{2}",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_chi2_phi")),
          "Cluster #chi^{2} vs. #varphi",
          "#varphi [rad.]",
          "#chi^{2}",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_energy")),
          "Cluster E_{T}",
          "E_{T} [GeV]",
          "counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_energy_eta")),
          "Cluster E_{T} vs. #eta",
          "#eta",
          "E_{T} [GeV]",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_energy_phi")),
          "Cluster E_{T} vs. #varphi",
          "#varphi [rad.]",
          "E_{T} [GeV]",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_eta_phi_with_cuts")),
          "Cluster #varphi vs. #eta (after #chi^{2} and E_{T} cuts)",
          "#varphi [rad.]",
          "#eta",
          "counts",
          0.25,
          false,
          true
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_eta_with_cuts")),
          "Cluster #eta (after #chi^{2} and E_{T} cuts)",
          "#eta",
          "counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(histName + "_phi_with_cuts")),
          "Cluster #varphi (after #chi^{2} and E_{T} cuts)",
          "#varphi [rad.]",
          "counts",
          "",
          0.25,
          true,
          false
        }
      };

      // draw chi2 plots on one page
      DrawHists("PhotonKinematics_Chi2", {0, 1, 2, 3}, hists, trig);

      // draw et plots on one page
      DrawHists("PhotonKinematics_Energy", {5, 6, 4}, hists, trig);

      // draw eta/phi plots on one page
      DrawHists("PhotonKinematics_EtaPhi", {8, 9, 7}, hists, trig);
      return;
    }

};  // end PhotonKinematicDrawer

#endif
