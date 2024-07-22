#include <qahtml/QADrawClient.h>
#include <intt/INTTRawHitDraw.h>

R__LOAD_LIBRARY(libqadrawinttrawhit.so)

void draw_intt_rawhit(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new INTTRawHitDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
