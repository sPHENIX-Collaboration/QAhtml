#ifndef CALODRAW_H__
#define CALODRAW_H__

#include <qahtml/OnlProdDraw.h>

#include <vector>

class OnlProdDB;
class OnlProdDBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class CaloDraw: public OnlProdDraw
{

 public: 
  CaloDraw(const std::string &name = "CaloQA");
  virtual ~CaloDraw();

  int Draw(const std::string &what = "ALL");
  int MakeHtml(const std::string &what = "ALL");
  int DBVarInit();

 protected:
  int MakeCanvas(const std::string &name, int num);
  int DrawCemc();
  int DrawIhcal();
  int DrawOhcal();
  int DrawCorr();
  int DrawZdc();
  TH1F* proj(TH2F* h2);
  TH1F* FBratio(TH1F* h);
  void myText(double x,double y,int color, const char *text, double tsize=0.04);
  OnlProdDB *db;
  TCanvas *TC[7];
  TPad *transparent[7];
  TPad *Pad[7][4];
  const char *histprefix;
};

#endif
