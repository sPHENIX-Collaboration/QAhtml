#ifndef QA_DATABASE_QADRAWDB_H
#define QA_DATABASE_QADRAWDB_H

#include "QADrawDBVar.h"

#include <fun4all/Fun4AllBase.h>

#include <ctime>
#include <map>
#include <string>
#include <vector>

class QADraw;
class QADrawDBVar;
class QADrawDBodbc;

class QADrawDB: public Fun4AllBase
{
 public:
  QADrawDB(Fun4AllBase *caller);
  virtual ~QADrawDB();

  int registerVar(const std::string &varname);
  int SetVar(const std::string &varname, const float var, const float varerr);
  int SetVar(const std::string &varname, const float var[2]);
  int DBcommit();
  int DBInit();
  void Print() const;
  int GetVar(const time_t begin, const time_t end, const std::string &varname, std::vector<QADrawDBVar> &DBVars);

 protected:
  std::map<const std::string, QADrawDBVar *> varmap;
  QADrawDBodbc *db {nullptr};
};

#endif //QA_DATABASE_QADrawDB_H
