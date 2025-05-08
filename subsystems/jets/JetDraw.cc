//Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "JetDraw.h"
#include <sPhenixStyle.C>
#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TString.h>
#include <TPad.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TGaxis.h>
#include <boost/format.hpp>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Subsystem constructor
// ----------------------------------------------------------------------------
JetDraw::JetDraw(const std::string &name)
  : QADraw(name)
  , m_do_debug(false)
  , m_jet_type("towersub1_antikt")
  , m_rho_prefix("h_eventwiserho")
  , m_constituent_prefix("h_constituentsinjets")
  , m_kinematic_prefix("h_jetkinematiccheck")
  , m_seed_prefix("h_jetseedcount")
{
  DBVarInit();
}

// ----------------------------------------------------------------------------
//! Subsystem destructor
// ----------------------------------------------------------------------------
JetDraw::~JetDraw()
{
  /* delete db; */
}

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Draw plots
// ----------------------------------------------------------------------------
/*! Draws plots based on options. Implemented options:
 *    - `"RHO"` = draw event-wise rho plots,
 *    - `"CONSTITUENTS"` = draw jet calorimeter constituent plots,
 *    - `"KINEMATIC"` = draw jet kinematic plots,
 *    - `"SEED"` = draw jet seed plots,
 *    - `"ALL"` = draw all of the above.
 *
 *  \param what drawing option
 */
int JetDraw::Draw(const std::string& what)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Drawing component " << what << std::endl;
  }

  // reserve enough space in relevant vectors
  m_rhoPlots.resize( m_vecTrigToDraw.size() );
  m_cstPlots.resize( m_vecTrigToDraw.size(), JetDrawDefs::VPlotPads2D(m_vecResToDraw.size()) );
  m_kinePlots.resize( m_vecTrigToDraw.size(), JetDrawDefs::VPlotPads2D(m_vecResToDraw.size()) );
  m_seedPlots.resize( m_vecTrigToDraw.size(), JetDrawDefs::VPlotPads2D(m_vecResToDraw.size()) );

  // loop over triggers
  int iret = 0;
  int idraw = 0;
  for (uint32_t trigToDraw : m_vecTrigToDraw)
  {
    // add rows to reso-dependent vectors
    m_cstPlots.push_back( {} );
    m_kinePlots.push_back( {} );
    m_seedPlots.push_back( {} );

    // add row for rho pages
    m_rhoPlots.push_back( {} );

    // draw rho plots
    if  (what == "ALL" || what == "RHO")
    {
      iret = DrawRho(trigToDraw);
      ++idraw;
    }

    // now loop over resolutions to draw
    for (uint32_t resToDraw : m_vecResToDraw)
    {
      // add columns for rho-dependent vectors
      m_cstPlots.back().push_back( {} );
      m_kinePlots.back().push_back( {} );
      m_seedPlots.back().push_back( {} );

      // draw constituent plots
      if (what == "ALL" || what == "CONTSTITUENTS")
      {
        iret = DrawConstituents(trigToDraw, static_cast<JetRes>(resToDraw));
        ++idraw;
      }

      // draw kinematic plots
      if (what == "ALL" || what == "KINEMATIC")
      {
        iret = DrawJetKinematics(trigToDraw, static_cast<JetRes>(resToDraw));
        ++idraw;
      }

      // draw seed plots
      if (what == "ALL" || what == "SEED")
      {
        iret = DrawJetSeed(trigToDraw, static_cast<JetRes>(resToDraw));
        ++idraw;
      }
    }
  }

  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    return -1;
  }

  // should return -1 if error, 0 otherwise
  return iret;
}

// ----------------------------------------------------------------------------
//! Draw plots and generate HTML pages
// ----------------------------------------------------------------------------
/*! Draws plots based on provided option (see `JetDraw::Draw(std::string&)`)
 *  and generates html pages for each.
 *
 *  \param what drawing option
 */
