#include "FileTransfer.h"
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
#include <TLegend.h>
#include <TPaveText.h>
#include <TText.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

FileTransfer::FileTransfer(const std::string &name)
  : QADraw(name)
{
  memset(TC, 0, sizeof(TC));
  memset(Pad, 0, sizeof(Pad));
  return;
}

FileTransfer::~FileTransfer()
{
  return;
}

int FileTransfer::Draw(const std::string &what)
{
  int iret = 0;
  int idraw = 0;
  

  if (what == "ALL")
  {
    iret += DrawTransfer(what);
    idraw++;
  }
  if (!idraw)
  {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

TH1* FileTransfer::create_subsytems_histogram( const std::string& name, const std::string& title, const subsystem_info_t::list& subsystems = default_subsystems )
{

  const size_t count = subsystems.size();
  auto h = new TH1F( name.c_str(), title.c_str(), count, 0, count );

  // assign x axis bin labels
  for( size_t i = 0; i < count; ++i )
  { h->GetXaxis()->SetBinLabel( i+1, (subsystems[i].subsystem+"/"+subsystems[i].host).c_str()); }

  h->GetYaxis()->SetTitle( "segment count" );

  return h;
}

void FileTransfer::CheckFileTransfer()
{
 // const runnumber_set_t runnumbers = {69566, 69567, 69746,71251};

  // map runnumber with missing subsystems
  std::map<int, subsystem_info_t::list> missing;
  std::map<int, subsystem_info_t::list> missing_first_segment;

  // runnumbers with completes DB
  runnumber_set_t incomplete_db_runs;
  runnumber_set_t complete_db_runs;

  // set to true to verify if transfered files are effectively on disk
  const bool check_db_consistency = false;
  fileinfo_set_t inconsistent_files;

  // files missing from DB
  filename_set_t missing_files_from_db;

  // loop over runnumbers
int runnumber = QADrawClient::instance()->RunNumber();
    if( verbosity )
    { std::cout << "runnumber: " << runnumber << std::endl; }

    // create histograms
    TH1* h_expected = create_subsytems_histogram( "h_expected", "total number of segments", default_subsystems ) ;
    TH1* h_ref = create_subsytems_histogram( "h_ref", "total number of segments", default_subsystems ) ;
    TH1* h_transfered = create_subsytems_histogram( "h_transfered", "transfered segments", default_subsystems ) ;
   TH1* h_transfered_first_segment = create_subsytems_histogram( "h_first_segment_transfered", "first segment transfered", default_subsystems ) ;

    // counters
    unsigned int n_segments_expected = 0;
    unsigned int n_first_segment_expected = 0;

    unsigned int n_segments_total = 0;
    unsigned int n_first_segment_total = 0;

    unsigned int n_segments_transfered = 0;
    unsigned int n_first_segment_transfered = 0;

    // loop over subsystems
    for( size_t i = 0; i<default_subsystems.size(); ++i )
    {
      // get subsystem
      const auto& subsystem = default_subsystems[i];

      // get the files from database
      const auto daqdb_files = DBUtils::get_files_from_db( {runnumber}, subsystem );
      h_ref->Fill(i, daqdb_files.size());

      if( verbosity )
      { std::cout << "subsystem: " << subsystem << " files: " << daqdb_files << std::endl << std::endl; }

      // do nothing if daqdb_files is empty
      if( daqdb_files.empty() ) continue;

      // get max segment and expected filenames
      const auto max_segment = Utils::get_segment( std::max_element( daqdb_files.begin(), daqdb_files.end(),
        []( const fileinfo_t& first, const fileinfo_t& second )
        { return Utils::get_segment(first.filename) < Utils::get_segment(second.filename); } )->filename );
      const auto expected_filenames = Utils::get_expected_filenames( runnumber, max_segment+1, subsystem );

      n_segments_expected += expected_filenames.size();
      n_first_segment_expected += std::count_if( expected_filenames.begin(), expected_filenames.end(),
        []( const std::string& filename ) { return Utils::get_segment(filename)==0; } );

      h_expected->Fill(i, expected_filenames.size());

      // print
      if( verbosity )
      { std::cout << "subsystem: " << subsystem << " expected files: " << expected_filenames << std::endl << std::endl; }

      // look for missing filenames in the database by comparing expected to daqdb files
      std::copy_if( expected_filenames.begin(), expected_filenames.end(), std::inserter(missing_files_from_db,missing_files_from_db.end()),
        [&daqdb_files](const std::string& filename ){
          return std::find_if(daqdb_files.begin(),daqdb_files.end(), [&filename](const fileinfo_t& fileinfo)
          { return Utils::get_local_filename(fileinfo.filename)==filename; } ) == daqdb_files.end(); });

      // loop over files check if requested segments have been transfered
      bool transferred = true;
      bool transferred_first_segment = true;
      for( const auto& file_info:daqdb_files )
      {
        const bool is_first_segment = (Utils::get_segment(file_info.filename) == 0);

        // increment counters
        ++n_segments_total;
        if( is_first_segment ) { ++n_first_segment_total; }

        if( check_db_consistency && file_info.in_sdcc )
        {
          const auto lustre_filename = Utils::get_lustre_filename( Utils::get_local_filename(file_info.filename), subsystem );
          const bool consistent = std::filesystem::exists(lustre_filename);
          if( !consistent ) inconsistent_files.insert( file_info );
        }

        // check sdcc transfer status
        if( !file_info.in_sdcc )
        {
          transferred = false;
          if( is_first_segment )
          {
            transferred_first_segment = false;
          }
        } else {

          // fill histograms
          h_transfered->Fill(i);
          if( is_first_segment )
          { h_transfered_first_segment->Fill(i); }

          // increment counters
          ++n_segments_transfered;
          if( is_first_segment ) { ++n_first_segment_transfered; }

        }

      }

      if(!transferred) { missing[runnumber].emplace_back(subsystem); }
      if(!transferred_first_segment) { missing_first_segment[runnumber].emplace_back(subsystem); }

    }

    if( n_segments_total == n_segments_expected )
    {
      complete_db_runs.insert(runnumber);
    } else {
      incomplete_db_runs.insert(runnumber);
    }


 // make canvas and save
      //TC[0]->Divide(1,2 );

      // adjust pad dimensions
      //TC[0]->GetPad(1)->SetPad(0, 0.3, 1, 1);
      //TC[0]->GetPad(2)->SetPad(0, 0, 1, 0.3);

      // status histogram
      Pad[0]->cd();
      h_expected->SetStats(0);
      h_expected->SetTitle(Form( "File transfer status for run %i", runnumber ));
      h_expected->SetFillStyle(3001);
      h_expected->SetFillColor(kYellow-10);
      h_expected->SetMinimum(0.5);
      h_expected->GetXaxis()->SetLabelSize(0.03);
      h_expected->DrawCopy("hist");

      h_ref->SetFillStyle(3001);
      h_ref->SetFillColor(kYellow-7);
      h_ref->DrawCopy("hist same");

      h_transfered->SetFillStyle(3001);
      h_transfered->SetFillColor(kGreen-8);
      h_transfered->DrawCopy("hist same");

      h_transfered_first_segment->SetFillStyle(3001);
      h_transfered_first_segment->SetFillColor(kGreen+2);
      h_transfered_first_segment->DrawCopy("hist same");

      // legend
      auto legend = new TLegend( 0.7, 0.7, 0.95, 0.85, "", "NDC" );
      legend->SetFillStyle(0);
      legend->AddEntry( h_expected, "expected files", "f" );
      legend->AddEntry( h_ref, "files in DB", "f" );
      legend->AddEntry( h_transfered, "transfered", "f" );
      legend->AddEntry( h_transfered_first_segment, "first segment transfered", "f" );
      legend->Draw();

      Pad[0]->SetBottomMargin(0.15);
      Pad[0]->SetLeftMargin(0.1);
      Pad[0]->SetRightMargin(0.03);
      Pad[0]->SetLogy();

      // summary
      Pad[1]->cd();
      TPaveText* text = new TPaveText(0.1,0.1,0.9,0.9, "NDC" ) ;
      text->SetFillColor(0);
      text->SetFillStyle(0);
      text->SetBorderSize(0);
      text->SetTextAlign(11);

      text->AddText( "File transfer summary:" );

      if( n_segments_total == n_segments_expected )
      {
        text->AddText( Form("Number of files in db: %i, expected: %i - good",n_segments_total, n_segments_expected ))->SetTextColor(kGreen+1);
      } else {
        text->AddText( Form("Number of files in db: %i, expected: %i - bad",n_segments_total, n_segments_expected ))->SetTextColor(kRed+1);
      }

      if( n_first_segment_total == n_first_segment_expected )
      {
        text->AddText( Form("Number of first segment files in db: %i, expected: %i - good",n_first_segment_total, n_first_segment_expected ))->SetTextColor(kGreen+1);
      } else {
        text->AddText( Form("Number of first segment files in db: %i, expected: %i - bad",n_first_segment_total, n_first_segment_expected ))->SetTextColor(kRed+1);
      }


      if( n_segments_transfered == n_segments_total )
      {
        text->AddText( Form("Number of files transfered: %i/%i - good",n_segments_transfered,n_segments_total ))->SetTextColor(kGreen+1);
      } else {
        text->AddText( Form("Number of files transfered: %i/%i - bad",n_segments_transfered,n_segments_total ))->SetTextColor(kRed+1);
      }

      if( n_first_segment_transfered == n_first_segment_total )
      {
        text->AddText( Form("Number of first segment files transfered: %i/%i - good",n_first_segment_transfered,n_first_segment_total ))->SetTextColor(kGreen+1);
      } else {

        text->AddText( Form("Number of first segment files transfered: %i/%i - bad",n_first_segment_transfered,n_first_segment_total ))->SetTextColor(kRed+1);
      }

      text->Draw();


}
int FileTransfer::MakeCanvas(const std::string &name)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  if (name == "filetransfer")
  {
    // xpos (-1) negative: do not draw menu bar
    TC[0] = new TCanvas(name.c_str(), "FileTransfers", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
    Pad[0] = new TPad("pad0", "filetransfer0", 0.01, 0.35, 0.99, 0.97, 0);
    Pad[1] = new TPad("pad1", "filetransfer1", 0.05, 0.05, 0.95, 0.3, 0);
    Pad[0]->Draw();
    Pad[1]->Draw();
  }

  return 0;
}

int FileTransfer::DrawTransfer(const std::string & /*what*/)
{
  MakeCanvas("filetransfer");

  TC[0]->Clear("D");
  CheckFileTransfer();
  Pad[0]->Update();
  Pad[1]->Update();
  //TC[0]->Update();
  return 0;
}


int FileTransfer::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret)  // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();

  // Register the 1st canvas png file to the menu and produces the png file.
  std::string pngfile = cl->htmlRegisterPage(*this, "FileTransfer", "0", "png");
  cl->CanvasToPng(TC[0], pngfile);

  return 0;
}

