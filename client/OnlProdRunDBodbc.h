#ifndef ONLPRODRUNDBODBC_H__
#define ONLPRODRUNDBODBC_H__

#include <ctime>
#include <set>
#include <string>

class OnlProdRunDBodbc
{

 public:
  OnlProdRunDBodbc();
  OnlProdRunDBodbc(const int runnumber);
  virtual ~OnlProdRunDBodbc() {}
  //  void Dump(const int nrows = 0) const;
  void identify() const;
  void Verbosity(const int i) {verbosity = i;}
  void FillFromdaqDB(const int runnumber);

  std::string RunType() {return runtype;}
  int EventsInRun() {return eventsinrun;}
  time_t BeginRunUnixTime() {return brunixtime;}
  time_t EndRunUnixTime() {return erunixtime;}

 private:
  int runnumber;
  int verbosity;
  std::string dbname;
  std::string dbowner;
  std::string dbpasswd;
  std::string runtype;
  int eventsinrun;
  time_t brunixtime;
  time_t erunixtime;
};

#endif
