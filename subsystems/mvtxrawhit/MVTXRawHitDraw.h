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

class MVTXRawHitDraw : public QADraw
{
 public: 
  MVTXRawHitDraw(const std::string &name = "MVTXRAWHITQA");
  ~MVTXRawHitDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawChipInfo();
  TCanvas *TC[1]{};
  TPad *transparent[1]{};
  TPad *Pad[1][6]{};
  const char *histprefix;
};

#endif
