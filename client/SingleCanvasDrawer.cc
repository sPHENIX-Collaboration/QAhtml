#include "SingleCanvasDrawer.h"
#include "QADrawClient.h"

#include <phool/phool.h>

#include <TCanvas.h>
#include <TROOT.h>
#include <TSystem.h>

#include <boost/format.hpp>

#include <iostream>

SingleCanvasDrawer::SingleCanvasDrawer(std::string const& name)
  : m_name{name}
{
  // Do nothing
}

SingleCanvasDrawer::~SingleCanvasDrawer()
{
  delete gROOT->FindObject(m_name.c_str());
}

int SingleCanvasDrawer::MakeCanvas(int width, int height)
{
  TObject* found_object = gROOT->FindObject(m_name.c_str());
  if(found_object && found_object == m_canvas)
  {
    /// No redraw is necessary
    return 1;
  }

  if(found_object)
  {
    /// There is a TCanvas called m_name somewhere, but it's not our m_canvas
    std::cerr << PHWHERE << "\n"
              << "\tMultiple instances of classes trying to manage '" << m_name << "' are running together\n"
              << "\tFound existing TObject at: " << (boost::format("0x%08x") % found_object).str() << ")\n"
              << "\tNot the member TCanvas at: " << (boost::format("0x%08x") %     m_canvas).str() << ")\n"
              << "\tDeleting found TCanvas at: " << (boost::format("0x%08x") % found_object).str() << ")" << std::endl;
    delete found_object;
  }

  QADrawClient *cl = QADrawClient::instance();
  if (width <= 0) width = cl->GetDisplaySizeX();
  if (height <= 0) height = cl->GetDisplaySizeY();

  // Running the macro in batch mode -b causes the rootWindow to have size 0
  // These guard clauses should really be in the QADrawClient implementation
  // Batch mode also causes TASImage::WriteImage errors
  // So this shouldn't be run in batch mode at all, anyways
  if(width <= 0)
  {
    width = 1920;
    std::cerr << PHWHERE << "\n"
              << "\tQADrawClient::GetDisplaySizeX returned <= 0\n"
              << "\tUsing width = " << width << std::endl;
  }
  if(height <= 0)
  {
    height = 1080;
    std::cerr << PHWHERE << "\n"
              << "\tQADrawClient::GetDisplaySizeY returned <= 0\n"
              << "\tUsing height = " << height << std::endl;
  }

  m_canvas = new TCanvas(m_name.c_str(), m_name.c_str(), -1, 0, width, height);
  gSystem->ProcessEvents();

  return 0;
}

int SingleCanvasDrawer::DrawCanvas()
{
  MakeCanvas();
  m_canvas->SetEditable(true);

  //...

  m_canvas->Update();
  m_canvas->Show();
  m_canvas->SetEditable(false);

  return 0;
}
