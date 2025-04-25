#ifndef BCO_BCODRAW_H__
#define BCO_BCODRAW_H__

#include <qahtml/QADraw.h>

#include <TStyle.h>
#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class BCODraw : public QADraw
{
 public: 
  BCODraw(const std::string &name = "BCOQA");
  ~BCODraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawMVTX();
  int DrawINTT();
  int DrawTPC();
  int DrawTPOT();
  void myText(Double_t x,Double_t y,Color_t color,const char *text, Double_t tsize = 0.05, double angle = -1);
  TCanvas *TC[6]{};
  TPad *transparent[6]{};
  TPad *Pad[6][8]{};
};

#endif
