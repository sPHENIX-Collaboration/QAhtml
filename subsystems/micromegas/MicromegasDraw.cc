#include "MicromegasDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TLine.h>
#include <TPad.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLegend.h>

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
    iret += draw_bco_info();
    idraw++;
  }

  if (what == "ALL" || what == "CLUSTERS_RAW")
  {
    iret += draw_raw_cluster_info();
    idraw++;
  }

  if (what == "ALL" || what == "CLUSTERS_AVG")
  {
    iret += draw_average_cluster_info();
    idraw++;
  }

  if (what == "ALL" || what == "SUMMARY")
  {
    iret += draw_summary();
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
TH1* MicromegasDraw::get_detector_average(TH2* source, double offset)
{
  auto h = new TH1F( Form("%s_average", source->GetName()), "", source->GetNbinsX(), source->GetXaxis()->GetXmin(), source->GetXaxis()->GetXmax());

  //
  for (int ix = 1; ix <= source->GetNbinsX(); ++ix)
  {
    std::unique_ptr<TH1> p( source->ProjectionY("proj", ix, ix ));
    h->SetBinContent(ix, p->GetMean()+offset );
    h->SetBinError(ix, p->GetMeanError() );
    h->GetXaxis()->SetBinLabel(ix, source->GetXaxis()->GetBinLabel(ix) );
  }

  h->SetMarkerStyle(20);

  return h;
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

  if (name == "TPOT_BCO") {

    auto cv = new TCanvas(name.c_str(), "TPOT BCO information", -1, 0,xsize/1.2, ysize/1.2);
    gSystem->ProcessEvents();
    divide_canvas(cv, 2, 1);
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
  } else if (name == "TPOT_CLUSTERS_AVG") {

    auto cv = new TCanvas(name.c_str(), "TPOT cluster mean distributions", -1, 0,xsize/1.2, ysize/1.2);
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
int MicromegasDraw::draw_bco_info()
{
  QADrawClient *cl = QADrawClient::instance();

  // load histograms
  auto h_waveform_bco_dropped = static_cast<TH1*>(cl->getHisto("h_MicromegasBCOQA_waveform_count_dropped_bco"));
  auto h_waveform_pool_dropped = static_cast<TH1*>(cl->getHisto("h_MicromegasBCOQA_waveform_count_dropped_pool"));
  auto h_waveform_total = static_cast<TH1*>(cl->getHisto("h_MicromegasBCOQA_waveform_count_total"));
  auto h_gl1_raw = static_cast<TH1*>(cl->getHisto("h_MicromegasBCOQA_packet_stat"));

  if (h_waveform_bco_dropped && h_waveform_pool_dropped && h_waveform_total )
  {
    auto h_drop= new TH1F("h_drop", "Drop Rate", 3, 0, 3);
    h_drop->SetStats(0);
    h_drop->GetXaxis()->SetBinLabel(1, "5001");
    h_drop->GetXaxis()->SetBinLabel(2, "5002");
    h_drop->GetXaxis()->SetBinLabel(3, "all");
    h_drop->GetXaxis()->SetTitle("Packet");
    h_drop->GetYaxis()->SetTitle("Waveform Drop Rate");
    h_drop->SetTitle("Fraction of Dropped Waveforms by packet");

    h_drop->SetBinContent(1, double(h_waveform_bco_dropped->GetBinContent(1)+ h_waveform_pool_dropped->GetBinContent(1))/h_waveform_total->GetBinContent(1));
    h_drop->SetBinContent(2, double(h_waveform_bco_dropped->GetBinContent(2)+ h_waveform_pool_dropped->GetBinContent(2))/h_waveform_total->GetBinContent(2));
    h_drop->SetBinContent(3, double(h_waveform_bco_dropped->GetBinContent(1)+ h_waveform_pool_dropped->GetBinContent(1)+h_waveform_bco_dropped->GetBinContent(2)+ h_waveform_pool_dropped->GetBinContent(2))/(h_waveform_total->GetBinContent(1)+h_waveform_total->GetBinContent(2)) );

    auto cv = get_canvas("TPOT_BCO");
    CanvasEditor cv_edit(cv);
    cv->cd(1);
    h_drop->SetMinimum(0);
    h_drop->SetMaximum(1.1);
    h_drop->SetFillStyle(1001);
    h_drop->SetFillColor(kYellow);
    h_drop->Draw();

    auto legend_drop = new TLegend(0.56, 0.6, 0.85, 0.84);
    legend_drop->SetHeader("Values", "C");
    legend_drop->SetTextSize(0.045);
    legend_drop->SetBorderSize(0);
    legend_drop->SetFillStyle(0);

    for (int i = 1; i <= h_drop->GetNbinsX(); ++i)
    {
      legend_drop->AddEntry((TObject*)0, Form("%s: %.4f", h_drop->GetXaxis()->GetBinLabel(i), h_drop->GetBinContent(i)), "");
    }
    legend_drop->Draw();
  }

  if (h_gl1_raw)
  {
    auto h_gl1= new TH1F("h_gl1", "Match Rate", 3, 0, 3);
    h_gl1->SetStats(0);
    h_gl1->GetXaxis()->SetTitle("Packet");
    h_gl1->GetYaxis()->SetTitle("GL1 Match Rate");
    h_gl1->SetTitle("Matching Tagger Rate by packet");

    h_gl1->GetXaxis()->SetBinLabel(1, "5001");
    h_gl1->GetXaxis()->SetBinLabel(2, "5002");
    h_gl1->GetXaxis()->SetBinLabel(3, "all");

    h_gl1->SetBinContent(3,double(h_gl1_raw->GetBinContent(4))/h_gl1_raw->GetBinContent(1));
    h_gl1->SetBinContent(2,double(h_gl1_raw->GetBinContent(3))/h_gl1_raw->GetBinContent(1));
    h_gl1->SetBinContent(1,double(h_gl1_raw->GetBinContent(2))/h_gl1_raw->GetBinContent(1));

    auto cv = get_canvas("TPOT_BCO");
    CanvasEditor cv_edit(cv);
    cv->cd(2);
    h_gl1->SetMinimum(0);
    h_gl1->SetMaximum(1.1);
    h_gl1->SetFillStyle(1001);
    h_gl1->SetFillColor(kYellow);
    h_gl1->Draw();

    auto legend_gl1 = new TLegend(0.65, 0.6, 0.85, 0.84);
    legend_gl1->SetHeader("Values", "C");
    legend_gl1->SetTextSize(0.045);
    legend_gl1->SetBorderSize(0);
    legend_gl1->SetFillStyle(0);

    for (int i = 1; i <= h_gl1->GetNbinsX(); ++i)
    {
      legend_gl1->AddEntry((TObject*)0, Form("%s: %.4f", h_gl1->GetXaxis()->GetBinLabel(i), h_gl1->GetBinContent(i)), "");
    }
    legend_gl1->Draw();

  }

  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::draw_raw_cluster_info()
{
  QADrawClient *cl = QADrawClient::instance();
  auto h_cluster_multiplicity = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_multiplicity"));
  auto h_cluster_size = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_size"));
  auto h_cluster_charge = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_charge"));

  auto draw_profile = []( TH2* h, double offset = 0 )
  {
    std::unique_ptr<TProfile> p(h->ProfileX());
    auto hp = new TH1F(Form( "%s_p", h->GetName()), "", h->GetNbinsX(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax() );
    for( int i = 0; i < p->GetNbinsX(); ++i )
    {
      hp->SetBinContent(i+1, p->GetBinContent(i+1)+offset );
      hp->SetBinError(i+1, p->GetBinError(i+1) );
    }
    hp->SetMarkerStyle(20);
    hp->Draw("p same");
  };

  if( h_cluster_multiplicity )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_RAW");
    CanvasEditor cv_edit(cv);
    cv->cd(1);
    h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
    h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
    h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
    h_cluster_multiplicity->DrawCopy("COLZ");
    /* correct profile by 0.5 due to incorrect binning, with integer values at bin edges rather than bin center */
    draw_profile(h_cluster_multiplicity, -0.5);
    gPad->Update();
  }

  if( h_cluster_size )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_RAW");
    CanvasEditor cv_edit(cv);
    cv->cd(2);
    h_cluster_size->SetTitle("Cluster Size");
    h_cluster_size->GetXaxis()->SetTitle("Chamber");
    h_cluster_size->GetYaxis()->SetTitle("Size");
    h_cluster_size->DrawCopy("COLZ");
    /* correct profile by 0.5 due to incorrect binning, with integer values at bin edges rather than bin center */
    draw_profile(h_cluster_size, -0.5);
    gPad->Update();
  }

  if( h_cluster_charge )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_RAW");
    CanvasEditor cv_edit(cv);
    cv->cd(3);
    h_cluster_charge->SetTitle("Cluster Charge");
    h_cluster_charge->GetXaxis()->SetTitle("Chamber");
    h_cluster_charge->GetYaxis()->SetTitle("Charge");
    h_cluster_charge->DrawCopy("COLZ");
    draw_profile(h_cluster_charge);
    gPad->Update();
  }

  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::draw_average_cluster_info()
{
  QADrawClient *cl = QADrawClient::instance();

  auto h_cluster_count_ref = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_ref"));
  auto h_cluster_count_found = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_found"));
  auto h_cluster_multiplicity_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_multiplicity"));
  auto h_cluster_size_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_size"));
  auto h_cluster_charge_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_charge"));

  if( h_cluster_multiplicity_raw )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_AVG");
    CanvasEditor cv_edit(cv);
    cv->cd(1);

    // per chamber averaged distributions
    /* correct profile by 0.5 due to incorrect binning,
     * with integer values at bin edges rather than bin center
     */
    auto h_cluster_multiplicity = get_detector_average(h_cluster_multiplicity_raw, -0.5);
    h_cluster_multiplicity->SetTitle("Cluster Multiplicity");
    h_cluster_multiplicity->GetXaxis()->SetTitle("Chamber");
    h_cluster_multiplicity->GetYaxis()->SetTitle("Multiplicity");
    h_cluster_multiplicity->SetStats(0);
    h_cluster_multiplicity->SetMinimum(0);
    h_cluster_multiplicity->SetMaximum(10);
    h_cluster_multiplicity->DrawCopy("P");
    gPad->Update();
    draw_range( gPad, m_cluster_multiplicity_range );
    gPad->Update();
  }

  if( h_cluster_size_raw )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_AVG");
    CanvasEditor cv_edit(cv);
    cv->cd(2);

    // per chamber averaged distributions
    /* correct profile by 0.5 due to incorrect binning,
     * with integer values at bin edges rather than bin center
     */
    auto h_cluster_size = get_detector_average(h_cluster_size_raw, -0.5);
    h_cluster_size->SetTitle("Cluster Size");
    h_cluster_size->GetXaxis()->SetTitle("Chamber");
    h_cluster_size->GetYaxis()->SetTitle("Size");
    h_cluster_size->SetStats(0);
    h_cluster_size->SetMinimum(0);
    h_cluster_size->SetMaximum(8);
    h_cluster_size->DrawCopy("P");
    gPad->Update();
    draw_range( gPad, m_cluster_size_range );
    gPad->Update();
  }

  if( h_cluster_charge_raw )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_AVG");
    CanvasEditor cv_edit(cv);
    cv->cd(3);

    // per chamber averaged distributions
    auto h_cluster_charge = get_detector_average(h_cluster_charge_raw);
    h_cluster_charge->SetTitle("Cluster Charge");
    h_cluster_charge->GetXaxis()->SetTitle("Chamber");
    h_cluster_charge->GetYaxis()->SetTitle("Charge");
    h_cluster_charge->SetStats(0);
    h_cluster_charge->SetMinimum(0);
    h_cluster_charge->SetMaximum(1000);
    h_cluster_charge->DrawCopy("P");
    gPad->Update();
    draw_range( gPad, m_cluster_charge_range );
    gPad->Update();
  }

  if( h_cluster_count_ref && h_cluster_count_found )
  {
    auto cv = get_canvas("TPOT_CLUSTERS_AVG");
    CanvasEditor cv_edit(cv);
    cv->cd(4);

    // efficiency histogram
    auto efficiency = static_cast<TH1*>(h_cluster_count_found->Clone("efficiency"));
    efficiency->Divide(h_cluster_count_found, h_cluster_count_ref, 1, 1, "B" );
    efficiency->SetMarkerStyle(20);
    efficiency->SetMinimum(0);
    efficiency->SetMaximum(1);
    efficiency->SetTitle("Efficiency Estimate by Chamber");
    efficiency->GetXaxis()->SetTitle("Chamber");
    efficiency->GetYaxis()->SetTitle("Efficiency");
    efficiency->SetStats(0);
    efficiency->DrawCopy( "P" );
    gPad->Update();
    draw_range( gPad, m_efficiency_range );
    gPad->Update();
  }
  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::draw_summary()
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

  // raw cluster information (experts)
  if (what == "ALL" || what == "CLUSTERS_RAW")
  {
    pngfile = cl->htmlRegisterPage(*this, "raw_cluster_info", "2", "png");
    auto cv = get_canvas("TPOT_CLUSTERS_RAW" );
    cl->CanvasToPng(cv, pngfile);
  }

  // average cluster information
  if (what == "ALL" || what == "CLUSTERS_AVG")
  {
    pngfile = cl->htmlRegisterPage(*this, "cluster_info", "1", "png");
    auto cv = get_canvas("TPOT_CLUSTERS_AVG" );
    cl->CanvasToPng(cv, pngfile);
  }

  // summary page
  if (what == "ALL" || what == "SUMMARY")
  {
    pngfile = cl->htmlRegisterPage(*this, "tpot summary", "3", "png");
    auto cv = get_canvas("SUMMARY" );
    cl->CanvasToPng(cv, pngfile);
  }

  return 0;
}

//________________________________________________________________
int MicromegasDraw::DBVarInit()
{
  return 0;
}
