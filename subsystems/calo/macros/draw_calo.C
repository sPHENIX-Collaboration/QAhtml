#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>
#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>

R__LOAD_LIBRARY(libemcNoisyTowerFinder.so)
R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {
  
  // Produce the dead/hot tower file
  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder","");
  std::string infile = rootfile;
  std::string rootfile_base = infile.substr(infile.find_last_of("/") + 1);
  std::string outfile = "deadHotTowers_" + rootfile_base;
  calo->FindHot(infile, outfile, "h_CaloValid_cemc_etaphi_wQA"); 

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new CaloDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  cl->ReadHistogramsFromFile(outfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  gSystem->Exit(0);
  return ;
}
