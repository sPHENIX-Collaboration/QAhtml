#include "QADrawDB.h"
#include "QADrawDBVar.h"
#include "QADrawDBodbc.h"

#include <qahtml/QADrawClient.h>

#include <algorithm>
#include <iostream>
#include <sstream>

QADrawDB::QADrawDB(Fun4AllBase *caller)
  : Fun4AllBase(caller->Name())
{
  db = nullptr;
  return;
}

QADrawDB::~QADrawDB()
{
  delete db;
  while (varmap.begin() != varmap.end())
  {
    delete varmap.begin()->second;
    varmap.erase(varmap.begin());
  }
  return;
}

void QADrawDB::Print(const std::string & /*what*/) const
{
  std::cout << "QADrawDB Name: " << Name() << std::endl;
  std::map<const std::string, QADrawDBVar *>::const_iterator iter;
  for (iter = varmap.begin(); iter != varmap.end(); ++iter)
  {
    iter->second->Print();
  }
  return;
}

int QADrawDB::registerVar(const std::string &varname)
{
  std::string cpstring = varname;
  transform(cpstring.begin(), cpstring.end(), cpstring.begin(), (int (*)(int)) tolower);
  std::map<const std::string, QADrawDBVar *>::const_iterator iter = varmap.find(cpstring);
  if (iter != varmap.end())
  {
    std::cout << "Variable " << varname << " allready registered in DB" << std::endl;
    return -1;
  }
  varmap[cpstring] = new QADrawDBVar();
  return 0;
}

int QADrawDB::SetVar(const std::string &varname, const float var, const float varerr)
{
  float vararray[2];
  vararray[0] = var;
  vararray[1] = varerr;
  return SetVar(varname, vararray);
}

int QADrawDB::SetVar(const std::string &varname, const float var[2])
{
  std::string cpstring = varname;
  transform(cpstring.begin(), cpstring.end(), cpstring.begin(), (int (*)(int)) tolower);
  std::map<const std::string, QADrawDBVar *>::iterator iter = varmap.find(cpstring);
  if (iter != varmap.end())
  {
    iter->second->SetVar(var);
    return 0;
  }
  std::cout << " Could not find Variable " << varname << " in DB list" << std::endl;
  return -1;
}

int QADrawDB::DBInit()
{
  if (!db)
  {
    db = new QADrawDBodbc();
  }
  db->CheckAndCreateTable(Name(), varmap);
  return 0;
}

int QADrawDB::DBcommit()
{
  QADrawClient *cl = QADrawClient::instance();

  int iret = 0;
  if (!db)
  {
    std::cout << "Data Base not initialized, fix your code." << std::endl;
    std::cout << "You need to call DBInit() after you registered your variables" << std::endl;
    return -1;
  }
  time_t now = time(nullptr);
  iret = db->AddRow(cl->RunNumber(), now, cl->BeginRunUnixTime(), cl->EndRunUnixTime(), varmap);
  if (iret)
  {
    std::cout << " error in dbcommit" << std::endl;
    return iret;
  }
  // db->Dump();
  return iret;
}

int QADrawDB::GetVar(const time_t begin, const time_t end, const std::string &varname, std::vector<QADrawDBVar> &DBVars)
{
  if (!db)
  {
    db = new QADrawDBodbc();
  }
  int iret = db->GetVar(Name(), begin, end, varname, DBVars);
  return iret;
}
