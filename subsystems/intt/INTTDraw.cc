#include "INTTDraw.h"

#include "INTTChipDrawer.h"
#include "INTTClusterDrawer.h"
//...

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLatex.h>
#include <TColor.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

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
    iret += option->DrawCanvas();
	++idraw;
  }

  if(!idraw)
  {
    std::cerr << "Unimplemented Drawing option:\n"
	          << "\t" << what << "\n"
	          << "Implemented options:" << std::endl;
	for(auto const& [name, option] : m_options)
	{
		std::cerr << "\t" << name << std::endl;
	}
  }

  return iret;
}
 
int INTTDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
  if (iret) // on error no html output please
  {
    return iret;
  }

  QADrawClient *cl = QADrawClient::instance();
  std::string pngfile;

  // Register the 1st canvas png file to the menu and produces the png file.

  int idraw = 0;
  for(auto const& [name, option] : m_options)
  {
    ++idraw;
    pngfile = cl->htmlRegisterPage(*this, name, std::to_string(idraw), "png");
    cl->CanvasToPng(option->GetCanvas(), pngfile);
  }

  return 0;
}

int INTTDraw::DBVarInit()
{
  /* db = new QADrawDB(this); */
  /* db->DBInit(); */
  return 0;
}
