#ifndef ONLPRODDB_H__
#define ONLPRODDB_H__

#include "OnlProdDBVar.h"
#include <fun4all/Fun4AllBase.h>

#include <ctime>
#include <map>
#include <string>
#include <vector>

class OnlProd;
class OnlProdDraw;
class OnlProdDBVar;
class OnlProdDBodbc;

class OnlProdDB: public Fun4AllBase
{
 public:
  OnlProdDB(Fun4AllBase *caller);
  virtual ~OnlProdDB();

  int registerVar(const std::string &varname);
  int SetVar(const std::string &varname, const float var, const float varerr);
  int SetVar(const std::string &varname, const float var[2]);
  int DBcommit();
  int DBInit();
  void Print() const;
  int GetVar(const time_t begin, const time_t end, const std::string &varname, std::vector<OnlProdDBVar> &DBVars);

 protected:
  std::map<const std::string, OnlProdDBVar *> varmap;
  OnlProdDBodbc *db;
};

#endif
