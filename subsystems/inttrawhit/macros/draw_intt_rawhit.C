#include <qahtml/QADrawClient.h>
#include <intt/INTTRawHitDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawinttrawhit.so)

void draw_intt_rawhit(const std::string &rootfile) {
  SetsPhenixStyle();
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
