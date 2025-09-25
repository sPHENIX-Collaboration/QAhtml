#include "TPCLasersDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLine.h>
#include <TPaveLabel.h>
#include <TPad.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TPaveText.h>

#include <boost/format.hpp>

#include <sstream>


TPCLasersDraw::TPCLasersDraw(const std::string &name)
  : QADraw(name)
{
  gStyle->SetOptStat(0);
  //memset(TC, 0, sizeof(TC));
  //memset(transparent, 0, sizeof(transparent));
  //memset(Pad, 0, sizeof(Pad));
  DBVarInit();
  histprefix = "h_TpcLaserQA_"; 
  return;
}

TPCLasersDraw::~TPCLasersDraw()
{
  /* delete db; */
  return;
}

int TPCLasersDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL")
  {
    iret += DrawLaserInfo();
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int TPCLasersDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC[num] = new TCanvas(name.c_str(), "TPC Laser QA Plots", -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  if(num < 2)
  {
    TC[num]->Divide(2,1);
  }
  if(num == 3 || num == 4)
  {
    TC[num]->Divide(4,3);
  }
  gSystem->ProcessEvents();

  if(num < 2)
  {
    Pad[num][0] = new TPad("mypad0","South",0.51,0.0,0.95,0.95,0);
    Pad[num][1] = new TPad("mypad1","North",0.0,0.0,0.49,0.95,0);


    Pad[num][0]->Draw();
    Pad[num][1]->Draw();
  }
  if(num == 3 || num == 4)
  {
    for(int sec = 0; sec<12; sec++)
    {
      Pad[num][sec] = new TPad((boost::format("mypad%i_%s") % sec % (num == 3 ? "South" : "North")).str().c_str(),(boost::format("%s") % (num == 3 ? "South" : "North")).str().c_str(),1.0*(sec%4)*0.25,1.0*(2-sec/4)*0.31,1.0*((sec%4)+1)*0.25,1.0*((2-sec/4)+1)*0.31,0);

      Pad[num][sec]->Draw();
    }
  }

  transparent = new TPad("transparent", "this does not show", 0, 0, 1, 1);
  transparent->SetFillStyle(4000);
  transparent->Draw();
  

  return 0;
}

int TPCLasersDraw::DrawLaserInfo()
{
  std::cout << "TPCLasers DrawLaserInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  
  
  TH1* h_nLaserEvents = dynamic_cast<TH1 *>(cl->getHisto(histprefix + std::string("nLaserEvents")));
  if (!h_nLaserEvents)
  {
    std::cout << "nLaserEvents hist not found. Returning -1" << std::endl;
    return -1;
  }
  
  TH2* h_TPCWheel[2];
  TH2* h_saturation[2];
  TH1* h_nLaserClusters[2];
  TH1 *h_timeSamples[2][12][3];
  TH2* dummyHist[2];
  TPaveLabel* labels[2][12];

  int minTimeSample = 320;
  int maxTimeSample = 330;
  
  double labelNumbers[12][4] = {{1.046586,-0.1938999,1.407997,0.2144871},{0.962076,0.4382608,1.323487,0.8466479},{0.4801947,0.8802139,0.8416056,1.288601},{-0.1823921,1.011681,0.1790189,1.425662},{-0.8449788,0.8690253,-0.4835679,1.288601},{-1.30879,0.441058,-0.9473786,0.8550394},{-1.411009,-0.2050886,-1.049598,0.2144871},{-1.302585,-0.7757116,-0.9471979,-0.3561359},{-0.8449788,-1.309971,-0.4835679,-0.8848013},{-0.1823921,-1.426557,0.1790189,-1.006982},{0.4801947,-1.309076,0.8416056,-0.8839062},{0.9622567,-0.7785088,1.323668,-0.3533387}};
  
  double maxZ = 0.0;
  double maxZ_sat = 0.0;
  double maxY[2][12] = {0.0};
  //double maxY = 0.0;

  int nGoodModules = 0;
  int nGoodTimeSamples = 0;
  
  for (int side = 0; side < 2; side++)
  {
    h_TPCWheel[side] = dynamic_cast<TH2 *>(cl->getHisto(histprefix + std::string("TPCWheel_") + (side == 1 ? "North" : "South")));
    h_TPCWheel[side]->Scale(1.0/h_nLaserEvents->GetEntries());
    if(h_TPCWheel[side]->GetMaximum() > maxZ)
    {
      maxZ = h_TPCWheel[side]->GetMaximum();
    }

    for(int XX=1; XX <= h_TPCWheel[side]->GetNbinsX(); XX++)
    {
      for(int YY=1; YY <= h_TPCWheel[side]->GetNbinsY(); YY++)
      {
	if(h_TPCWheel[side]->GetBinContent(XX,YY) > 1000)
	{
	  nGoodModules++;
	}
      }
    }

    h_saturation[side] = dynamic_cast<TH2 *>(cl->getHisto(histprefix + std::string("saturation_") + (side == 1 ? "North" : "South")));
    h_saturation[side]->Scale(1.0/h_nLaserEvents->GetEntries());
    if(h_saturation[side]->GetMaximum() > maxZ_sat)
    {
      maxZ_sat = h_saturation[side]->GetMaximum();
    }

    h_nLaserClusters[side] = dynamic_cast<TH1 *>(cl->getHisto(histprefix + std::string("nLaserClusters_") + (side == 1 ? "North" : "South")));
    //if(h_nLaserClusters[side]->GetMaximum() > maxY)
    //{
    //maxY = h_nLaserClusters[side]->GetMaximum();
    //}
    
    dummyHist[side] = new TH2D((boost::format("dummy%i") % side).str().c_str(),"",100,-1.5,1.5,100,-1.5,1.5);
    dummyHist[side]->GetXaxis()->SetLabelSize(0.0);
    dummyHist[side]->GetYaxis()->SetLabelSize(0.0);
    for(int sec=0; sec<12; sec++)
    {
      h_timeSamples[side][sec][0] = dynamic_cast<TH1 *>(cl->getHisto(histprefix + std::string("sample_R1_") + (side == 1 ? "North_" : "South_") + (boost::format("%i") % sec).str().c_str()));
      if(h_timeSamples[side][sec][0]->GetMaximum() > maxY[side][sec])
      {
	maxY[side][sec] = h_timeSamples[side][sec][0]->GetMaximum();
      }
      h_timeSamples[side][sec][1] = dynamic_cast<TH1 *>(cl->getHisto(histprefix + std::string("sample_R2_") + (side == 1 ? "North_" : "South_") + (boost::format("%i") % sec).str().c_str()));
      if(h_timeSamples[side][sec][1]->GetMaximum() > maxY[side][sec])
      {
	maxY[side][sec] = h_timeSamples[side][sec][1]->GetMaximum();
      }
      h_timeSamples[side][sec][2] = dynamic_cast<TH1 *>(cl->getHisto(histprefix + std::string("sample_R3_") + (side == 1 ? "North_" : "South_") + (boost::format("%i") % sec).str().c_str()));
      if(h_timeSamples[side][sec][2]->GetMaximum() > maxY[side][sec])
      {
	maxY[side][sec] = h_timeSamples[side][sec][2]->GetMaximum();
      }

      for(int mod=0; mod<3; mod++)
      {
	double binCenter = h_timeSamples[side][sec][mod]->GetBinCenter(h_timeSamples[side][sec][mod]->GetMaximumBin());
	if(binCenter >= minTimeSample && binCenter <= maxTimeSample)
	{
	  nGoodTimeSamples++;
	}
      }
      
      int sector = sec;
      if(side == 0) sector += 12;
      labels[side][sec] = new TPaveLabel(labelNumbers[sec][0], labelNumbers[sec][1], labelNumbers[sec][2], labelNumbers[sec][3],(boost::format("%02i") % sector).str().c_str());
      labels[side][sec]->SetFillColor(0);
    }
  }
  
  MakeCanvas("laser_adc",0);
  TC[0]->Clear("D");
  
  for(int side = 0; side < 2; side++)
  {
    Pad[0][side]->cd();
    Pad[0][side]->SetLogz();
    if(h_TPCWheel[side])
    {
      dummyHist[side]->Draw();
      h_TPCWheel[side]->GetZaxis()->SetRangeUser(1,1.5*maxZ);
      h_TPCWheel[side]->Draw("colpolzsame0");
      for(int sec=0; sec<12; sec++)
      {
	labels[side][sec]->Draw("same");
      }
      gPad->SetRightMargin(0.15);
      gPad->SetLeftMargin(0.05);
      gPad->SetBottomMargin(0.05);

      TPaveText *title = new TPaveText();
      title->SetTextSize(0.06);
      title->SetX1NDC(0.3);
      title->SetX2NDC(0.7);
      title->SetY1NDC(0.95);
      title->SetY2NDC(1.0);
      title->SetFillStyle(0);
      title->SetBorderSize(0);
      title->SetTextAlign(22);
      title->AddText((boost::format("TPC %s") % (side == 1 ? "North" : "South")).str().c_str());
      title->Draw("same");
		     
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
  runnostream1 << Name() << "_laserADC_perEvent Run " << cl->RunNumber() << ", build " << cl->build();
  runstring1 = runnostream1.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
  
  TC[0]->Update(); 


  std::cout << "made ADC" << std::endl;
  
  MakeCanvas("laser_saturation",1);
  TC[1]->Clear("D");
  
  for(int side = 0; side < 2; side++)
  {
    Pad[1][side]->cd();
    Pad[1][side]->SetLogz();
    if(h_saturation[side])
    {
      dummyHist[side]->Draw();
      h_saturation[side]->GetZaxis()->SetRangeUser(1,1.5*maxZ_sat);
      h_saturation[side]->Draw("colpolzsame0");
      for(int sec=0; sec<12; sec++)
      {
	labels[side][sec]->Draw("same");
      }
      gPad->SetRightMargin(0.15);
      gPad->SetLeftMargin(0.05);
      gPad->SetBottomMargin(0.05);

      TPaveText *title = new TPaveText();
      title->SetTextSize(0.06);
      title->SetX1NDC(0.3);
      title->SetX2NDC(0.7);
      title->SetY1NDC(0.95);
      title->SetY2NDC(1.0);
      title->SetFillStyle(0);
      title->SetBorderSize(0);
      title->SetTextAlign(22);
      title->AddText((boost::format("TPC %s") % (side == 1 ? "North" : "South")).str().c_str());
      title->Draw("same");
    		     
    }
    else
    {
      //histogram is missing
      return -1;
    }
  }
			       
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  std::ostringstream runnostream2;
  std::string runstring2;
  runnostream2 << Name() << "_laserSaturation_perEvent Run " << cl->RunNumber() << ", build " << cl->build();
  runstring2 = runnostream2.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring2.c_str());
  
  TC[1]->Update(); 

  std::cout << "made saturation" << std::endl;

  MakeCanvas("laser_num_clusters",2);
  TC[2]->Clear("D");
  
  THStack *hs = new THStack("hs", "Stacked Histograms");
  TLegend *leg = new TLegend(0.45, 0.7, 0.7, 0.9);

  h_nLaserClusters[0]->SetLineColor(kBlue);
  h_nLaserClusters[1]->SetLineColor(kRed);
  
  for(int side = 0; side < 2; side++)
  {
    if(h_nLaserClusters[side])
    {
      leg->AddEntry(h_nLaserClusters[side], (boost::format("TPC %s") % (side == 1 ? "North" : "South")).str().c_str(), "L");
      //h_saturation[side]->GetYaxis()->SetRangeUser(1,1.5*maxY);
      //hs->Add(h_saturation[side]);
      /*
      if(side == 0)
      {
	h_saturation[side]->Draw("P");
      }
      else
      {
	h_saturation[side]->Draw("PSAME");
	}
	
	gPad->SetRightMargin(0.15);
      gPad->SetLeftMargin(0.05);
      gPad->SetBottomMargin(0.05);
      */
      
      
    }
    else
    {
      //histogram is missing
      return -1;
    }
  }

  hs->Add(h_nLaserClusters[0]);
  hs->Add(h_nLaserClusters[1]);
			       
  hs->Draw("nostack HIST");
  hs->GetXaxis()->SetTitle("Number of Laser Clusters");

  leg->SetFillStyle(0);
  leg->Draw();

  gPad->SetLogy();
  
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates
  PrintRun.SetTextAlign(23); // center/top alignment
  std::ostringstream runnostream3;
  std::string runstring3;
  runnostream3 << Name() << "_laserNumClusters Run " << cl->RunNumber() << ", build " << cl->build();
  runstring3 = runnostream3.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring3.c_str());
  
  TC[2]->Update(); 


  MakeCanvas("laser_time_sample_South",3);
  TC[3]->Clear("D");

  TLine *lLow[2][12];
  TLine *lHigh[2][12];
  
  for(int sec = 0; sec<12; sec++)
  {
    
    Pad[3][sec]->cd();
    Pad[3][sec]->SetLogy();
    //TC[3]->cd(sec+1);
    gPad->SetLeftMargin(0.1);
    gPad->SetBottomMargin(0.05);
    h_timeSamples[0][sec][0]->SetLineColor(kRed);
    h_timeSamples[0][sec][1]->SetLineColor(kBlue);
    h_timeSamples[0][sec][2]->SetLineColor(kGreen+2);
    
    h_timeSamples[0][sec][0]->GetYaxis()->SetRangeUser(0.1,1.1*maxY[0][sec]);
    h_timeSamples[0][sec][0]->Draw("HIST");
    h_timeSamples[0][sec][1]->Draw("HIST SAME");
    h_timeSamples[0][sec][2]->Draw("HIST SAME");

    if(sec == 0)
    {
      TLegend *legS = new TLegend(0.75,0.6,1,1);
      legS->AddEntry(h_timeSamples[0][sec][0],"R1","l");
      legS->AddEntry(h_timeSamples[0][sec][1],"R2","l");
      legS->AddEntry(h_timeSamples[0][sec][2],"R3","l");
      legS->Draw("SAME");
    }

    lLow[0][sec] = new TLine(minTimeSample,0,minTimeSample,1.1*maxY[0][sec]);
    lLow[0][sec]->SetLineColor(kBlack);
    lLow[0][sec]->Draw("Same");
    
    lHigh[0][sec] = new TLine(maxTimeSample,0,maxTimeSample,1.1*maxY[0][sec]);
    lHigh[0][sec]->SetLineColor(kBlack);
    lHigh[0][sec]->Draw("Same");
    
    
     TPaveText *title = new TPaveText();
     title->SetTextSize(0.06);
     title->SetX1NDC(0.3);
     title->SetX2NDC(0.7);
     title->SetY1NDC(0.95);
     title->SetY2NDC(1.0);
     title->SetFillStyle(0);
     title->SetBorderSize(0);
     title->SetTextAlign(22);
     title->AddText((boost::format("TPC Sector %i") % sec).str().c_str());
     title->Draw("same");
  }
  

  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates                                                                                                                                                   
  PrintRun.SetTextAlign(23); // center/top alignment                                                                                                                                                     
  std::ostringstream runnostream4;
  std::string runstring4;
  runnostream4 << Name() << "_laserSouthTimeSamples Run " << cl->RunNumber() << ", build " << cl->build();
  runstring4 = runnostream4.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring4.c_str());

  TC[3]->Update();

  MakeCanvas("laser_time_sample_North",4);
  TC[4]->Clear("D");

  for(int sec = 0; sec<12; sec++)
  {
    Pad[4][sec]->cd();
    Pad[4][sec]->SetLogy();
    //TC[4]->cd(sec+1);
    gPad->SetLeftMargin(0.1);
    gPad->SetBottomMargin(0.05);
    h_timeSamples[1][sec][0]->SetLineColor(kRed);
    h_timeSamples[1][sec][1]->SetLineColor(kBlue);
    h_timeSamples[1][sec][2]->SetLineColor(kGreen+2);

    h_timeSamples[1][sec][0]->GetYaxis()->SetRangeUser(0.1,1.1*maxY[1][sec]);
    h_timeSamples[1][sec][0]->Draw("HIST");
    h_timeSamples[1][sec][1]->Draw("HIST SAME");
    h_timeSamples[1][sec][2]->Draw("HIST SAME");

    if(sec == 0)
    {
      TLegend *legS = new TLegend(0.75,0.6,1,1);
      legS->AddEntry(h_timeSamples[1][sec][0],"R1","l");
      legS->AddEntry(h_timeSamples[1][sec][1],"R2","l");
      legS->AddEntry(h_timeSamples[1][sec][2],"R3","l");
      legS->Draw("SAME");
    }

    lLow[1][sec] = new TLine(minTimeSample,0,minTimeSample,1.1*maxY[1][sec]);
    lLow[1][sec]->SetLineColor(kBlack);
    lLow[1][sec]->Draw("Same");
    
    lHigh[1][sec] = new TLine(maxTimeSample,0,maxTimeSample,1.1*maxY[1][sec]);
    lHigh[1][sec]->SetLineColor(kBlack);
    lHigh[1][sec]->Draw("Same");
    
    
     TPaveText *title = new TPaveText();
     title->SetTextSize(0.06);
     title->SetX1NDC(0.3);
     title->SetX2NDC(0.7);
     title->SetY1NDC(0.95);
     title->SetY2NDC(1.0);
     title->SetFillStyle(0);
     title->SetBorderSize(0);
     title->SetTextAlign(22);
     title->AddText((boost::format("TPC Sector %i") % sec).str().c_str());
     title->Draw("same");
  }
  

  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates                                                                                                                                                   
  PrintRun.SetTextAlign(23); // center/top alignment                                                                                                                                                     
  std::ostringstream runnostream5;
  std::string runstring5;
  runnostream5 << Name() << "_laserNorthTimeSamples Run " << cl->RunNumber() << ", build " << cl->build();
  runstring5 = runnostream5.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring5.c_str());

  TC[4]->Update();


  MakeCanvas("laserSummary",5);
  TC[5]->Clear("D");

  TPaveText *laserSummary = new TPaveText();
  laserSummary->SetX1NDC(0.0);
  laserSummary->SetX2NDC(1.0);
  laserSummary->SetY1NDC(0.0);
  laserSummary->SetY2NDC(1.0);
  TText *t0 = laserSummary->AddText((boost::format("TPC Laser Summary: TPC Laser QA is %s") % (nGoodModules == 72 && nGoodTimeSamples == 72 ? "Good" : "Bad")).str().c_str());
  t0->SetTextColor(kGreen+1);
  if(nGoodModules < 72 || nGoodTimeSamples < 72)
  {
    t0->SetTextColor(kRed);
  }
  TText *t1 = laserSummary->AddText((boost::format("TPC Lasers have good ADC in %i/72 modules") % nGoodModules).str().c_str());
  t1->SetTextColor(kGreen+1);
  if(nGoodModules < 72)
  {
    t1->SetTextColor(kRed);
  }
  TText *t2 = laserSummary->AddText((boost::format("TPC Lasers have good timing in %i/72 modules") % nGoodTimeSamples).str().c_str());
  t2->SetTextColor(kGreen+1);
  if(nGoodTimeSamples<72)
  {
    t2->SetTextColor(kRed);
  }
  laserSummary->Draw();

  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates                                                                                                                                            
 
  PrintRun.SetTextAlign(23); // center/top alignment                                                                                                                                            

  std::ostringstream runnostream6;
  std::string runstring6;
  runnostream6 << Name() << "_laserSummary Run " << cl->RunNumber() << ", build " << cl->build();
  runstring6 = runnostream6.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring6.c_str());

  TC[5]->Update();
  
  std::cout << "DrawLaserInfo Ending" << std::endl;
  return 0;
}
 

int TPCLasersDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  
  if (iret) // on error no html output please
  {
      return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.
  if (what == "ALL")
  {
    pngfile = cl->htmlRegisterPage(*this, "laser_summary", "1", "png");
    cl->CanvasToPng(TC[5], pngfile);

    pngfile = cl->htmlRegisterPage(*this, "laser_adc", "2", "png");
    cl->CanvasToPng(TC[0], pngfile);
    
    pngfile = cl->htmlRegisterPage(*this, "laser_saturation", "3", "png");
    cl->CanvasToPng(TC[1], pngfile);

    pngfile = cl->htmlRegisterPage(*this, "laser_num_clusters", "4", "png");
    cl->CanvasToPng(TC[2], pngfile);

    pngfile = cl->htmlRegisterPage(*this, "laser_time_sample_South", "5", "png");
    cl->CanvasToPng(TC[3], pngfile);

    pngfile = cl->htmlRegisterPage(*this, "laser_time_sample_North", "6", "png");
    cl->CanvasToPng(TC[4], pngfile);
}
  return 0;
}

int TPCLasersDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
