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
  range_list_t m_cluster_multiplicity_range ={
    {2.,2.6}, // SCOP
    {2.,2.6}, // SCIP
    {2.,2.6}, // NCIP
    {2.,2.8}, // NCOP
    {2.,2.6}, // SEIP
    {2.,2.8}, // NEIP
    {2.,2.6}, // SWIP
    {2.,2.6}, // NWIP
    {1.8,2.6}, // SCOZ
    {2.,2.8}, // SCIZ
    {2.,2.8}, // NCIZ
    {2.,2.8}, // NCOZ
    {2.,2.8}, // SEIZ
    {2.,2.8}, // NEIZ
    {2.,2.8}, // SWIZ
    {2.,2.8} // NWIZ
  };

  //! acceptable numbers of good detectors for cluster multiplicity
  detector_range_t m_detector_cluster_mult_range = {7,13};

  //! acceptable cluster size range
  range_list_t m_cluster_size_range = {
    {2.6,3.}, // SCOP
    {2.6,3.}, // SCIP
    {2.6,3.}, // NCIP
    {2.8,3.2}, // NCOP
    {2.6,3.}, // SEIP
    {2.6,3.}, // NEIP
    {2.6,3.}, // SWIP
    {2.6,3.}, // NWIP
    {2.,2.4}, // SCOZ
    {2.,2.4}, // SCIZ
    {2.,2.4}, // NCIZ
    {2.3,2.7}, // NCOZ
    {1.8,2.2}, // SEIZ
    {2.,2.4}, // NEIZ
    {2.,2.4}, // SWIZ
    {2.,2.4} // NWIZ
  };

  //! acceptable numbers of good detectors for cluster size
  detector_range_t m_detector_cluster_size_range = {8,13};

  //! acceptable cluster charge range
  range_list_t m_cluster_charge_range = {
    {500,600}, // SCOP
    {450,550}, // SCIP
    {400,500}, // NCIP
    {500,600}, // NCOP
    {500,600}, // SEIP
    {500,600}, // NEIP
    {400,500}, // SWIP
    {450,550}, // NWIP
    {450,550}, // SCOZ
    {500,600}, // SCIZ
    {450,550}, // NCIZ
    {450,550}, // NCOZ
    {450,550}, // SEIZ
    {450,550}, // NEIZ
    {450,550}, // SWIZ
    {450,550} // NWIZ
  };


  //! acceptable numbers of good detectors for cluster charge
  detector_range_t m_detector_cluster_charge_range = {8,13};

  //! acceptable efficiency range
  range_list_t m_efficiency_range =
  {
    {0.85,1.0}, // SCOP
    {0.70,1.0}, // SCIP
    {0.66,1.0}, // NCIP
    {0.76,1.0}, // NCOP
    {0.72,1.0}, // SEIP
    {0.72,1.0}, // NEIP
    {0.58,1.0}, // SWIP
    {0.72,1.0}, // NWIP
    {0.46,1.0}, // SCOZ
    {0.76,1.0}, // SCIZ
    {0.80,1.0}, // NCIZ
    {0.74,1.0}, // NCOZ
    {0.74,1.0}, // SEIZ
    {0.72,1.0}, // NEIZ
    {0.78,1.0}, // SWIZ
    {0.74,1.0}  // NWIZ
  };

  //! acceptable numbers of good detectors for efficiency estimate
  detector_range_t m_detector_efficiency_range = {9,13};

};

#endif
