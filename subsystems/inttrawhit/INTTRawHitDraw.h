#ifndef INTT_INTT_RAW_HIT_DRAW_H__
#define INTT_INTT_RAW_HIT_DRAW_H__

#include <qahtml/QADraw.h>

#include <map>

class SingleCanvasDrawer;

class INTTRawHitDraw : public QADraw
{
 public:
  INTTRawHitDraw(const std::string &name = "INTTRAWHITQA");
  ~INTTRawHitDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  std::map<std::string, SingleCanvasDrawer*> m_options;

};

#endif
