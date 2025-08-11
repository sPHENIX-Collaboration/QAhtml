// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "BeamBackgroundDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
BeamBackgroundDrawer::BeamBackgroundDrawer(const std::string& name,
                                           const std::string& modu,
                                           const std::string& type,
                                           const std::string& pref,
                                           const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
BeamBackgroundDrawer::~BeamBackgroundDrawer() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Run histogram drawing for each trigger
// ----------------------------------------------------------------------------
int BeamBackgroundDrawer::Draw(const std::vector<uint32_t> /*vecTrigToDraw*/,
                               const std::vector<uint32_t> /*vecResToDraw*/)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing component " << m_name << std::endl;
  }

  // create 1 row & column (since module is NOT differential
  // in trigger OR resolution)
  m_plots.AddRow();
  m_plots.AddColumn();

  // run drawing routine 
  DoDrawing(0, 0);
  return 1;
}

// ----------------------------------------------------------------------------
//! Generate HTML pages for each trigger
// ----------------------------------------------------------------------------
int BeamBackgroundDrawer::MakeHtml(const std::vector<uint32_t> /*vecTrigToDraw*/,
                                   const std::vector<uint32_t> /*vecResToDraw*/,
                                   const QADraw& subsystem)
{
  // emit debugging messages
  if (m_do_debug)
  {
    std::cout << "  -- Creating HTML pages for " << m_name << std::endl;
  }

  // instantiate draw client
  QADrawClient* cl = QADrawClient::instance();

  // make html pages
  //   - n.b. this module is NOT differential in trigger
  for (const auto& plot : m_plots.GetVecPlotPads(0, 0))
  {
    const std::string name = plot.canvas->GetName();
    const std::string dir = name;
    const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
    cl->CanvasToPng(plot.canvas, png);
  }

  // return w/o error
  return 0;
}

// inherited private methods ==================================================

// ----------------------------------------------------------------------------
//! Draw histograms for a given trigger
// ----------------------------------------------------------------------------
void BeamBackgroundDrawer::DoDrawing(const uint32_t /*trig*/, const uint32_t /*res*/)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing calo status histograms" << std::endl;
  }

  // for histogram names
  const std::string histName = m_hist_prefix + "_";

  // connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D hists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "nevts_overall")),
      "N beam-background events ID'd (overall)",
      "",
      "N_{evt}",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "nevts_streaksideband")),
      "N beam-background events ID'd by Streak-Sideband Algorithm",
      "",
      "N_{evt}",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "streaksideband_nmaxstreak")),
      "N towers in longest streak in event",
      "N_{twr}",
      "counts",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "streaksideband_nstreakperphi")),
      "N streaky towers per i_{#varphi}",
      "i_{#varphi}",
      "N_{twr}",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "streaksideband_nstreaktwretavsphi")),
      "N streaky towers per (i_{#eta}, i_{#varphi})",
      "i_{#eta}",
      "i_{#varphi}",
      "N_{twr}",
      0.8,
      0.25,
      false,
      true
    }
  };

  // make histogram titles and axis titles
  for (std::size_t i = 0; i < hists.size(); ++i)
  {
    if (hists[i].hist)
    {
      // make hist title
      const std::string title = hists[i].title
                              + ";"
                              + hists[i].titlex
                              + ";"
                              + hists[i].titley
                              + ";"
                              + hists[i].titlez;

      hists[i].hist->SetTitle(title.data());
    }
  }

  // draw nevt hists on one page
  DrawHists("BeamBackground_NEvts", {0, 1}, hists);

  // draw streak-sideband hists on one page
  DrawHists("BeamBackground_StreakSideband", {4, 3, 2}, hists);
  return;
}
