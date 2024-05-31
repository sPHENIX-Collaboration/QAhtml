#include <qahtml/QADrawClient.h>
#include <micromegas/MicromegasDraw.h>

R__LOAD_LIBRARY(libqadrawmicromegas.so)

void draw_micromegas(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new MicromegasDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
