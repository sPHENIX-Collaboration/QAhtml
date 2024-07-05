#include <qahtml/QADrawClient.h>
#include <trackseeds/SiliconSeedsDraw.h>

R__LOAD_LIBRARY(libqadrawsiliconseeds.so)

void draw_siliconseeds(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new SiliconSeedsDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
