// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova, Jinglin Liu

#include "EMCalShowerShapesDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

EMCalShowerShapesDrawer::EMCalShowerShapesDrawer(const std::string& name,
                                                 const std::string& modu,
                                                 const std::string& type,
                                                 const std::string& pref,
                                                 const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

EMCalShowerShapesDrawer::~EMCalShowerShapesDrawer() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Run histogram drawing for each trigger
// ----------------------------------------------------------------------------
int EMCalShowerShapesDrawer::Draw(const std::vector<uint32_t> vecTrigToDraw,
                                  const std::vector<uint32_t> /*vecResToDraw*/)
{
  if (m_do_debug)
  {
    std::cout << "  -- Drawing component " << m_name << std::endl;
  }

  m_plots.AddRow();

  int nDraw = 0;
  for (const uint32_t trigToDraw : vecTrigToDraw)
  {
    m_plots.AddColumn();
    DoDrawing(trigToDraw, 0);
    ++nDraw;
  }
  return nDraw;
}

// ----------------------------------------------------------------------------
//! Generate HTML pages for each trigger
// ----------------------------------------------------------------------------
int EMCalShowerShapesDrawer::MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                                      const std::vector<uint32_t> /*vecResToDraw*/,
                                      const QADraw& subsystem)
{
  if (m_do_debug)
  {
    std::cout << "  -- Creating HTML pages for " << m_name << std::endl;
  }

  QADrawClient* cl = QADrawClient::instance();

  for (std::size_t iTrig = 0; iTrig < vecTrigToDraw.size(); ++iTrig)
  {
    const uint32_t idTrig = vecTrigToDraw[iTrig];
    const std::string nameTrig = JetDrawDefs::MapTrigToName().at(idTrig);

    for (const auto& plot : m_plots.GetVecPlotPads(0, iTrig))
    {
      const std::string name = plot.canvas->GetName();
      const std::string dir = nameTrig + "/" + name;
      const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
      cl->CanvasToPng(plot.canvas, png);
    }
  }

  return 0;
}

// inherited private methods ==================================================

// ----------------------------------------------------------------------------
//! Draw histograms for a given trigger
// ----------------------------------------------------------------------------
void EMCalShowerShapesDrawer::DoDrawing(const uint32_t trig, const uint32_t /*res*/)
{
  if (m_do_debug)
  {
    std::cout << "  -- Drawing EMCal shower-shape histograms (trig = " << trig << ")" << std::endl;
  }

  const std::string histBase = m_hist_prefix 
                             + "_"
                             + JetDrawDefs::MapTrigToTag().at(trig);

  QADrawClient* cl = QADrawClient::instance();

  JetDrawDefs::VHistAndOpts1D hists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(histBase + "_e11_to_e33")),
      "EMCal Cluster e11/e33",
      "e11/e33",
      "Normalized Counts",
      "",
      0.8,
      0.25,
      false,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histBase + "_e32_to_e35")),
      "EMCal Cluster e32/e35",
      "e32/e35",
      "Normalized Counts",
      "",
      0.8,
      0.25,
      false,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histBase + "_weta_cogx")),
      "EMCal Cluster w#eta_cogx",
      "w#eta_cogx",
      "Normalized Counts",
      "",
      0.8,
      0.25,
      false,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(histBase + "_wphi_cogx")),
      "EMCal Cluster w#phi_cogx",
      "w#phi_cogx",
      "Normalized Counts",
      "",
      0.8,
      0.25,
      false,
      false,
      true
    }
  };

  // reference histograms, using same index as hists
  auto refs = BuildRefHists(hists);
  std::string currRunMsg = "Current Run " + std::to_string(cl->RunNumber());
  std::string refRunMsg  = "Reference Run " + refRunNum;

  // Draw all hist on the same page for now
  DrawHists("EMCalShowerShapes", {0, 1, 2, 3}, hists, trig);

  // draw reference hists on relevant pads
  DrawHistOnPad(0, 1, refs,  m_plots.GetBackPlotPad());
  DrawHistOnPad(0, 1, hists, m_plots.GetBackPlotPad()); // current on top of ref
  DrawTextOnPad(1, m_plots.GetBackPlotPad(), 0.60, 0.80, kBlack, currRunMsg);
  DrawTextOnPad(1, m_plots.GetBackPlotPad(), 0.60, 0.75, kRed,   refRunMsg);

  DrawHistOnPad(1, 2, refs,  m_plots.GetBackPlotPad());
  DrawHistOnPad(1, 2, hists, m_plots.GetBackPlotPad());
  DrawTextOnPad(2, m_plots.GetBackPlotPad(), 0.60, 0.80, kBlack, currRunMsg);
  DrawTextOnPad(2, m_plots.GetBackPlotPad(), 0.60, 0.75, kRed,   refRunMsg);

  DrawHistOnPad(2, 3, refs,  m_plots.GetBackPlotPad());
  DrawHistOnPad(2, 3, hists, m_plots.GetBackPlotPad());
  DrawTextOnPad(3, m_plots.GetBackPlotPad(), 0.60, 0.80, kBlack, currRunMsg);
  DrawTextOnPad(3, m_plots.GetBackPlotPad(), 0.60, 0.75, kRed,   refRunMsg);

  DrawHistOnPad(3, 4, refs,  m_plots.GetBackPlotPad());
  DrawHistOnPad(3, 4, hists, m_plots.GetBackPlotPad());
  DrawTextOnPad(4, m_plots.GetBackPlotPad(), 0.60, 0.80, kBlack, currRunMsg);
  DrawTextOnPad(4, m_plots.GetBackPlotPad(), 0.60, 0.75, kRed,   refRunMsg);

}

