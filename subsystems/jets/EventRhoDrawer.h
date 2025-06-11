// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef EVENT_RHO_DRAWER_H
#define EVENT_RHO_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Event-Wise Rho
// ============================================================================
/*! Class to draw histograms produced by the RhosinEvent module
 */
class EventRhoDrawer : public BaseJetdrawer
{
  public:

    // ------------------------------------------------------------------------
    //! Default ctor
    // ------------------------------------------------------------------------
    EventRhoDrawer(const std::string& name = "EventRho",
                   const std::string& modu = "JetDraw",
                   const std::string& type = "towersub1_antikt",
                   const std::string& pref = "h_eventwiserho",
                   const bool debug = false)
      : BaseJetDrawer(name, modu, type, pref, debug) {};

    // ------------------------------------------------------------------------
    //! Default dtor
    // ------------------------------------------------------------------------
    ~EventRhoDrawer() {};

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
                 const std::vector<uint32_t> /*vecResToDraw*/) override
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
        const uint32_t idTrig   = vecTrigToDraw[iTrig];
        const std::string nameTrig = JetDrawDefs::MapTrigToName().at(idTrig);

        // make an html page for each plot
        for (const auto& rho : m_plots.GetVecPlotPads(0, iTrig))
        {
          const std::string name = rho.canvas->GetName();
          const std::string dirRho  = nameTrig + "/" + nameRho;
          const std::string pngRho  = cl->htmlRegisterPage(*this, dirRho, nameRho, "png");
          cl->CanvasToPng(rho.canvas, pngRho);
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

      // for rho histogram names
      const std::string rhoHistName = m_hist_prefix + "_" + JetDrawDefs::MapTrigToTag[trig];

      // connect to draw client
      QADrawClient* cl = QADrawClient::instance();

      // grab histograms to draw and set options
      JetDrawDefs::VHistAndOpts1D rhoHists = {
        {
          dynamic_cast<TH1*>(cl->getHisto(rhoHistName + "_rhoarea")),
          "Rho, Area Method",
          "#rho_{area}",
          "Counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(rhoHistName + "_rhomult")),
          "Rho, Multiplicity Method",
          "#rho_{mult}",
          "Counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(rhoHistName + "_sigmaarea")),
          "Sigma, Area Method",
          "#sigma_{area}",
          "Counts",
          "",
          0.25,
          true,
          false
        },
        {
          dynamic_cast<TH1*>(cl->getHisto(rhoHistName + "_sigmamult")),
          "Sigma, Multiplicity Method",
          "#sigma_{mult}",
          "Counts",
          "",
          0.25,
          true,
          false
        }
      };

      // draw rho plots on one page
      DrawHists("EvtRho", {0, 1, 2, 3}, rhoHists, trig);
      return;
    }

};  // end EventRhoDrawer

#endif
