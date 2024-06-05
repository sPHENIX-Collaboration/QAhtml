#ifndef MVTX_MVTXDRAW_H__
#define MVTX_MVTXDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class MVTXDraw : public QADraw
{
 public: 
  MVTXDraw(const std::string &name = "MVTXQA");
  ~MVTXDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawChipInfo();
  TCanvas *TC[1]{};
  TPad *transparent[1]{};
  TPad *Pad[1][2]{};
  const char *histprefix;
};

#endif
