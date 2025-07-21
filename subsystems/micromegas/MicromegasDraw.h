#ifndef MICROMEGAS_MICROMEGASDRAW_H__
#define MICROMEGAS_MICROMEGASDRAW_H__

#include <qahtml/QADraw.h>

#include <cassert>
#include <vector>

class TCanvas;
class TPad;
class TH1;
class TH2;

class MicromegasDraw : public QADraw
{
 public:
  MicromegasDraw(const std::string &name = "MicromegasQA");

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

  using range_t = std::pair<double,double>;
  using range_list_t = std::vector<range_t>;

  //! acceptable n detector range
  /**
   * first is range below which status is bad
   * second is range below which status us questionable
   * and above which status is good
   */
  using detector_range_t = std::pair<int, int>;

  //! acceptable cluster multiplicity range
  void set_gl1_drop_rate_range( const range_t& value )
  {
    for( size_t i=0; i<m_gl1_drop_rate_range.size(); ++i )
    { m_gl1_drop_rate_range[i] = value; }
  }

  //! acceptable gl1 drop rate range
  void set_gl1_drop_rate_range( size_t i, const range_t& value )
  {
    assert( i<m_gl1_drop_rate_range.size());
    m_gl1_drop_rate_range[i] = value;
  }

  //! acceptable number of good packets for gl1 drop rate
  void set_packet_gl1_drop_rate_range( int n_questionable, int n_good )
  { m_packet_gl1_drop_rate_range = {n_questionable,n_good}; }

  //! acceptable per packet waveform drop rate range
  void set_waveform_drop_rate_range( const range_t& value )
  {
    for( size_t i=0; i<m_waveform_drop_rate_range.size(); ++i )
    { m_waveform_drop_rate_range[i] = value; }
  }

  //! acceptable per packet waveform drop rate range
  void set_waveform_drop_rate_range( size_t i, const range_t& value )
  {
    assert( i<m_waveform_drop_rate_range.size());
    m_waveform_drop_rate_range[i] = value;
  }

  //! acceptable number of good packets for waveform drop rate
  void set_packet_wf_drop_rate_range( int n_questionable, int n_good )
  { m_packet_wf_drop_rate_range = {n_questionable,n_good}; }

  //! acceptable per fee waveform drop rate range
  void set_fee_waveform_drop_rate_range( const range_t& value )
  {
    for( size_t i=0; i<m_fee_waveform_drop_rate_range.size(); ++i )
    { m_fee_waveform_drop_rate_range[i] = value; }
  }

  //! acceptable per fee waveform drop rate range
  void set_fee_waveform_drop_rate_range( size_t i, const range_t& value )
  {
    assert( i<m_fee_waveform_drop_rate_range.size());
    m_fee_waveform_drop_rate_range[i] = value;
  }

  //! acceptable number of good fee for waveform drop rate
  void set_fee_wf_drop_rate_range( int n_questionable, int n_good )
  { m_fee_wf_drop_rate_range = {n_questionable,n_good}; }

  //! acceptable cluster multiplicity range
  void set_cluster_multiplicity_range( const range_t& value )
  {
    for( size_t i=0; i<m_cluster_multiplicity_range.size(); ++i )
    { m_cluster_multiplicity_range[i] = value; }
  }

  //! acceptable cluster multiplicity range
  void set_cluster_multiplicity_range( size_t i, const range_t& value )
  {
    assert( i<m_cluster_multiplicity_range.size());
    m_cluster_multiplicity_range[i] = value;
  }

  //! acceptable number of good detectors for cluster multiplicity
  void set_detector_cluster_mult_range( int n_questionable, int n_good )
  { m_detector_cluster_mult_range = {n_questionable,n_good}; }

  //! acceptable cluster size range
  void set_cluster_size_range( const range_t& value )
  {
    for( size_t i=0; i<m_cluster_size_range.size(); ++i )
    { m_cluster_size_range[i] = value; }
  }

  //! acceptable cluster size range
  void set_cluster_size_range( size_t i, const range_t& value )
  {
    assert( i<m_cluster_size_range.size());
    m_cluster_size_range[i] = value;
  }

  //! acceptable number of good detectors for cluster size
  void set_detector_cluster_size_range( int n_questionable, int n_good )
  { m_detector_cluster_size_range = {n_questionable,n_good}; }

  //! acceptable charge range
  void set_cluster_charge_range( const range_t& value )
  {
    for( size_t i=0; i<m_cluster_charge_range.size(); ++i )
    { m_cluster_charge_range[i] = value; }
  }

  //! acceptable charge range
  void set_cluster_charge_range( size_t i, const range_t& value )
  {
    assert( i<m_cluster_charge_range.size());
    m_cluster_charge_range[i] = value;
  }

  //! acceptable number of good detectors for cluster charge
  void set_detector_cluster_charge_range( int n_questionable, int n_good )
  { m_detector_cluster_charge_range = {n_questionable,n_good}; }

  //! acceptable efficiency range
  void set_efficiency_range( const range_t& value )
  {
    for( size_t i=0; i<m_efficiency_range.size(); ++i )
    { m_efficiency_range[i] = value; }
  }

