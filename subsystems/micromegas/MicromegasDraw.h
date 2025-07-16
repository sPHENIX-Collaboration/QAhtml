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

  private:
  TH1* get_detector_average(TH2*, double /*offset*/ = 0);

  // get canvas by name
  TCanvas* get_canvas(const std::string& name, bool clear = true );

  // create canbas
  TCanvas* create_canvas(const std::string &name);

  //! bco information
  int draw_bco_info();

  //! raw cluster information
  int draw_raw_cluster_info();

  //! averaged cluster information
  int draw_average_cluster_info();

  //! summary
  int draw_summary();

  //! canvases
  std::vector<TCanvas*> m_canvas;

  //! acceptable cluster multiplicity range
  range_list_t m_cluster_multiplicity_range = range_list_t(16, {1.5,4});

  //! acceptable cluster size range
  range_list_t m_cluster_size_range = range_list_t(16, {1.5,4});

  //! acceptable cluster charge range
  range_list_t m_cluster_charge_range = range_list_t(16, {300,700});

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

};

#endif
