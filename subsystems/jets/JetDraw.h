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
class TGraphErrors;
class TPad;
class TH1;
class TH2;
//class JetGoodRunChecker; TBD

// aliases for convenience
using VPad2D    = std::vector<std::vector<TPad*>>;
using VPad3D    = std::vector<std::vector<std::vector<TPad*>>>;
using VCanvas1D = std::vector<TCanvas*>;
using VCanvas2D = std::vector<std::vector<TCanvas*>>;


class JetDraw : public QADraw
{
 public:
  JetDraw(const std::string &name = "JetQA");
  ~JetDraw() override;
  // tags for jet resolutions
    enum JetRes
    {
      R02,
      R03,
      R04,
      R05
    }; 
  // int Draw(listOfTrigsToDraw);
  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;   // rather than a string do draw from a list of ints
  int DBVarInit();
  void SetJetSummary(TCanvas* c);
 
  // void SetCemcChecker(JetGoodRunChecker* ch) {cemc_checker = ch;} TBD -- topical group input

 private:
  int MakeCanvas(const std::string &name, int num);
  QADrawClient* cl; // Declare cl here   
  int nDrawError;
  int idraw;

  //  int Draw(const std::string &what);
  int DrawTrigAndRes(int trigToDraw, JetRes resToDraw);  // or uint32_t based on your types
  int DrawConstituents(uint32_t trigToDraw, JetRes resToDraw);
  int DrawRho(uint32_t trigger);
  int DrawJetKinematics(uint32_t trigger, JetRes reso);
  int DrawJetSeed(uint32_t trigger, JetRes reso);

  TH1 *proj(TH2 *h2);
  //  TH1 *FBratio(TH1 *h);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);
  //  QADB *db {nullptr};
// REMOVE
  const static int ncanvases = 8;
  const static int maxpads = 6;
  TCanvas *TC[ncanvases]{};
  TPad *transparent[ncanvases]{};
  TPad *Pad[ncanvases][maxpads]{};
//
  TCanvas* jetSummary = nullptr;

  // canvases for drawing
  VCanvas1D m_vecRhoCanvas;
  VCanvas2D m_vecCstCanvas;
  VCanvas2D m_vecKineCanvas;
  VCanvas2D m_vecSeedCanvas;

  // for adding run numbers to canvases
  VCanvas1D m_vecRhoRun;
  VCanvas2D m_vecCstRun;
  VCanvas2D m_vecKineRun;
  VCanvas2D m_vecSeedRun;

  // for individual pads on each canvas
  VPad2D m_vecRhoPad;
  VPad3D m_vecCstPad;
  VPad3D m_vecKinePad;
  VPad3D m_vecSeedPad;

  // add summary canvases for hcal etc later
  const char *histprefix;
  const char *histprefix1;
  const char *histprefix2;
  const char *histprefix3;
  //  JetGoodRunChecker* cemc_checker = nullptr;

  // triggers we want to draw
  std::vector<uint32_t> m_vecTrigToDraw = {
    /* maybe some others */
    JetQADefs::GL1::Jet1,
    JetQADefs::GL1::Jet2,
    JetQADefs::GL1::Jet3,
    JetQADefs::GL1::Jet4
  };
  // vector  we want to draw
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
    /* maybe some others */
    {JetQADefs::GL1::Jet1, "Jet6GeV"},
    {JetQADefs::GL1::Jet2, "Jet8GeV"},
    {JetQADefs::GL1::Jet3, "Jet10GeV"},
    {JetQADefs::GL1::Jet4, "Jet12GeV"}
  };

  // map trig to tag
  std::map<uint32_t, std::string> m_mapTrigToTag = {
    //    {JetQADefs::GL1::MBDN, "inclusive"}, // uint32_t name ??
    {JetQADefs::GL1::MBDNS1, "mbdns1"}, //  uint32_t name ??
    {JetQADefs::GL1::Jet1, "mbdnsjet1"},
    {JetQADefs::GL1::Jet2, "mbdnsjet2"},
    {JetQADefs::GL1::Jet3, "mbdnsjet3"},
    {JetQADefs::GL1::Jet4, "mbdnsjet4"}
  };
};
#endif
