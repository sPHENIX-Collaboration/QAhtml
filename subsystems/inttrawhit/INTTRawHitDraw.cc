#include "INTTRawHitDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TPad.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

INTTRawHitDraw::INTTRawHitDraw(const std::string &name)
  : QADraw(name)
{
  gStyle->SetOptStat(0);
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_InttRawHitQA_";
  return;
}

INTTRawHitDraw::~INTTRawHitDraw()
{
  /* delete db; */
  return;
}

int INTTRawHitDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "SUMMARY")
  {
    iret += DrawSummary();
    idraw++;
  }
  if (what == "ALL" || what == "CHIP")
  {
    iret += DrawChipInfo();
    idraw++;
  }
  if (what == "ALL" || what == "HITMAP")
  {
    // iret += DrawHitMapInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}
int INTTRawHitDraw::MakeChipCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  int cannum = num + 1;
  chipCanvas[num] = new TCanvas(name.c_str(), (boost::format("INTT chip plots felix %i") % cannum).str().c_str(), 400, 400, (int) (xsize / 2.2), (int) (ysize / 2.2));
  chipCanvas[num]->SetCanvasSize(xsize, ysize * 2.2);
  gSystem->ProcessEvents();
  chipCanvas[num]->Divide(4, 4);
  return 0;
}

int INTTRawHitDraw::MakeCanvas(const std::string &name, int num)
{
  std::cout << "INTT Raw Hit MakeCanvas() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("INTT Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 2.2), (int) (ysize / 2.2));
  TC[num]->SetCanvasSize(xsize, ysize * 2.2);
  gSystem->ProcessEvents();

  int nrow = 4;
  double yoffset = 0.02;
  double ywidth = (1. - yoffset - yoffset) / (double) nrow;
  double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  for (int i = 0; i < 2 * nrow; i++)
  {
    if (i % 2 == 0)
    {
      x1 = 0.5;
      x2 = 0.95;
    }
    else if (i % 2 == 1)
    {
      x1 = 0.05;
      x2 = 0.5;
    }
    y1 = 0.02 + (i / 2) * ywidth;
    y2 = 0.02 + (i / 2 + 1) * ywidth;

    Pad[num][2 * nrow - 1 - i] = new TPad(
        (boost::format("mypad_%1%_%2%") % num % i).str().c_str(),
        "pad",
        x1, y1, x2, y2);
  }

  for (int i = 0; i < 2 * nrow; i++) Pad[num][2 * nrow - 1 - i]->Draw();

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}
int INTTRawHitDraw::DrawSummary()
{
  std::cout << "INTT DrawSummary() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  TH1 *nhit = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%snhit") % histprefix).str().c_str()));
  TH1 *nhitnorth = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%snhit_north") % histprefix).str().c_str()));
  TH1 *nhitsouth = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%snhit_south") % histprefix).str().c_str()));
  TH1 *pid = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%spid") % histprefix).str().c_str()));
  TH1 *adc = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%sadc") % histprefix).str().c_str()));
  TH1 *bco = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%sbco") % histprefix).str().c_str()));
  TH1 *bcofull = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%sbco_full") % histprefix).str().c_str()));

  if (!gROOT->FindObject("summary_info"))
  {
    MakeCanvas("summary_info", 0);
  }
  Pad[0][0]->cd();
  if (nhit)
  {
    nhit->GetXaxis()->SetTitle("N_{hit}");
    nhit->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][1]->cd();
  if (nhitnorth)
  {
    nhitnorth->GetYaxis()->SetTitle("N_{hit}");
    nhitnorth->SetTitle("INTT Hits North");
    nhitnorth->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][2]->cd();
  if (nhitsouth)
  {
    nhitsouth->GetYaxis()->SetTitle("N_{hit}");
    nhitsouth->SetTitle("INTT Hits South");
    nhitsouth->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][3]->cd();
  if (pid)
  {
    pid->GetXaxis()->SetTitle("Packet ID");
    pid->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][4]->cd();
  if (adc)
  {
    adc->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][5]->cd();
  if (bco)
  {
    bco->GetXaxis()->SetTitle("Crossing");
    bco->DrawCopy();
  }
  else
  {
    return -1;
  }
  Pad[0][6]->cd();
  if (bcofull)
  {
    bcofull->DrawCopy();
  }
  else
  {
    return -1;
  }

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.06);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << Name() << "_intt Summary Info Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  Pad[0][7]->cd();
  PrintRun.DrawText(0.5, 0.5, runstring1.c_str());

  TC[0]->Update();
  std::cout << "DrawSummary ending" << std::endl;
  return 0;
}

int INTTRawHitDraw::DrawChipInfo()
{
  std::cout << "INTT DrawChipInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  TH1 *h_nhits[8][14] = {{nullptr}};

  for (int felix = 0; felix < 8; felix++)
  {
    for (int lad = 0; lad < 14; lad++)
    {
      h_nhits[felix][lad] = dynamic_cast<TH1 *>(cl->getHisto((boost::format("%sintt%i_%i") % histprefix % felix % lad).str().c_str()));
    }
  }

  for (int felix = 0; felix < 8; felix++)
  {
    if (!gROOT->FindObject((boost::format("chip_info%i") % felix).str().c_str()))
    {
      MakeChipCanvas((boost::format("chip_info%i") % felix).str().c_str(), felix);
    }
    for (int lad = 0; lad < 14; lad++)
    {
      chipCanvas[felix]->cd(lad + 1);
      gPad->SetRightMargin(0.13);
      gPad->SetLeftMargin(0.13);
      if (h_nhits[felix][lad])
      {
        h_nhits[felix][lad]->SetTitle((boost::format("FEE %i") % lad).str().c_str());

        h_nhits[felix][lad]->DrawCopy("colz");
      }
    }
    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.06);
    PrintRun.SetNDC();          // set to normalized coordinates
    PrintRun.SetTextAlign(23);  // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << "Server " << felix << " FEE Hit Count Run " << cl->RunNumber();
    runstring1 = runnostream1.str();

    chipCanvas[felix]->cd(15);
    PrintRun.DrawText(0.5, 0.5, runstring1.c_str());
    chipCanvas[felix]->Update();
  }

  std::cout << "DrawChipInfo Ending" << std::endl;
  return 0;
}

int INTTRawHitDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret)  // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL" || what == "CHIP")
  {
    for (int felix = 0; felix < 8; felix++)
    {
      pngfile = cl->htmlRegisterPage(*this, (boost::format("chip_info%i") % felix).str().c_str(), (std::to_string(felix)).c_str(), "png");
      cl->CanvasToPng(chipCanvas[felix], pngfile);
    }
  }
  if (what == "ALL" || what == "SUMMARY")
  {
    pngfile = cl->htmlRegisterPage(*this, "summary_info", "8", "png");
    cl->CanvasToPng(TC[0], pngfile);
  }

  if (what == "ALL" || what == "HITMAP")
  {
    // pngfile = cl->htmlRegisterPage(*this, "hitmap_info", "2", "png");
    // cl->CanvasToPng(TC[1], pngfile);
  }
  return 0;
}

int INTTRawHitDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
