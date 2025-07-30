#ifndef JET_DRAW_H
#define JET_DRAW_H

#include "JetDrawDefs.h"
#include <qahtml/QADrawClient.h>
#include <qahtml/QADraw.h>
#include <jetqa/JetQADefs.h>
#include <map>
#include <string>
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

// ============================================================================
//! Draw jet QA histograms
// ============================================================================
/*! A QAhtml subsystem to draw jet QA histograms
 *  and generate relevant HTML pages.
 */
class JetDraw : public QADraw
{
  public:

    // ========================================================================
    //! Tags for jet resolutions
    // ========================================================================
    /*! Enumerates possible jet resolutions to plot. */
    enum JetRes
    {
      R02,
      R03,
      R04,
      R05
    };

    // ctor/dtor
    JetDraw(const std::string &name = "JetQA");
    ~JetDraw() override;

    // setters
    void SetDoDebug(const bool debug) {m_do_debug = debug;}
    void SetJetType(const std::string& type) {m_jet_type = type;}
    void SetRhoPrefix(const std::string& prefix) {m_rho_prefix = prefix;}
    void SetCstPrefix(const std::string& prefix) {m_constituent_prefix = prefix;}
    void SetKinePrefix(const std::string& prefix) {m_kinematic_prefix = prefix;}
    void SetSeedPrefix(const std::string& prefix) {m_seed_prefix = prefix;}

    // getters 
    bool GetDoDebug() const {return m_do_debug;}
    std::string GetJetType() const {return m_jet_type;}
    std::string GetRhoPrefix() const {return m_rho_prefix;}
    std::string GetCstPrefix() const {return m_constituent_prefix;}
    std::string GetKinePrefix() const {return m_kinematic_prefix;}
    std::string GetSeedPrefix() const {return m_seed_prefix;}

    // inherited public methods
    int Draw(const std::string& what = "ALL") override;
    int MakeHtml(const std::string& what = "ALL") override;

    // other public methods
    int DBVarInit();
    void SetJetSummary(TCanvas* c);
    void SaveCanvasesToFile(TFile* file);

  private:

    // private methods
    int DrawRho(uint32_t trigger);
    int DrawConstituents(uint32_t trigToDraw, JetRes resToDraw);
    int DrawJetKinematics(uint32_t trigger, JetRes reso);
    int DrawJetSeed(uint32_t trigger, JetRes reso);
    void DrawRunAndBuild(const std::string& what, TPad* pad, const int trig = -1, const int res = -1);
    void DrawHists(const std::string& what, const std::vector<std::size_t>& indices, const JetDrawDefs::VHistAndOpts1D& hists, JetDrawDefs::VPlotPads1D& plots, const int trig = -1, const int res = -1);
    void DrawHistOnPad(const std::size_t iHist, const std::size_t iPad, const JetDrawDefs::VHistAndOpts1D& hists, JetDrawDefs::PlotPads& plot);
    void DrawEmptyHistogram(const std::string& what = "histogram");
    void MakeCanvas(const std::string& name, const int nHist, JetDrawDefs::VPlotPads1D& plots);
    void UpdatePadStyle(const JetDrawDefs::HistAndOpts& hist);
    void myText(double x, double y, int color, const char* text, double tsize = 0.04);

    ///! turn debugging statements on/off
    bool m_do_debug;

    ///! type of jet input, used in histogram names
    std::string m_jet_type;

    ///! prefix of event-wise rho qa histograms
    std::string m_rho_prefix;

    ///! prefix of constituent qa histograms 
    std::string m_constituent_prefix;

    ///! prefix of jet kinematic qa histograms
    std::string m_kinematic_prefix;

    ///! prefix of jet seed qa histograms
    std::string m_seed_prefix;

    ///! summary of overall jet qa goodness (currently unused)
    TCanvas* m_jetSummary{nullptr};

    ///! event-wise rho canvases
    JetDrawDefs::VPlotPads2D m_rhoPlots;

    ///! constituent canvases
    JetDrawDefs::VPlotPads3D m_cstPlots;

    ///! jet kinematic canvases
    JetDrawDefs::VPlotPads3D m_kinePlots;

    ///! jet seed canvases
    JetDrawDefs::VPlotPads3D m_seedPlots;

    ///! triggers we want to draw
    std::vector<uint32_t> m_vecTrigToDraw = {
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4
    };

    ///! resolutions we want to draw
    std::vector<uint32_t> m_vecResToDraw = {
      JetRes::R02,
      JetRes::R03,
      JetRes::R04,
      JetRes::R05
    };

    ///! map of resolution index to name
    std::map<uint32_t, std::string> m_mapResToName = {
      {JetRes::R02, "Res02"},
      {JetRes::R03, "Res03"},
      {JetRes::R04, "Res04"},
      {JetRes::R05, "Res05"}
    };

    ///! map of resultion index to tag
    std::map<uint32_t, std::string> m_mapResToTag = {
      {JetRes::R02, "r02"},
      {JetRes::R03, "r03"},
      {JetRes::R04, "r04"},
      {JetRes::R05, "r05"}
    };

    ///! map of trigger index onto name
    std::map<uint32_t, std::string> m_mapTrigToName = {
      {JetQADefs::GL1::MBDNSJet1, "JetCoin6GeV"},
      {JetQADefs::GL1::MBDNSJet2, "JetCoin8GeV"},
      {JetQADefs::GL1::MBDNSJet3, "JetCoin10GeV"},
      {JetQADefs::GL1::MBDNSJet4, "JetCoin12GeV"}
    };

    ///! map trig to tag (FIXME use those from Jet_QA.C)
    std::map<uint32_t, std::string> m_mapTrigToTag = {
      {JetQADefs::GL1::MBDNS1, "mbdns1"},
      {JetQADefs::GL1::MBDNSJet1, "mbdnsjet1"},
      {JetQADefs::GL1::MBDNSJet2, "mbdnsjet2"},
      {JetQADefs::GL1::MBDNSJet3, "mbdnsjet3"},
      {JetQADefs::GL1::MBDNSJet4, "mbdnsjet4"}
    };
};

#endif
