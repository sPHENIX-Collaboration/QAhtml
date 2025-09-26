#include "InttbcoDraw.h"

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

#include <TLatex.h>
#include <TColor.h>
#include <TLegend.h>


#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

InttbcoDraw::InttbcoDraw(std::string const& name)
  : SingleCanvasDrawer(name)
{
  //
}


InttbcoDraw::~InttbcoDraw()
{
  //
}


int InttbcoDraw::MakeCanvas(int width, int height)
{
  
  if(SingleCanvasDrawer::MakeCanvas(width, height))
    {
      return 0;
    }
  m_canvas->SetTitle("INTT BCO Plots 1");
  
  Pad[0] = new TPad("mypad0", "server0", 0.05, 0.52, 0.27, 0.97, 0);
  Pad[1] = new TPad("mypad1", "server1", 0.28, 0.52, 0.50, 0.97, 0);
  Pad[2] = new TPad("mypad2", "server2", 0.51, 0.52, 0.73, 0.97, 0);
  Pad[3] = new TPad("mypad3", "server3", 0.74, 0.52, 0.96, 0.97, 0);
  Pad[4] = new TPad("mypad4", "server4", 0.05, 0.02, 0.27, 0.47, 0);
  Pad[5] = new TPad("mypad5", "server5", 0.28, 0.02, 0.50, 0.47, 0);
  Pad[6] = new TPad("mypad6", "server6", 0.51, 0.02, 0.73, 0.47, 0);
  Pad[7] = new TPad("mypad7", "server7", 0.74, 0.02, 0.96, 0.47, 0);
  
  Pad[0]->Draw();
  Pad[1]->Draw();
  Pad[2]->Draw();
  Pad[3]->Draw();
  Pad[4]->Draw();
  Pad[5]->Draw();
  Pad[6]->Draw();
  Pad[7]->Draw();
  
  // this one is used to plot the run number on the canvas
  transparent = new TPad("transparent0", "this does not show", 0, 0, 1, 1);
  transparent->SetFillStyle(4000);
  transparent->Draw();
  
  return 0;
}

int InttbcoDraw::DrawCanvas(){
  std::cout << "INTT DrawClusterInfo() Beginning" << std::endl;
  QADrawClient *cl = QADrawClient::instance();
  TH1I* h_InttCalib_BCOOffSet[8];
  for (int i =0; i<8; i++){
    h_InttCalib_BCOOffSet[i]=dynamic_cast<TH1I*>(cl->getHisto(Form("h_InttCalib_BCOOffSet_INTT%d",i)));
  }
  MakeCanvas();
  m_canvas->Clear("D");
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);
  for (int i =0; i<8; i++){
    Pad[i]->cd();
    h_InttCalib_BCOOffSet[i]->SetMarkerStyle(20);
    h_InttCalib_BCOOffSet[i]->GetYaxis()->SetTitle("BCO Diff");
    h_InttCalib_BCOOffSet[i]->GetXaxis()->SetTitle("Felix Channel");
    h_InttCalib_BCOOffSet[i]->Draw("HIST,P");
    latex.DrawLatex(0.6, 0.7, Form("INTT%d",i));
  }

  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();  // set to normalized coordinates                                                                      
  PrintRun.SetTextAlign(23); // center/top alignment                                                                        
  std::ostringstream runnostream1;
  std::string runstring1;
  // runnostream1 << Name() << "_intt Info Run " << cl->RunNumber() << ", build " << cl->build();                           
  runnostream1 << "INTTQA_intt Info Run " << cl->RunNumber() << ", build " << cl->build();
  runstring1 = runnostream1.str();
  transparent->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());

  m_canvas->Update();

  return 0;
}


