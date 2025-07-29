// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "TrackInJetDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
TrackInJetDrawer::TrackInJetDrawer(const std::string& name,
                                   const std::string& modu,
                                   const std::string& type,
                                   const std::string& pref,
                                   const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

// ------------------------------------------------------------------------
//! Default dtor
// ------------------------------------------------------------------------
TrackInJetDrawer::~TrackInJetDrawer() {};

// inherited public methods ===================================================

// ------------------------------------------------------------------------
//! Run histogram drawing for each trigger and resolution
// ------------------------------------------------------------------------
int TrackInJetDrawer::Draw(const std::vector<uint32_t> vecTrigToDraw,
                           const std::vector<uint32_t> vecResToDraw)
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
int TrackInJetDrawer::MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                               const std::vector<uint32_t> vecResToDraw,
                               const QADraw& subsystem)
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
        const std::string dir = dirRes + "/TrackInJet/" + name;
        const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
        cl->CanvasToPng(plot.canvas, png);
      }
    }
  }
  return 0;
}

// inherited private methods ==================================================

// ------------------------------------------------------------------------
//! Draw histograms for a given trigger and resolution
// ------------------------------------------------------------------------
void TrackInJetDrawer::DoDrawing(const uint32_t trig, const uint32_t res)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (res)
    {
      case JetDrawDefs::JetRes::R02:
        std::cout << "  -- Drawing tracks in jets histograms (trig = " << trig << ", R = 0.2)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R03:
        std::cout << "  -- Drawing tracks in jets histograms (trig = " << trig << ", R = 0.3)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R04:
        std::cout << "  -- Drawing tracks in jets histograms (trig = " << trig << ", R = 0.4)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R05:
        std::cout << "  -- Drawing tracks in jets histograms (trig = " << trig << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to draw tracks in jets histograms with Unknown resolution" << std::endl;
        return;
    }
  }

  // for hist names
  const std::string histName = m_hist_prefix
                             + "_"
                             + JetDrawDefs::MapTrigToTag().at(trig)
                             + "_"
                             + m_jet_type
                             + "_"
                             + JetDrawDefs::MapResToTag().at(res);

  // connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D hists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposr_injet")),
      "In-jet track cluster r",
      "r [cm]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposrvsz_injet")),
      "In-jet track cluster r vs. z",
      "z [cm]",
      "r [cm]",
      "Counts",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposx_injet")),
      "In-jet track cluster x",
      "x [cm]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposy_injet")),
      "In-jet track cluster y",
      "y [cm]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposyvsx_injet")),
      "In-jet track cluster y vs. x",
      "x [cm]",
      "y [cm]",
      "",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_allclustposz_injet")),
      "In-jet track cluster z",
      "z [cm]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alljetntrks_injet")),
      "N_{trk} in jet",
      "N_{trk}",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alljetntrksvseta_injet")),
      "N_{trk} in jet vs. jet #eta",
      "#eta^{jet}",
      "N_{trk}",
      "",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alljetntrksvspt_injet")),
      "N_{trk} in jet vs. jet p_{T}",
      "p_{T}^{jet} [GeV/c]",
      "N_{trk}",
      "Counts",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alljetptvseta_injet")),
      "Jet p_{T} vs. #eta",
      "#eta^{jet}",
      "p_{T}^{jet}",
      "Counts",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltracketa_injet")),
      "In-jet track #eta",
      "#eta^{trk}",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltracketavsphi_injet")),
      "In-jet track #eta vs. #varphi",
      "#varphi^{trk} [rad.]",
      "#eta^{trk}",
      "Counts",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackphi_injet")),
      "In-jet track #varphi",
      "#varphi^{trk} [rad.]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackpt_injet")),
      "In-jet track p_{T}",
      "p_{T}^{trk} [GeV/c]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackptvsqual_injet")),
      "In-jet track p_{T} vs. #chi^{2}/NDF",
      "#chi^{2}/NDF",
      "p_{T}^{trk} [GeV/c]",
      "Counts",
      0.8,
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackqual_injet")),
      "In-jet track #chi^{2}/NDF",
      "#chi^{2}/NDF",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackz_injet")),
      "In-jet track z",
      "z = p^{trk}#upointp^{jet}/|p^{jet}|^{2}",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_alltrackjt_injet")),
      "In-jet track j_{T}",
      "j_{T} = |p^{jet}#timesp^{trk}|/|p^{jet}|^{2}",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false
    }
  };

  // draw track cluster (x,y,z) hists on one page
  DrawHists("InJetTrkClustXYZ", {2, 3, 5, 4}, hists, trig, res);

  // draw track cluster (r,z) hists on one page
  DrawHists("InJetTrkClustRZ", {0, 1}, hists, trig, res);

  // draw track (pt,eta,phi) hists on one page
  DrawHists("InJetTrkPtEtaPhi", {13, 10, 12, 11}, hists, trig, res);

  // draw track quality hists on one page
  DrawHists("InJetTrkQuality", {15, 14}, hists, trig, res);

  // draw track vs. jet hists on one page
  DrawHists("InJetTrkVsJet", {16, 17}, hists, trig, res);

  // draw track jet hists on one page
  DrawHists("TrkJets", {6, 8, 7, 9}, hists, trig, res);
  return;
}
