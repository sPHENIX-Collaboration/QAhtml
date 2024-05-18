#include "CaloDraw.h"

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

#include <boost/foreach.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

CaloDraw::CaloDraw(const string &name): 
  QADraw(name)
{
  memset(TC,0,sizeof(TC));
  memset(transparent,0,sizeof(transparent));
  memset(Pad,0,sizeof(Pad));
  DBVarInit();
  histprefix = "h_CaloValid_";
  return ;
}

CaloDraw::~CaloDraw()
{
  /* delete db; */
  return;
}

int CaloDraw::Draw(const string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "CEMC")
    {
      iret += DrawCemc();
      idraw ++;
    }
  if (what == "ALL" || what == "IHCAL")
    {
      iret += DrawIhcal();
      idraw ++;
    }
  if (what == "ALL" || what == "OHCAL")
    {
      iret += DrawOhcal();
      idraw ++;
    }
  if (what == "ALL" || what == "CORR")
    {
      iret += DrawCorr();
      idraw ++;
    }
  if (what == "ALL" || what == "ZDC")
    {
      iret += DrawZdc();
      idraw ++;
    }
  if (!idraw)
    {
      std::cout << " Unimplemented Drawing option: " << what << std::endl;
      iret = -1;
    }
  return iret;
}

int CaloDraw::MakeCanvas(const string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), Form("Calo Plots %d", num), -1, 0, (int)(xsize/1.2) , (int)(ysize/1.2));
  gSystem->ProcessEvents();

  if (num < 6) {
    Pad[num][0] = new TPad(Form("mypad%d0", num), "put", 0.05, 0.52, 0.45, 0.97, 0);
    Pad[num][1] = new TPad(Form("mypad%d1", num), "a", 0.5, 0.52, 0.95, 0.97, 0);
    Pad[num][2] = new TPad(Form("mypad%d2", num), "name", 0.05, 0.02, 0.45, 0.47, 0);
    Pad[num][3] = new TPad(Form("mypad%d3", num), "here", 0.5, 0.02, 0.95, 0.47, 0);
 
    /* Pad[0]->SetLogy(); */
    /* Pad[1]->SetLogz(); */
  
    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
    Pad[num][3]->Draw();
  }
  else if (num == 6) {
    Pad[num][0] = new TPad(Form("mypad%d0", num), "no", 0.05, 0.25, 0.45, 0.75);
    Pad[num][1] = new TPad(Form("mypad%d1", num), "no", 0.5, 0.25, 0.95, 0.75);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
  }

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad(Form("transparent%d", num), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  return 0;
}

