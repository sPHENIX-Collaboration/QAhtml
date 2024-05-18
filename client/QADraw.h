#ifndef QA_CLIENT_DRAW_H
#define QA_CLIENT_DRAW_H

#include <fun4all/Fun4AllBase.h>
#include <string>

class Event;

class QADraw: public Fun4AllBase
{
 public:
  QADraw(const std::string &name = "NONE");
  virtual ~QADraw() {}

  virtual int Init() {return 0;}
  virtual int Draw(const std::string &what = "ALL");
  virtual int MakeHtml(const std::string &what = "ALL");

 protected:
};

#endif /* QA_CLIENT_DRAW_H */