int JetDraw::MakeHtml(const std::string& what)
{
  // emit debugging messages
  if (m_do_debug)
  {
    std::cout << "Creating HTML pages for " << what << std::endl;
  }

  // draw relevant plots
  const int drawError = Draw(what);  // Call to Draw
  if (drawError) {
    return drawError;  // Return if there is an error in Draw
  }

  // instantiate draw client
  QADrawClient *cl = QADrawClient::instance();

  // loop over triggers to draw
  for (std::size_t iTrigToDraw = 0; iTrigToDraw < m_vecTrigToDraw.size(); ++iTrigToDraw)
  {
    // grab index & name of trigger being drawn
    const uint32_t    idTrig   = m_vecTrigToDraw[iTrigToDraw];
    const std::string nameTrig = m_mapTrigToName[idTrig];

    // draw rho plots
    for (const auto& rho : m_rhoPlots[iTrigToDraw])
    {
      const std::string nameRho = rho.canvas->GetName();
      const std::string dirRho  = nameTrig + "/" + nameRho;
      const std::string pngRho  = cl->htmlRegisterPage(*this, dirRho, nameRho, "png");
      cl->CanvasToPng(rho.canvas, pngRho);
    }

    // loop over jet resolutions to draw
    for (std::size_t iResToDraw = 0; iResToDraw < m_vecResToDraw.size(); ++iResToDraw)
    {
      // grab index & name of resolution being drawn
      const uint32_t    idRes   = m_vecResToDraw[iResToDraw];
      const std::string nameRes = m_mapResToName[idRes];
      const std::string dirRes  = nameTrig + "/" + nameRes;
      const std::string fileRes = nameTrig + "_" + nameRes;

      // draw constituent plots 
      for (const auto& cst : m_cstPlots[iTrigToDraw][iResToDraw])
      {
        const std::string nameCst = cst.canvas->GetName();
        const std::string dirCst  = dirRes + "/Constituents/" + nameCst;
        const std::string pngCst  = cl->htmlRegisterPage(*this, dirCst, nameCst, "png");
        cl->CanvasToPng(cst.canvas, pngCst);
      }

      // draw kinematic plots 
      for (const auto& kine : m_kinePlots[iTrigToDraw][iResToDraw])
      {
        const std::string nameKine = kine.canvas->GetName();
        const std::string dirKine  = dirRes + "JetKinematics/" + nameKine;
        const std::string pngKine  = cl->htmlRegisterPage(*this, dirKine, nameKine, "png");
        cl->CanvasToPng(kine.canvas, pngKine);
      }

      // draw seed plots
      for (const auto& seed : m_seedPlots[iTrigToDraw][iResToDraw])
      {
        const std::string nameSeed = seed.canvas->GetName();
        const std::string dirSeed  = dirRes + "JetSeeds/" + nameSeed;
        const std::string pngSeed  = cl->htmlRegisterPage(*this, dirSeed, nameSeed, "png");
        cl->CanvasToPng(seed.canvas, pngSeed);
      }
    }
  }

  // return w/o error
  return 0;
}

// other public methods =======================================================

// ----------------------------------------------------------------------------
//! Initialize QADraw Database
// ----------------------------------------------------------------------------
/*! Currently unused. */
int JetDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}

// ----------------------------------------------------------------------------
//! Set summary of overall jet QA goodness
// ----------------------------------------------------------------------------
/*! Currently unused. */
void JetDraw::SetJetSummary(TCanvas* c)
{
  m_jetSummary = c;
  m_jetSummary->cd();

  QADrawClient *cl = QADrawClient::instance();
  TPad* tr = new TPad("transparent_jet", "", 0, 0, 1, 1);
  tr->SetFillStyle(4000);
  tr->Draw();
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream;
  std::string runstring;
  runnostream << Name() << "_jet_summary Run " << cl->RunNumber() << ", build " << cl->build();
  runstring = runnostream.str();
  tr->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());
  m_jetSummary->Update();
}

// ----------------------------------------------------------------------------
//! Save canvases to file
// ----------------------------------------------------------------------------
/*! Helper method to save all canvases to a specified file.
 *  This is useful for debugging and quick testing when
 *  adjusting plotting details/etc.
 *
 *  \param[out] file file to write canvases to
 */
