#include <fun4all/Fun4AllUtils.h>
#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>

R__LOAD_LIBRARY(libqadrawcalo.so)

void goodrunlist_calo(const std::string &rootfile, const std::string emcal_goodrunfile="emcal_runlist.csv", const std::string ihcal_goodrunfile="ihcal_runlist.csv", const std::string ohcal_goodrunfile="ohcal_runlist.csv") {
  // Use the client to get some info
  QADrawClient *cl = QADrawClient::instance();
  cl->ReadHistogramsFromFile(rootfile);
  int runnum = cl->RunNumber();
  int n_events_db = cl->EventsInRun();
  std::string runtype = cl->RunType();
  std::cout << "Got runnum = " << runnum << std::endl;

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();
  // Perform good run checks
  std::cout << "Performing good run checks... ";
  ch->CemcCheckGoodRun();
  ch->ihcalCheckGoodRun();
  ch->ohcalCheckGoodRun();
  std::cout << "Done!" << std::endl;
  // Update triage database
  std::cout << "Updating run triage database..." << std::endl;
  ch->CaloWriteDB("emcal");
  std::cout << "EMCal done!" << std::endl;
  ch->CaloWriteDB("ihcal");
  std::cout << "iHCal done!" << std::endl;
  ch->CaloWriteDB("ihcal");
  ch->CaloWriteDB("ohcal");
  std::cout << "oHCal done!" << std::endl;
  ch->CaloWriteDB("ihcal");

  
  // Write to good run csv file

  // EMCal
  std::cout << "Writing EMCal file... ";
  ofstream of;
  of.open(emcal_goodrunfile.c_str(), ios::app);
  std::string outstr = "";  // run#, runtype, isGood, comments, #events(DB), #events(CaloValid), #dead, #cold, #hot, meantime, sigmatime, meanvtx, sigmavtx
  outstr += (std::to_string(runnum) + ",");
  outstr += (runtype + ",");
  outstr += (std::to_string(ch->cemc_isgood) + ",");
  std::string comments = ch->CemcGetComments();
  comments += ",";
  outstr += comments;
  outstr += (std::to_string(n_events_db) + ",");
  outstr += (std::to_string(ch->n_events) + ",");
  outstr += (std::to_string(ch->cemc_dead_towers) + ",");
  outstr += (std::to_string(ch->cemc_cold_towers) + ",");
  outstr += (std::to_string(ch->cemc_hot_towers) + ",");
  outstr += (std::to_string(ch->cemc_time_mean) + ",");
  outstr += (std::to_string(ch->cemc_time_sigma) + ",");
  outstr += (std::to_string(ch->vtxz_mean) + ",");
  outstr += (std::to_string(ch->vtxz_sigma));
  if (of)
  {
    of << outstr << std::endl;
  }
  else
  {
    std::cout << "Problem opening " << emcal_goodrunfile << "! Did not write." << std::endl;
  }
  of.close();
  std::cout << "Done!" << std::endl;

  // iHCal
  std::cout << "Writing iHCal file... ";
  of.open(ihcal_goodrunfile.c_str(), ios::app);
  outstr = "";  // run#, runtype, isGood, comments, #events(DB), #events(CaloValid), #dead, #cold, #hot, meantime, sigmatime, meanvtx, sigmavtx
  outstr += (std::to_string(runnum) + ",");
  outstr += (runtype + ",");
  outstr += (std::to_string(ch->ihcal_isgood) + ",");
  comments = ch->IhcalGetComments();
  comments += ",";
  outstr += comments;
  outstr += (std::to_string(n_events_db) + ",");
  outstr += (std::to_string(ch->n_events) + ",");
  outstr += (std::to_string(ch->ihcal_dead_towers) + ",");
  outstr += (std::to_string(ch->ihcal_cold_towers) + ",");
  outstr += (std::to_string(ch->ihcal_hot_towers) + ",");
  outstr += (std::to_string(ch->ihcal_time_mean) + ",");
  outstr += (std::to_string(ch->ihcal_time_sigma) + ",");
  outstr += (std::to_string(ch->vtxz_mean) + ",");
  outstr += (std::to_string(ch->vtxz_sigma));
  if (of)
  {
    of << outstr << std::endl;
  }
  else
  {
    std::cout << "Problem opening " << ihcal_goodrunfile << "! Did not write." << std::endl;
  }
  of.close();
  std::cout << "Done!" << std::endl;

  // oHCal
  std::cout << "Writing oHCal file... ";
  of.open(ohcal_goodrunfile.c_str(), ios::app);
  outstr = "";  // run#, runtype, isGood, comments, #events(DB), #events(CaloValid), #dead, #cold, #hot, meantime, sigmatime, meanvtx, sigmavtx
  outstr += (std::to_string(runnum) + ",");
  outstr += (runtype + ",");
  outstr += (std::to_string(ch->ohcal_isgood) + ",");
  comments = ch->OhcalGetComments();
  comments += ",";
  outstr += comments;
  outstr += (std::to_string(n_events_db) + ",");
  outstr += (std::to_string(ch->n_events) + ",");
  outstr += (std::to_string(ch->ohcal_dead_towers) + ",");
  outstr += (std::to_string(ch->ohcal_cold_towers) + ",");
  outstr += (std::to_string(ch->ohcal_hot_towers) + ",");
  outstr += (std::to_string(ch->ohcal_time_mean) + ",");
  outstr += (std::to_string(ch->ohcal_time_sigma) + ",");
  outstr += (std::to_string(ch->vtxz_mean) + ",");
  outstr += (std::to_string(ch->vtxz_sigma));
  if (of)
  {
    of << outstr << std::endl;
  }
  else
  {
    std::cout << "Problem opening " << ohcal_goodrunfile << "! Did not write." << std::endl;
  }
  of.close();
  std::cout << "Done!" << std::endl;

  ch->DeleteHotColdDeadMaps();
  delete cl;
  gSystem->Exit(0);
  return;
}
