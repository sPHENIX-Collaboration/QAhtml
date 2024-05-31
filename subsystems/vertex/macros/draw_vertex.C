#include <qahtml/QADrawClient.h>
#include <vertex/VertexDraw.h>

R__LOAD_LIBRARY(libqadrawvertex.so)

void draw_vertex(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new VertexDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
