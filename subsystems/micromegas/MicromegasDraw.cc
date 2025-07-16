#include "MicromegasDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TLine.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>


#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
 //! make canvas editable in creator, and non-editable in destructor
  class CanvasEditor
  {
    public:
    CanvasEditor( TCanvas* cv ):m_cv(cv)
    { if( m_cv ) m_cv->SetEditable(true); }

    ~CanvasEditor()
    // {}
    { if( m_cv ) m_cv->SetEditable(false); }

    private:
    TCanvas* m_cv = nullptr;
  };

  TPad* create_transparent_pad( const std::string& name )
  {
    auto transparent = new TPad( (name+"_transparent").c_str(), "", 0, 0, 1, 1);
    transparent->SetFillStyle(4000);
    transparent->Draw();
    return transparent;
  };

  // divide canvas, adjusting canvas positions to leave room for a banner at the top
  void divide_canvas( TCanvas* cv, int ncol, int nrow )
  {
    static constexpr double max_height = 0.94;

    cv->Divide( ncol, nrow );
    for( int i = 0; i < ncol*nrow; ++i )
    {
      auto pad = cv->GetPad( i+1 );
      int col = i%ncol;
      int row = i/ncol;
      const double xmin = double(col)/ncol;
      const double xmax = double(col+1)/ncol;

      const double ymin = max_height*(1. - double(row+1)/nrow);
      const double ymax = max_height*(1. - double(row)/nrow);
      pad->SetPad( xmin, ymin, xmax, ymax );
    }
  }


  // draw an horizontal line that extends automatically from both sides of a canvas
  [[maybe_unused]] TLine* horizontal_line( TVirtualPad* pad, Double_t y )
  {
    Double_t xMin = pad->GetUxmin();
    Double_t xMax = pad->GetUxmax();

    if( pad->GetLogx() )
    {
      xMin = std::pow( 10, xMin );
      xMax = std::pow( 10, xMax );
    }

    return new TLine( xMin, y, xMax, y );
  }

  //____________________________________________________________________________________________________
  void draw_range( TVirtualPad* pad, const MicromegasDraw::range_t& range )
  {
    for( const auto& y:{range.first,range.second} )
    {
      auto line = horizontal_line( pad, y );
      line->SetLineColor(2);
      line->SetLineStyle(2);
      line->SetLineWidth(2);
      line->Draw();
    }
  }

}

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



//__________________________________________________________________________________
TCanvas* MicromegasDraw::get_canvas(const std::string& name, bool clear )
{
  auto cv = dynamic_cast<TCanvas*>( gROOT->FindObject( name.c_str() ) );
  if( !cv ) cv = create_canvas( name );
  if( cv && clear ) cv->Clear("D");
  return cv;
}

