#include <qahtml/QADrawClient.h>
#include <qahtml/kfparticle/KFParticleDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawkfparticle.so)

void draw_kfp(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new KFParticleDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