void JetDraw::SaveCanvasesToFile(TFile* file)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Saving plots to file:\n"
              << "  " << file->GetName()
              << std::endl;
  }

  // check if you can cd into file
  //   - if not, exit
  const bool isGoodCD = file->cd();
  if (!isGoodCD)
  {
    if (m_do_debug)
    {
      std::cerr << PHWHERE << "WARNING: couldn't cd into output file!" << std::endl;
    }
    return;
  }

  // for tracking how many plots were saved
  std::size_t nWrite = 0;

  // save rho canvases
  for (auto rhoRow : m_rhoPlots)
  {
    for (auto rho : rhoRow)
    {
      rho.canvas->Draw();
      rho.canvas->Write();
      ++nWrite;
    }
  }
  if (m_do_debug) std::cout << "  -- Saved rho plots." << std::endl;

  // save constituent canvases
  for (auto cstRow : m_cstPlots)
  {
    for (auto cstCol : cstRow)
    {
      for (auto cst : cstCol)
      {
        cst.canvas->Draw();
        cst.canvas->Write();
        ++nWrite;
      }
    }
  }
  if (m_do_debug) std::cout << "  -- Saved constituent plots." << std::endl;

  // save kinematics canvases
  for (auto kinRow : m_kinePlots)
  {
    for (auto kinCol : kinRow)
    {
      for (auto kin : kinCol)
      {
        kin.canvas->Draw();
        kin.canvas->Write();
        ++nWrite;
      }
    }
  }
  if (m_do_debug) std::cout << "  -- Saved kinematic plots." << std::endl;

  // save seed canvases
  for (auto sedRow : m_seedPlots)
  {
    for (auto sedCol : sedRow)
    {
      for (auto sed : sedCol)
      {
        sed.canvas->Draw();
        sed.canvas->Write();
        ++nWrite;
      }
    }
  }
  if (m_do_debug) std::cout << "  -- Saved seed plots." << std::endl;

  // announce how many plots saved & exit
  if (m_do_debug)
  {
    std::cout << "Finished saving plots: " << nWrite << " plots written." << std::endl;
  }
}

// private methods ============================================================

// ----------------------------------------------------------------------------
//! Draw rho histograms
// ----------------------------------------------------------------------------
/*! Draws histograms from the `RhosInEvent` module.
 *
 *  \param trigToDraw index of trigger of histograms being drawn
 */
int JetDraw::DrawRho(const uint32_t trigToDraw)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Drawing rho histograms (trig = " << trigToDraw << ")" << std::endl;
  }

  // for rho histogram names
  const std::string rhoHistName = m_rho_prefix + "_" + m_mapTrigToTag[trigToDraw];

  // connect to draw client
  QADrawClient *cl = QADrawClient::instance();

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
  DrawHists("EvtRho", {0, 1, 2, 3}, rhoHists, m_rhoPlots.back(), trigToDraw);

  // return w/o error
  return 0;
}

// ----------------------------------------------------------------------------
//! Draw constituent histograms
// ----------------------------------------------------------------------------
/*! Draws histograms from the `ConstituentsinJets` module.
 *
 *  \param trigToDraw index of trigger of histograms being drawn
 *  \param resToDraw  index of jet resolution of histograms being drawn
 */