//__________________________________________________________________________________
TCanvas* MicromegasDraw::create_canvas(const std::string &name)
{

  if( Verbosity() )
  { std::cout << "MicromegasDraw::create_canvas - name: " << name << std::endl; }

  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();

  if (name == "TPOT_CLUSTERS_MEAN")
  {

    auto cv = new TCanvas(name.c_str(), "TPOT cluster mean distributions", -1, 0,xsize/1.2, ysize/1.2);
    gSystem->ProcessEvents();
    divide_canvas(cv, 2, 2);
    create_transparent_pad(name);
    cv->SetEditable(false);
    m_canvas.push_back( cv );
    return cv;

  } else if (name == "TPOT_CLUSTERS_RAW") {

    auto cv = new TCanvas(name.c_str(), "TPOT cluster raw distributions [expert]", -1, 0,xsize/1.2, ysize/1.2);
    gSystem->ProcessEvents();
    divide_canvas(cv, 2, 2);
    create_transparent_pad(name);
    cv->SetEditable(false);
    m_canvas.push_back( cv );
    return cv;

  }

  return nullptr;
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

  auto cv = get_canvas("TPOT_CLUSTERS_MEAN");
  if( !cv )
  {
    if( Verbosity() ) std::cout << "MicromegasDraw::DrawClusterInfo - no canvas" << std::endl;
    return -1;
  }

  CanvasEditor cv_edit(cv);
  cv->cd(1);
  h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
  h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
  h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
  h_cluster_multiplicity->SetStats(0);
  h_cluster_multiplicity->SetMinimum(0);
  h_cluster_multiplicity->SetMaximum(10);
  h_cluster_multiplicity->DrawCopy("P");
  gPad->Update();
  draw_range( gPad, m_cluster_multiplicity_range );

  cv->cd(2);
  h_cluster_size->SetTitle("Cluster Size");
  h_cluster_size->GetXaxis()->SetTitle("Chamber");
  h_cluster_size->GetYaxis()->SetTitle("Size");
  h_cluster_size->SetStats(0);
  h_cluster_size->SetMinimum(0);
  h_cluster_size->SetMaximum(8);
  h_cluster_size->DrawCopy("P");
  gPad->Update();
  draw_range( gPad, m_cluster_size_range );

  cv->cd(3);
  h_cluster_charge->SetTitle("Cluster Charge");
  h_cluster_charge->GetXaxis()->SetTitle("Chamber");
  h_cluster_charge->GetYaxis()->SetTitle("Charge");
  h_cluster_charge->SetStats(0);
  h_cluster_charge->SetMinimum(0);
  h_cluster_charge->SetMaximum(1000);
  h_cluster_charge->DrawCopy("P");
  gPad->Update();
  draw_range( gPad, m_cluster_charge_range );

  cv->cd(4);
  efficiency->SetMinimum(0);
  efficiency->SetMaximum(1);
  efficiency->SetTitle("Efficiency Estimate by Chamber");
  efficiency->GetXaxis()->SetTitle("Chamber");
  efficiency->GetYaxis()->SetTitle("Efficiency");
  efficiency->SetStats(0);
  efficiency->DrawCopy( "P" );
  gPad->Update();
  draw_range( gPad, m_efficiency_range );

  cv->Update();
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

  auto cv = get_canvas("TPOT_CLUSTERS_RAW");
  if( !cv )
  {
    if( Verbosity() ) std::cout << "MicromegasDraw::DrawRawInfo - no canvas" << std::endl;
    return -1;
  }
  CanvasEditor cv_edit(cv);

  auto draw_profile = []( TH2* h )
  {
    auto p = h->ProfileX( Form( "%s_p", h->GetName() ) );
    p->SetMarkerStyle(20);
    p->Draw("same");
  };


  cv->cd(1);
  h_cluster_charge->SetTitle("Cluster Charge");
  h_cluster_charge->GetXaxis()->SetTitle("Chamber");
  h_cluster_charge->GetYaxis()->SetTitle("Charge");
  h_cluster_charge->DrawCopy("COLZ");
  draw_profile(h_cluster_charge);

  cv->cd(2);
  h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
  h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
  h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
  h_cluster_multiplicity->DrawCopy("COLZ");
  draw_profile(h_cluster_multiplicity);

  cv->cd(3);
  h_cluster_size->SetTitle("Cluster Size");
  h_cluster_size->GetXaxis()->SetTitle("Chamber");
  h_cluster_size->GetYaxis()->SetTitle("Size");
  h_cluster_size->DrawCopy("COLZ");
  draw_profile(h_cluster_size);

  cv->Update();
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
    pngfile = cl->htmlRegisterPage(*this, "cluster_info", "1", "png");
    auto cv = get_canvas("TPOT_CLUSTERS_MEAN" );
    cl->CanvasToPng(cv, pngfile);
  }

  // raw cluster information (experts)
  if (what == "ALL" || what == "RAW")
  {
    pngfile = cl->htmlRegisterPage(*this, "raw_cluster_info", "2", "png");
    auto cv = get_canvas("TPOT_CLUSTERS_RAW" );
    cl->CanvasToPng(cv, pngfile);
  }

  // summary page
  if (what == "ALL" || what == "SUMMARY")
  {
    pngfile = cl->htmlRegisterPage(*this, "tpot summary", "3", "png");
    auto cv = get_canvas("TPOT_CLUSTERS_RAW" );
    cl->CanvasToPng(cv, pngfile);
  }

  return 0;
}

//________________________________________________________________
int MicromegasDraw::DBVarInit()
{
  return 0;
}
