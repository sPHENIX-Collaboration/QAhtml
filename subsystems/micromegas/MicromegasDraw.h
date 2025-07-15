#ifndef MICROMEGAS_MICROMEGASDRAW_H__
#define MICROMEGAS_MICROMEGASDRAW_H__

#include <qahtml/QADraw.h>
#include <vector>

class TCanvas;
class TPad;
class TH1;
class TH2;

class MicromegasDraw : public QADraw
{
 public:
  MicromegasDraw(const std::string &name = "MicromegasQA");

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  TH1* ClusterAverage(TH2*, std::string);

  // get canvas by name
  TCanvas* get_canvas(const std::string& name, bool clear = true );

  // create canbas
  TCanvas* create_canvas(const std::string &name);

  int DrawBCOInfo();
  int DrawClusterInfo();
  int DrawRawInfo();
  int DrawSummary();

  // canvases
  std::vector<TCanvas*> m_canvas;

};

#endif
