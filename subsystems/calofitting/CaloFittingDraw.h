#ifndef CALOFITTING_CALOFITTINGDRAW_H
#define CALOFITTING_CALOFITTINGDRAW_H

#include <qahtml/QADraw.h>

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <array>

class TCanvas;
class TPad;

class CaloFittingDraw : public QADraw
{
 public:

  CaloFittingDraw( const std::string &name = "CaloFittingQA" );
  ~CaloFittingDraw() override {} 

  int Draw( const std::string &what = "ALL" ) override;
  int MakeHtml( const std::string &what = "ALL" ) override;

  // int DBVarInit();

  enum PacketStatus {
    VOID = -1,
    GOOD = 0,
    INCOMPLETE = 1,
    EMPTY = 2,
    MISSING = 3 
  };


 private:

  int MakeCanvas( const std::string &name, int num );
  int MakeDets( const std::string &name, int num );
  int DrawCalo( int num );

  int GetPacketInfo();
  int MakeAlignmentSummary(const std::string &name, int num);
  bool gl1daq_dropped() const;
  std::vector< int > FilterPackets( CaloFittingDraw::PacketStatus status  = CaloFittingDraw::PacketStatus::VOID ) const;
  void myText( double x, double y, int color, const char *text, double tsize = 0.04 );

  const static int ncanvases { 9 };
  const static int maxpads { 9 };

  TCanvas *TC[ncanvases]{};
  TPad *transparent[ncanvases]{};
  TPad *Pad[ncanvases][maxpads]{};

  int canvas_xsize {1600};
  int canvas_ysize {800};

  const char * histprefix {"h_CaloFittingQA_"};

  int n_events_db { 0 };
  int n_events { 0 };
  int run_number { 0 };
  std::string run_time {""};
  std::string qa_time {""};
  std::string end_time {""};


  std::map< int, PacketStatus > packet_status_map {};
  std::map< int, int > packet_events_map {};

  static const int nsebs_emcal = 16;
  static const int nsebs_hcal = 2;
  static const int nsebs = 18;
  static const int packets_per_seb = 8;
  static const int npackets_total = 144;
  static const std::array < int, packets_per_seb > seb00_packet_ids,
    seb01_packet_ids, seb02_packet_ids, seb03_packet_ids,
    seb04_packet_ids, seb05_packet_ids, seb06_packet_ids,
    seb07_packet_ids, seb08_packet_ids, seb09_packet_ids,
    seb10_packet_ids, seb11_packet_ids, seb12_packet_ids,
    seb13_packet_ids, seb14_packet_ids, seb15_packet_ids,
    seb16_packet_ids, seb17_packet_ids,
    inner_hcal_packet_ids, outer_hcal_packet_ids;
  static const std::array< int, packets_per_seb*nsebs_hcal > hcal_packet_ids;
  static const std::array< int, npackets_total > valid_packet_ids;
  static const std::pair < int, int > emcal_packet_range, hcal_packet_range, inner_hcal_packet_range, outer_hcal_packet_range;

  


};

#endif