int JetDraw::DrawConstituents(const uint32_t trigToDraw, const JetRes resToDraw)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (resToDraw)
    {
      case R02:
        std::cout << "Drawing constituent histograms (trig = " << trigToDraw << ", R = 0.2)" << std::endl;
        break;
      case R03:
        std::cout << "Drawing constituent histograms (trig = " << trigToDraw << ", R = 0.3)" << std::endl;
        break;
      case R04:
        std::cout << "Drawing constituent histograms (trig = " << trigToDraw << ", R = 0.4)" << std::endl;
        break;
      case R05:
        std::cout << "Drawing constituent histograms (trig = " << trigToDraw << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to constituent histograms for unknown resolution" << std::endl;
        return -1;
    }
  }

  // for constituent hist names
  const std::string cstHistName = m_constituent_prefix + "_" + m_mapTrigToTag[trigToDraw] + "_" + m_jet_type + "_" + m_mapResToTag[resToDraw];

  // connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D cstHists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_ncsts_cemc")),
      "Jet N Constituents in CEMC",
      "N Constituents",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_ncsts_ihcal")),
      "Jet N Constituents in IHCal",
      "N Constituents",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_ncsts_ohcal")),
      "Jet N Constituents in OHCal",
      "N Constituents",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_ncsts_total")),
      "Jet N Constituents",
      "N Constituents",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_ncstsvscalolayer")),
      "Jet N Constituents vs Calo Layer",
      "Calo Layer",
      "N Constituents",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_efracjet_cemc")),
      "Jet E Fraction in CEMC",
      "Jet E Fraction",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_efracjet_ihcal")),
      "Jet E Fraction in IHCal",
      "Jet E Fraction",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_efracjet_ohcal")),
      "Jet E Fraction in OHCal",
      "Jet E Fraction",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(cstHistName + "_efracjetvscalolayer")),
      "Jet E Fraction vs Calo Layer",
      "Calo Layer",
      "Jet E Fraction",
      "Counts",
      0.25,
      false,
      true
    }
  };

  // draw 1d ncst hists on a page
  DrawHists("JetCsts_NCsts", {0, 1, 2, 3}, cstHists, m_cstPlots.back().back(), trigToDraw, resToDraw);

  // draw e fraction on a page
  DrawHists("JetCsts_EFrac", {5, 6, 7}, cstHists, m_cstPlots.back().back(), trigToDraw, resToDraw);

  // draw ncst and e faction vs. calo layer on a page
  DrawHists("JetCsts_VsCaloLayer", {4, 8}, cstHists, m_cstPlots.back().back(), trigToDraw, resToDraw);

  // return w/o error
  return 0;
}

// ----------------------------------------------------------------------------
//! Draw jet kinematic histograms
// ----------------------------------------------------------------------------
/*! Draw histograms from the `JetKinematicCheck` module.
 *
 *  \param trigToDraw index of trigger of histograms being drawn
 *  \param resToDraw  index of jet resolution of histograms being drawn
 */
int JetDraw::DrawJetKinematics(const uint32_t trigToDraw, const JetRes resToDraw)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (resToDraw)
    {
      case R02:
        std::cout << "Drawing jet kinematic histograms (trig = " << trigToDraw << ", R = 0.2)" << std::endl;
        break;
      case R03:
        std::cout << "Drawing jet kinematic histograms (trig = " << trigToDraw << ", R = 0.3)" << std::endl;
        break;
      case R04:
        std::cout << "Drawing jet kinematic histograms (trig = " << trigToDraw << ", R = 0.4)" << std::endl;
        break;
      case R05:
        std::cout << "Drawing jet kinematic histograms (trig = " << trigToDraw << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to draw jet kinemtic histograms for Unknown resolution" << std::endl;
        return -1;
    }
  }

  // for kinematic hist names
  const std::string kineHistPrefix = m_kinematic_prefix + "_" + m_mapTrigToTag[trigToDraw] + "_" + m_jet_type;
  const std::string kineProfSuffix = m_mapResToTag[resToDraw] + "_pfx";

  // connect to draw client
  QADrawClient *cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D kineHists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_etavsphi_" + m_mapResToTag[resToDraw])),
      "Jet #eta vs. #phi",
      "Jet #eta",
      "Jet #phi",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_jetmassvseta_" + m_mapResToTag[resToDraw])),
      "Jet Mass vs #eta",
      "Jet #eta",
      "Jet Mass [GeV/c^{2}]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_jetmassvseta_" + kineProfSuffix)),
      "Jet Mass vs #eta",
      "Jet #eta",
      "Jet Mass [GeV/c^{2}]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_jetmassvspt_" + m_mapResToTag[resToDraw])),
      "Jet Mass vs p_{T}",
      "Jet p_{T} [GeV/c]",
      "Jet Mass [GeV/c^{2}]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_jetmassvspt_" + kineProfSuffix)),
      "Jet Mass vs p_{T}",
      "Jet p_{T} [GeV/c]",
      "Jet Mass [GeV/c^{2}]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(kineHistPrefix + "_spectra_" + m_mapResToTag[resToDraw])),
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
  DrawHists("JetKinematics", {0, 1, 3, 5}, kineHists, m_kinePlots.back().back(), trigToDraw, resToDraw);

  // draw profiles on relevant pads
  DrawHistOnPad(2, 2, kineHists, m_kinePlots.back().back().back());
  DrawHistOnPad(4, 3, kineHists, m_kinePlots.back().back().back());

  // return w/o error
  return 0;
}

