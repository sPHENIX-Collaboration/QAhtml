#include "MicromegasDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TLine.h>
#include <TPad.h>
#include <TPaveText.h>
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

  //____________________________________________________________________________________________________
  void draw_range( TH1* h, const MicromegasDraw::range_list_t& range_list )
  {

    TLine line;
    line.SetLineColor(2);
    line.SetLineStyle(2);
    line.SetLineWidth(2);

    for( int i=0; i<16; ++i )
    {
      auto x_min = h->GetXaxis()->GetBinLowEdge(i+1);
      auto x_max = h->GetXaxis()->GetBinUpEdge(i+1);

      // lower edge
      line.DrawLine(x_min, range_list[i].first, x_max, range_list[i].first );
      if( i>0 && range_list[i-1].first != range_list[i].first) { line.DrawLine(x_min, range_list[i-1].first, x_min, range_list[i].first ); }

      // upper edge
      line.DrawLine(x_min, range_list[i].second, x_max, range_list[i].second );
      if( i>0 && range_list[i-1].second != range_list[i].second) { line.DrawLine(x_min, range_list[i-1].second, x_min, range_list[i].second ); }

    }
  }

  //____________________________________________________________________________________________________
  // from histogram, get number of detectors in acceptable range
  int get_num_valid_detectors( TH1* h, const MicromegasDraw::range_list_t& range_list )
  {
    auto nbins = h->GetNbinsX();
    assert( (int) range_list.size() == nbins );

    int out = 0;
    for( int ibin = 0; ibin < nbins; ++ibin )
    {
      auto content = h->GetBinContent( ibin+1 );
      if( content >= range_list[ibin].first && content<= range_list[ibin].second ) { ++out; }
    }

    return out;
  }

  //! status flag
  enum class Status
  {
    status_unknown,
    status_bad,
    status_questionable,
    status_good
  };

  //! status strings
  const std::map<Status,std::string> status_string = {
    { Status::status_unknown, "unknown" },
    { Status::status_bad, "bad" },
    { Status::status_questionable, "questionable" },
    { Status::status_good, "good" }
  };

  //! get status based on how many detectors are in acceptable range
  Status get_status( int n_detectors, const MicromegasDraw::detector_range_t acceptable_range )
  {
    if( n_detectors < acceptable_range.first ) { return Status::status_bad; }
    else if( n_detectors < acceptable_range.second ) { return Status::status_questionable; }
    else { return Status::status_good; }
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

  auto cl = QADrawClient::instance();
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

  } else if (name == "TPOT_SUMMARY") {

    auto cv = new TCanvas(name.c_str(), "TPOT summary", -1, 0,xsize/1.2, ysize/1.2);
    gSystem->ProcessEvents();
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
  auto cl = QADrawClient::instance();

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
  auto cl = QADrawClient::instance();
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
  auto cl = QADrawClient::instance();

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
    draw_range( h_cluster_multiplicity, m_cluster_multiplicity_range );
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
    draw_range( h_cluster_size, m_cluster_size_range );
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
    draw_range( h_cluster_charge, m_cluster_charge_range );
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
    draw_range( efficiency, m_efficiency_range );
    gPad->Update();
  }
  return 0;
}

