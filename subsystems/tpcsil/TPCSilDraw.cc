#include "TPCSilDraw.h"

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
#include <TColor.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

TPCSilDraw::TPCSilDraw(const std::string &name)
  : QADraw(name)
{
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_TpcSiliconQA_"; 
  return;
}

TPCSilDraw::~TPCSilDraw()
{
  /* delete db; */
  return;
}

int TPCSilDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "MATCHING")
  {
    iret += DrawMatchingInfo();
    idraw++;
  }
  if (what == "ALL" || what == "POSITION")
  {
    iret += DrawPositionInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int TPCSilDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("TPCSil Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  gSystem->ProcessEvents();

  if (num == 1)
  {
    Pad[num][0] = new TPad((boost::format("mypad%d0") % num).str().c_str(), "put", 0.05, 0.52, 0.32, 0.97, 0);
    Pad[num][1] = new TPad((boost::format("mypad%d1") % num).str().c_str(), "a", 0.37, 0.52, 0.64, 0.97, 0);
    Pad[num][2] = new TPad((boost::format("mypad%d2") % num).str().c_str(), "name", 0.69, 0.52, 0.96, 0.97, 0);
    Pad[num][3] = new TPad((boost::format("mypad%d3") % num).str().c_str(), "here", 0.05, 0.02, 0.32, 0.47, 0);
    Pad[num][4] = new TPad((boost::format("mypad%d4") % num).str().c_str(), "hi", 0.37, 0.02, 0.64, 0.47, 0);
    Pad[num][5] = new TPad((boost::format("mypad%d5") % num).str().c_str(), "hello", 0.69, 0.02, 0.96, 0.47, 0);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
    Pad[num][3]->Draw();
    Pad[num][4]->Draw();
    Pad[num][5]->Draw();
  }
  else
  {
    Pad[num][0] = new TPad((boost::format("mypad%d0") % num).str().c_str(), "no", 0.05, 0.25, 0.45, 0.75);
    Pad[num][1] = new TPad((boost::format("mypad%d1") % num).str().c_str(), "no", 0.5, 0.25, 0.95, 0.75);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
  }

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}

int TPCSilDraw::DrawMatchingInfo()
{
  std::cout << "TPCSil DrawMatchingInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1 *h_crossing = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("crossing"))); 
  TH1 *h_trackMatch = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("trackMatch"))); 

  if (! gROOT->FindObject("matching_info"))
  {
    MakeCanvas("matching_info", 0);
  }
  TC[0]->Clear("D");
  Pad[0][0]->cd();
  if (h_crossing)
  {
    h_crossing->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][1]->cd();
  if (h_trackMatch)
  {
    h_trackMatch->Scale(1./h_trackMatch->Integral());
    h_trackMatch->DrawCopy();
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
  runnostream1 << Name() << "_matching Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[0]->Update();
 
  std::cout << "DrawMatchingInfo Ending" << std::endl;
  return 0;
}

int TPCSilDraw::DrawPositionInfo()
{
  std::cout << "TPCSil DrawPositionInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1F *h_xDiff = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("xDiff")));
  TH1F *h_yDiff = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("yDiff")));
  TH1F *h_zDiff = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("zDiff")));
  TH1F *h_phiDiff = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("phiDiff")));
  TH1F *h_etaDiff = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("etaDiff")));

  if (! gROOT->FindObject("position_info"))
  {
    MakeCanvas("position_info", 1);
  }
  TC[1]->Clear("D");

  Pad[1][0]->cd();
  if (h_xDiff)
  {
    h_xDiff->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][1]->cd();
  if (h_yDiff)
  {
    h_yDiff->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][2]->cd();
  if (h_zDiff)
  {
    h_zDiff->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][3]->cd();
  if (h_etaDiff)
  {
    h_etaDiff->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][4]->cd();
  if (h_phiDiff)
  {
    h_phiDiff->DrawCopy();
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
  runnostream1 << Name() << "_position Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[1]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[1]->Update();
 
  std::cout << "DrawPositionInfo Ending" << std::endl;
  return 0;
}
 
int TPCSilDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL" || what == "MATCHING")
  {
    pngfile = cl->htmlRegisterPage(*this, "matching_info", "1", "png");
    cl->CanvasToPng(TC[0], pngfile);
  }
  if (what == "ALL" || what == "POSITION")
  {
    pngfile = cl->htmlRegisterPage(*this, "position_info", "2", "png");
    cl->CanvasToPng(TC[1], pngfile);
  }
  return 0;
}

int TPCSilDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