// ----------------------------------------------------------------------------
//! Draw jet seed histograms
// ----------------------------------------------------------------------------
/*! Draw histograms from the `JetSeedCount` module.
 *
 *  \param trigToDraw index of trigger of histograms being drawn
 *  \param resToDraw  index of jet resolution of histograms being drawn
 */
int JetDraw::DrawJetSeed(const uint32_t trigToDraw, const JetRes resToDraw)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (resToDraw)
    {
      case R02:
        std::cout << "Drawing jet seed histograms (trig = " << trigToDraw << ", R = 0.2)" << std::endl;
        break;
      case R03:
        std::cout << "Drawing jet seed histograms (trig = " << trigToDraw << ", R = 0.3)" << std::endl;
        break;
      case R04:
        std::cout << "Drawing jet seed histograms (trig = " << trigToDraw << ", R = 0.4)" << std::endl;
        break;
      case R05:
        std::cout << "Drawing jet seed histograms (trig = " << trigToDraw << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to draw jet seed histograms with Unknown resolution" << std::endl;
        return -1;
    }
  }

  // for seed hist names
  const std::string seedHistName = m_seed_prefix + "_" + m_mapTrigToTag[trigToDraw] + "_" + m_jet_type + "_" + m_mapResToTag[resToDraw];

  // connect to draw client
  QADrawClient *cl = QADrawClient::instance();

  // grab histograms to draw and set options
  JetDrawDefs::VHistAndOpts1D seedHists = {
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_rawetavsphi")),
      "Raw Seed #eta vs #phi",
      "Seed #eta_{raw} [Rads.]",
      "Seed #phi_{raw} [Rads.]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_rawpt")),
      "Raw Seed p_{T}",
      "Seed p_{T,raw} [GeV/c]",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_rawptall")),
      "Raw Seed p_{T} (all jet seeds)",
      "Seed p_{T,raw} [GeV/c]",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_rawseedcount")),
      "Raw Seed Count per Event",
      "N Seed per Event",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_subetavsphi")),
      "Subtracted Seed #eta vs #phi",
      "Seed #eta_{sub} [Rads.]",
      "Seed #phi_{sub} [Rads.]",
      "Counts",
      0.25,
      false,
      true
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_subpt")),
      "Subtracted Seed p_{T}",
      "Seed p_{T,sub} [GeV/c]",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_subptall")),
      "Subtracted Seed p_{T} (all jet seeds)",
      "Seed p_{T,sub} [GeV/c]",
      "Counts",
      "",
      0.25,
      true,
      false
    },
    {
      dynamic_cast<TH1*>(cl->getHisto(seedHistName + "_subseedcount")),
      "Subtracted Seed Count per Event",
      "N Seed per Event",
      "Counts",
      "",
      0.25,
      true,
      false
    }
  };

  // draw raw seed hists
  DrawHists("JetSeeds_Raw", {0, 1, 2, 3}, seedHists, m_seedPlots.back().back(), trigToDraw, resToDraw);

  // draw subtracted seed hists
  DrawHists("JetSeeds_Sub", {4, 5, 6, 7}, seedHists, m_seedPlots.back().back(), trigToDraw, resToDraw);

  // return w/o error
  return 0;
}

