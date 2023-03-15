#include "OnlProdDB.h"
#include "OnlProdDBVar.h"
#include "OnlProdDBodbc.h"
#include <qahtml/OnlProdClient.h>

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

OnlProdDB::OnlProdDB(Fun4AllBase *caller): Fun4AllBase(caller->Name())
{
  
  db = 0;
  return ;
}

OnlProdDB::~OnlProdDB()
{
  delete db;
  while(varmap.begin() != varmap.end())
    {
      delete varmap.begin()->second;
      varmap.erase(varmap.begin());
    }
  return;
}


void
OnlProdDB::Print() const
{
  cout << "OnlProdDB Name: " << Name() << endl;
  map<const std::string, OnlProdDBVar *>::const_iterator iter;
  for (iter = varmap.begin(); iter != varmap.end(); iter++)
    {
      iter->second->Print();
    }
  return ;
}

int 
OnlProdDB::registerVar(const string &varname)
{
  string cpstring = varname;
  transform(cpstring.begin(), cpstring.end(), cpstring.begin(), (int(*)(int))tolower);
  map<const string,OnlProdDBVar *>::const_iterator iter = varmap.find(cpstring);
  if (iter != varmap.end())
    {
      cout << "Variable " << varname << " allready registered in DB" << endl;
      return -1;
    }
  varmap[cpstring] = new OnlProdDBVar();
  return 0;
}

int 
OnlProdDB::SetVar(const std::string &varname, const float var, const float varerr)
{
  float vararray[2];
  vararray[0] = var;
  vararray[1] = varerr;
  return SetVar(varname,vararray);
}

int 
OnlProdDB::SetVar(const string &varname, const float var[2])
{
  string cpstring = varname;
  transform(cpstring.begin(), cpstring.end(), cpstring.begin(), (int(*)(int))tolower);
  map<const string,OnlProdDBVar *>::iterator iter = varmap.find(cpstring);
  if (iter != varmap.end())
    {
      iter->second->SetVar(var);
      return 0;
    }
  cout << " Could not find Variable " << varname << " in DB list" << endl;
  return -1;
}

int
OnlProdDB::DBInit()
{
  if (!db)
    {
      db = new OnlProdDBodbc();
    }
  db->CheckAndCreateTable(Name(),varmap);
  return 0;
}

int 
OnlProdDB::DBcommit()
{
  OnlProdClient *cl = OnlProdClient::instance();
  
  int iret = 0;
  if (!db)
    {
      cout << "Data Base not initialized, fix your code." << endl;
      cout << "You need to call DBInit() after you registered your variables" << endl;
      return -1;
    }
  time_t now = time(0);
  iret = db->AddRow(cl->RunNumber(), now, cl->BeginRunUnixTime(), cl->EndRunUnixTime(), varmap);
  if (iret)
    {
      cout << " error in dbcommit" << endl;
      return iret;
    }
  //db->Dump();
  return iret;
}

int
OnlProdDB::GetVar(const time_t begin, const time_t end, const std::string &varname, std::vector<OnlProdDBVar> &DBVars)
{
  if (!db)
    {
      db = new OnlProdDBodbc();
    }
  int iret = db->GetVar(Name(), begin, end, varname, DBVars);
  return iret;
}

