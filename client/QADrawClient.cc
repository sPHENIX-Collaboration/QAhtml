#include "QADrawClient.h"
#include "QADraw.h"
#include "QAHtml.h"
#include "QARunDBodbc.h"
#include "ClientHistoList.h"

#include <TCanvas.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TGFrame.h>
#include <TH1.h>
#include <TImage.h>
#include <TIterator.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <sys/utsname.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <algorithm>

using namespace std;

QADrawClient *QADrawClient::__instance = NULL;

QADrawClient *QADrawClient::instance()
{
  if (__instance)
    {
      return __instance;
    }
  __instance = new QADrawClient();
  return __instance;
}

QADrawClient::QADrawClient(): 
  Fun4AllBase("ONCALCLIENT"),
  fHtml(NULL),
  rdb(NULL),
  runnumber(0)
{
  for (int i = 0; i < 15; i++)
    {
      gSystem->IgnoreSignal((ESignals)i);
    }
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  defaultStyle = gStyle;
  InitAll();
}

void
QADrawClient::InitAll()
{
  if (!gClient)
    {
      utsname ThisNode;
      uname(&ThisNode);
      cout << "virtual framebuffer not running on " << ThisNode.nodename
	   << ", check if process /usr/X11R6/bin/Xvfb is alive" << endl;
      exit(1);
    }

  fHtml = new QAHtml(getenv("QA_HTMLDIR"));
  TGFrame* rootWin = (TGFrame*) gClient->GetRoot();
  display_sizex = rootWin->GetDefaultWidth();
  display_sizey = rootWin->GetDefaultHeight();
  return ;
}

QADrawClient::~QADrawClient()
{
  delete fHtml;
  delete rdb;
  while(Histo.begin() != Histo.end())
    {
      delete Histo.begin()->second;
      Histo.erase(Histo.begin());
    }
  return ;
}

void
QADrawClient::registerHisto(const char *hname, const char *subsys)
{
  map<string, ClientHistoList *>::const_iterator histoiter = Histo.find(hname);
  if (histoiter != Histo.end())
    {
#ifdef DEBUG
      cout << "deleting histogram " << hname << " at " << Histo[hname] << endl;
#endif

      histoiter->second->SubSystem(subsys);
    }
  else
    {
      ClientHistoList *newhisto = new ClientHistoList();
      newhisto->SubSystem(subsys);
      Histo[hname] = newhisto;
#ifdef DEBUG

      cout << "new histogram " << hname << " at " << Histo[hname] << endl;
#endif

    }
  return ;
}

void
QADrawClient::registerDrawer(QADraw *Drawer)
{
  const char *DrawerName = Drawer->Name().c_str();
  map<string, QADraw *>::iterator iter = DrawerList.find(DrawerName);
  if (iter != DrawerList.end())
    {
      cout << "Drawer " << DrawerName << " already registered, I won't overwrite it" << endl;
      cout << "Use a different name and try again" << endl;
    }
  else
    {
      DrawerList[DrawerName] = Drawer;
      Drawer->Init();
    }
  defaultStyle->cd();
  return ;
}

int QADrawClient::Draw(const char *who, const char *what)
{
  return DoSomething(who, what, "DRAW");
}

int QADrawClient::MakeHtml(const char *who, const char *what)
{
  mode_t old_umask = 0;
  char *onlprod_real_html = getenv("ONLPROD_REAL_HTML");
  if (! onlprod_real_html)
    {
      old_umask = umask(S_IWOTH);
      cout << "Making html output group writable so others can run tests as well" << endl;
    }
  fHtml->runNumber(RunNumber()); // do not forget this !
  fHtml->RunType(RunType());
  int iret =  DoSomething(who, what, "HTML");
  if (! onlprod_real_html)
    {
      umask(old_umask);
    }
  return iret;
}

int QADrawClient::DoSomething(const char *who, const char *what, const char *opt)
{
  int i = 0;

  map<string, QADraw *>::iterator iter;
  if (strcmp(who, "ALL"))
    {
      iter = DrawerList.find(who);
      if (iter != DrawerList.end())
        {
          if (!strcmp(opt, "DRAW"))
            {
              iter->second->Draw(what);
            }
          else if (!strcmp(opt, "HTML"))
            {
              if (Verbosity() > 0)
                {
                  cout << " creating html output for "
		       << iter->second->Name() << endl;
                }
              if ( iter->second->MakeHtml(what))
                {
                  cout << "subsystem " << iter->second->Name()
		       << " not in root file, skipping" << endl;
                }
            }
          defaultStyle->cd();
          return 0;
        }
      else
        {
          cout << "Drawer " << who << " not in list" << endl;
          Print("DRAWER");
          return -1;
        }
    }
  else
    {
      for (iter = DrawerList.begin(); iter != DrawerList.end(); ++iter)
        {
          if (!strcmp(opt, "DRAW"))
            {
              i += iter->second->Draw(what);
            }
          else if (!strcmp(opt, "HTML"))
            {
              if (Verbosity() > 0)
                {
                  cout << " creating html output for "
		       << iter->second->Name() << endl;
                }
              gROOT->Reset();
              int iret = iter->second->MakeHtml(what);
              if (iret)
                {
                  cout << "subsystem " << iter->second->Name()
		       << " not in root file, skipping" << endl;
                  // delete all canvases (no more piling up of 50 canvases)
                  // if run for a single subsystem this leaves the canvas intact
                  // for debugging
                  TSeqCollection* allCanvases = gROOT->GetListOfCanvases();
                  TCanvas *canvas = NULL;
                  while ((canvas = (TCanvas *) allCanvases->First()))
                    {
                      cout << "Deleting Canvas " << canvas->GetName() << endl;
                      delete canvas;
                    }
                }
              i += iret;
            }
          defaultStyle->cd(); // restore default style
        }
    }
  return 0;
}


