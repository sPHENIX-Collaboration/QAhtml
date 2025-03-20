#include "INTTDraw.h"

#include "INTTChipDrawer.h"
#include "INTTClusterDrawer.h"
//...

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>
#include <qahtml/SingleCanvasDrawer.h>

#include <iostream>

INTTDraw::INTTDraw(const std::string &name)
  : QADraw(name)
{
  m_options["CHIP"] =    new INTTChipDrawer   ("intt_chip_info");
  m_options["CLUSTER"] = new INTTClusterDrawer("intt_cluster_info");
  //...

  DBVarInit();

  return;
}

INTTDraw::~INTTDraw()
{
  // delete db;
  for(auto& [name, option] : m_options)
  {
    delete option;
  }

  return;
}

int INTTDraw::Draw(const std::string &what)
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
 
int INTTDraw::MakeHtml(const std::string &what)
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

int INTTDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
