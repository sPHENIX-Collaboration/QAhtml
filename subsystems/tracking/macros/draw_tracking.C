#include <qahtml/QADrawClient.h>
#include <tracking/TrackingDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawtracking.so)

void draw_tracking(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new TrackingDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