//____________________________________________________________________________________________________
int MicromegasDraw::draw_summary()
{
  auto cl = QADrawClient::instance();

  Status status_cluster_multiplicity = Status::status_unknown;
  Status status_cluster_size = Status::status_unknown;
  Status status_cluster_charge = Status::status_unknown;
  Status status_efficiency = Status::status_unknown;
  Status status_global = Status::status_unknown;

  // load histograms
  auto h_cluster_count_ref = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_ref"));
  auto h_cluster_count_found = static_cast<TH1*>(cl->getHisto("h_MicromegasClusterQA_clustercount_found"));
  auto h_cluster_multiplicity_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_multiplicity"));
  auto h_cluster_size_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_size"));
  auto h_cluster_charge_raw = static_cast<TH2*>(cl->getHisto("h_MicromegasClusterQA_cluster_charge"));

  if( !((h_cluster_count_ref&&h_cluster_count_found)||h_cluster_multiplicity_raw||h_cluster_size_raw||h_cluster_charge_raw))
  {
    std::cout << "MicromegasDraw::draw_summary - no histograms found." << std::endl;
    return 0;
  }

  auto cv = get_canvas("TPOT_SUMMARY" );
  CanvasEditor cv_edit(cv);

  auto text = new TPaveText(0.1,0.1,0.9,0.9, "NDC" );
  text->SetFillColor(0);
  text->SetFillStyle(0);
  text->SetBorderSize(0);
  text->SetTextAlign(11);

  text->AddText( "TPOT summary:" );

  if( h_cluster_multiplicity_raw )
  {
    std::unique_ptr<TH1> h_cluster_multiplicity( get_detector_average(h_cluster_multiplicity_raw, -0.5) );
    auto ngood = get_num_valid_detectors( h_cluster_multiplicity.get(), m_cluster_multiplicity_range );
    status_cluster_multiplicity = get_status( ngood, m_detector_cluster_mult_range );
    text->AddText( Form("Number of detectors with cluster multiplicity in acceptable range: %i/16 - %s",ngood, status_string.at(status_cluster_multiplicity).c_str()));
  }

  if( h_cluster_size_raw )
  {
    std::unique_ptr<TH1> h_cluster_size( get_detector_average(h_cluster_size_raw, -0.5) );
    auto ngood = get_num_valid_detectors( h_cluster_size.get(), m_cluster_size_range );
    status_cluster_size = get_status( ngood, m_detector_cluster_size_range );
    text->AddText( Form("Number of detectors with cluster size in acceptable range: %i/16 - %s",ngood,status_string.at(status_cluster_size).c_str()));
  }

  if( h_cluster_charge_raw )
  {
    std::unique_ptr<TH1> h_cluster_charge( get_detector_average(h_cluster_charge_raw) );
    auto ngood = get_num_valid_detectors( h_cluster_charge.get(), m_cluster_charge_range );
    status_cluster_charge = get_status( ngood, m_detector_cluster_charge_range );
    text->AddText( Form("Number of detectors with cluster charge in acceptable range: %i/16 - %s",ngood,status_string.at(status_cluster_charge).c_str()));
  }

  if( h_cluster_count_ref&&h_cluster_count_found )
  {
    std::unique_ptr<TH1> efficiency(static_cast<TH1*>(h_cluster_count_found->Clone("efficiency_summary")));
    efficiency->Divide(h_cluster_count_found, h_cluster_count_ref, 1, 1, "B" );
    auto ngood = get_num_valid_detectors( efficiency.get(), m_efficiency_range );
    status_efficiency = get_status( ngood, m_detector_efficiency_range );
    text->AddText( Form("Number of detectors with efficiency estimate in acceptable range: %i/16 - %s",ngood,status_string.at(status_efficiency).c_str()));
  }

  // global status
  const std::vector<Status> all_status = { status_cluster_multiplicity, status_cluster_size, status_cluster_charge, status_efficiency };
  status_global = Status::status_questionable;

  // run is good if all is good
  if( std::all_of( all_status.begin(), all_status.end(), [](const Status& status){ return status == Status::status_good;}))
  { status_global = Status::status_good; }

  // run is bad if any is bad
  else if( std::any_of( all_status.begin(), all_status.end(), [](const Status& status){ return status == Status::status_bad;}))
  { status_global = Status::status_bad; }

  // run is unknown if any is unknown
  else if( std::any_of( all_status.begin(), all_status.end(), [](const Status& status){ return status == Status::status_unknown;}))
  { status_global = Status::status_unknown; }

  text->AddText( Form("Overall run status: %s",status_string.at(status_global).c_str()));


  text->Draw();
  cv->Update();

  return 0;
}

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
  if (what == "ALL" || what == "BCO")
  {
    auto cv = dynamic_cast<TCanvas*>( gROOT->FindObject( "TPOT_BCO" ) );
    if( cv )
    {
      pngfile = cl->htmlRegisterPage(*this, "tpot_bco", "1", "png");
      cl->CanvasToPng(cv, pngfile);
    }
  }

  // raw cluster information (experts)
  if (what == "ALL" || what == "CLUSTERS_RAW")
  {
    auto cv = dynamic_cast<TCanvas*>( gROOT->FindObject( "TPOT_CLUSTERS_RAW" ) );
    if( cv )
    {
      pngfile = cl->htmlRegisterPage(*this, "tpot_raw_cluster_info", "2", "png");
      cl->CanvasToPng(cv, pngfile);
    }
  }

  // average cluster information
  if (what == "ALL" || what == "CLUSTERS_AVG")
  {
    auto cv = dynamic_cast<TCanvas*>( gROOT->FindObject( "TPOT_CLUSTERS_AVG" ) );
    if( cv )
    {
      pngfile = cl->htmlRegisterPage(*this, "tpot_average_cluster_info", "3", "png");
      cl->CanvasToPng(cv, pngfile);
    }
  }

  // summary page
  if (what == "ALL" || what == "SUMMARY")
  {
    auto cv = dynamic_cast<TCanvas*>( gROOT->FindObject( "TPOT_SUMMARY" ) );
    if( cv )
    {
      pngfile = cl->htmlRegisterPage(*this, "tpot_summary", "4", "png");
      cl->CanvasToPng(cv, pngfile);
    }
  }

  return 0;
}

//________________________________________________________________
int MicromegasDraw::DBVarInit()
{
  return 0;
}
