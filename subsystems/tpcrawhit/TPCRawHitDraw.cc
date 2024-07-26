#include "TPCRawHitDraw.h"

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
#include <TLine.h>
#include <TEllipse.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

TPCRawHitDraw::TPCRawHitDraw(const std::string &name)
  : QADraw(name)
{
  gStyle->SetOptStat(0);
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_TpcRawHitQA_"; 
  return;
}

TPCRawHitDraw::~TPCRawHitDraw()
{
  /* delete db; */
  return;
}

int TPCRawHitDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "SECTOR")
  {
    iret += DrawSectorInfo();
    idraw++;
  }
  if (what == "ALL" || what == "ONLMON")
  {
    iret += DrawOnlMon();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int TPCRawHitDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), (boost::format("TPC Raw Hit Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  gSystem->ProcessEvents();

  if (num < 8 || (num > 8 && num < 17))
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

int TPCRawHitDraw::DrawSectorInfo()
{
  std::cout << "TPCRawHit DrawSectorInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  std::vector<TH1*> h_hits_secs;
  std::vector<TH2*> h_hits_secs_fees;

  for (int s = 0; s < 24; s++)
  {
    TH1* h = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_sec") + std::to_string(s)));
    TH2* h2 = dynamic_cast <TH2 *> (cl->getHisto(histprefix + std::string("nhits_sec") + std::to_string(s) + std::string("_fees")));

    h_hits_secs.push_back(h); 
    h_hits_secs_fees.push_back(h2); 
  }
  
  TH1* h_bco = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("bco")));

  for (int q = 0; q < 4; q++)
  {
    if (! gROOT->FindObject((boost::format("sec_rawhits_%i") % q).str().c_str()))
    {
      MakeCanvas((boost::format("sec_rawhits_%i") % q).str(), q);
    }
    TC[q]->Clear("D");

    for (int i = 0; i < 6; i++)
    {
      Pad[q][i]->cd();
      if (h_hits_secs[q*6 + i])
      {
        h_hits_secs[q*6 + i]->DrawCopy();
        gPad->SetRightMargin(0.15);
      }
      else
      {
        //histogram is missing
        return -1;
      }
    }
  
    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.04);
    PrintRun.SetNDC();  // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_nhits Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[q]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[q]->Update(); 
  }
  
  for (int q = 0; q < 4; q++)
  {
    if (! gROOT->FindObject((boost::format("sec_fee_rawhits_%i") % q).str().c_str()))
    {
      MakeCanvas((boost::format("sec_fee_rawhits_%i") % q).str(), q + 4);
    }
    TC[q + 4]->Clear("D");

    for (int i = 0; i < 6; i++)
    {
      Pad[q + 4][i]->cd();
      if (h_hits_secs_fees[q*6 + i])
      {
        h_hits_secs_fees[q*6 + i]->DrawCopy("COLZ");
        gPad->SetRightMargin(0.15);
      }
      else
      {
        //histogram is missing
        return -1;
      }
    }
  
    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.04);
    PrintRun.SetNDC();  // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_nhits_fees Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[q + 4]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[q + 4]->Update(); 
  }

  if (! gROOT->FindObject("rawhit_bco"))
  {
    MakeCanvas("rawhit_bco", 8);
  }
  TC[8]->Clear("D");
  Pad[8][0]->cd();
  if (h_bco)
  {
    h_bco->DrawCopy();
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
  runnostream1 << Name() << "_bco Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[8]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[8]->Update(); 
 
  std::cout << "DrawChipInfo Ending" << std::endl;
  return 0;
}
 
