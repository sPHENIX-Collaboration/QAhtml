#include "INTTRawHitDraw.h"

#include "DrawFeeTiming.h"
#include "DrawBarrelHitmaps.h"
#include "DrawServerHitmaps.h"
#include "DrawTimingOkay.h"
#include "InttbcoDraw.h"
//...

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>
#include <qahtml/SingleCanvasDrawer.h>

#include <iostream>
#include <boost/format.hpp>

INTTRawHitDraw::INTTRawHitDraw(const std::string &name)
  : QADraw(name)
{
  m_options = {
    {"intt_barrel_hitmaps", new DrawBarrelHitmaps("intt_barrel_hitmaps")},
    {"intt_server_hitmaps", new DrawServerHitmaps("intt_server_hitmaps")},
    {"intt_timing_okay", new DrawTimingOkay("intt_timing_okay")},
    {"intt_bco_draw", new InttbcoDraw("intt_bco_draw")},
  };

  for(int felix_server = 0; felix_server < 8; ++felix_server)
  {
    std::string option_name = (boost::format("intt_%01d_timing") % felix_server).str();
    m_options[option_name] = new DrawFeeTiming(option_name, felix_server);
  }

  DBVarInit();

  return;
}

INTTRawHitDraw::~INTTRawHitDraw()
{
  // delete db;
  for(auto& [name, option] : m_options)
  {
    delete option;
  }

  return;
}

int INTTRawHitDraw::Draw(const std::string &what)
{
  // SetsPhenixStyle();

  int iret = 0;
  int idraw = 0;
  for(auto const& [name, option] : m_options)
  {
    if(what != "ALL" && what != name)continue;
    // I've seen people returning -1 on error instead of 1
    // Increment if the return value is nonzero
    iret += (option->DrawCanvas() != 0);
    ++idraw;
  }

  if(!idraw)
  {
    std::cerr << "Unimplemented drawing option:\n"
              << "\t" << what << "\n"
              << "Implemented options:\n"
              << "\tALL" << std::endl;
    for(auto const& [name, option] : m_options)
    {
        std::cerr << "\t" << name << std::endl;
    }
    ++iret;
  }

  return iret;
}
 
int INTTRawHitDraw::MakeHtml(const std::string &what)
{
  QADrawClient *cl = QADrawClient::instance();

  int iret = 0;
  int idraw = 0;
  for(auto const& [name, option] : m_options)
  {
    ++idraw;
    if(what != "ALL" && what != name)continue;

    // I've seen people returning -1 on error instead of 1
    // Increment if the return value is nonzero
    int rv = option->DrawCanvas() != 0;
    iret += rv;

    // on error no html output please
    if(rv || !option->GetCanvas())continue;

    // Registers the canvas png file to the menu and produces the png file
    std::string pngfile = cl->htmlRegisterPage(*this, name, std::to_string(idraw), "png");
    cl->CanvasToPng(option->GetCanvas(), pngfile);
  }

  if(!idraw)
  {
    std::cerr << "Unimplemented drawing option:\n"
              << "\t" << what << "\n"
              << "Implemented options:\n"
              << "\tALL" << std::endl;
    for(auto const& [name, option] : m_options)
    {
        std::cerr << "\t" << name << std::endl;
    }
    ++iret;
  }

  return iret;
}

int INTTRawHitDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
