#ifndef BCO_BCODRAW_H__
#define BCO_BCODRAW_H__

#include <qahtml/QADraw.h>

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
  TCanvas *TC[6]{};
  TPad *transparent[6]{};
  TPad *Pad[6][8]{};
};

#endif
