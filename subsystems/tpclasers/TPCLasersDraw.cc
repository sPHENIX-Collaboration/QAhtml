#include "TPCLasersDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
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

int TPCLasersDraw::MakeCanvas(const std::string &name)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TC = new TCanvas(name.c_str(), "TPC Laser QA Plots", -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  TC->Divide(2,1);
  gSystem->ProcessEvents();

  
  Pad[0] = new TPad("mypad0","South",0.51,0.0,0.95,0.95,0);
  Pad[1] = new TPad("mypad1","North",0.0,0.0,0.49,0.95,0);

  Pad[0]->Draw();
  Pad[1]->Draw();
  
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
  TH2* dummyHist[2];
  TPaveLabel* labels[2][12];

  double labelNumbers[12][4] = {{1.046586,-0.1938999,1.407997,0.2144871},{0.962076,0.4382608,1.323487,0.8466479},{0.4801947,0.8802139,0.8416056,1.288601},{-0.1823921,1.011681,0.1790189,1.425662},{-0.8449788,0.8690253,-0.4835679,1.288601},{-1.30879,0.441058,-0.9473786,0.8550394},{-1.411009,-0.2050886,-1.049598,0.2144871},{-1.302585,-0.7757116,-0.9471979,-0.3561359},{-0.8449788,-1.309971,-0.4835679,-0.8848013},{-0.1823921,-1.426557,0.1790189,-1.006982},{0.4801947,-1.309076,0.8416056,-0.8839062},{0.9622567,-0.7785088,1.323668,-0.3533387}};

  double maxZ = 0.0;
  
  for (int side = 0; side < 2; side++)
  {
    h_TPCWheel[side] = dynamic_cast<TH2 *>(cl->getHisto(histprefix + std::string("TPCWheel_") + (side == 1 ? "North" : "South")));
    h_TPCWheel[side]->Scale(1.0/h_nLaserEvents->GetEntries());
    if(h_TPCWheel[side]->GetMaximum() > maxZ)
    {
      maxZ = h_TPCWheel[side]->GetMaximum();
    }
    dummyHist[side] = new TH2D((boost::format("dummy%i") % side).str().c_str(),"",100,-1.5,1.5,100,-1.5,1.5);
    dummyHist[side]->GetXaxis()->SetLabelSize(0.0);
    dummyHist[side]->GetYaxis()->SetLabelSize(0.0);
    for(int sec=0; sec<12; sec++)
    {
      int sector = sec;
      if(side == 0) sector += 12;
      labels[side][sec] = new TPaveLabel(labelNumbers[sec][0], labelNumbers[sec][1], labelNumbers[sec][2], labelNumbers[sec][3],(boost::format("%02i") % sector).str().c_str());
      labels[side][sec]->SetFillColor(0);
    }
  }
  
  MakeCanvas("TPCCanvas");
  TC->Clear("D");
  
  for(int side = 0; side < 2; side++)
  {
    Pad[side]->cd();
    Pad[side]->SetLogz();
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
  
  TC->Update(); 

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
    pngfile = cl->htmlRegisterPage(*this, "TPCCanvas", "1", "png");
    cl->CanvasToPng(TC, pngfile);
  }
  return 0;
}

int TPCLasersDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