void
QADrawClient::updateHistoMap(const char *hname, TNamed *h1d)
{
  map<string, ClientHistoList *>::const_iterator histoiter = Histo.find(hname);
  if (histoiter != Histo.end())
    {
#ifdef DEBUG
      cout << "deleting histogram " << hname << " at " << Histo[hname] << endl;
#endif

      delete histoiter->second->Histo(); // delete old histogram
      histoiter->second->Histo(h1d);
    }
  else
    {
      ClientHistoList *newhisto = new ClientHistoList();
      newhisto->Histo(h1d);
      Histo[hname] = newhisto;
#ifdef DEBUG

      cout << "new histogram " << hname << " at " << Histo[hname] << endl;
#endif

    }
  return ;
}

TNamed *
QADrawClient::getHisto(const string &hname)
{
  map<string, ClientHistoList *>::const_iterator histoiter = Histo.find(hname);
  if (histoiter != Histo.end())
    {
      return histoiter->second->Histo();
    }
  return NULL;
}

void
QADrawClient::Print(const string &what)
{
  if (what == "ALL" || what == "DRAWER")
    {
      // loop over the map and print out the content (name and location in memory)
      cout << "--------------------------------------" << endl << endl;
      cout << "List of Drawers in QADrawClient:" << endl;

      map<string, QADraw*>::const_iterator hiter;
      for (hiter = DrawerList.begin(); hiter != DrawerList.end(); ++hiter)
        {
          cout << hiter->first << " is at " << hiter->second << endl;
        }
      cout << endl;
    }
  if (what == "ALL" || what == "HISTOS")
    {
      // loop over the map and print out the content (name and location in memory)
      cout << "--------------------------------------" << endl << endl;
      cout << "List of Histograms in QADrawClient:" << endl;

      map<string, ClientHistoList*>::const_iterator hiter;
      for (hiter = Histo.begin(); hiter != Histo.end(); ++hiter)
        {
          cout << hiter->first << " Address " << hiter->second->Histo()
	       << ", subsystem " << hiter->second->SubSystem() << endl;
        }
      cout << endl;
    }
  return ;
}


int
QADrawClient::ReadHistogramsFromFile(const string &filename)
{
  TDirectory* save = gDirectory; // save current dir (which will be overwritten by the following fileopen)
  TFile *histofile = new TFile(filename.c_str(), "READ");
  if (! histofile)
    {
      cout << "Can't open " << filename << endl;
      return -1;
    }
  save->cd();

  RunNumber(ExtractRunNumber(filename));
  TIterator *titer = histofile->GetListOfKeys()->MakeIterator();
  TObject *obj;
  TNamed *histo, *histoptr;
  while ((obj = titer->Next()))
    {
      if (Verbosity() > 0)
        {
          cout << "TObject at " << obj << endl;
          cout << obj->GetName() << endl;
          cout << obj->ClassName() << endl;
        }
      histofile->GetObject(obj->GetName(), histoptr);
      if (histoptr)
        {
          histo = (TNamed *) (histoptr->Clone());
          updateHistoMap(histo->GetName(), histo);
          if (Verbosity() > 0)
            {
              cout << "HistoName: " << histo->GetName() << endl;
              cout << "HistoClass: " << histo->ClassName() << endl;
            }
        }
    }
  delete histofile;
  delete titer;
  return 0;
}

int
QADrawClient::GetHistoList(set<string> &histolist)
{
  map<string, ClientHistoList *>::const_iterator iter;
  for (iter = Histo.begin(); iter != Histo.end(); ++iter)
    {
      histolist.insert(iter->first);
    }
  return 0;
}

//_____________________________________________________________________________
void
QADrawClient::VerbosityAll(const int v)
{
  Verbosity(v);
  if ( fHtml )
    {
      fHtml->verbosity(v);
    }
}

//_____________________________________________________________________________
void
QADrawClient::htmlAddMenu(const QADraw& drawer,
                         const string& path,
                         const string& relfilename)
{
  fHtml->addMenu(drawer.Name(), path, relfilename);
}

