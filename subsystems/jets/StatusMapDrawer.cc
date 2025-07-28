// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "StatusMapDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
StatusMapDrawer::StatusMapDrawer(const std::string& name,
                                 const std::string& modu,
                                 const std::string& type,
                                 const std::string& pref,
                                 const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
StatusMapDrawer::~StatusMapDrawer() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Run histogram drawing for each trigger
// ----------------------------------------------------------------------------
int StatusMapDrawer::Draw(const std::vector<uint32_t> /*vecTrigToDraw*/,
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
int StatusMapDrawer::MakeHtml(const std::vector<uint32_t> /*vecTrigToDraw*/,
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
void StatusMapDrawer::DoDrawing(const uint32_t /*trig*/, const uint32_t /*res*/)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing calo status histograms" << std::endl;
  }

  // for histogram names
  const std::string histName = m_hist_prefix;
  const std::string caloType = m_jet_type;  // n.b. this should be the type of tower container

  // lambda to make hist name
  auto makeHistName = [&histName, &caloType](const std::string& var, const std::string& calo)
  {
    return histName + "_" + var + "_" + caloType + "_" + calo;
  };

  // connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D hists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_npereta", "cemc"))),
      "No. of good EMCal towers per i_{#eta}",
      "i_{#eta}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_npereta", "hcalin"))),
      "No. of good IHCal towers per i_{#eta}",
      "i_{#eta}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_npereta", "hcalout"))),
      "No. of good OHCal otwers per i_{#eta}",
      "i_{#eta}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_nperphi", "cemc"))),
      "No. of good EMCal towers per i_{#varphi}",
      "i_{#varphi}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_nperphi", "hcalin"))),
      "No. of good IHCal towers per i_{#varphi}",
      "i_{#varphi}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_nperphi", "hcalout"))),
      "No. of good OHCal towers per i_{#varphi}",
      "i_{#varphi}",
      "N_{good}",
      "",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_phivseta", "cemc"))),
      "No. of good EMCal towers per (i_{#eta}, i_{#varphi})",
      "i_{#eta}",
      "i_{#varphi}",
      "N_{good}",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_phivseta", "hcalin"))),
      "No. of good IHCal towers per (i_{#eta}, i_{#varphi})",
      "i_{#eta}",
      "i_{#varphi}",
      "N_{good}",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("good_phivseta", "hcalout"))),
      "No. of good OHCal towers per (i_{#eta}, i_{#varphi})",
      "i_{#eta}",
      "i_{#varphi}",
      "N_{good}",
      0.8,
      0.25,
      false,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("status", "cemc"))),
      "N EMCal towers per status",
      "",
      "N_{twr}",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("status", "hcalin"))),
      "N IHCal towers per status",
      "",
      "N_{twr}",
      "",
      0.8,
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(makeHistName("status", "hcalout"))),
      "N OHCal towers per status",
      "",
      "N_{twr}",
      "",
      0.8,
      0.25,
      true,
      false
    }
  };

  // draw emcal hists on one page
  DrawHists("CaloStatusMap_EMCal", {9, 0, 3, 6}, hists);

  // draw ihcal hists on one page
  DrawHists("CaloStatusMap_IHCal", {10, 1, 4, 7}, hists);

  // draw ohcal hists on one page
  DrawHists("CaloStatusMap_OHCal", {11, 2, 5, 8}, hists);
  return;
}
