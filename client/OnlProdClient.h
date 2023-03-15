#ifndef ONLPRODCLIENT_H__
#define ONLPRODCLIENT_H__

#include <fun4all/Fun4AllBase.h>
#include <map>
#include <set>
#include <string>
#include <vector>

class ClientHistoList;
class OnlProdDraw;
class OnlProdHtml;
class OnlProdRunDBodbc;
class TCanvas;
class TH1;
class TNamed;
class TStyle;

class OnlProdClient: public Fun4AllBase
{
 public:
  static OnlProdClient *instance();
  virtual ~OnlProdClient();
  void Print(const std::string &what = "ALL");
  TNamed *getHisto(const std::string &hname);
  void registerHisto(const char *hname, const char *subsys);

  void registerDrawer(OnlProdDraw *Drawer);
  int ReadHistogramsFromFile(const std::string &filename);
  int GetHistoList(std::set<std::string> &histolist);
  void updateHistoMap(const char *hname, TNamed *h1d);
  int Draw(const char *who = "ALL", const char *what = "ALL");
  int MakeHtml(const char *who = "ALL", const char *what = "ALL");

  std::string htmlRegisterPage(const OnlProdDraw& drawer,
			       const std::string& path,
			       const std::string& basefilename,
			       const std::string& ext);
  
  void htmlAddMenu(const OnlProdDraw& drawer, const std::string& path,
		   const std::string& relfilename);

  void htmlNamer(const OnlProdDraw& drawer, const std::string& basefilename,
		 const std::string& ext, std::string& fullfilename,
		 std::string& filename);

  void VerbosityAll(const int v);

  int RunNumber() const {return runnumber;}
  void RunNumber(const int runno);

  void SetDisplaySizeX(const int xsize) { display_sizex = xsize;}
  void SetDisplaySizeY(const int ysize) { display_sizey = ysize;}
  int GetDisplaySizeX() {return display_sizex;}
  int GetDisplaySizeY() {return display_sizey;}
  int CanvasToPng(TCanvas *canvas, std::string const &filename);
  int HistoToPng(TH1 *histo, std::string const &pngfilename, const char *drawopt = "", const int statopt = 11);

  int SaveLogFile(const OnlProdDraw& drawer);
  int ExtractRunNumber(const std::string &fname);

  std::string RunType();
  int EventsInRun();
  time_t BeginRunUnixTime();
  time_t EndRunUnixTime();

 private:
  OnlProdClient();
  int DoSomething(const char *who, const char *what, const char *opt);
  void InitAll();

  static OnlProdClient *__instance;
  OnlProdHtml* fHtml;
  OnlProdRunDBodbc *rdb;
  int runnumber;
  int display_sizex;
  int display_sizey;
  TStyle *defaultStyle;
  std::map<std::string, ClientHistoList *> Histo;
  std::map<std::string, OnlProdDraw*> DrawerList;
};

#endif /* __ONCALCLIENT_H__ */
