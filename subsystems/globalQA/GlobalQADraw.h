#ifndef GLOBALQADRAW_H__
#define GLOBALQADRAW_H__

#include <qahtml/QADraw.h>
#include <Rtypes.h>         // For ClassDef/ClassImp
#include <TObject.h> 
#include <RooRealVar.h> 
#include <RooLandau.h>
#include <RooGaussian.h>
#include <RooFFTConvPdf.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooFitResult.h>
#include <RooConstVar.h>

class QADrawDB;
class TCanvas;
class TPad;

struct ChannelFitData {
  RooRealVar* x;
  RooRealVar* ml;
  RooRealVar* sl;
  RooRealVar* sg;
  RooLandau* landau;
  RooGaussian* gauss;
  RooFFTConvPdf* convPDF;
  RooDataHist* data;
  RooPlot* frame;
  RooFitResult* fitResult;

  ChannelFitData() : 
    x(nullptr), ml(nullptr), sl(nullptr), sg(nullptr),
    landau(nullptr), gauss(nullptr), convPDF(nullptr),
    data(nullptr), frame(nullptr), fitResult(nullptr) {}
    
  ~ChannelFitData() {
    delete x; delete ml; delete sl; delete sg;
    delete landau; delete gauss; delete convPDF;
    delete data; delete frame; delete fitResult;
  }
};

class GlobalQADraw : public QADraw {
 public:
  GlobalQADraw(const std::string &name = "GlobalQA");
  ~GlobalQADraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 protected:
  int MakeCanvas(const std::string &name, int num);
  int DrawMBD(const std::string &what = "ALL");
  int DrawZDC(const std::string &what = "ALL");
  int DrawsEPD(const std::string &what = "ALL");
  int DrawsEPD_fits(const std::string &what = "ALL");
  double FindHistogramPeak(TH1* hist, double min_adc);
  void DetermineFitRange(TH1* hist, double& fit_lo, double& fit_hi);
  //bool LoadEPDChannelMap();
  void InitializeFailureMap();

  QADrawDB *db{nullptr};
  TCanvas *TC[4]{};
  TPad *transparent[4]{};
  TPad *Pad[3][7]{};

  std::vector<TCanvas*> m_epdFitCanvases;
  std::vector<ChannelFitData*> m_epdFitData;
  //TH2F* m_failureMap;
  TH2F* m_failureMapPolarS = nullptr;  // South
  TH2F* m_failureMapPolarS01 = nullptr; // tile 0
  TH2F* m_failureMapPolarN = nullptr;  // North
  TH2F* m_failureMapPolarN01 = nullptr; // tile 0
  std::vector<std::pair<int, int>> m_channelPositions;
  std::vector<int> m_channelArm;
  //CDBTTree* m_cdbttreeEPD{nullptr};
  int run_type = 0; // AuAu=0, pp=1
};

#endif