  //! acceptable efficiency range
  void set_efficiency_range( size_t i, const range_t& value )
  {
    assert( i<m_efficiency_range.size());
    m_efficiency_range[i] = value;
  }

  //! acceptable number of good detectors for cluster charge
  void set_detector_efficiency_range( int n_questionable, int n_good )
  { m_detector_efficiency_range = {n_questionable,n_good}; }

  private:
  TH1* get_detector_average(TH2*, double /*offset*/ = 0);

  // get canvas by name
  TCanvas* get_canvas(const std::string& name, bool clear = true );

  // create canbas
  TCanvas* create_canvas(const std::string &name);

  // draw run and time in a given pad
  void draw_title( TPad*);

  //! bco information
  int draw_bco_info();

  //! raw cluster information
  int draw_raw_cluster_info();

  //! averaged cluster information
  int draw_average_cluster_info();

  //! summary
  int draw_bco_summary();

  //! summary
  int draw_summary();

  //! canvases
  std::vector<TCanvas*> m_canvas;

  //! number of active fee boards
  static constexpr int m_nfee_active = 16;

  // maximum number of packets
  static constexpr int m_npackets_active = 2;

  //! number of fee boards
  static constexpr int m_nfee_max = 26;

  //! acceptable gl1 drop rate
  range_list_t m_gl1_drop_rate_range = range_list_t(m_npackets_active+1, {0, 0.01});

  //! acceptable numbers of good packets for g1l drop rate
  detector_range_t m_packet_gl1_drop_rate_range = {3,3};

  //! acceptable per packet waveform drop rate
  range_list_t m_waveform_drop_rate_range = range_list_t(m_npackets_active+1, {0, 0.05});

  //! acceptable numbers of good packets for waveform drop rate
  detector_range_t m_packet_wf_drop_rate_range = {3,3};

  //! acceptable per packet waveform drop rate
  range_list_t m_fee_waveform_drop_rate_range = {
    {0, 0.05}, {0, 0.05}, {0, 0.00}, {0, 0.00}, {0, 0.00},
    {0, 0.05}, {0, 0.05}, {0, 0.05}, {0, 0.05}, {0, 0.05},
    {0, 0.00}, {0, 0.00}, {0, 0.05}, {0, 0.00}, {0, 0.05},
    {0, 0.05}, {0, 0.00}, {0, 0.00}, {0, 0.05}, {0, 0.05},
    {0, 0.00}, {0, 0.05}, {0, 0.00}, {0, 0.05}, {0, 0.05},
    {0, 0.05} };

  //! acceptable numbers of good fee for waveform drop rate
  detector_range_t m_fee_wf_drop_rate_range = {8,13};

  //! acceptable cluster multiplicity range
  range_list_t m_cluster_multiplicity_range = range_list_t(m_nfee_active, {1.5,4});

  //! acceptable numbers of good detectors for cluster multiplicity
  detector_range_t m_detector_cluster_mult_range = {7,13};

  //! acceptable cluster size range
  range_list_t m_cluster_size_range = {
    {2,4}, // SCOP
    {2,4}, // SCIP
    {2,4}, // NCIP
    {2,4}, // NCOP
    {2,4}, // SEIP
    {2,4}, // NEIP
    {2,4}, // SWIP
    {2,4}, // NWIP
    {1.5,3.5}, // SCOZ
    {1.5,3.5}, // SCIZ
    {1.5,3.5}, // NCIZ
    {1.5,3.5}, // NCOZ
    {1.5,3.5}, // SEIZ
    {1.5,3.5}, // NEIZ
    {1.5,3.5}, // SWIZ
    {1.5,3.5}  // NWIZ
  };

  //! acceptable numbers of good detectors for cluster size
  detector_range_t m_detector_cluster_size_range = {8,13};

  //! acceptable cluster charge range
  range_list_t m_cluster_charge_range = range_list_t(m_nfee_active, {300,700});

  //! acceptable numbers of good detectors for cluster charge
  detector_range_t m_detector_cluster_charge_range = {8,13};

  //! acceptable efficiency range
  range_list_t m_efficiency_range =
  {
    {0.6,1.0}, // SCOP
    {0.6,1.0}, // SCIP
    {0.5,1.0}, // NCIP
    {0.6,1.0}, // NCOP
    {0.6,1.0}, // SEIP
    {0.6,1.0}, // NEIP
    {0.5,1.0}, // SWIP
    {0.4,1.0}, // NWIP
    {0.4,1.0}, // SCOZ
    {0.6,1.0}, // SCIZ
    {0.6,1.0}, // NCIZ
    {0.6,1.0}, // NCOZ
    {0.6,1.0}, // SEIZ
    {0.6,1.0}, // NEIZ
    {0.6,1.0}, // SWIZ
    {0.6,1.0}  // NWIZ
  };

  //! acceptable numbers of good detectors for efficiency estimate
  detector_range_t m_detector_efficiency_range = {9,13};

};

#endif
