#include "MVTXRawHitDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLatex.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

MVTXRawHitDraw::MVTXRawHitDraw(const std::string &name)
  : QADraw(name)
{
  gStyle->SetOptStat(0);
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_MvtxRawHitQA_"; 
  return;
}

MVTXRawHitDraw::~MVTXRawHitDraw()
{
  /* delete db; */
  return;
}

int MVTXRawHitDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "CHIP")
  {
    iret += DrawChipInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int MVTXRawHitDraw::MakeCanvas(const std::string &name, int num)
{
  std::cout << "MVTX Raw Hit MakeCanvas() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("MVTX Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 2.2) , (int) (ysize / 2.2));
  TC[num]->SetCanvasSize(xsize, ysize * 2.2);
  gSystem->ProcessEvents();


  int nrow = 3;
  double yoffset = 0.02;
  double ywidth=(1.-yoffset-yoffset)/(double)nrow;
  double x1=0, y1=0, x2=0, y2=0;
  for (int i=0; i<2*nrow; i++)
  {
      if (i%2==0)
      {
        x1=0.5;
        x2=0.95;
      }
      else if (i%2==1)
      {
        x1=0.05;
        x2=0.5;
      }
      y1=0.02+(i/2)*ywidth;
      y2=0.02+(i/2+1)*ywidth;

      Pad[num][2*nrow-1-i] = new TPad(
                              (boost::format("mypad_%1%_%2%") % num % i).str().c_str(),
                              "pad",
                              x1, y1, x2, y2
                              );
  }

  for (int i=0; i<2*nrow; i++) Pad[num][2*nrow-1-i]->Draw();

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}

int MVTXRawHitDraw::DrawChipInfo()
{
  std::cout << "MVTX DrawChipInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1 *h_nhits_per_chip_layper0 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_per_chip_layper0")));
  TH1 *h_nhits_per_chip_layper1 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_per_chip_layper1")));
  TH1 *h_nhits_per_chip_layper2 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_per_chip_layper2")));

  TH1 *h_chipocc_layper0 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("chipocc_layper0")));
  TH1 *h_chipocc_layper1 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("chipocc_layper1")));
  TH1 *h_chipocc_layper2 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("chipocc_layper2")));

  TH1 *h_bco = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("bco")));
  TH1 *h_strobe_bc = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("strobe_bc")));
  TH1 *h_chip_bc = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("chip_bc")));

  if (! gROOT->FindObject("chip_info"))
  {
    MakeCanvas("chip_info", 0);
  }
  TC[0]->Clear("D");
  Pad[0][0]->cd();
  if (h_chipocc_layper0 && h_chipocc_layper1 && h_chipocc_layper2)
  {
    h_chipocc_layper0->SetTitle("MVTX Chip Occupancy");
    h_chipocc_layper0->SetXTitle("Chip Occupancy [%]");
    h_chipocc_layper0->SetYTitle("Entries");
    h_chipocc_layper0->SetMarkerColor(kRed);
    h_chipocc_layper0->SetLineColor(kRed);
    h_chipocc_layper0->GetXaxis()->SetNdivisions(5, kTRUE);
    auto ymax_l0 = h_chipocc_layper0->GetMaximum();
    auto ymax_l1 = h_chipocc_layper1->GetMaximum();
    auto ymax_l2 = h_chipocc_layper2->GetMaximum();
    h_chipocc_layper0->SetMaximum(1.2*std::max({ymax_l0, ymax_l1, ymax_l2}));
    h_chipocc_layper0->DrawCopy();
    h_chipocc_layper1->SetMarkerColor(kBlue);
    h_chipocc_layper1->SetLineColor(kBlue);
    h_chipocc_layper1->DrawCopy("same");
    h_chipocc_layper2->SetMarkerColor(kBlack);
    h_chipocc_layper2->SetLineColor(kBlack);
    h_chipocc_layper2->DrawCopy("same");
    auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
    legend->AddEntry(h_chipocc_layper0, "Layer0", "pl");
    legend->AddEntry(h_chipocc_layper1, "Layer1", "pl");
    legend->AddEntry(h_chipocc_layper2, "Layer2", "pl");
    legend->Draw();
    gPad->SetLogy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][1]->cd();
  if (h_nhits_per_chip_layper0 && h_nhits_per_chip_layper1 && h_nhits_per_chip_layper2)
  {
    h_nhits_per_chip_layper0->SetTitle("MVTX Raw Hit Number");
    h_nhits_per_chip_layper0->SetXTitle("Hit number");
    h_nhits_per_chip_layper0->SetYTitle("Entries");
    h_nhits_per_chip_layper0->SetMarkerColor(kRed);
    h_nhits_per_chip_layper0->SetLineColor(kRed);
    auto ymax_l0 = h_nhits_per_chip_layper0->GetMaximum();
    auto ymax_l1 = h_nhits_per_chip_layper1->GetMaximum();
    auto ymax_l2 = h_nhits_per_chip_layper2->GetMaximum();
    h_nhits_per_chip_layper0->SetMaximum(1.2*std::max({ymax_l0, ymax_l1, ymax_l2}));
    h_nhits_per_chip_layper0->DrawCopy();
    h_nhits_per_chip_layper1->SetMarkerColor(kBlue);
    h_nhits_per_chip_layper1->SetLineColor(kBlue);
    h_nhits_per_chip_layper1->DrawCopy("same");
    h_nhits_per_chip_layper2->SetMarkerColor(kBlack);
    h_nhits_per_chip_layper2->SetLineColor(kBlack);
    h_nhits_per_chip_layper2->DrawCopy("same");
    auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
    legend->AddEntry(h_nhits_per_chip_layper0, "Layer0", "pl");
    legend->AddEntry(h_nhits_per_chip_layper1, "Layer1", "pl");
    legend->AddEntry(h_nhits_per_chip_layper2, "Layer2", "pl");
    legend->Draw();
    gPad->SetLogy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }

  Pad[0][2]->cd();
  if (h_bco)
  {
    h_bco->SetTitle("MVTX BCO");
    h_bco->SetXTitle("MVTX BCO");
    h_bco->SetYTitle("Entries");
    h_bco->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][3]->cd();
  if (h_strobe_bc)
  {
    h_strobe_bc->SetTitle("MVTX Strobe BC");
    h_strobe_bc->SetXTitle("MVTX Strobe BC");
    h_strobe_bc->SetYTitle("Entries");
    h_strobe_bc->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][4]->cd();
  if (h_chip_bc)
  {
    h_chip_bc->SetTitle("MVTX Chip BC");
    h_chip_bc->SetXTitle("MVTX Chip BC");
    h_chip_bc->SetYTitle("Entries");
    h_chip_bc->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << Name() << "_mvtx Info Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[0]->Update();
 
  std::cout << "DrawChipInfo Ending" << std::endl;
  return 0;
}
 
int MVTXRawHitDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL" || what == "HITS")
  {
    pngfile = cl->htmlRegisterPage(*this, "chip_info", "1", "png");
    cl->CanvasToPng(TC[0], pngfile);
  }
  return 0;
}

int MVTXRawHitDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
