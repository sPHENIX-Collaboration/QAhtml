#include "GlobalQADraw.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TPad.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLegend.h>
#include <TF1.h>
#include <TGaxis.h>
#include <TSQLServer.h>
#include <TSQLRow.h>
#include <TSQLResult.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

void get_scaledowns(int runnumber, int scaledowns[])
{

  TSQLServer *db = TSQLServer::Connect("pgsql://sphnxdaqdbreplica:5432/daq","phnxro","");

  if (db)
  {
    printf("Server info: %s\n", db->ServerInfo());
  }
  else
  {
    printf("bad\n");
  }

  TSQLRow *row;
  TSQLResult *res;
  char sql[1000];

  for (int is = 0; is < 64; is++)
  {
    sprintf(sql, "select scaledown%02d from gl1_scaledown where runnumber = %d;", is, runnumber);

    res = db->Query(sql);

    int nrows = res->GetRowCount();

    int nfields = res->GetFieldCount();
    for (int i = 0; i < nrows; i++) 
    {
      row = res->Next();
      for (int j = 0; j < nfields; j++) 
      {
        scaledowns[is] = std::stoi(row->GetField(j));
      }
      delete row;
    }

    delete res;
  }
  delete db;
}

GlobalQADraw::GlobalQADraw(const std::string &name)
  : QADraw(name)
{
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  return;
}

GlobalQADraw::~GlobalQADraw()
{
  delete db;
  return;
}

int GlobalQADraw::Draw(const std::string &what)
{
  /* sPHENIX style */ 
  gStyle->SetTitleSize(gStyle->GetTitleSize("X")*2.0, "X");
  gStyle->SetTitleSize(gStyle->GetTitleSize("Y")*2.0, "Y");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleOffset(0.85, "XY");
  gStyle->SetLabelSize(gStyle->GetLabelSize("X")*1.5, "X");
  gStyle->SetLabelSize(gStyle->GetLabelSize("Y")*1.5, "Y");
  gStyle->SetLabelSize(gStyle->GetLabelSize("Z")*1.5, "Z");
  TGaxis::SetMaxDigits(4);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptStat(0);
  gROOT->ForceStyle();
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "MBD")
  {
    iret += DrawMBD(what);
    idraw++;
  }
  if (what == "ALL" || what == "ZDC")
  {
    iret += DrawZDC(what);
    idraw++;
  }
  if (what == "ALL" || what == "Trigger")
  {
    iret += DrawTrigger(what);
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}