int TPCRawHitDraw::DrawOnlMon()
{
  std::cout << "TPCRawHit DrawOnlMon() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();

  std::vector<TH1*> h_hits_sample_1;
  std::vector<TH1*> h_hits_sample_2;
  std::vector<TH1*> h_hits_sample_3;
  std::vector<TH1*> h_adc_1;
  std::vector<TH1*> h_adc_2;
  std::vector<TH1*> h_adc_3;

  for (int s = 0; s < 24; s++)
  {
    {
      TH1* h1 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_sample_sec") + std::to_string(s) + std::string("_R0")));
      TH1* h2 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_sample_sec") + std::to_string(s) + std::string("_R1")));
      TH1* h3 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("nhits_sample_sec") + std::to_string(s) + std::string("_R2")));
      h_hits_sample_1.push_back(h1);
      h_hits_sample_2.push_back(h2);
      h_hits_sample_3.push_back(h3);
    }
    {
      TH1* h1 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("adc_sec") + std::to_string(s) + std::string("_R0")));
      TH1* h2 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("adc_sec") + std::to_string(s) + std::string("_R1")));
      TH1* h3 = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("adc_sec") + std::to_string(s) + std::string("_R2")));
      h_adc_1.push_back(h1);
      h_adc_2.push_back(h2);
      h_adc_3.push_back(h3);
    }
  }
  
  TH1* h_northHits = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("xyPos_North")));
  TH1* h_southHits = dynamic_cast <TH1 *> (cl->getHisto(histprefix + std::string("xyPos_South")));

  for (int q = 0; q < 4; q++)
  {
    if (! gROOT->FindObject((boost::format("sampleDist_%i") % q).str().c_str()))
    {
      MakeCanvas((boost::format("sampleDist_%i") % q).str(), q + 9);
    }
    TC[q + 9]->Clear("D");

    for (int i = 0; i < 6; i++)
    {
      Pad[q + 9][i]->cd();
      if (h_hits_sample_1[q*6 + i] && h_hits_sample_2[q*6 + i] && h_hits_sample_3[q*6 + i])
      {
        h_hits_sample_1[q*6 + i]->SetLineColor(kRed);
        h_hits_sample_1[q*6 + i]->DrawCopy();
        h_hits_sample_2[q*6 + i]->SetLineColor(kGreen);
        h_hits_sample_2[q*6 + i]->DrawCopy("same");
        h_hits_sample_3[q*6 + i]->SetLineColor(kBlue);
        h_hits_sample_3[q*6 + i]->DrawCopy("same");

        double maxBinContent = 0;
        for (int b = 1; b <= h_hits_sample_1[q*6 + i]->GetNbinsX(); b++) 
        {
          double binContent = std::max({h_hits_sample_1[q*6 + i]->GetBinContent(b), h_hits_sample_2[q*6 + i]->GetBinContent(b), 
                                        h_hits_sample_3[q*6 + i]->GetBinContent(b)});
          if (binContent > maxBinContent) maxBinContent = binContent;
        }
        h_hits_sample_1[q*6 + i]->GetYaxis()->SetRangeUser(0, maxBinContent * 1.1); 
        if (i == 0)
        {
          auto legend = new TLegend(0.7, 0.7, 0.9, 0.9);
          legend->AddEntry(h_hits_sample_1[q*6 + i], "R1", "pl");
          legend->AddEntry(h_hits_sample_2[q*6 + i], "R2", "pl");
          legend->AddEntry(h_hits_sample_3[q*6 + i], "R3", "pl");
          legend->SetFillStyle(0);
          legend->Draw();
        }
        gPad->SetRightMargin(0.15);
      }
      else
      {
        //histogram is missing
        return -1;
      }
    }
  
    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.04);
    PrintRun.SetNDC();  // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_sample Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[q + 9]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[q + 9]->Update(); 
  }
  
  for (int q = 0; q < 4; q++)
  {
    if (! gROOT->FindObject((boost::format("sec_adc_%i") % q).str().c_str()))
    {
      MakeCanvas((boost::format("sec_adc_%i") % q).str(), q + 13);
    }
    TC[q + 13]->Clear("D");

    for (int i = 0; i < 6; i++)
    {
      Pad[q + 13][i]->cd();
      if (h_adc_1[q*6 + i] && h_adc_2[q*6 + i] && h_adc_3[q*6 + i])
      {
        h_adc_1[q*6 + i]->SetLineColor(kRed);
        h_adc_1[q*6 + i]->DrawCopy();
        h_adc_2[q*6 + i]->SetLineColor(kGreen);
        h_adc_2[q*6 + i]->DrawCopy("same");
        h_adc_3[q*6 + i]->SetLineColor(kBlue);
        h_adc_3[q*6 + i]->DrawCopy("same");
        double maxBinContent = 0;
        for (int b = 1; b <= h_adc_1[q*6 + i]->GetNbinsX(); b++) 
        {
          double binContent = std::max({h_adc_1[q*6 + i]->GetBinContent(b), h_adc_2[q*6 + i]->GetBinContent(b), 
                                        h_adc_3[q*6 + i]->GetBinContent(b)});
          if (binContent > maxBinContent) maxBinContent = binContent;
        }
        h_adc_1[q*6 + i]->GetYaxis()->SetRangeUser(0, maxBinContent * 1.1); 
        if (i == 0)
        {
          auto legend = new TLegend(0.45, 0.7, 0.7, 0.9);
          legend->AddEntry(h_adc_1[q*6 + i], "R1", "pl");
          legend->AddEntry(h_adc_2[q*6 + i], "R2", "pl");
          legend->AddEntry(h_adc_3[q*6 + i], "R3", "pl");
          legend->SetFillStyle(0);
          legend->Draw();
        }
        gPad->SetLogy();
        gPad->SetRightMargin(0.15);
      }
      else
      {
        //histogram is missing
        return -1;
      }
    }
  
    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.04);
    PrintRun.SetNDC();  // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_adc Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[q + 13]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[q + 13]->Update(); 
  }

  if (! gROOT->FindObject("rawhit_xy"))
  {
    MakeCanvas("rawhit_xy", 17);
  }
  TC[17]->Clear("D");
  Pad[17][0]->cd();
  if (h_northHits)
  {
    h_northHits->DrawCopy("COLZ");

    Double_t sec_gap_inner = (2*M_PI - 0.5024*12.0)/12.0;
    Double_t sec_gap_outer = (2*M_PI - 0.5097*12.0)/12.0;
    Double_t sec_gap = (sec_gap_inner + sec_gap_outer)/2.0;
    Double_t sec_phi = (0.5024 + 0.5097)/2.0;
    TLine *lines[12];
    for(int ln=0;ln<12;ln++)
    {
      lines[ln] = new TLine(311.05*cos((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),311.05*sin((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),759.11*cos((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),759.11*sin((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)));
    }
    TEllipse *e1 = new TEllipse(0.0,0.0,311.05,311.05);
    TEllipse *e2 = new TEllipse(0.0,0.0,(402.49+411.53)/2.0,(402.49+411.53)/2.0);
    TEllipse *e3 = new TEllipse(0.0,0.0,(583.67+574.75)/2.0,(583.67+574.75)/2.0);
    TEllipse *e4 = new TEllipse(0.0,0.0,759.11,759.11);
    e1->SetFillStyle(0);
    e2->SetFillStyle(0);
    e3->SetFillStyle(0);
    e4->SetFillStyle(0);

    e1->Draw("same");
    e2->Draw("same");
    e3->Draw("same");
    e4->Draw("same");
    for(int ln2=0;ln2<12;ln2++)
    {
      lines[ln2]->Draw("same"); 
    }
    gPad->SetRightMargin(0.15);
  }
  else
  {
    // histogram is missing
    return -1;
  }
  Pad[17][1]->cd();
  if (h_southHits)
  {
    h_southHits->DrawCopy("COLZ");

    Double_t sec_gap_inner = (2*M_PI - 0.5024*12.0)/12.0;
    Double_t sec_gap_outer = (2*M_PI - 0.5097*12.0)/12.0;
    Double_t sec_gap = (sec_gap_inner + sec_gap_outer)/2.0;
    Double_t sec_phi = (0.5024 + 0.5097)/2.0;
    TLine *lines[12];
    for(int ln=0;ln<12;ln++)
    {
      lines[ln] = new TLine(311.05*cos((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),311.05*sin((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),759.11*cos((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)),759.11*sin((sec_phi+sec_gap)/2.0+ln*(sec_phi+sec_gap)));
    }
    TEllipse *e1 = new TEllipse(0.0,0.0,311.05,311.05);
    TEllipse *e2 = new TEllipse(0.0,0.0,(402.49+411.53)/2.0,(402.49+411.53)/2.0);
    TEllipse *e3 = new TEllipse(0.0,0.0,(583.67+574.75)/2.0,(583.67+574.75)/2.0);
    TEllipse *e4 = new TEllipse(0.0,0.0,759.11,759.11);
    e1->SetFillStyle(0);
    e2->SetFillStyle(0);
    e3->SetFillStyle(0);
    e4->SetFillStyle(0);

    e1->Draw("same");
    e2->Draw("same");
    e3->Draw("same");
    e4->Draw("same");
    for(int ln2=0;ln2<12;ln2++)
    {
      lines[ln2]->Draw("same"); 
    }
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
  runnostream1 << Name() << "_hitsXY Run " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[17]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  TC[17]->Update(); 
 
  std::cout << "DrawOnlMon Ending" << std::endl;
  return 0;
}
int TPCRawHitDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL" || what == "SECTOR")
  {
    for (int q = 0; q < 4; q++)
    {
      pngfile = cl->htmlRegisterPage(*this, (boost::format("sec_rawhits_%i") % q).str(), (boost::format("%i") % (q + 1)).str(), "png");
      cl->CanvasToPng(TC[q], pngfile);
    }
    for (int q = 0; q < 4; q++)
    {
      pngfile = cl->htmlRegisterPage(*this, (boost::format("sec_fee_rawhits_%i") % q).str(), (boost::format("%i") % (q + 5)).str(), "png");
      cl->CanvasToPng(TC[q + 4], pngfile);
    }
    pngfile = cl->htmlRegisterPage(*this, "rawhit_bco", "9", "png");
    cl->CanvasToPng(TC[8], pngfile);
  }
  if (what == "ALL" || what == "ONLMON")
  {
    for (int q = 0; q < 4; q++)
    {
      pngfile = cl->htmlRegisterPage(*this, (boost::format("sampleDist_%i") % q).str(), (boost::format("%i") % (q + 10)).str(), "png");
      cl->CanvasToPng(TC[q + 9], pngfile);
    }
    for (int q = 0; q < 4; q++)
    {
      pngfile = cl->htmlRegisterPage(*this, (boost::format("sec_adc_%i") % q).str(), (boost::format("%i") % (q + 14)).str(), "png");
      cl->CanvasToPng(TC[q + 13], pngfile);
    }
    pngfile = cl->htmlRegisterPage(*this, "rawhit_xy", "18", "png");
    cl->CanvasToPng(TC[17], pngfile);
  }
  return 0;
}

int TPCRawHitDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
