#ifndef __ONCALDRAW_H__
#define __ONCALDRAW_H__

#include <fun4all/Fun4AllBase.h>
#include <string>

class Event;

class OnlProdDraw: public Fun4AllBase
{
 public:
  OnlProdDraw(const std::string &name = "NONE");
  virtual ~OnlProdDraw() {}

  virtual int Init() {return 0;}
  virtual int Draw(const std::string &what = "ALL");
  virtual int MakeHtml(const std::string &what = "ALL");

 protected:
};

#endif /* __ONCALDRAW_H__ */

