#ifndef __SCALERDBODBC_H__
#define __SCALERDBODBC_H__

#include <ctime>
#include <map>
#include <string>

class Scalers;

class ScalerDBodbc
{

 public:
  ScalerDBodbc(const int runnumber);
  virtual ~ScalerDBodbc();
  //  void Dump(const int nrows = 0) const;
  unsigned long long ScalerRaw(const std::string &triggername);
  unsigned long long ScalerLive(const std::string &triggername);
  unsigned long long ScalerScaled(const std::string &triggername);
  void identify();
  void Verbosity(const int i) {verbosity = i;}
  int GetRunInfo();
  void RunNumber(const int runnumber);
  unsigned int BeginRunTime() const {return brunixtime;}
  unsigned int EndRunTime() const {return erunixtime;}

 private:
  int GetConnection();
  Scalers * GetScalers(const std::string &triggername);
  void FlushScalerMap();
  int verbosity;
  int runnumber;
  std::string dbname;
  std::string dbowner;
  std::string dbpasswd;
  std::map<std::string, Scalers *> scalermap;
  std::string runtype;
  std::string runstate;
  time_t brunixtime;
  time_t erunixtime;

};

#endif
