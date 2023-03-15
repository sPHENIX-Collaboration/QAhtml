#include "OnlProdDBodbc.h"
#include "OnlProdDBVar.h"
#include "OnlProdDBReturnCodes.h"

#include <odbc++/connection.h>
#include <odbc++/setup.h>
#include <odbc++/types.h>
#include <odbc++/errorhandler.h>
#include <sql.h>
#include <odbc++/drivermanager.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/databasemetadata.h>

#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace odbc;
using namespace std;

static const unsigned int DEFAULTCOLUMNS = 4; // standard columns (run, inserttime, startvaltime, endvaltime 
static const unsigned int COLUMNSPARVAR = 2;
static const string addvarname[2] = {"", "err"};

static Connection *con = 0;

#define VERBOSE

OnlProdDBodbc::OnlProdDBodbc()
{
  dbname = "OnlProdDB";
  dbowner = "phnxrc";
  dbpasswd = "";
  table = "aargh";
}

OnlProdDBodbc::~OnlProdDBodbc()
{
  if (con)
    {
      delete con;
      con = 0;
    }
}

int
OnlProdDBodbc::CheckAndCreateTable(const string &name, const map<const string, OnlProdDBVar *> &varmap)
{
  if (GetConnection())
    {
      return -1;
    }
  string name_lowercase = name;
  transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), (int(*)(int))tolower);
  int iret = 0;

  Statement* stmt = con->createStatement();
  // dumb but this seems to be the way to ask SQL if a table exists
  // (get first row, if it doesn't exist one gets the sql exception)
  ostringstream cmd;
  cmd << "SELECT * FROM " << name_lowercase << " LIMIT 1" ;
  ResultSet *rs = NULL;
  try
    {
      rs = stmt->executeQuery(cmd.str());
    }
  catch (SQLException& e)
    {
      cout << "Table " << name_lowercase << " does not exist, will create it" << endl;
      //      cout << "Message: " << e.getMessage() << endl;
    }
  map<const string, OnlProdDBVar *>::const_iterator iter;
  if (! rs)
    {
      cmd.str("");
      cmd << "CREATE TABLE " << name_lowercase << "(runnumber int NOT NULL, inserttime bigint NOT NULL, startvaltime bigint NOT NULL, endvaltime bigint NOT NULL, ";
      unsigned int ientry = 0;
      for (iter = varmap.begin(); iter != varmap.end(); iter++)
        {
          cmd << iter->first << addvarname[0] << " real, ";
          cmd << iter->first << addvarname[1] << " real";
          ientry++;
          if (ientry < varmap.size())
            {
              cmd << ", ";
            }
        }
      cmd << ", primary key(runnumber,inserttime,startvaltime,endvaltime))" ;
#ifdef VERBOSE

      cout << "Executing " << cmd.str() << endl;
#endif

      iret = stmt->executeUpdate(cmd.str());

    }
  else
    {
      ResultSetMetaData* meta = rs->getMetaData();
      unsigned int nocolumn = rs->getMetaData()->getColumnCount();
      for (iter = varmap.begin(); iter != varmap.end(); iter++)
        {
          string varname = iter->first;
          // column names are lower case only, so convert string to lowercase
          // The bizarre cast here is needed for newer gccs
          transform(varname.begin(), varname.end(), varname.begin(), (int(*)(int))tolower);
          for (short int j = 0;j < 2;j++)
            {
              string thisvar = varname + addvarname[j];
              for (unsigned int i = DEFAULTCOLUMNS + 1; i <= nocolumn;i++)
                {
                  if (meta->getColumnName(i) == thisvar)
                    {
#ifdef VERBOSE
                      cout << thisvar << " is in table" << endl;
#endif

                      goto foundvar;
                    }
                }
              cmd.str("");
              cmd << "ALTER TABLE " << name_lowercase << " ADD COLUMN "
		  << thisvar << " real";
#ifdef VERBOSE

              cout << "command: " << cmd.str() << endl;
#endif

              try
                {
                  Statement* chgtable = con->createStatement();
                  iret = chgtable->executeUpdate(cmd.str());
                }
              catch (SQLException& e)
                {
                  cout << "Table " << name_lowercase << " update failed" << endl;
                  cout << "Message: " << e.getMessage() << endl;
                }

	    foundvar:
              continue;
            }
        }

      // check columns
    }
  table = name_lowercase;
  return iret;
}

