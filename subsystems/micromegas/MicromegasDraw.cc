#include "MicromegasDraw.h"

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


#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

//____________________________________________________________________________________________________
MicromegasDraw::MicromegasDraw(const std::string &name)
  : QADraw(name)
{
  DBVarInit();
  return;
}

//____________________________________________________________________________________________________
int MicromegasDraw::Draw(const std::string &what)
{
  /* SetsPhenixStyle(); */
  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "BCO" )
    {
      iret += DrawBCOInfo();
      idraw++;
    }
  if (what == "ALL" || what == "CLUSTERS")
    {
      iret += DrawClusterInfo();
      idraw++;
    }
  if (what == "ALL" || what == "RAW")
    {
      iret += DrawRawInfo();
      idraw++;
    }
  if (what == "ALL" || what == "SUMMARY")
    {
      iret += DrawSummary();
      idraw++;
    }


  if (!idraw)
    {
      std::cout << " Unimplemented Drawing option: " << what << std::endl;
      iret = -1;
    }
  return iret;
}

//____________________________________________________________________________________________________
TH1* MicromegasDraw::ClusterAverage(TH2* hist, std::string type)
{
  const auto nX = hist->GetNbinsX();
  auto graph = new TH1F( Form("avg_%s", type.c_str()), "", nX, hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

  //
  for (int ix = 1; ix <= nX; ++ix)
  {
    std::unique_ptr<TH1> h( hist->ProjectionY("proj", ix, ix ));
    graph->SetBinContent(ix, h->GetMean() );
    graph->SetBinError(ix, h->GetMeanError() );
    graph->GetXaxis()->SetBinLabel(ix, hist->GetXaxis()->GetBinLabel(ix) );
  }

  graph->SetMarkerStyle(8);

  return graph;
}

//____________________________________________________________________________________________________
int MicromegasDraw::MakeCanvas(const std::string &name, int num)
{
  QADrawClient *cl = QADrawClient::instance();
  gStyle->SetOptTitle(1);
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  TC[num] = new TCanvas(name.c_str(), (boost::format("Micromegas Plots %d") % num).str().c_str(), -1, 0, (int) (xsize / 1.2) , (int) (ysize / 1.2));
  gSystem->ProcessEvents();

  Pad[num][0] = new TPad((boost::format("mypad%d0") % num).str().c_str(), "put", 0.05, 0.52, 0.45, 0.97, 0);
  Pad[num][1] = new TPad((boost::format("mypad%d1") % num).str().c_str(), "a", 0.5, 0.52, 0.95, 0.97, 0);
  Pad[num][2] = new TPad((boost::format("mypad%d2") % num).str().c_str(), "name", 0.05, 0.02, 0.45, 0.47, 0);
  Pad[num][3] = new TPad((boost::format("mypad%d3") % num).str().c_str(), "here", 0.5, 0.02, 0.95, 0.47, 0);

  for (int i=0; i<4; i++)
    {
      Pad[num][i]->SetTopMargin(0.15);
      Pad[num][i]->SetBottomMargin(0.15);
      Pad[num][i]->SetRightMargin(0.15);
      Pad[num][i]->SetLeftMargin(0.15);
    }

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

//____________________________________________________________________________________________________
int MicromegasDraw::DrawBCOInfo()
{ return 0; }

//____________________________________________________________________________________________________
int MicromegasDraw::DrawClusterInfo()
{
  QADrawClient *cl = QADrawClient::instance();

  auto h_cluster_count_ref = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_ref"));
  auto h_cluster_count_found = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_found"));
  auto h_cluster_multiplicity_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_multiplicity"));
  auto h_cluster_size_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_size"));
  auto h_cluster_charge_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_charge"));

  if (!h_cluster_count_ref || !h_cluster_count_found || !h_cluster_multiplicity_raw || !h_cluster_size_raw || !h_cluster_charge_raw)
  {
    std::cerr << "Error: One or more histograms could not be retrieved." << std::endl;
    return -1;
  }

  // efficiency histogram
  auto efficiency = static_cast<TH1*>(h_cluster_count_found->Clone("efficiency"));
  efficiency->Divide(h_cluster_count_found, h_cluster_count_ref, 1, 1, "B" );
  efficiency->SetMarkerStyle(20);

  // per chamber efficiency distributions
  auto h_cluster_multiplicity = ClusterAverage(h_cluster_multiplicity_raw, "mult");
  auto h_cluster_size = ClusterAverage(h_cluster_size_raw, "size");
  auto h_cluster_charge = ClusterAverage(h_cluster_charge_raw, "charge");

  if (!TC[0])
  {
    MakeCanvas("ClusterQA", 0);
  }

  TC[0]->cd();
  TC[0]->Clear("D");

  Pad[0][1]->cd();
  h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
  h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
  h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
  h_cluster_multiplicity->SetStats(0);
  h_cluster_multiplicity->SetMinimum(0);
  h_cluster_multiplicity->SetMaximum(10);
  h_cluster_multiplicity->DrawCopy("P");

  Pad[0][2]->cd();
  h_cluster_size->SetTitle("Cluster Size");
  h_cluster_size->GetXaxis()->SetTitle("Chamber");
  h_cluster_size->GetYaxis()->SetTitle("Size");
  h_cluster_size->SetStats(0);
  h_cluster_size->SetMinimum(0);
  h_cluster_size->SetMaximum(8);
  h_cluster_size->DrawCopy("P");

  Pad[0][0]->cd();
  h_cluster_charge->SetTitle("Cluster Charge");
  h_cluster_charge->GetXaxis()->SetTitle("Chamber");
  h_cluster_charge->GetYaxis()->SetTitle("Charge");
  h_cluster_charge->SetStats(0);
  h_cluster_charge->SetMinimum(0);
  h_cluster_charge->SetMaximum(1000);
  h_cluster_charge->DrawCopy("P");

  Pad[0][3]->cd();
  efficiency->SetMinimum(0);
  efficiency->SetMaximum(1);
  efficiency->SetTitle("Efficiency Estimate by Chamber");
  efficiency->GetXaxis()->SetTitle("Chamber");
  efficiency->GetYaxis()->SetTitle("Efficiency");
  efficiency->SetStats(0);
  efficiency->DrawCopy( "P" );

  TC[0]->Update();

  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::DrawRawInfo()
{
  QADrawClient *cl = QADrawClient::instance();
  auto h_cluster_multiplicity = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_multiplicity"));
  auto h_cluster_size = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_size"));
  auto h_cluster_charge = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_charge"));

  if (!h_cluster_multiplicity || !h_cluster_size || !h_cluster_charge)
    {
      std::cerr << "Error: One or more histograms could not be retrieved." << std::endl;
      return -1;
    }

  if (!TC[1])
    {
      MakeCanvas("RawQA", 1);
    }

  TC[1]->cd();
  TC[1]->Clear("D");

  auto draw_profile = []( TH2* h )
  {
    auto p = h->ProfileX( Form( "%s_p", h->GetName() ) );
    p->SetMarkerStyle(20);
    p->Draw("same");
  };


  Pad[1][0]->cd();
  h_cluster_charge->SetTitle("Cluster Charge");
  h_cluster_charge->GetXaxis()->SetTitle("Chamber");
  h_cluster_charge->GetYaxis()->SetTitle("Charge");
  h_cluster_charge->DrawCopy("COLZ");
  draw_profile(h_cluster_charge);

  Pad[1][1]->cd();
  h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
  h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
  h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
  h_cluster_multiplicity->DrawCopy("COLZ");
  draw_profile(h_cluster_multiplicity);

  Pad[1][2]->cd();
  h_cluster_size->SetTitle("Cluster Size");
  h_cluster_size->GetXaxis()->SetTitle("Chamber");
  h_cluster_size->GetYaxis()->SetTitle("Size");
  h_cluster_size->DrawCopy("COLZ");
  draw_profile(h_cluster_size);

  TC[1]->Update();
  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::DrawSummary()
{ return 0; }

//________________________________________________________________
int MicromegasDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  auto cl = QADrawClient::instance();
  std::string pngfile;

  // average cluster information
  if (what == "ALL" || what == "CLUSTERS")
  {
    if( TC[0] )
    {
      pngfile = cl->htmlRegisterPage(*this, "cluster_info", "1", "png");
      cl->CanvasToPng(TC[0], pngfile);
    }
  }

  // raw cluster information (experts)
  if (what == "ALL" || what == "RAW")
  {
    if( TC[1] )
    {
      pngfile = cl->htmlRegisterPage(*this, "raw_cluster_info", "2", "png");
      cl->CanvasToPng(TC[1], pngfile);
    }
  }

  // summary page
  if (what == "ALL" || what == "SUMMARY")
  {
    if( TC[2] )
    {
      pngfile = cl->htmlRegisterPage(*this, "raw_cluster_info", "2", "png");
      cl->CanvasToPng(TC[2], pngfile);
    }
  }

  return 0;
}

//________________________________________________________________
int MicromegasDraw::DBVarInit()
{
  return 0;
}
