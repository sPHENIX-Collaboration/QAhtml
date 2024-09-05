#ifndef SINGLE_CANVAS_DRAWER_H
#define SINGLE_CANVAS_DRAWER_H

#include <string>
#include <TCanvas.h>

class SingleCanvasDrawer
{
public:
  SingleCanvasDrawer(std::string const&);
  virtual ~SingleCanvasDrawer();

  std::string GetName() {return m_name;}
  TCanvas* GetCanvas() {return m_canvas;}

  virtual int DrawCanvas();

protected:
  virtual int MakeCanvas();

  TCanvas* m_canvas{nullptr};
  std::string m_name;
};

#endif//SINGLE_CANVAS_DRAWER_H
