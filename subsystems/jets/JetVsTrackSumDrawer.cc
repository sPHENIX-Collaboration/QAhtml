// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "JetVsTrackSumDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
JetVsTrackSumDrawer::JetVsTrackSumDrawer(const std::string& name,
                                         const std::string& modu,
                                         const std::string& type,
                                         const std::string& pref,
                                         const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

// ------------------------------------------------------------------------
//! Default dtor
// ------------------------------------------------------------------------
JetVsTrackSumDrawer::~JetVsTrackSumDrawer() {};

// inherited public methods ===================================================

// ------------------------------------------------------------------------
//! Run histogram drawing for each trigger and resolution
// ------------------------------------------------------------------------
int JetVsTrackSumDrawer::Draw(const std::vector<uint32_t> vecTrigToDraw,
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
int JetVsTrackSumDrawer::MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
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
        const std::string dir = dirRes + "/JetVsTrackSum/" + name;
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
void JetVsTrackSumDrawer::DoDrawing(const uint32_t trig, const uint32_t res)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (res)
    {
      case JetDrawDefs::JetRes::R02:
        std::cout << "  -- Drawing track sum vs. jet histograms (trig = " << trig << ", R = 0.2)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R03:
        std::cout << "  -- Drawing track sum vs. jet histograms (trig = " << trig << ", R = 0.3)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R04:
        std::cout << "  -- Drawing track sum vs. jet histograms (trig = " << trig << ", R = 0.4)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R05:
        std::cout << "  -- Drawing track sum vs. jet histograms (trig = " << trig << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to draw track sum vs. jet histograms with Unknown resolution" << std::endl;
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
      dynamic_cast<TH1*>(cl->getHisto(histName + "_sumtrkoverjtpt")),
      "Sum of track p_{T} over jet p_{T}",
      "#Sigmap_{T}^{trk} / p_{T}^{jet}",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_sumtrkpt")),
      "Sum of track p_{T}",
      "#Sigmap_{T}^{trk} [GeV/c]",
      "Counts",
      "",
      0.8,
      0.25,
      true,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_sumtrkvsjetpt")),
      "Sum of track p_{T} vs. jet p_{T}",
      "p_{T}^{jet}",
      "#Sigmap_{T}^{trk}",
      "Counts",
      0.8,
      0.25,
      false,
      true,
      true
    }
  };

  // draw all hists on one page
  DrawHists("JetVsTrackSum", {2, 0, 1}, hists, trig, res);
  return;
}
