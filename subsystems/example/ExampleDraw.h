#ifndef EXAMPLEDRAW_H__
#define EXAMPLEDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADrawDB;
class QADrawDBVar;
class TCanvas;
class TGraphErrors;
class TPad;

class ExampleDraw : public QADraw
{
 public:
  ExampleDraw(const std::string &name = "EXAMPLE");
  ~ExampleDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 protected:
  int MakeCanvas(const std::string &name);
  int DrawFirst(const std::string &what = "ALL");
  int DrawGraph(TPad *pad, const std::vector<QADrawDBVar> &history, const time_t begin, const time_t end);
  QADrawDB *db{nullptr};
  TCanvas *TC[1]{};
  TPad *transparent[1]{};
  TPad *Pad[4]{};
  TGraphErrors *gr[1]{};
};

#endif
