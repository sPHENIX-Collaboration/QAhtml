#ifndef GLOBALQADRAW_H__
#define GLOBALQADRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADrawDB;
class QADrawDBVar;
class TCanvas;
class TPad;

class GlobalQADraw : public QADraw
{
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
  int DrawTrigger(const std::string &what = "ALL");
  QADrawDB *db{nullptr};
  TCanvas *TC[4]{};
  TPad *transparent[4]{};
  TPad *Pad[4][5]{};
};

#endif
