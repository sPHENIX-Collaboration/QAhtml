#ifndef JET_DRAW_H
#define JET_DRAW_H

#include <qahtml/QADrawClient.h>
#include <qahtml/QADraw.h>
#include <jetqa/JetQADefs.h>
#include <map>
#include <utility>
#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TFile;
class TGraphErrors;
class TH1;
class TH2;
class TPad;

// aliases for convenience
using VPad1D    = std::vector<TPad*>;
using VPad2D    = std::vector<std::vector<TPad*>>;
using VCanvas1D = std::vector<TCanvas*>;
using VCanvas2D = std::vector<std::vector<TCanvas*>>;

class JetDraw : public QADraw
{
  public:

    // ------------------------------------------------------------------------
    //! Helper struct to consolidate pads for drawing
    // ------------------------------------------------------------------------
    struct PlotPads
    {
      TCanvas* canvas  {nullptr};  ///< canvas to draw on
      TPad*    histPad {nullptr};  ///< pad for histograms
      TPad*    runPad  {nullptr};  ///< pad for run/build info
    };  // end PlotPads

    // vector of plot pads
    typedef std::vector<PlotPads> VPlotPads1D;
    typedef std::vector<std::vector<PlotPads>> VPlotPads2D;
    typedef std::vector<std::vector<std::vector<PlotPads>>> VPlotPads3D;

    // ------------------------------------------------------------------------
    //! Helper struct to consolidate histogram and drawing options
    // ------------------------------------------------------------------------
    struct HistAndOpts
    {
      TH1*        hist   {nullptr}; ///< histogram
      std::string title  {""};      ///< histogram title
      std::string titlex {""};      ///< x-axis title
      std::string titley {""};      ///< y-axis title
      std::string titlez {""};      ///< z-axis title
      float       margin {0.25};    ///< right margin of pad
      bool        logy   {false};   ///< make y axis log
      bool        logz   {false};   ///< make z axis log
    };  // end HistAndOpts

    // vector of histogram and options
    typedef std::vector<HistAndOpts> VHistAndOpts1D;

    // tags for jet resolutions
    enum JetRes
    {
      R02,
      R03,
      R04,
      R05
    };

    JetDraw(const std::string &name = "JetQA");
    ~JetDraw() override;
    int MakeHtml(const std::string &what = "ALL") override;
    int Draw(const std::string &what = "ALL") override;
    int DBVarInit();
    void SetJetSummary(TCanvas* c);
    void SetDoDebug(const bool debug);
    void SaveCanvasesToFile(TFile* file);

  private:

    int DrawRho(uint32_t trigger);
    int DrawConstituents(uint32_t trigToDraw, JetRes resToDraw);
    int DrawJetKinematics(uint32_t trigger, JetRes reso);
    int DrawJetSeed(uint32_t trigger, JetRes reso);
    void MakeCanvas(const std::string &name, const int nHist, VPlotPads1D& plots);
    void DrawRunAndBuild(const std::string& what, TPad* pad, const int trig = -1, const int res = -1);
    void DrawHists(const std::string& what, const std::vector<std::size_t>& indices, const VHistAndOpts1D& hists, VPlotPads1D& plots, const int trig = -1, const int res = -1);
    void UpdatePadStyle(const HistAndOpts& hist);
    void myText(double x, double y, int color, const char *text, double tsize = 0.04);

    TCanvas* jetSummary = nullptr;

    // canvas/pads for drawing
    VPlotPads2D m_rhoPlots;
    VPlotPads3D m_cstPlots;
    VPlotPads3D m_kinePlots;
    VPlotPads3D m_seedPlots;

    // turn debugging statements on/off
    bool m_do_debug;

    const char* m_constituent_prefix;
    const char* m_rho_prefix;
    const char* m_kinematic_prefix;
    const char* m_seed_prefix;

    // jet type (TODO will need to expand this to a vector in the future)
    const char* m_jet_type;

    // triggers we want to draw
    std::vector<uint32_t> m_vecTrigToDraw = {
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4
    };

    // resolutions we want to draw
    std::vector<uint32_t> m_vecResToDraw = {
      JetRes::R02,
      JetRes::R03,
      JetRes::R04,
      JetRes::R05
    };

    // map of resolution index to name
    std::map<uint32_t, std::string> m_mapResToName = {
      {JetRes::R02, "Res02"},
      {JetRes::R03, "Res03"},
      {JetRes::R04, "Res04"},
      {JetRes::R05, "Res05"}
    };

    //map res to Tag
    std::map<uint32_t, std::string> m_mapResToTag = {
      {JetRes::R02, "r02"},
      {JetRes::R03, "r03"},
      {JetRes::R04, "r04"},
      {JetRes::R05, "r05"}
    };

    // map of trigger index onto name
    std::map<uint32_t, std::string> m_mapTrigToName = {
      {JetQADefs::GL1::MBDNSJet1, "JetCoin6GeV"},
      {JetQADefs::GL1::MBDNSJet2, "JetCoin8GeV"},
      {JetQADefs::GL1::MBDNSJet3, "JetCoin10GeV"},
      {JetQADefs::GL1::MBDNSJet4, "JetCoin12GeV"}
    };

    // map trig to tag
    std::map<uint32_t, std::string> m_mapTrigToTag = {
      {JetQADefs::GL1::MBDNS1, "mbdns1"},
      {JetQADefs::GL1::MBDNSJet1, "mbdnsjet1"},
      {JetQADefs::GL1::MBDNSJet2, "mbdnsjet2"},
      {JetQADefs::GL1::MBDNSJet3, "mbdnsjet3"},
      {JetQADefs::GL1::MBDNSJet4, "mbdnsjet4"}
    };

};  // end JetDraw

#endif