// ----------------------------------------------------------------------------
//! Draw run and build info on a TPad
// ----------------------------------------------------------------------------
/*! By default, trigger and resolution aren't added to the
 *  text.  However, if trigger or resolution indices are
 *  provided, then that info will be added.
 *
 *  \param[in]  what the pad's associated QA component (e.g. JetKinematics)
 *  \param[out] pad  the pad to draw text on
 *  \param      trig trigger index (optional)
 *  \param      res  jet resolution index (optional)
 */
void JetDraw::DrawRunAndBuild(const std::string& what,
                              TPad* pad,
                              const int trig,
                              const int res)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Drawing run and build info for " << what << std::endl;
  }

  // connect to draw client
  QADrawClient *cl = QADrawClient::instance();

  // Generate run string from client
  std::ostringstream runnostream;
  runnostream << cl->RunNumber() << ", build " << cl->build();

  // prepend module name, component, and other info as needed
  std::string runstring = Name();
  runstring.append("_" + what);
  if (trig > -1)
  {
    runstring.append("_" + m_mapTrigToName.at(trig));
  }
  if (res > -1)
  {
    runstring.append("_" + m_mapResToName.at(res));
  }

  // now add run
  runstring += " Run ";
  runstring += runnostream.str();

  // create TText for info
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(12);  // center/center alignment

  // and finally draw on pad
  pad -> cd();
  PrintRun.DrawText(0.50, 0.70, runstring.data());
}

// ----------------------------------------------------------------------------
//! Draw histograms on canvas
// ----------------------------------------------------------------------------
/*! By default, trigger and resolution aren't added to the
 *  canvas names or run/build text. However, if trigger or
 *  resolution indices are provided, then that info will
 *  be added.
 *
 *  \param[in]  what    the histograms' associated QA component (e.g. JetSeed)
 *  \param[in]  indices the indices of the histograms to draw
 *  \param[in]  hists   the histograms to select from
 *  \param[out] plots   the canvas+pads to draw on 
 *  \param      trig    trigger index (optional)
 *  \param      res     jet resolution index (optional)
 */
void JetDraw::DrawHists(const std::string& what,
                        const std::vector<std::size_t>& indices,
                        const JetDrawDefs::VHistAndOpts1D& hists,
                        JetDrawDefs::VPlotPads1D& plots,
                        const int trig,
                        const int res)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Drawing histograms for " << what << std::endl;
  }

  // form canvas name
  std::string canName = what;
  if (trig > -1)
  {
    canName.append("_" + m_mapTrigToTag.at(trig));
  }
  if (res > -1)
  {
    canName.append("_" + m_mapResToTag.at(res));
  }

  // if canvas doesn't exist yet, create it
  if (!gROOT->FindObject(canName.data()))
  {
    MakeCanvas(canName, indices.size(), plots);
  }

  // draw selected histograms
  //   - n.b. COLZ does nothing for 1D histograms
  for (std::size_t iPad = 0; iPad < indices.size(); ++iPad)
  {
    plots.back().histPad->cd(iPad + 1);
    if (hists.at(indices[iPad]).hist)
    {
      UpdatePadStyle(hists.at(indices[iPad]));
      hists.at(indices[iPad]).hist->DrawCopy("COLZ");
    }
    else
    {
      DrawEmptyHistogram(hists.at(indices[iPad]).title);
    }
  }

  // add run/build info to canvas
  DrawRunAndBuild(what, plots.back().runPad, trig, res);
  plots.back().canvas->Update();
}

// ----------------------------------------------------------------------------
//! Draw a histogram on a pad
// ----------------------------------------------------------------------------
/*! Draw a particular histogram (entry iHist in provided hist vector)
 *  on a particular pad (pad iPad in canvas). Note that
 *    1. the indices of pads in a TCanvas from 1 (not 0!) on up, and
 *    2. that this function assumes something has already been drawn
 *        on the pad.
 *
 *  \param iHist index of histogram in vector `hists` to draw
 *  \param iPad  index of pad to draw histogram on
 *  \param hists vector histograms containing histogram to be drawn
 *  \param plot  canvas containing pad to be drawn on
 */