//_____________________________________________________________________________
void
QADrawClient::htmlNamer(const QADraw& drawer,
                       const string& basefilename,
                       const string& ext,
                       string& fullfilename,
                       string& filename)
{
  fHtml->namer(drawer.Name(), basefilename, ext, fullfilename, filename);
}

//_____________________________________________________________________________
string
QADrawClient::htmlRegisterPage(const QADraw& drawer,
                              const string& path,
                              const string& basefilename,
                              const string& ext)
{
  return fHtml->registerPage(drawer.Name(), path, basefilename, ext);
}

int
QADrawClient::CanvasToPng(TCanvas *canvas, std::string const &pngfilename)
{
  // in order not to collide when running multiple html generators
  // create a unique filename (okay tempnam is not totally safe against
  // multiple procs getting the same name but the local /tmp filesystem should
  // prevent at least multiple machines colliding)
  char *tmpname = tempnam("/tmp", "TC");
  canvas->Print(tmpname, "gif"); // write gif format
  TImage *img = TImage::Open(tmpname);
  img->WriteImage(pngfilename.c_str());
  delete img;
  remove(tmpname);
  free(tmpname);
  return 0;
}

int
QADrawClient::HistoToPng(TH1 *histo, std::string const &pngfilename, const char *drawopt, const int statopt)
{
  TCanvas *cgiCanv = new TCanvas("cgiCanv", "cgiCanv", 200, 200, 650, 500);
  gStyle->SetOptStat(statopt);
  cgiCanv->SetFillColor(0);
  cgiCanv->SetBorderMode(0);
  cgiCanv->SetBorderSize(2);
  cgiCanv->SetFrameFillColor(0);
  cgiCanv->SetFrameBorderMode(0);
  cgiCanv->SetTickx();
  cgiCanv->SetTicky();
  cgiCanv->cd();
  histo->SetMarkerStyle(8);
  histo->SetMarkerSize(0.15);
  histo->Draw(drawopt);
  // returned char array from tempnam() needs to be free'd after use
  char *tmpname = tempnam("/tmp", "HI");
  cgiCanv->Print(tmpname,"gif");
  TImage *img = TImage::Open(tmpname);
  img->WriteImage(pngfilename.c_str());
  remove(tmpname);
  free(tmpname);
  delete cgiCanv;
  return 0;
}

int
QADrawClient::SaveLogFile(const QADraw& drawer)
{
  // sendfile example shamelessly copied from
  // http://www.linuxgazette.com/issue91/tranter.html
  ostringstream logfilename, msg;
  const char *logdir = getenv("ONCAL_LOGDIR");
  if (logdir)
    {
      logfilename << logdir << "/";
    }
  int irun = RunNumber();
  logfilename << drawer.Name() << "_" << irun << ".log" << ends;
  int src;              /* file descriptor for source file */
  int dest;             /* file descriptor for destination file */
  struct stat stat_buf; /* hold information about input file */
  off_t offset = 0;     /* byte offset used by sendfile */
  src = open(logfilename.str().c_str(), O_RDONLY);
  if (src != -1)
    {
      fstat(src, &stat_buf);
      string outfile = htmlRegisterPage(drawer, "Logfile", "log", "txt");
      dest = open(outfile.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode);
      sendfile(dest, src, &offset, stat_buf.st_size);
      close(dest);
      close(src);
    }
  return 0;
}

int
QADrawClient::ExtractRunNumber(const std::string &filename)
{
  int runno = 0;
  boost::char_separator<char> sep("_"); 
  boost::tokenizer<boost::char_separator<char> > tok(filename,sep);
  boost::tokenizer<boost::char_separator<char> >::iterator tokiter = tok.begin();
  
  ++tokiter;
  try
    {
      runno = boost::lexical_cast<int>(*tokiter);
    }
  catch ( boost::bad_lexical_cast const& )
    {
      cout << "Cannot extract run number from filename "
	   << filename << endl;
      cout << "Segment string after parsing: input string " 
           << *tokiter
	   << " is not valid run number" << endl;
      cout << "filename " << filename << " not standard Run_<runno>_XXX.ext"
	   << endl;
    }

  return runno;
}

void
QADrawClient::RunNumber(const int runno)
{
  if (runnumber != runno)
    {
      if (rdb)
        {
          rdb->FillFromdaqDB(runno);
        }
      else
        {
          rdb = new QARunDBodbc(runno);
        }
      runnumber = runno;
    }
}

string
QADrawClient::RunType()
{
  if (!rdb)
    {
      rdb = new QARunDBodbc();
    }
  return rdb->RunType();
}

int
QADrawClient::EventsInRun()
{
  if (!rdb)
    {
      rdb = new QARunDBodbc();
    }
  return rdb->EventsInRun();
}

time_t
QADrawClient::BeginRunUnixTime()
{
  if (!rdb)
    {
      rdb = new QARunDBodbc();
    }
  return rdb->BeginRunUnixTime();
}

time_t
QADrawClient::EndRunUnixTime()
{
  if (!rdb)
    {
      rdb = new QARunDBodbc();
    }
  return rdb->EndRunUnixTime();
}
