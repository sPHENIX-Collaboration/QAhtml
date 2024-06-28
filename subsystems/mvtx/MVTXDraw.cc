#include "MVTXDraw.h"

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

MVTXDraw::MVTXDraw(const std::string &name)
  : QADraw(name)
{
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_MvtxClusterQA_"; 
  return;
}

MVTXDraw::~MVTXDraw()
{
  /* delete db; */
  return;
}

int MVTXDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "CHIP")
  {
    iret += DrawChipInfo();
    idraw++;
  }
  if (what == "ALL" || what == "CLUSTER")
  {
    iret += DrawClusterInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int MVTXDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("MVTX Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  gSystem->ProcessEvents();

  Pad[num][0] = new TPad((boost::format("mypad%d0") % num).str().c_str(), "put", 0.05, 0.52, 0.45, 0.97, 0);
  Pad[num][1] = new TPad((boost::format("mypad%d1") % num).str().c_str(), "a", 0.5, 0.52, 0.95, 0.97, 0);
  Pad[num][2] = new TPad((boost::format("mypad%d2") % num).str().c_str(), "name", 0.05, 0.02, 0.45, 0.47, 0);
  Pad[num][3] = new TPad((boost::format("mypad%d3") % num).str().c_str(), "here", 0.5, 0.02, 0.95, 0.47, 0);

  Pad[num][0]->Draw();
  Pad[num][1]->Draw();
  Pad[num][2]->Draw();
  Pad[num][3]->Draw();

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}

int MVTXDraw::DrawChipInfo()
{
  std::cout << "MVTX DrawChipInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1F *h_occupancy = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("chipOccupancy")));
  TH1F *h_clusSize = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("clusterSize")));
  TH1I *h_strobe = dynamic_cast <TH1I *> (cl->getHisto(histprefix + std::string("strobeTiming")));

  if (! gROOT->FindObject("chip_info"))
  {
    MakeCanvas("chip_info", 0);
  }
  TC[0]->Clear("D");
  Pad[0][0]->cd();
  if (h_occupancy)
  {
    h_occupancy->SetTitle("MVTX Chip Occupancy");
    h_occupancy->SetXTitle("Chip Occupancy [%]");
    h_occupancy->SetYTitle("Normalized Entries");
    h_occupancy->Scale(1./h_occupancy->Integral());
    h_occupancy->DrawCopy();
    gPad->SetLogy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][1]->cd();
  if (h_clusSize)
  {
    h_clusSize->SetTitle("MVTX Cluster Size");
    h_clusSize->SetXTitle("Cluster Size");
    h_clusSize->SetYTitle("Normalized Entries");
    h_clusSize->Scale(1./h_clusSize->Integral());
    h_clusSize->DrawCopy();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[0][2]->cd();
  if (h_strobe)
  {
    h_strobe->DrawCopy();
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

int MVTXDraw::DrawClusterInfo()
{
  std::cout << "MVTX DrawChipInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1F *h_clusPhi_incl = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("clusterPhi_incl")));
  TH1F *h_clusPhi_l0 = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("clusterPhi_l0")));
  TH1F *h_clusPhi_l1 = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("clusterPhi_l1")));
  TH1F *h_clusPhi_l2 = dynamic_cast <TH1F *> (cl->getHisto(histprefix + std::string("clusterPhi_l2")));
  TH2F *h_clusZ_clusPhi_l0 = dynamic_cast <TH2F *> (cl->getHisto(histprefix + std::string("clusterZ_clusPhi_l0")));
  TH2F *h_clusZ_clusPhi_l1 = dynamic_cast <TH2F *> (cl->getHisto(histprefix + std::string("clusterZ_clusPhi_l1")));
  TH2F *h_clusZ_clusPhi_l2 = dynamic_cast <TH2F *> (cl->getHisto(histprefix + std::string("clusterZ_clusPhi_l2")));

  if (! gROOT->FindObject("cluster_info"))
  {
    MakeCanvas("cluster_info", 1);
  }

  TC[1]->Clear("D");

  Pad[1][0]->cd();
  if (h_clusPhi_incl && h_clusPhi_l0 && h_clusPhi_l1 && h_clusPhi_l2)
  {
    h_clusPhi_incl->SetTitle("MVTX Cluster #phi");
    h_clusPhi_incl->SetXTitle("Cluster #phi wrt origin [rad]");
    h_clusPhi_incl->SetYTitle("Entries");
    h_clusPhi_incl->SetMarkerSize(0.5);
    h_clusPhi_incl->DrawCopy();
    h_clusPhi_l0->SetMarkerSize(0.5);
    h_clusPhi_l0->SetMarkerColor(kRed);
    h_clusPhi_l0->SetLineColor(kRed);
    h_clusPhi_l0->DrawCopy("same");
    h_clusPhi_l1->SetMarkerSize(0.5);
    h_clusPhi_l1->SetMarkerColor(kBlue);
    h_clusPhi_l1->SetLineColor(kBlue);
    h_clusPhi_l1->DrawCopy("same");
    h_clusPhi_l2->SetMarkerSize(0.5);
    h_clusPhi_l2->SetMarkerColor(kGreen+2);
    h_clusPhi_l2->SetLineColor(kGreen+2);
    h_clusPhi_l2->DrawCopy("same");
    auto legend = new TLegend(0.45, 0.7, 0.7, 0.9);
    legend->AddEntry(h_clusPhi_incl, "Inclusive", "pl");
    legend->AddEntry(h_clusPhi_l0, "Layer 0", "pl");
    legend->AddEntry(h_clusPhi_l1, "Layer 1", "pl");
    legend->AddEntry(h_clusPhi_l2, "Layer 2", "pl");
    legend->SetFillStyle(0);
    legend->Draw();
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][1]->cd();
  if (h_clusZ_clusPhi_l0)
  {
    h_clusZ_clusPhi_l0->SetTitle("MVTX Cluster Z vs phi Layer 0");
    h_clusZ_clusPhi_l0->SetXTitle("Cluster (layer 0) Z [cm]");
    h_clusZ_clusPhi_l0->SetYTitle("Cluster (layer 0) #phi wrt origin [rad]");
    h_clusZ_clusPhi_l0->SetZTitle("Entries");
    h_clusZ_clusPhi_l0->DrawCopy("colz");
    gPad->SetRightMargin(0.17);
    gPad->SetLogz();
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][2]->cd();
  if (h_clusZ_clusPhi_l1)
  {
    h_clusZ_clusPhi_l1->SetTitle("MVTX Cluster Z vs phi Layer 1");
    h_clusZ_clusPhi_l1->SetXTitle("Cluster (layer 1) Z [cm]");
    h_clusZ_clusPhi_l1->SetYTitle("Cluster (layer 1) #phi wrt origin [rad]");
    h_clusZ_clusPhi_l1->SetZTitle("Entries");
    h_clusZ_clusPhi_l1->DrawCopy("colz");
    gPad->SetRightMargin(0.17);
    gPad->SetLogz();
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[1][3]->cd();
  if (h_clusZ_clusPhi_l2)
  {
    h_clusZ_clusPhi_l2->SetTitle("MVTX Cluster Z vs phi Layer 2");
    h_clusZ_clusPhi_l2->SetXTitle("Cluster (layer 2) Z [cm]");
    h_clusZ_clusPhi_l2->SetYTitle("Cluster (layer 2) #phi wrt origin [rad]");
    h_clusZ_clusPhi_l2->SetZTitle("Entries");
    h_clusZ_clusPhi_l2->DrawCopy("colz");
    gPad->SetRightMargin(0.17);
    gPad->SetLogz();
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
  transparent[1]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[1]->Update();
 
  std::cout << "DrawClusterInfo Ending" << std::endl;
  return 0;
}
 
int MVTXDraw::MakeHtml(const std::string &what)
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
  if (what == "ALL" || what == "CLUSTER")
  {
    pngfile = cl->htmlRegisterPage(*this, "cluster_info", "2", "png");
    cl->CanvasToPng(TC[1], pngfile);
  }
  return 0;
}

int MVTXDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
