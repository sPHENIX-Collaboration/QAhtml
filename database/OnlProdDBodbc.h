#ifndef ONLPRODDBODBC_H__
#define ONLPRODDBODBC_H__

#include "OnlProdDBVar.h"
#include <ctime>
#include <map>
#include <string>
#include <vector>

class OnlProdDBVar;

class OnlProdDBodbc
{

 public:
  OnlProdDBodbc();
  virtual ~OnlProdDBodbc();
  int CheckAndCreateTable(const std::string &name, const std::map<const std::string, OnlProdDBVar *> &varmap);
  void identify() const;
  int AddRow(const int runnumber, const time_t inserttime, const time_t startvaltime, const time_t endvaltime, const std::map<const std::string, OnlProdDBVar *> &varmap);
  int GetVar(const std::string &name, const time_t begin, const time_t end, const std::string &varname, std::vector<OnlProdDBVar> &DBVars);
  
 private:
  int GetConnection();
  std::string dbname;
  std::string dbowner;
  std::string dbpasswd;
  std::string table;
};

#endif
