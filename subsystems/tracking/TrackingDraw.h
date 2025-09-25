#ifndef TRACKING_TRACKINGDRAW_H__
#define TRACKING_TRACKINGDRAW_H__

#include <qahtml/QADraw.h>

#include <map>
#include <string>

class SingleCanvasDrawer;

class TrackingDraw : public QADraw
{
 public: 
  TrackingDraw(const std::string &name = "TRACKINGDRAWQA");
  ~TrackingDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int SaveCanvas(const std::string &what = "ALL");
  int DBVarInit();

 private:
  std::map<std::string, SingleCanvasDrawer*> m_options;

};

#endif
