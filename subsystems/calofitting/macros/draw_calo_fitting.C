#include <qahtml/QADrawClient.h>

#include <calofitting/CaloFittingDraw.h>

#include <sPhenixStyle.C>

R__LOAD_LIBRARY(libqadrawcalofitting.so)

void draw_calo_fitting(const std::string &rootfile = "/sphenix/data/data02/sphnxpro/QAhtml/aggregated/HHIST_CALOFITTINGQA_run3auau_new_newcdbtag_v005-00066624-9000.root") 
{

  SetsPhenixStyle();

  QADrawClient *cl = QADrawClient::instance();
  // cl->Verbosity(1);

  CaloFittingDraw *ex = new CaloFittingDraw();
  cl->registerDrawer(ex);
  cl->ReadHistogramsFromFile(rootfile);
  cl->MakeHtml();

  delete cl;

  gSystem->Exit(0);
  
  return ;
}
