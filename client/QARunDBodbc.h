#ifndef QA_CLIENT_QARUNDBODBC_H
#define QA_CLIENT_QARUNDBODBC_H

#include <ctime>
#include <set>
#include <string>

class QARunDBodbc
{

 public:
  QARunDBodbc();
  QARunDBodbc(const int runnumber);
  virtual ~QARunDBodbc() {}
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

#endif // QA_CLIENT_QARUNDBODBC_H
