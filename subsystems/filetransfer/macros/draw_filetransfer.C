#include <qahtml/QADrawClient.h>
#include <filetransfer/FileTransfer.h>
//#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawfiletransfer.so)

void draw_filetransfer(const int runnumber) {
  //SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new FileTransfer();
  cl->registerDrawer(ex);

  cl->RunNumber(runnumber);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
