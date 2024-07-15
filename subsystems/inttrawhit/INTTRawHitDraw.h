#ifndef INTT_INTTDRAW_H__
#define INTT_INTTDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class INTTRawHitDraw : public QADraw
{
 public:
  INTTRawHitDraw(const std::string &name = "INTTRAWHITQA");
  ~INTTRawHitDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int MakeChipCanvas(const std::string &name, int num);
  int DrawChipInfo();
  int DrawSummary();
  TCanvas *TC[2]{};
  TCanvas *chipCanvas[8]{};
  TPad *transparent[2]{};
  TPad *Pad[2][8]{};
  const char *histprefix;
};

#endif
