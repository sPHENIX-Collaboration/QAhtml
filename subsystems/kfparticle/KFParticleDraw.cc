#include "KFParticleDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TH1.h>
#include <TH2.h>
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

KFParticleDraw::KFParticleDraw(const std::string &name)
  : QADraw(name)
{
  gStyle->SetOptStat(0);
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_QAKFParticle_SvtxTrackMap_"; 
  return;
}

KFParticleDraw::~KFParticleDraw()
{
  /* delete db; */
  return;
}

int KFParticleDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "KINGEO")
  {
    iret += DrawKinematicGeometricInfo();
    idraw++;
  }
  if (what == "ALL" || what == "TRIGGER")
  {
    iret += DrawTriggerInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int KFParticleDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("KFParticle Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  gSystem->ProcessEvents();

  if (num < 1)
  {
    Pad[num][0] = new TPad((boost::format("mypad%d0") % num).str().c_str(), "put", 0.05, 0.52, 0.45, 0.97, 0);
    Pad[num][1] = new TPad((boost::format("mypad%d1") % num).str().c_str(), "a", 0.5, 0.52, 0.95, 0.97, 0);
    Pad[num][2] = new TPad((boost::format("mypad%d2") % num).str().c_str(), "name", 0.05, 0.02, 0.45, 0.47, 0);
    Pad[num][3] = new TPad((boost::format("mypad%d3") % num).str().c_str(), "here", 0.5, 0.02, 0.95, 0.47, 0);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
    Pad[num][3]->Draw();
  } 
  else
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
  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}

int KFParticleDraw::DrawKinematicGeometricInfo()
{
  std::cout << "KFParticle DrawKinematicGeometricInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1* h = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP")));
  TH2* h2 = dynamic_cast <TH2 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_Eta")));
  TH2* h3 = dynamic_cast <TH2 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_Phi")));
  TH2* h4 = dynamic_cast <TH2 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_pT")));

  if (! gROOT->FindObject((boost::format("kfp_kingeo_%i") % (0)).str().c_str()))
  {
    MakeCanvas((boost::format("kfp_kingeo_%i") % (0)).str(), (0));
  }
  TC[0]->Clear("D");

  Pad[0][0]->cd();
  if (h)
  {
    h->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
  }
  else
  {
    //histogram is missing
    return -1;
  }
  
  Pad[0][1]->cd();
  if (h2)
  {
    h2->DrawCopy("COLZ");
    gPad->SetRightMargin(0.15);
  }
  else
  {
    //histogram is missing
    return -1;
  }
  
  Pad[0][2]->cd();
  if (h3)
  {
    h3->DrawCopy("COLZ");
    gPad->SetRightMargin(0.15);
  }
  else
  {
    //histogram is missing
    return -1;
  }
  
  Pad[0][3]->cd();
  if (h4)
  {
    h4->DrawCopy("COLZ");
    gPad->SetRightMargin(0.15);
  }
  else
  {
    //histogram is missing
    return -1;
  }

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << "QAKFParticle Kinematic/Geometric Distributions Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[0]->Update(); 

  std::cout << "DrawKinematicGeometricInfo() Ending" << std::endl;
  return 0;
}

