#ifndef QA_DATABASE_QADRAWODBC_H
#define QA_DATABASE_QADRAWODBC_H

#include "QADrawDBVar.h"
#include <ctime>
#include <map>
#include <string>
#include <vector>

class QADrawDBVar;

class QADrawDBodbc
{

 public:
  QADrawDBodbc() = default;
  virtual ~QADrawDBodbc();
  int CheckAndCreateTable(const std::string &name, const std::map<const std::string, QADrawDBVar *> &varmap);
  void identify() const;
  int AddRow(const int runnumber, const time_t inserttime, const time_t startvaltime, const time_t endvaltime, const std::map<const std::string, QADrawDBVar *> &varmap);
  int GetVar(const std::string &name, const time_t begin, const time_t end, const std::string &varname, std::vector<QADrawDBVar> &DBVars);
  
 private:
  int GetConnection();
  std::string dbname {"QADrawDB"};
  std::string dbowner {"phnxrc"};
  std::string dbpasswd {""};
  std::string table {"aargh"};
};

#endif
