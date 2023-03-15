#include "ExampleDraw.h"
#include <qahtml/OnlProdClient.h>
#include <qahtml/OnlProdDB.h>

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

#include <boost/foreach.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

ExampleDraw::ExampleDraw(const string &name): 
  OnlProdDraw(name),
  db(NULL)
{
  memset(TC,0,sizeof(TC));
  memset(transparent,0,sizeof(transparent));
  memset(Pad,0,sizeof(Pad));
  memset(gr,0,sizeof(gr));
  DBVarInit();
  return ;
}

ExampleDraw::~ExampleDraw()
{
  delete db;
  for (int i=0; i<1; i++)
    {
      delete gr[i];
    }
  return;
}

int ExampleDraw::Draw(const string &what)
{
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "FIRST")
    {
      iret += DrawFirst(what);
      idraw ++;
    }
  if (!idraw)
    {
      std::cout << " Unimplemented Drawing option: " << what << std::endl;
      iret = -1;
    }
  return iret;
}

int ExampleDraw::MakeCanvas(const string &name)
{
  OnlProdClient *cl = OnlProdClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  if (name ==  "Example1")
    {
      // xpos (-1) negative: do not draw menu bar
       TC[0] = new TCanvas(name.c_str(), "Example Prod Plots", -1, 0, (int)(xsize/1.2) , (int)(ysize/1.2));
      gSystem->ProcessEvents();

      Pad[0] = new TPad("mypad0", "px", 0.05, 0.52, 0.45, 0.97, 0);
      Pad[1] = new TPad("mypad1", "pxpy", 0.5, 0.52, 0.95, 0.97, 0);
      Pad[2] = new TPad("mypad2", "profile", 0.05, 0.02, 0.45, 0.47, 0);
      Pad[3] = new TPad("mypad3", "history", 0.5, 0.02, 0.95, 0.47, 0);
     
      Pad[0]->SetLogy();
      Pad[1]->SetLogz();
      
      Pad[0]->Draw();
      Pad[1]->Draw();
      Pad[2]->Draw();
      Pad[3]->Draw();

      // this one is used to plot the run number on the canvas
      transparent[0] = new TPad("transparent0", "this does not show", 0, 0, 1, 1);
      transparent[0]->SetFillStyle(4000);
      transparent[0]->Draw();
    }

  return 0;
}

int ExampleDraw::DrawFirst(const string &/*what*/)
{
  OnlProdClient *cl = OnlProdClient::instance();
  TH1 *px = dynamic_cast <TH1 *> (cl->getHisto("example_px"));
  TH1 *pxpy    = dynamic_cast <TH2 *> (cl->getHisto("example_pxpy"));
  TH1 *prof       = dynamic_cast <TProfile *> (cl->getHisto("example_hprof"));

  if (! gROOT->FindObject("Example1"))
    {
      MakeCanvas("Example1");
    }
  TC[0]->Clear("D");
  Pad[0]->cd();
  if (px)
    {
      px->DrawCopy();
      db->SetVar("meanpx",px->GetMean(),px->GetMeanError());
      db->SetVar("rms",px->GetRMS(),px->GetRMSError());
    }
  else // its not my histos, return with error code
    {
      return -1;
    }
  Pad[1]->cd();
  if (pxpy)
    {
      pxpy->DrawCopy("COLZ");
    }
  Pad[2]->cd();
  if (prof)
    {
      prof->DrawCopy();
    }

  db->DBcommit();

  /*
  // retrieve variables from previous runs
  vector<OnlProdDBVar> history;
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
  ostringstream runnostream;
  string runstring;
  runnostream << Name() << "_1 Run " << cl->RunNumber();
  runstring = runnostream.str();
  transparent[0]->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());
  TC[0]->Update();
  return 0;
}

int
ExampleDraw::DrawGraph(TPad *pad, const std::vector<OnlProdDBVar> &history, const time_t begin, const time_t end)
{
  pad->cd();
  unsigned int nhistory = history.size();
  float *x = new float[history.size()];
  float *y = new float[history.size()];
  float *ex = new float[history.size()];
  float *ey = new float[history.size()];
  // this TimeOffsetTicks is neccessary to get the time axis right
  TDatime T0(2012, 01, 01, 00, 00, 00);
  int TimeOffsetTicks = T0.Convert();
  float ymax = -100000;
  float ymin = 100000;
  for (unsigned int i = 0; i < nhistory; i++)
    {
      // put value in the middle of the run
      x[i] = history[i].Startvaltime() + (history[i].Endvaltime() - history[i].Startvaltime()) / 2. - TimeOffsetTicks;
      y[i] = history[i].Value();
      ex[i] = 0;
      ey[i] = history[i].Error();

      if (y[i] + ey[i] > ymax) ymax = y[i] + ey[i];
      if (y[i] - ey[i] < ymin) ymin = y[i] - ey[i];
    }
  TH2 *h1 = 0;
  // leave 10 minutes at beginning and end
  h1 = new TH2F("example_graph", "example graph", 2, begin - TimeOffsetTicks - 600, end - TimeOffsetTicks + 600, 2, ymin*0.8, ymax*1.2);
  h1->SetMinimum(ymin*0.8);
  h1->SetMaximum(ymax*1.2);
  h1->SetStats(kFALSE);
  h1->GetXaxis()->SetTimeDisplay(1);
  h1->GetYaxis()->SetLabelSize(0.03);
  h1->GetXaxis()->SetLabelSize(0.03);
  h1->GetXaxis()->SetTimeOffset(TimeOffsetTicks);
  h1->GetXaxis()->SetTimeFormat("%H:%M");
  h1->DrawCopy();
  delete h1;
  gr[0] = new TGraphErrors(nhistory, x, y, ex, ey);
  gr[0]->SetMarkerColor(4);
  gr[0]->SetMarkerStyle(21);
  gr[0]->Draw("LP");
  delete [] x;
  delete [] y;
  delete [] ex;
  delete [] ey;

  return 0;
}


int ExampleDraw::MakeHtml(const string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
    {
      return iret;
    }

  OnlProdClient *cl = OnlProdClient::instance();

  // Register the 1st canvas png file to the menu and produces the png file.
  string pngfile = cl->htmlRegisterPage(*this,"ExamplePlots","1","png");
  cl->CanvasToPng(TC[0],pngfile);

  return 0;
}

int
ExampleDraw::DBVarInit()
{
  db = new OnlProdDB(this);
  db->registerVar("meanpx");
  db->registerVar("rms");
  db->DBInit();
  return 0;
}