int KFParticleDraw::DrawTriggerInfo()
{
  std::cout << "KFParticle DrawTriggerInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  TH1* h = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_crossing0")));
  std::cout << "Added 1" << std::endl;
  TH1* h2 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_non_crossing0")));
  std::cout << "Added 2" << std::endl;
  TH1* h3 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_ZDC_Coincidence")));
  std::cout << "Added 3" << std::endl;
  TH1* h4 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_MBD_NandS_geq_1_vtx_l_30_cm")));
  std::cout << "Added 4" << std::endl;
  TH1* h5 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("InvMass_KFP_Jet_6_GeV_MBD_NandS_geq_1_vtx_l_10_cm")));
  std::cout << "Added 5" << std::endl;

  if (! gROOT->FindObject((boost::format("kfp_trigger_%i") % (1)).str().c_str()))
  {
    MakeCanvas((boost::format("kfp_trigger_%i") % (1)).str(), (1));
  }
  TC[1]->Clear("D");

  Pad[1][0]->cd();
  if (h)
  {
    h->GetXaxis()->SetNdivisions(5);
    h->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
    TLatex *title = new TLatex();
    title->SetTextSize(0.06);
    title->SetNDC();
    title->DrawLatex(0.3, 0.75, "Crossing = 0");
  }
  else
  {
    std::cout << "InvMass_KFP_crossing0 is missing" << std::endl;
    //histogram is missing
    return -1;
  }
  
  Pad[1][1]->cd();
  if (h2)
  {
    h2->GetXaxis()->SetNdivisions(5);
    h2->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
    gPad->SetTopMargin(0.15);
    TLatex *title = new TLatex();
    title->SetTextSize(0.06);
    title->SetNDC();
    title->DrawLatex(0.3, 0.75, "Crossing != 0");
  }
  else
  {
    std::cout << "InvMass_KFP_non_crossing0 is missing" << std::endl;
    //histogram is missing
    return -1;
  }
  
  Pad[1][2]->cd();
  if (h3)
  {
    h3->GetXaxis()->SetNdivisions(5);
    h3->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
    gPad->SetTopMargin(0.15);
    TLatex *title = new TLatex();
    title->SetTextSize(0.06);
    title->SetNDC();
    title->DrawLatex(0.1, 0.75, "ZDC Coincidence Trigger");
  }
  else
  {
    std::cout << "InvMass_KFP_ZDC_Coincidence is missing" << std::endl;
    //histogram is missing
    return -1;
  }
  
  Pad[1][3]->cd();
  if (h4)
  {
    h4->GetXaxis()->SetNdivisions(5);
    h4->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
    gPad->SetTopMargin(0.15);
    TLatex *title = new TLatex();
    title->SetTextSize(0.06);
    title->SetNDC();
    title->DrawLatex(0.1, 0.75, "MBD NS >= 1, V < 30cm Trigger");
  }
  else
  {
    std::cout << "InvMass_KFP_MBD_NandS_geq_1_vtx_l_30_cm is missing" << std::endl;
    //histogram is missing
    return -1;
  }

  Pad[1][4]->cd();
  if (h5)
  {
    h5->GetXaxis()->SetNdivisions(5);
    h5->DrawCopy("HIST");
    gPad->SetRightMargin(0.15);
    gPad->SetTopMargin(0.15);
    TLatex *title = new TLatex();
    title->SetTextSize(0.06);
    title->SetNDC();
    title->DrawLatex(0.1, 0.75, "Jet 6 GeV,MBD NS >= 1, V < 10cm Trigger");
  }
  else
  {
    std::cout << "InvMass_KFP_Jet_6_GeV_MBD_NandS_geq_1_vtx_l_10_cm is missing" << std::endl;
    //histogram is missing
    return -1;
  }

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << "QAKFParticle Trigger Conditions Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[1]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[1]->Update(); 
  
  std::cout << "DrawTriggerInfo() Ending" << std::endl;
  return 0;
}

int KFParticleDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL" || what == "KINGEO")
  {
    pngfile = cl->htmlRegisterPage(*this, (boost::format("kfp_kingeo_%i") % (0)).str(), (boost::format("%i") % (1)).str(), "png");
    cl->CanvasToPng(TC[0], pngfile);
  }
  if (what == "ALL" || what == "TRIGGER")
  {
    pngfile = cl->htmlRegisterPage(*this, (boost::format("kfp_trigger_%i") % (1)).str(), (boost::format("%i") % (2)).str(), "png");
    cl->CanvasToPng(TC[1], pngfile);
  }
  return 0;
}

int KFParticleDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
