#ifndef INTT_INTTDRAW_H__
#define INTT_INTTDRAW_H__

#include "SingleCanvasDrawer.h"

#include <qahtml/QADraw.h>

#include <map>
#include <string>
#include <vector>

// class QADB;
// class QADBVar;
// class TCanvas;
// class TGraphErrors;
// class TPad;
// class TH1F;
// class TH2F;

class INTTDraw : public QADraw
{
 public: 
  INTTDraw(const std::string &name = "INTTQA");
  ~INTTDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  std::map<std::string, SingleCanvasDrawer*> m_options;

};

#endif