int GlobalQADraw::MakeCanvas(const std::string &name,int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  if (num == 0)
  {
    // xpos (-1) negative: do not draw menu bar
    TC[num] = new TCanvas(name.c_str(),"MBD Plots", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
    TC[num]->UseCurrentStyle();
    gSystem->ProcessEvents();

    //Pad[num][0] = new TPad("mypad00", "mbd_zvtx", 0.05, 0.69, 0.45, 0.97, 0);
    Pad[num][1] = new TPad("mypad01", "mbd_zvtx_wide", 0.5, 0.69, 0.95, 0.97, 0);
    Pad[num][2] = new TPad("mypad02", "mbd_charge", 0.05, 0.32, 0.45, 0.97, 0);
    Pad[num][3] = new TPad("mypad03", "mbd_nhit", 0.5, 0.32, 0.95, 0.64, 0);
    Pad[num][4] = new TPad("mypad04", "mbd_zvtxq", 0.05, 0.02, 0.45, 0.28, 0);


    //Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
    Pad[num][3]->Draw();
    Pad[num][4]->Draw();
  }
  
  if (num == 1)
  {
    TC[num] = new TCanvas(name.c_str(),"ZDC Plots", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
    TC[num]->UseCurrentStyle();
    gSystem->ProcessEvents();

    Pad[num][0] = new TPad("mypad10", "zdc_energy", 0.05, 0.52, 0.45, 0.97, 0);
    Pad[num][1] = new TPad("mypad11", "zdc_zvtx", 0.5, 0.52, 0.95, 0.97, 0);
    Pad[num][2] = new TPad("mypad12", "zdc_zvtx_wide", 0.5, 0.02, 0.95, 0.47, 0);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
  }

  if (num == 2)
  {
    TC[num] = new TCanvas(name.c_str(),"Trigger Plots", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
    TC[num]->UseCurrentStyle();
    gSystem->ProcessEvents();

    Pad[num][0] = new TPad("mypad20", "trigger_eff", 0.05, 0.52, 0.45, 0.97, 0);
    Pad[num][1] = new TPad("mypad21", "trigger_persistency", 0.5, 0.52, 0.95, 0.97, 0);
    Pad[num][2] = new TPad("mypad22", "triggers", 0.05, 0.02, 0.45, 0.47, 0);

    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
    Pad[num][2]->Draw();
  }

  // this one is used to plot the run number on the canvas
  transparent[num] = new TPad("transparent0", "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();


  return 0;
}

int GlobalQADraw::DrawMBD(const std::string & /*what*/)
{
  QADrawClient *cl = QADrawClient::instance();
  //TH1 *h_GlobalQA_mbd_zvtx = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_zvtx"));
  TH1 *h_GlobalQA_mbd_zvtx_wide = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_zvtx_wide"));
  //TH1 *h_GlobalQA_calc_zvtx = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_calc_zvtx"));
  TH1 *h_GlobalQA_calc_zvtx_wide = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_calc_zvtx_wide"));
  TH1 *h_GlobalQA_mbd_charge_s = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_charge_s"));
  TH1 *h_GlobalQA_mbd_charge_n = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_charge_n"));
  TH1 *h_GlobalQA_mbd_nhit_s  = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_nhit_s"));
  TH1 *h_GlobalQA_mbd_nhit_n = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_nhit_n"));
  TH1 *h_GlobalQA_mbd_zvtxq = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_mbd_zvtxq"));

  if (!gROOT->FindObject("Global1"))
  {
    MakeCanvas("Global1",0);
  }
  TC[0]->Clear("D");
  // Plot the z-vertices zoomed
  /*Pad[0][0]->cd();
  TLegend * leg00 = new TLegend(.6,.7,.9,.9);
  if (h_GlobalQA_mbd_zvtx && h_GlobalQA_calc_zvtx)
  {
    
    h_GlobalQA_mbd_zvtx->SetLineColor(kBlue);
    h_GlobalQA_mbd_zvtx->DrawCopy("hist");
    gPad->UseCurrentStyle();
    
    h_GlobalQA_calc_zvtx->SetLineColor(kRed);
    h_GlobalQA_calc_zvtx->DrawCopy("hist same");
    
    leg00->AddEntry(h_GlobalQA_mbd_zvtx,"Provided z-vertex");
    leg00->AddEntry(h_GlobalQA_calc_zvtx,"Calculated z-vertex"); 
    leg00->Draw();
    
  }
  else 
  {
    return -1;
  }
*/
  // Plot the z-vertices wide
  TLegend * leg02 = new TLegend(0.6, 0.7, 0.9, 0.9);
  Pad[0][2]->cd();
  if (h_GlobalQA_mbd_zvtx_wide && h_GlobalQA_calc_zvtx_wide)
  {
    TF1 * f = new TF1("f", "gaus", -100,100);
    f->SetParameters(h_GlobalQA_mbd_zvtx_wide->GetMaximum(), h_GlobalQA_mbd_zvtx_wide->GetMean(), h_GlobalQA_mbd_zvtx_wide->GetRMS() );
    h_GlobalQA_mbd_zvtx_wide->Fit("f");
    
    h_GlobalQA_mbd_zvtx_wide->SetLineColor(kBlue);
    leg02->AddEntry(h_GlobalQA_mbd_zvtx_wide,"Provided z-vertex");
    h_GlobalQA_mbd_zvtx_wide->DrawCopy("hist");
     
    gPad->UseCurrentStyle();
    
    f->SetLineColor(kBlack);
    f->DrawCopy("same");
    
    h_GlobalQA_calc_zvtx_wide->SetLineColor(kRed);
    leg02->AddEntry(h_GlobalQA_calc_zvtx_wide,"Calculated z-vertex");
    h_GlobalQA_calc_zvtx_wide->DrawCopy("hist same");
    
    TText printmean;
    TText printrms;
    printmean.SetTextFont(62);
    printrms.SetTextFont(62);
    printmean.SetTextSize(0.06);
    printrms.SetTextSize(0.06);
    printmean.SetNDC();
    printrms.SetNDC();
    float mean = f->GetParameter(1);
    float rms = f->GetParameter(2);
    printmean.DrawText(0.2,0.7,Form("Mean: %.2f",mean));
    printrms.DrawText(0.2,0.5,Form("RMS: %.2f",rms));
  
    
    leg02->Draw();
  }
  else
  {
    return -1;
  }

  // Plot the charge distribution 
  TLegend * leg01 = new TLegend(0.7, 0.7, 0.9, 0.9);
  Pad[0][1]->cd();
  if (h_GlobalQA_mbd_charge_s && h_GlobalQA_mbd_charge_n)
  {
    h_GlobalQA_mbd_charge_s->SetLineColor(kBlue);
    leg01->AddEntry(h_GlobalQA_mbd_charge_s,"South");
    h_GlobalQA_mbd_charge_s->DrawCopy("hist");
    
    gPad->UseCurrentStyle();
    gPad->SetLogy();
      
    h_GlobalQA_mbd_charge_n->SetLineColor(kRed);
    leg01->AddEntry(h_GlobalQA_mbd_charge_n,"North");
    h_GlobalQA_mbd_charge_n->DrawCopy("hist same");
  
    leg01->Draw();
  }
  else
  {
    return -1;
  }

  // Plot the hit distribution
  TLegend * leg03 = new TLegend(0.7, 0.7, 0.9, 0.9);
  Pad[0][3]->cd();
  if (h_GlobalQA_mbd_nhit_s && h_GlobalQA_mbd_nhit_n)
  {
    h_GlobalQA_mbd_nhit_s->SetLineColor(kBlue);
    leg03->AddEntry(h_GlobalQA_mbd_nhit_s,"South");
    h_GlobalQA_mbd_nhit_s->DrawCopy("hist");
  
    gPad->UseCurrentStyle();
    gPad->SetLogy();

    h_GlobalQA_mbd_nhit_n->SetLineColor(kRed);
    leg03->AddEntry(h_GlobalQA_mbd_nhit_n,"North");
    
    TText printnhits;
    TText printnhitn;
    printnhits.SetTextFont(62);
    printnhitn.SetTextFont(62);
    printnhits.SetTextSize(0.06);
    printnhitn.SetTextSize(0.06);
    printnhits.SetNDC();
    printnhitn.SetNDC();
    float means = h_GlobalQA_mbd_nhit_s->GetMean();
    float meann = h_GlobalQA_mbd_nhit_n->GetMean();
    
    h_GlobalQA_mbd_nhit_n->DrawCopy("hist same");
    printnhits.DrawText(0.5,0.6,Form("South mean: %.2f",means));
    printnhitn.DrawText(0.5,0.4,Form("North mean: %.2f",meann));

    leg03->Draw();
  }

  // Plot the number of mbd plots available
  Pad[0][4]->cd();
  gPad->UseCurrentStyle();
  if (h_GlobalQA_mbd_zvtxq)
  {
    h_GlobalQA_mbd_zvtxq->Scale(1.0 / h_GlobalQA_mbd_zvtxq->GetEntries());
    h_GlobalQA_mbd_zvtxq->GetYaxis()->SetRangeUser(0,1);
    
    TText printyes;
    TText printno;
    printyes.SetTextFont(62);
    printno.SetTextFont(62);
    printyes.SetTextSize(0.06);
    printno.SetTextSize(0.06);
    printyes.SetNDC();
    printno.SetNDC();
    float yes = h_GlobalQA_mbd_zvtxq->GetBinContent(2) * 100;
    float no = h_GlobalQA_mbd_zvtxq->GetBinContent(1) * 100;
    
    h_GlobalQA_mbd_zvtxq->DrawCopy("hist");
    printyes.DrawText(0.6,0.7,Form("zvtx provided:\n %.2f%s",yes,"%"));
    printno.DrawText(0.2,0.7,Form("No zvtx:\n %.2f%s",no,"%"));
    
    gPad->UseCurrentStyle();
  }
  else
  {
    return -1;
  }
  

  //db->DBcommit();

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream;
  std::string runstring;
  runnostream << Name() << "_MBD Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());
  TC[0]->Update();
  return 0;
}
int GlobalQADraw::DrawZDC(const std::string & /*what*/)
{
  QADrawClient *cl = QADrawClient::instance();
  TH1 *h_GlobalQA_zdc_zvtx = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_zdc_zvtx"));
  TH1 *h_GlobalQA_zdc_zvtx_wide = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_zdc_zvtx_wide"));
  TH1 *h_GlobalQA_zdc_energy_s = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_zdc_energy_s"));
  TH1 *h_GlobalQA_zdc_energy_n = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_zdc_energy_n"));

  if (!gROOT->FindObject("Global2"))
  {
    MakeCanvas("Global2",1);
  }
  // Plot the ZDC energy distributions
  TLegend * leg10 = new TLegend(0.7, 0.7, 0.9 ,0.9);
  Pad[1][0]->cd();
  if (h_GlobalQA_zdc_energy_s && h_GlobalQA_zdc_energy_n)
  {
    h_GlobalQA_zdc_energy_s->SetLineColor(kBlue);
    leg10->AddEntry(h_GlobalQA_zdc_energy_s,"South");
    h_GlobalQA_zdc_energy_s->DrawCopy("hist");
    
    gPad->UseCurrentStyle();

    h_GlobalQA_zdc_energy_n->SetLineColor(kRed);
    leg10->AddEntry(h_GlobalQA_zdc_energy_n,"North");
    h_GlobalQA_zdc_energy_n->DrawCopy("hist same");
    
    leg10->Draw();
  }
  else
  {
    return -1;
  }

  // Plot the ZDC z-vertex
  Pad[1][1]->cd();
  if (h_GlobalQA_zdc_zvtx)
  {
    h_GlobalQA_zdc_zvtx->DrawCopy();
    gPad->UseCurrentStyle();
  }
  else 
  {
    return -1;
  }

  // Plot the ZDC z-vertex wide
  Pad[1][2]->cd();
  if (h_GlobalQA_zdc_zvtx_wide)
  {
    TF1 * f = new TF1("f", "gaus", -100,100);
    f->SetParameters(h_GlobalQA_zdc_zvtx_wide->GetMaximum(), h_GlobalQA_zdc_zvtx_wide->GetMean(), h_GlobalQA_zdc_zvtx_wide->GetRMS() );
    h_GlobalQA_zdc_zvtx_wide->Fit("f");

    h_GlobalQA_zdc_zvtx_wide->DrawCopy();
    gPad->UseCurrentStyle();
    
    f->SetLineColor(kBlack);
    f->DrawCopy("same");
    
    TText printmean;
    TText printrms;
    printmean.SetTextFont(62);
    printrms.SetTextFont(62);
    printmean.SetTextSize(0.06);
    printrms.SetTextSize(0.06);
    printmean.SetNDC();
    printrms.SetNDC();
    float mean = f->GetParameter(1);
    float rms = f->GetParameter(2);
    printmean.DrawText(0.2,0.7,Form("Mean: %.2f",mean));
    printrms.DrawText(0.2,0.5,Form("RMS: %.2f",rms));
  }
  else 
  {
    return -1;
  }

  //db->DBcommit();
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream;
  std::string runstring;
  runnostream << Name() << "_ZDC Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[1]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());
  TC[1]->Update();
  return 0;
}

int GlobalQADraw::DrawTrigger(const std::string & /*what*/)
{
  QADrawClient *cl = QADrawClient::instance();
  int scaledowns[64] = { 0 };
  get_scaledowns(cl->RunNumber(), scaledowns);

  TH1 *h_GlobalQA_ldClus_trig10 = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_ldClus_trig10"));
  TH1 *h_GlobalQA_ldClus_trig25 = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_ldClus_trig25"));
  TH1 *h_GlobalQA_ldClus_trig26 = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_ldClus_trig26"));
  TH1 *h_GlobalQA_ldClus_trig27 = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_ldClus_trig27"));

  TH1 *pr_GlobalQA_evtNum_ldClus_trig10 = dynamic_cast<TH1 *>(cl->getHisto("pr_GlobalQA_evtNum_ldClus_trig10"));
  TH1 *pr_GlobalQA_evtNum_ldClus_trig25 = dynamic_cast<TH1 *>(cl->getHisto("pr_GlobalQA_evtNum_ldClus_trig25"));
  TH1 *pr_GlobalQA_evtNum_ldClus_trig26 = dynamic_cast<TH1 *>(cl->getHisto("pr_GlobalQA_evtNum_ldClus_trig26"));
  TH1 *pr_GlobalQA_evtNum_ldClus_trig27 = dynamic_cast<TH1 *>(cl->getHisto("pr_GlobalQA_evtNum_ldClus_trig27"));

  TH1 *h_GlobalQA_triggerVec = dynamic_cast<TH1 *>(cl->getHisto("h_GlobalQA_triggerVec"));

  if (!gROOT->FindObject("Global3"))
  {
    MakeCanvas("Global3",2);
  }
  
  // Plot the Trigger efficiency
  TLegend * leg20 = new TLegend(0.5, 0.7, 0.9 ,0.9);
  Pad[2][0]->cd();
  if (h_GlobalQA_ldClus_trig10 && h_GlobalQA_ldClus_trig25 && h_GlobalQA_ldClus_trig26 && h_GlobalQA_ldClus_trig27)
  {
    h_GlobalQA_ldClus_trig10->SetXTitle("Leading cluster energy [GeV]");
    h_GlobalQA_ldClus_trig10->SetYTitle("Counts scaled by (prescale + 1)"); 
    
    gPad->UseCurrentStyle();
    gPad->SetLogy();
    h_GlobalQA_ldClus_trig10->SetLineColor(kBlack);
    h_GlobalQA_ldClus_trig25->SetLineColor(kRed);
    h_GlobalQA_ldClus_trig26->SetLineColor(kGreen);
    h_GlobalQA_ldClus_trig27->SetLineColor(kBlue);

    leg20->AddEntry(h_GlobalQA_ldClus_trig10, "10. MBD N+S >= 1");
    leg20->AddEntry(h_GlobalQA_ldClus_trig25, "25. MBD N+S >= 1 && Photon 3");
    leg20->AddEntry(h_GlobalQA_ldClus_trig26, "26. MBD N+S >= 1 && Photon 4");
    leg20->AddEntry(h_GlobalQA_ldClus_trig27, "27. MBD N+S >= 1 && Photon 5");
   
    h_GlobalQA_ldClus_trig10->Scale(scaledowns[10] + 1);
    h_GlobalQA_ldClus_trig25->Scale(scaledowns[25] + 1);
    h_GlobalQA_ldClus_trig26->Scale(scaledowns[26] + 1);
    h_GlobalQA_ldClus_trig27->Scale(scaledowns[27] + 1);

    h_GlobalQA_ldClus_trig10->SetMinimum(0.5); 
    h_GlobalQA_ldClus_trig10->DrawCopy("hist");
    h_GlobalQA_ldClus_trig25->DrawCopy("hist same");
    h_GlobalQA_ldClus_trig26->DrawCopy("hist same");
    h_GlobalQA_ldClus_trig27->DrawCopy("hist same");
    
    leg20->Draw();
  }
  else
  {
    return -1;
  }
  
  // Plot the trigger persistency
  TLegend * leg21 = new TLegend(0.5, 0.7, 0.9 ,0.9);
  Pad[2][1]->cd();
  if (pr_GlobalQA_evtNum_ldClus_trig10 && pr_GlobalQA_evtNum_ldClus_trig25 && pr_GlobalQA_evtNum_ldClus_trig26 && pr_GlobalQA_evtNum_ldClus_trig27)
  {
    pr_GlobalQA_evtNum_ldClus_trig27->SetXTitle("Events binned by one thousand");
    pr_GlobalQA_evtNum_ldClus_trig27->SetYTitle("Average leading cluster E [GeV]");

    gPad->UseCurrentStyle();

    pr_GlobalQA_evtNum_ldClus_trig10->SetLineColor(kBlack);
    pr_GlobalQA_evtNum_ldClus_trig25->SetLineColor(kRed);
    pr_GlobalQA_evtNum_ldClus_trig26->SetLineColor(kGreen);
    pr_GlobalQA_evtNum_ldClus_trig27->SetLineColor(kBlue);
    
    int last = 0;
    for (int i = 1; i < pr_GlobalQA_evtNum_ldClus_trig10->GetNbinsX(); i++)
    {
      if (pr_GlobalQA_evtNum_ldClus_trig10->GetBinContent(i) != 0)
      {
        last = i;
      }
      else 
      {
        break;
      }
    }

    pr_GlobalQA_evtNum_ldClus_trig10->GetXaxis()->SetRangeUser(0,last); 
    pr_GlobalQA_evtNum_ldClus_trig25->GetXaxis()->SetRangeUser(0,last);
    pr_GlobalQA_evtNum_ldClus_trig26->GetXaxis()->SetRangeUser(0,last);
    pr_GlobalQA_evtNum_ldClus_trig27->GetXaxis()->SetRangeUser(0,last);

    leg21->AddEntry(pr_GlobalQA_evtNum_ldClus_trig10, "10. MBD N+S >= 1");
    leg21->AddEntry(pr_GlobalQA_evtNum_ldClus_trig25, "25. MBD N+S >= 1 && Photon 3 GeV");
    leg21->AddEntry(pr_GlobalQA_evtNum_ldClus_trig26, "26. MBD N+S >= 1 && Photon 4 GeV");
    leg21->AddEntry(pr_GlobalQA_evtNum_ldClus_trig27, "27. MBD N+S >= 1 && Photon 5 GeV");
   
    pr_GlobalQA_evtNum_ldClus_trig27->SetMinimum(0);
    pr_GlobalQA_evtNum_ldClus_trig27->SetMinimum(8);
    pr_GlobalQA_evtNum_ldClus_trig27->DrawCopy("hist");
    pr_GlobalQA_evtNum_ldClus_trig26->DrawCopy("hist same");
    pr_GlobalQA_evtNum_ldClus_trig25->DrawCopy("hist same");
    pr_GlobalQA_evtNum_ldClus_trig10->DrawCopy("hist same");

    leg21->Draw();
  }
  else
  {
    return 1;
  }

  // Plot the triggervector
  Pad[2][2]->cd();
  if (h_GlobalQA_triggerVec)
  {
    h_GlobalQA_triggerVec->SetXTitle("Trigger index");
    h_GlobalQA_triggerVec->SetYTitle("Counts");
    gPad->UseCurrentStyle();
    h_GlobalQA_triggerVec->DrawCopy("hist");
  }
  else
  {
    return 1;
  }

  //db->DBcommit();
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream;
  std::string runstring;
  runnostream << Name() << "_Trigger Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[2]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());
  TC[2]->Update();
  return 0;
}

int GlobalQADraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret)  // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();

  // Register the 1st canvas png file to the menu and produces the png file.
  std::string pngfile = cl->htmlRegisterPage(*this, "MBD", "global1", "png");
  cl->CanvasToPng(TC[0], pngfile);
  pngfile = cl->htmlRegisterPage(*this, "ZDC", "global2", "png");
  cl->CanvasToPng(TC[1], pngfile);
  pngfile = cl->htmlRegisterPage(*this, "Trigger", "global3", "png");
  cl->CanvasToPng(TC[2], pngfile);

  return 0;
}

int GlobalQADraw::DBVarInit()
{
  //db = new QADrawDB(this);
  //db->registerVar("meanpx");
  //db->registerVar("rms");
  //db->DBInit();
  return 0;
}
