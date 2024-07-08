#include <fun4all/Fun4AllUtils.h>
#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>

R__LOAD_LIBRARY(libqadrawcalo.so)

void goodrunlist_calo(const std::string &rootfile, const std::string goodrunfile="emcal_runlist.csv") {
  // Use the client to get some info
  QADrawClient *cl = QADrawClient::instance();
  cl->ReadHistogramsFromFile(rootfile);
  int runnum = cl->RunNumber();
  int n_events_db = cl->EventsInRun();
  std::cout << "Got runnum = " << runnum << std::endl;

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();
  bool cemc_isgood = ch->CemcGoodRun();

  // Write to good run file
  ofstream of;
  of.open(goodrunfile.c_str(), ios::app);
  std::string outstr = "";  // run#, isGood, #events(DB), #events(CaloValid), #dead, #cold, #hot, meantime, sigmatime, meanvtx, sigmavtx
  outstr += (std::to_string(runnum) + ",");
  outstr += (std::to_string(cemc_isgood) + ",");
  std::string failurepoint;
  if (cemc_isgood) failurepoint = "GoodRun";
  else failurepoint = "";
  if (ch->cemc_fails_vertex) failurepoint += "BadVertex";
  if (ch->cemc_fails_timing) failurepoint += "BadTiming";
  if (ch->cemc_fails_badtowers) failurepoint += "BadTowers";
  if (ch->cemc_fails_events) failurepoint += "NotEnoughEvents";
  outstr += failurepoint;
  /* outstr += (std::to_string(n_events_db) + ","); */
  /* outstr += (std::to_string(ch->n_events) + ","); */
  /* outstr += (std::to_string(ch->cemc_dead_towers) + ","); */
  /* outstr += (std::to_string(ch->cemc_cold_towers) + ","); */
  /* outstr += (std::to_string(ch->cemc_hot_towers) + ","); */
  /* outstr += (std::to_string(ch->cemc_time_mean) + ","); */
  /* outstr += (std::to_string(ch->cemc_time_sigma) + ","); */
  /* outstr += (std::to_string(ch->vtxz_mean) + ","); */
  /* outstr += (std::to_string(ch->vtxz_sigma)); */
  if (of)
  {
    of << outstr << std::endl;
  }

  ch->DeleteHotColdDeadMaps();
  delete cl;
  gSystem->Exit(0);
  return;
}
