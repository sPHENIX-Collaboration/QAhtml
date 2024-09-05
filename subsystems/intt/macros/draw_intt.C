#include <qahtml/QADrawClient.h>
#include <intt/INTTDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawintt.so)

void draw_intt(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new INTTDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