void JetDraw::DrawHistOnPad(const std::size_t iHist,
                            const std::size_t iPad,
                            const JetDrawDefs::VHistAndOpts1D& hists,
                            JetDrawDefs::PlotPads& plot)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Drawing histogram " << iHist << " on pad " << iPad << std::endl;
  }

  // draw histogram
  plot.histPad->cd(iPad);
  if (hists.at(iHist).hist)
  {
    UpdatePadStyle(hists.at(iHist));
    hists.at(iHist).hist->DrawCopy("SAME");
  }
  else
  {
    std::cerr << "Warning: trying to draw missing histogram " << iHist << " on pad " << iPad << std::endl;
  }
  plot.canvas->Update();
}

// ----------------------------------------------------------------------------
//! Draw empty histogram on current pad
// ----------------------------------------------------------------------------
/*! Helper function to draw an empty histogram on the current
 *  pad. Used when a histogram is missing.
 *
 *  \param[in] what what's missing (e.g. a histogram)
 */
void JetDraw::DrawEmptyHistogram(const std::string& what)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Printing message '" << what << "'" << std::endl;
  }

  // set up hist/text
  TH1D*   hEmpty = new TH1D("hEmpty", "", 10, 0, 10);
  TLatex* lEmpty = new TLatex();

  // set up message
  const std::string message = what + " is missing";

  // and draw them on current pad
  hEmpty->DrawCopy();
  lEmpty->DrawLatex(0.3, 0.5, message.data());
}

// ----------------------------------------------------------------------------
//! Create canvas to draw on
// ----------------------------------------------------------------------------
/*! Creates a TCanvas to hold QA histograms and run info.
 *
 *  \param name  name of canvas
 *  \param nhist number of histograms to draw
 *  \param plots vector of canvases to add canvas to
 */ 
void JetDraw::MakeCanvas(const std::string& name,
                         const int nHist,
                         JetDrawDefs::VPlotPads1D& plots)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "Making canvas " << name << std::endl;
  }

  // instantiate draw client & grab display size
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();

  // create canvas
  //   - n.b. xpos (-1) negative means do not draw menu bar
  TCanvas* canvas = new TCanvas(name.data(), "", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
  canvas->UseCurrentStyle();
  gSystem->ProcessEvents();

  // create pad for histograms
  const std::string histPadName = name + "_hist";
  TPad* histPad = new TPad(histPadName.data(), "for histograms", 0.0, 0.0, 1.0, 0.9);
  histPad->SetFillStyle(4000);
  canvas->cd();
  histPad->Draw();

  // divide hist pad into appropriate no. of pads
  const int   nRow = std::min(nHist, 2);
  const int   nCol = (nHist / 2) + (nHist % 2);
  const float bRow = 0.01;
  const float bCol = 0.01;
  if (nHist > 1)
  {
    histPad->Divide(nRow, nCol, bRow, bCol);
  }

  // create pad for run number
  const std::string runPadName = name + "_run";
  TPad* runPad = new TPad(runPadName.data(), "for run and build", 0.0, 0.9, 1.0, 1.0);
  runPad->SetFillStyle(4000);
  canvas->cd();
  runPad->Draw();

  // add canvas/pads to vector
  plots.push_back( {canvas, histPad, runPad} );

  // return w/o error
  return;
}

// ----------------------------------------------------------------------------
//! Update style of current pad based on options
// ----------------------------------------------------------------------------
void JetDraw::UpdatePadStyle(const JetDrawDefs::HistAndOpts& hist)
{
  gPad->UseCurrentStyle();
  gPad->Update();
  gPad->SetRightMargin(hist.margin);
  gPad->SetLogy(hist.logy);
  gPad->SetLogz(hist.logz);
}

// ----------------------------------------------------------------------------
//! Turn text into a TLatex object
// ----------------------------------------------------------------------------
/* Currently unused. */
void JetDraw::myText(double x, double y, int color, const char* text, double tsize)
{
  TLatex l;
  l.SetTextAlign(22);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}