void
OnlProdDBodbc::identify() const
{
  cout << "DB Name: " << dbname << endl;
  cout << "DB Owner: " << dbowner << endl;
  cout << "DB Pwd: " << dbpasswd << endl;
  cout << "DB table: " << table << endl;
  return ;
}

int
OnlProdDBodbc::AddRow(const int runnumber, const time_t inserttime, const time_t startvaltime, const time_t endvaltime, const map<const string, OnlProdDBVar *> &varmap)
{
  map<const string, OnlProdDBVar *>::const_iterator iter;
  int iret = 0;
  ostringstream cmd, cmd1;

  if (GetConnection())
    {
      return -1;
    }

  cmd.str("");
  cmd << "INSERT INTO " << table << "(runnumber, inserttime, startvaltime, endvaltime ";
  cmd1 << "VALUES(" << runnumber << ", " << inserttime 
<< ", " << startvaltime << ", " << endvaltime;
  for (iter = varmap.begin(); iter != varmap.end(); iter++)
    {
      for (unsigned int j = 0; j < 2; j++)
        {
          cmd << ", " << iter->first << addvarname[j];
          cmd1 << ", " << iter->second->GetVar(j);
        }
    }
  cmd << ") ";
  cmd1 << ")";
  cmd << cmd1.str() ;

#ifdef VERBOSE

  cout << cmd.str() << endl;
#endif

  Statement* stmt = con->createStatement();
  try
    {
      stmt->executeUpdate(cmd.str());
    }
  catch (SQLException& e)
    {
      string errmsg = e.getMessage();
      if (errmsg.find("Cannot insert a duplicate key into unique index") != string::npos)
        {
#ifdef VERBOSE
          cout << "Identical entry already in DB" << endl;
#endif

          iret = 0;
        }
      else
        {
          cout << __PRETTY_FUNCTION__ << " DB Error in execute stmt: " << e.getMessage() << endl;
          iret = -1;
        }
    }

  return iret;
}

int
OnlProdDBodbc::GetVar(const string &name, const time_t begin, const time_t end, const std::string &varname, std::vector<OnlProdDBVar> &DBVars)
{
  if (GetConnection())
    {
      return DBNOCON;
    }
  int iret = 0;
  string name_lowercase = name;
  transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), (int(*)(int))tolower);
  Statement* query = con->createStatement();
  string varnameerr = varname + addvarname[1];
  ostringstream cmd;
  cmd << "SELECT runnumber,inserttime,startvaltime,endvaltime,"
      << varname << "," << varnameerr << " FROM " << name_lowercase << " WHERE startvaltime >= " << begin
      << " and startvaltime <= " << end << " order by runnumber, inserttime"; // oldest one comes first so we can fill it into tgraph without resorting

#ifdef VERBOSE
  cout << "command: " << cmd.str() << endl;
#endif

  ResultSet *rs;
  try
    {
      rs = query->executeQuery(cmd.str());
    }
  catch (SQLException& e)
    {
      cout << "Exception caught, probably your table "
	   << name_lowercase
	   << " does not exist" << endl;
      cout << "Message: " << e.getMessage() << endl;
      return -1;
    }
  iret = DBNOENTRIES;
  DBVars.clear();
  int lastrun = -1;
  while (rs->next())
    {
      OnlProdDBVar newvar;
      int thisrun = rs->getInt("runnumber");
      if (thisrun == lastrun)
	{
	  continue;
	}
      lastrun = thisrun;
      newvar.RunNumber(thisrun);
      newvar.Startvaltime(rs->getLong("startvaltime"));
      newvar.Endvaltime(rs->getLong("endvaltime"));
      newvar.Value(rs->getFloat(varname));
      newvar.Error(rs->getFloat(varnameerr));
      DBVars.push_back(newvar);
      iret = 0;
    }
  return iret;
}

int
OnlProdDBodbc::GetConnection()
{
  if (con)
    {
      return 0;
    }
  try
    {
      con = DriverManager::getConnection(dbname.c_str(), dbowner.c_str(), dbpasswd.c_str());
    }
  catch (SQLException& e)
    {
      cout << " Exception caught during DriverManager::getConnection" << endl;
      cout << "Message: " << e.getMessage() << endl;
      if (con)
        {
          delete con;
	  con = 0;
        }
      return -1;
    }
  cout << "opened DB connection" << endl;
  return 0;
}