int CaloDraw::DrawCemc()
{
  QADrawClient *cl = QADrawClient::instance();

  TH2 *cemc_e_chi2 = dynamic_cast <TH2 *> (cl->getHisto(Form("%scemc_e_chi2", histprefix)));
  TH2F *cemc_etaphi = dynamic_cast <TH2F *> (cl->getHisto(Form("%scemc_etaphi", histprefix)));
  TH2 *cemc_etaphi_time = dynamic_cast <TH2 *> (cl->getHisto(Form("%scemc_etaphi_time", histprefix)));
  TH1F *emcal_proj = (TH1F*) proj(cemc_etaphi)->Clone("h_emcal_proj");
  TH1 *invMass = dynamic_cast <TH1 *> (cl->getHisto(Form("%sInvMass", histprefix)));
  TH2 *etaphi_clus = dynamic_cast <TH2 *> (cl->getHisto(Form("%setaphi_clus", histprefix)));

  // canvas 1
  if (! gROOT->FindObject("cemc_1"))
    {
      MakeCanvas("cemc_1", 0);
    }
  TC[0]->Clear("D");
  Pad[0][0]->cd();
  if (cemc_e_chi2)
    {
      cemc_e_chi2->SetTitle("EMCal #chi^{2} vs Energy");
      cemc_e_chi2->SetXTitle("Cluster E (GeV) EMCal");
      cemc_e_chi2->SetYTitle("Cluster #chi^{2} EMCal");
      cemc_e_chi2->GetXaxis()->SetNdivisions(505);
      cemc_e_chi2->GetXaxis()->SetRangeUser(-1,15);
      cemc_e_chi2->DrawCopy("COLZ");
      gPad->SetLogy();
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  else
    {
      // histogram is missing
      return -1;
    }
  Pad[0][1]->cd();
  if (cemc_etaphi)
    {
      cemc_etaphi->SetTitle("EMCal Hits");
      cemc_etaphi->SetXTitle("#it{#eta}_{i} EMCal");
      cemc_etaphi->SetYTitle("#it{#phi}_{i} EMCal");
      cemc_etaphi->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[0][2]->cd();
  if (cemc_etaphi_time)
    {
      cemc_etaphi_time->SetTitle("EMCal Mean Hit Peak Time");
      cemc_etaphi_time->SetXTitle("#it{#eta}_{i} EMCal");
      cemc_etaphi_time->SetYTitle("#it{#phi}_{i} EMCal");
      cemc_etaphi_time->GetXaxis()->SetNdivisions(505);
      cemc_etaphi_time->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[0][3]->cd();
  if (emcal_proj)
    {
      emcal_proj->SetTitle("EMCal #eta Projection");
      emcal_proj->SetXTitle("#it{#eta}_{i} EMCal");
      /* emcal_proj->GetXaxis()->SetNdivisions(505); */
      emcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");
      emcal_proj->DrawCopy("HIST");
    }

  // canvas 2
  if (! gROOT->FindObject("cemc_2"))
    {
      MakeCanvas("cemc_2", 1);
    }
  TC[1]->Clear("D");
  Pad[1][0]->cd();
  if (invMass)
    {
      invMass->SetTitle("EMCal Diphoton Invariant Mass");
      invMass->SetXTitle("M_{#gamma #gamma} (GeV)");
      invMass->SetYTitle("Counts");
      invMass->DrawCopy();
    }
  Pad[1][1]->cd();
  if (etaphi_clus)
    {
      etaphi_clus->SetTitle("EMCal Clusters");
      etaphi_clus->SetXTitle("#it{#eta}_{i} EMCal Clusters");
      etaphi_clus->SetYTitle("#it{#phi}_{i} EMCal Clusters");
      etaphi_clus->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  TH2 *ohcal_etaphi = dynamic_cast <TH2 *> (cl->getHisto(Form("%sohcal_etaphi", histprefix)));
  TH1 *ohcal_proj = (TH1F*) proj(ohcal_etaphi)->Clone("h_ohcal_proj");
  TH2 *ihcal_etaphi = dynamic_cast <TH2 *> (cl->getHisto(Form("%sihcal_etaphi", histprefix)));
  TH1 *ihcal_proj = (TH1*) proj(ihcal_etaphi)->Clone("h_ihcal_proj");
  TH1* h_fb_ratio_emcal = FBratio(emcal_proj);
  TH1* h_fb_ratio_ohcal = FBratio(ohcal_proj);
  TH1* h_fb_ratio_ihcal = FBratio(ihcal_proj);
  Pad[1][2]->cd();
  h_fb_ratio_emcal->Draw("ex0");
  h_fb_ratio_emcal->SetTitle("Calo North-South Ratio");
  h_fb_ratio_emcal->SetYTitle("N^{twr}(#eta_{i})/N^{twr}(#eta_{N-i})");
  h_fb_ratio_emcal->SetXTitle("#eta_{i}");
  h_fb_ratio_emcal->GetYaxis()->SetRangeUser(0.1,2);
  h_fb_ratio_ohcal->Draw("ex0 same");
  h_fb_ratio_ohcal->SetLineColor(kBlue);
  h_fb_ratio_ohcal->SetMarkerColor(kBlue);
  h_fb_ratio_ohcal->SetMarkerStyle(22);
  h_fb_ratio_ihcal->Draw("ex0 same");
  h_fb_ratio_ihcal->SetLineColor(kRed);
  h_fb_ratio_ihcal->SetMarkerColor(kRed);
  h_fb_ratio_ihcal->SetMarkerStyle(33);

  myText(0.52, 0.20,1,"EMCal");
  myText(0.67,0.20,kBlue,"oHCal");
  myText(0.82,0.20,kRed,"iHCal");



  /* db->DBcommit(); */

  /*
  // retrieve variables from previous runs
  vector<QADrawDBVar> history;
  time_t current = cl->BeginRunUnixTime();
  // go back 24 hours
  time_t back =   current - 24*3600;
  db->GetVar(back,current,"meanpx",history);
  DrawGraph(Pad[3],history,back,current);
  */
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  ostringstream runnostream1, runnostream2;
  string runstring1, runstring2;
  runnostream1 << Name() << "_cemc_1 Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  runnostream2 << Name() << "_cemc_2 Run " << cl->RunNumber();
  runstring2 = runnostream2.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
  transparent[1]->cd();
  PrintRun.DrawText(0.5, 1., runstring2.c_str());

  TC[0]->Update();
  TC[1]->Update();
  return 0;
}

int CaloDraw::DrawIhcal()
{
  QADrawClient *cl = QADrawClient::instance();

  TH2 *ihcal_e_chi2 = dynamic_cast <TH2 *> (cl->getHisto(Form("%sihcal_e_chi2", histprefix)));
  TH2F *ihcal_etaphi = dynamic_cast <TH2F *> (cl->getHisto(Form("%sihcal_etaphi", histprefix)));
  TH2 *ihcal_etaphi_time = dynamic_cast <TH2 *> (cl->getHisto(Form("%sihcal_etaphi_time", histprefix)));
  TH1F *ihcal_proj = (TH1F*) proj(ihcal_etaphi)->Clone("h_ihcal_proj");

  // canvas 1
  if (! gROOT->FindObject("ihcal"))
    {
      MakeCanvas("ihcal", 2);
    }
  TC[2]->Clear("D");
  Pad[2][0]->cd();
  if (ihcal_e_chi2)
    {
      ihcal_e_chi2->SetTitle("iHCal #chi^{2} vs Energy");
      ihcal_e_chi2->SetXTitle("Cluster E (GeV) iHCal");
      ihcal_e_chi2->SetYTitle("Cluster #chi^{2} iHCal");
      ihcal_e_chi2->GetXaxis()->SetNdivisions(505);
      ihcal_e_chi2->GetXaxis()->SetRangeUser(-1,15);
      ihcal_e_chi2->DrawCopy("COLZ");
      gPad->SetLogy();
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  else
    {
      // histogram is missing
      return -1;
    }
  Pad[2][1]->cd();
  if (ihcal_etaphi)
    {
      ihcal_etaphi->SetTitle("iHCal Hits");
      ihcal_etaphi->SetXTitle("#it{#eta}_{i} iHCal");
      ihcal_etaphi->SetYTitle("#it{#phi}_{i} iHCal");
      ihcal_etaphi->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[2][2]->cd();
  if (ihcal_etaphi_time)
    {
      ihcal_etaphi_time->SetTitle("iHCal Mean Hit Peak Time");
      ihcal_etaphi_time->SetXTitle("#it{#eta}_{i} iHCal");
      ihcal_etaphi_time->SetYTitle("#it{#phi}_{i} iHCal");
      ihcal_etaphi_time->GetXaxis()->SetNdivisions(505);
      ihcal_etaphi_time->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[2][3]->cd();
  if (ihcal_proj)
    {
      ihcal_proj->SetTitle("iHCal #eta Projection");
      ihcal_proj->SetXTitle("#it{#eta}_{i} iHCal");
      ihcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");
      ihcal_proj->DrawCopy("HIST");
    }

  /* db->DBcommit(); */

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  ostringstream runnostream;
  string runstring;
  runnostream << Name() << "_ihcal Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[2]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());

  TC[2]->Update();
  return 0;
}

int CaloDraw::DrawOhcal()
{
  QADrawClient *cl = QADrawClient::instance();

  TH2 *ohcal_e_chi2 = dynamic_cast <TH2 *> (cl->getHisto(Form("%sohcal_e_chi2", histprefix)));
  TH2F *ohcal_etaphi = dynamic_cast <TH2F *> (cl->getHisto(Form("%sohcal_etaphi", histprefix)));
  TH2 *ohcal_etaphi_time = dynamic_cast <TH2 *> (cl->getHisto(Form("%sohcal_etaphi_time", histprefix)));
  TH1F *ohcal_proj = (TH1F*) proj(ohcal_etaphi)->Clone("h_ohcal_proj");

  // canvas 1
  if (! gROOT->FindObject("ohcal"))
    {
      MakeCanvas("ohcal", 3);
    }
  TC[3]->Clear("D");
  Pad[3][0]->cd();
  if (ohcal_e_chi2)
    {
      ohcal_e_chi2->SetTitle("oHCal #chi^{2} vs Energy");
      ohcal_e_chi2->SetXTitle("Cluster E (GeV) oHCal");
      ohcal_e_chi2->SetYTitle("Cluster #chi^{2} oHCal");
      ohcal_e_chi2->GetXaxis()->SetNdivisions(505);
      ohcal_e_chi2->GetXaxis()->SetRangeUser(-1,15);
      ohcal_e_chi2->DrawCopy("COLZ");
      gPad->SetLogy();
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  else
    {
      // histogram is missing
      return -1;
    }
  Pad[3][1]->cd();
  if (ohcal_etaphi)
    {
      ohcal_etaphi->SetTitle("oHCal Hits");
      ohcal_etaphi->SetXTitle("#it{#eta}_{i} oHCal");
      ohcal_etaphi->SetYTitle("#it{#phi}_{i} oHCal");
      ohcal_etaphi->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[3][2]->cd();
  if (ohcal_etaphi_time)
    {
      ohcal_etaphi_time->SetTitle("oHCal Mean Hit Peak Time");
      ohcal_etaphi_time->SetXTitle("#it{#eta}_{i} oHCal");
      ohcal_etaphi_time->SetYTitle("#it{#phi}_{i} oHCal");
      ohcal_etaphi_time->GetXaxis()->SetNdivisions(505);
      ohcal_etaphi_time->DrawCopy("COLZ");
      gPad->SetRightMargin(0.15);
    }
  Pad[3][3]->cd();
  if (ohcal_proj)
    {
      ohcal_proj->SetTitle("oHCal #eta Projection");
      ohcal_proj->SetXTitle("#it{#eta}_{i} oHCal");
      ohcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");
      ohcal_proj->DrawCopy("HIST");
    }

  /* db->DBcommit(); */

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  ostringstream runnostream;
  string runstring;
  runnostream << Name() << "_ohcal Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[2]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());

  TC[3]->Update();
  return 0;
}

int CaloDraw::DrawCorr()
{
  QADrawClient *cl = QADrawClient::instance();

  TH2 *emcal_mbd = dynamic_cast <TH2 *> (cl->getHisto(Form("%semcal_mbd_correlation", histprefix)));
  TH2 *emcal_hcal = dynamic_cast <TH2 *> (cl->getHisto(Form("%semcal_hcal_correlation", histprefix)));
  TH2 *zdc_emcal = dynamic_cast <TH2 *> (cl->getHisto(Form("%szdc_emcal_correlation", histprefix)));
  TH2 *ihcal_mbd = dynamic_cast <TH2 *> (cl->getHisto(Form("%sihcal_mbd_correlation", histprefix)));
  TH2 *ohcal_mbd = dynamic_cast <TH2 *> (cl->getHisto(Form("%sohcal_mbd_correlation", histprefix)));
  TH1 *vtx_z = dynamic_cast <TH1 *> (cl->getHisto(Form("%svtx_z_raw", histprefix)));

  // canvas 1
  if (! gROOT->FindObject("corr_1"))
    {
      MakeCanvas("corr_1", 4);
    }
  TC[4]->Clear("D");
  Pad[4][0]->cd();
  if (emcal_mbd)
    {
      emcal_mbd->SetTitle("EMCal MBD Correlation");
      emcal_mbd ->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
      emcal_mbd ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
      emcal_mbd->GetXaxis()->SetNdivisions(505);
      emcal_mbd->DrawCopy("COLZ");
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  else
    {
      // histogram is missing
      return -1;
    }
  Pad[4][1]->cd();
  if (emcal_hcal)
    {
      emcal_hcal->SetTitle("EMCal HCal Correlation");
      emcal_hcal ->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
      emcal_hcal ->SetYTitle("#Sigma #it{E}^{HCal} [Arb]");
      emcal_hcal->GetXaxis()->SetNdivisions(505);
      emcal_hcal->DrawCopy("COLZ");
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  Pad[4][2]->cd();
  if (zdc_emcal)
    {
      zdc_emcal->SetTitle("ZDC EMCal Correlation");
      zdc_emcal ->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
      zdc_emcal ->SetYTitle("#Sigma #it{E}^{ZDC} [Arb]");
      zdc_emcal->GetXaxis()->SetNdivisions(505);
      zdc_emcal->DrawCopy("COLZ");
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }

  // canvas 2
  if (! gROOT->FindObject("corr_2"))
    {
      MakeCanvas("corr_2", 5);
    }
  TC[5]->Clear("D");
  Pad[5][0]->cd();
  if (ihcal_mbd)
    {
      ihcal_mbd->SetTitle("iHCal MBD Correlation");
      ihcal_mbd ->SetXTitle("#Sigma #it{E}^{iHCal} [Arb]");
      ihcal_mbd ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
      ihcal_mbd->GetXaxis()->SetNdivisions(505);
      ihcal_mbd->DrawCopy("COLZ");
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  Pad[5][1]->cd();
  if (ohcal_mbd)
    {
      ohcal_mbd->SetTitle("oHCal MBD Correlation");
      ohcal_mbd ->SetXTitle("#Sigma #it{E}^{oHCal} [Arb]");
      ohcal_mbd ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
      ohcal_mbd->GetXaxis()->SetNdivisions(505);
      ohcal_mbd->DrawCopy("COLZ");
      gPad->SetLogz();
      gPad->SetRightMargin(0.15);
    }
  Pad[5][2]->cd();
  if (vtx_z)
    {
      vtx_z->SetTitle("MBD Vertex z");
      vtx_z ->SetXTitle("MBD Vtx #it{z} (cm)");
      vtx_z ->SetYTitle("Counts");
      vtx_z->DrawCopy("");
    }

  /* db->DBcommit(); */

  /*
  // retrieve variables from previous runs
  vector<QADrawDBVar> history;
  time_t current = cl->BeginRunUnixTime();
  // go back 24 hours
  time_t back =   current - 24*3600;
  db->GetVar(back,current,"meanpx",history);
  DrawGraph(Pad[3],history,back,current);
  */
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  ostringstream runnostream1, runnostream2;
  string runstring1, runstring2;
  runnostream1 << Name() << "_corr_1 Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  runnostream2 << Name() << "_corr_2 Run " << cl->RunNumber();
  runstring2 = runnostream2.str();
  transparent[4]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
  transparent[5]->cd();
  PrintRun.DrawText(0.5, 1., runstring2.c_str());

  TC[4]->Update();
  TC[5]->Update();
  return 0;
}

int CaloDraw::DrawZdc()
{
  QADrawClient *cl = QADrawClient::instance();

  TH1 *zdc_Northcalib = dynamic_cast <TH1 *> (cl->getHisto(Form("%szdcNorthcalib", histprefix)));
  TH1 *zdc_Southcalib = dynamic_cast <TH1 *> (cl->getHisto(Form("%szdcSouthcalib", histprefix)));

  // canvas 1
  if (! gROOT->FindObject("zdc"))
    {
      MakeCanvas("zdc", 6);
    }
  TC[6]->Clear("D");
  Pad[6][0]->cd();
  if (zdc_Northcalib && zdc_Southcalib)
    {
      zdc_Northcalib->SetLineColor(kBlue);
      zdc_Northcalib->GetXaxis()->SetRangeUser(0.0,12000);
      zdc_Northcalib->SetTitle("ZDC Total Energy");
      zdc_Northcalib->SetXTitle("#Sigma #it{E}^{ZDC Side}");
      zdc_Northcalib->SetYTitle("Events");
      zdc_Northcalib->GetXaxis()->SetNdivisions(505);
      zdc_Northcalib->DrawCopy();

      zdc_Southcalib->SetLineColor(kRed);
      zdc_Southcalib->DrawCopy("same");
      gPad->SetLogy();

      myText(0.75, 0.80, kBlue,"North");
      myText(0.65, 0.80, kRed,"South");
    }
  else
    {
      // histogram is missing
      return -1;
    }
  Pad[6][1]->cd();
  if (zdc_Northcalib && zdc_Southcalib)
    {
      zdc_Northcalib->Draw();
      zdc_Northcalib->SetLineColor(kBlue);
      zdc_Northcalib->GetXaxis()->SetRangeUser(10,300);
      zdc_Northcalib->SetTitle("ZDC Total Energy");
      zdc_Northcalib->SetXTitle("#Sigma #it{E}^{ZDC Side}");
      zdc_Northcalib->SetYTitle("Events");

      TGraph* gr_1n = new TGraph();
      gr_1n->SetPoint(0,100,0);
      gr_1n->SetPoint(1,100,1e7);
      gr_1n->SetLineStyle(7);
      gr_1n->Draw("l");

      zdc_Southcalib->Draw("same");
      zdc_Southcalib->SetLineColor(kRed);
      gPad->SetLogy();

      myText(0.75, 0.80, kBlue,"North");
      myText(0.65, 0.80, kRed,"South");
    }

  /* db->DBcommit(); */

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  ostringstream runnostream;
  string runstring;
  runnostream << Name() << "_zdc Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[6]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());

  TC[6]->Update();
  return 0;
}

int CaloDraw::MakeHtml(const string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
    {
      return iret;
    }

  QADrawClient *cl = QADrawClient::instance();

  // Register the 1st canvas png file to the menu and produces the png file.
  string pngfile = cl->htmlRegisterPage(*this,"CEMC","1","png");
  cl->CanvasToPng(TC[0],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"CEMC","2","png");
  cl->CanvasToPng(TC[1],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"IHCAL","3","png");
  cl->CanvasToPng(TC[2],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"OHCAL","4","png");
  cl->CanvasToPng(TC[3],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"CORR","5","png");
  cl->CanvasToPng(TC[4],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"CORR","6","png");
  cl->CanvasToPng(TC[5],pngfile);
  pngfile = cl->htmlRegisterPage(*this,"ZDC","7","png");
  cl->CanvasToPng(TC[6],pngfile);

  return 0;
}

int
CaloDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}

TH1* CaloDraw::proj(TH2* h2)
{
  int x = h2->GetXaxis()->GetNbins();
  int y = h2->GetYaxis()->GetNbins();
  TH1* h = (TH1F*) h2->ProjectionX("temp");
  h->Reset();
  for (int ix=1; ix<x+1; ix++){
    float sum = 0;
    float cc = 0;
    for(int iy=1; iy<y+1; iy++){
      float bc = h2->GetBinContent(ix,iy);
      if (bc==0) cc+=1;
      sum += bc;
    }
    if (cc==y) continue;
    float sum_cor = sum*y/(y-cc);
    h->SetBinContent(ix,sum_cor);
    h->SetBinError(ix,sqrt(sum_cor));
  }
  return h;
}

TH1* CaloDraw::FBratio(TH1* h)
{
  const int Nbin =  h->GetXaxis()->GetNbins();
  TH1* hfb = new TH1F("temp32","",Nbin/2,0,Nbin/2);

  for(int i=0; i<Nbin/2; i++){
    int b1 = i+1;
    int b2 = Nbin-i;
    float ratio = h->GetBinContent(b1)/h->GetBinContent(b2);
    float err = sqrt( pow(h->GetBinError(b1)/h->GetBinContent(b1),2)+ pow(h->GetBinError(b2)/h->GetBinContent(b2),2))*pow(ratio,2);
    hfb->SetBinContent(i,ratio); 
    hfb->SetBinError(i,err);
  }
  return hfb;
}

void CaloDraw::myText(double x,double y,int color, const char *text, double tsize)
{
  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
