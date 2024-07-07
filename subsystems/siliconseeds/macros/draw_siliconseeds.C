#include <qahtml/QADrawClient.h>
#include "siliconseeds/SiliconSeedsDraw.h"
#include "siliconseeds/SiSeedsGoodRunChecker.h"
#include "siliconseeds/OfflineQAKSTest.h"

R__LOAD_LIBRARY(libqadrawsiliconseeds.so)

void draw_siliconseeds(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  SiliconSeedsDraw *ex = new SiliconSeedsDraw();
  cl->registerDrawer(ex);

  // Set up KS test
  int refrunnumber = 47332;
  std::string prodtag = "2024p004";
  std::string reffile = "/sphenix/data/data02/sphnxpro/QAhtml/aggregated/HIST_DST_TRKR_SEED_run2pp_new_"+prodtag+"-000"+std::to_string(refrunnumber)+"-9000.root";
  auto *qakstest = new OfflineQAKSTest(rootfile, reffile);
  qakstest->AddHistogramNames({"h_SiliconSeedsQA_avgnclus_eta_phi",
                               "h_SiliconSeedsQA_dcaxyorigin_phi",
                               "h_SiliconSeedsQA_dcaxyvtx_phi",
                               "h_SiliconSeedsQA_dcazorigin_phi",
                               "h_SiliconSeedsQA_dcazvtx_phi",
                               "h_SiliconSeedsQA_nintt",
                               "h_SiliconSeedsQA_nmaps",
                               "h_SiliconSeedsQA_nmaps_nintt",
                               "h_SiliconSeedsQA_nrecotracks",
                               "h_SiliconSeedsQA_nrecotracks1d",
                               "h_SiliconSeedsQA_nrecovertices",
                               "h_SiliconSeedsQA_ntrack_IsPosCharge",
                               "h_SiliconSeedsQA_ntrack_isfromvtx",
                               "h_SiliconSeedsQA_ntrackspervertex",
                               "h_SiliconSeedsQA_trackcrossing",
                               "h_SiliconSeedsQA_trackpt",
                               "h_SiliconSeedsQA_trackpt_neg",
                               "h_SiliconSeedsQA_trackpt_pos",
                               "h_SiliconSeedsQA_vertexchi2dof",
                               "h_SiliconSeedsQA_vertexcrossing",
                               "h_SiliconSeedsQA_vt",
                               "h_SiliconSeedsQA_vx",
                               "h_SiliconSeedsQA_vx_vy",
                               "h_SiliconSeedsQA_vy",
                               "h_SiliconSeedsQA_vz"});
  TH1D *hm_kssumary = qakstest->GenKSTestSummary();

  SiSeedsGoodRunChecker* ch = new SiSeedsGoodRunChecker();
  ch->SetKSTestSummary(hm_kssumary);
  bool siseeds_isgood = ch->SiSeedsGoodRun();
  TCanvas* siseeds_summ = ch->SiSeedsMakeSummary(cl->ExtractRunNumber(rootfile), siseeds_isgood); 
  ex->SetSiSeedsSummary(cl->ExtractRunNumber(rootfile), siseeds_summ);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
