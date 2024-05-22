#include "QADrawDBodbc.h"
#include "QADrawDBReturnCodes.h"
#include "QADrawDBVar.h"

#include <odbc++/connection.h>
#include <odbc++/databasemetadata.h>
#include <odbc++/drivermanager.h>
#include <odbc++/errorhandler.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>
#include <odbc++/setup.h>
#include <odbc++/types.h>
#include <sql.h>

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iostream>
#include <sstream>

static const unsigned int DEFAULTCOLUMNS = 4;  // standard columns (run, inserttime, startvaltime, endvaltime
// static const unsigned int COLUMNSPARVAR = 2;
static const std::string addvarname[2] = {"", "err"};

static odbc::Connection *con = nullptr;

#define VERBOSE

QADrawDBodbc::~QADrawDBodbc()
{
  if (con)
  {
    delete con;
    con = nullptr;
  }
}

int QADrawDBodbc::CheckAndCreateTable(const std::string &name, const std::map<const std::string, QADrawDBVar *> &varmap)
{
  if (GetConnection())
  {
    return -1;
  }
  std::string name_lowercase = name;
  transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), (int (*)(int)) tolower);
  int iret = 0;

  odbc::Statement *stmt = con->createStatement();
  // dumb but this seems to be the way to ask SQL if a table exists
  // (get first row, if it doesn't exist one gets the sql exception)
  std::ostringstream cmd;
  cmd << "SELECT * FROM " << name_lowercase << " LIMIT 1";
  odbc::ResultSet *rs = nullptr;
  try
  {
    rs = stmt->executeQuery(cmd.str());
  }
  catch (odbc::SQLException &e)
  {
    std::cout << "Table " << name_lowercase << " does not exist, will create it" << std::endl;
    //      std::cout << "Message: " << e.getMessage() << std::endl;
  }
  std::map<const std::string, QADrawDBVar *>::const_iterator iter;
  if (!rs)
  {
    cmd.str("");
    cmd << "CREATE TABLE " << name_lowercase << "(runnumber int NOT NULL, inserttime bigint NOT NULL, startvaltime bigint NOT NULL, endvaltime bigint NOT NULL, ";
    unsigned int ientry = 0;
    for (iter = varmap.begin(); iter != varmap.end(); ++iter)
    {
      cmd << iter->first << addvarname[0] << " real, ";
      cmd << iter->first << addvarname[1] << " real";
      ientry++;
      if (ientry < varmap.size())
      {
        cmd << ", ";
      }
    }
    cmd << ", primary key(runnumber,inserttime,startvaltime,endvaltime))";
#ifdef VERBOSE

    std::cout << "Executing " << cmd.str() << std::endl;
#endif

    iret = stmt->executeUpdate(cmd.str());
  }
  else
  {
    odbc::ResultSetMetaData *meta = rs->getMetaData();
    unsigned int nocolumn = rs->getMetaData()->getColumnCount();
    for (iter = varmap.begin(); iter != varmap.end(); ++iter)
    {
      std::string varname = iter->first;
      // column names are lower case only, so convert string to lowercase
      // The bizarre cast here is needed for newer gccs
      transform(varname.begin(), varname.end(), varname.begin(), (int (*)(int)) tolower);
      for (const auto &j : addvarname)
      {
        std::string thisvar = varname + j;
        for (unsigned int i = DEFAULTCOLUMNS + 1; i <= nocolumn; i++)
        {
          if (meta->getColumnName(i) == thisvar)
          {
#ifdef VERBOSE
            std::cout << thisvar << " is in table" << std::endl;
#endif

            goto foundvar;
          }
        }
        cmd.str("");
        cmd << "ALTER TABLE " << name_lowercase << " ADD COLUMN "
            << thisvar << " real";
#ifdef VERBOSE

        std::cout << "command: " << cmd.str() << std::endl;
#endif

        try
        {
          odbc::Statement *chgtable = con->createStatement();
          iret = chgtable->executeUpdate(cmd.str());
        }
        catch (odbc::SQLException &e)
        {
          std::cout << "Table " << name_lowercase << " update failed" << std::endl;
          std::cout << "Message: " << e.getMessage() << std::endl;
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

void QADrawDBodbc::identify() const
{
  std::cout << "DB Name: " << dbname << std::endl;
  std::cout << "DB Owner: " << dbowner << std::endl;
  std::cout << "DB Pwd: " << dbpasswd << std::endl;
  std::cout << "DB table: " << table << std::endl;
  return;
}

int QADrawDBodbc::AddRow(const int runnumber, const time_t inserttime, const time_t startvaltime, const time_t endvaltime, const std::map<const std::string, QADrawDBVar *> &varmap)
{
  std::map<const std::string, QADrawDBVar *>::const_iterator iter;
  int iret = 0;
  std::ostringstream cmd, cmd1;

  if (GetConnection())
  {
    return -1;
  }

  cmd.str("");
  cmd << "INSERT INTO " << table << "(runnumber, inserttime, startvaltime, endvaltime ";
  cmd1 << "VALUES(" << runnumber << ", " << inserttime
       << ", " << startvaltime << ", " << endvaltime;
  for (iter = varmap.begin(); iter != varmap.end(); ++iter)
  {
    for (unsigned int j = 0; j < 2; j++)
    {
      cmd << ", " << iter->first << addvarname[j];
      cmd1 << ", " << iter->second->GetVar(j);
    }
  }
  cmd << ") ";
  cmd1 << ")";
  cmd << cmd1.str();

#ifdef VERBOSE

  std::cout << cmd.str() << std::endl;
#endif

  odbc::Statement *stmt = con->createStatement();
  try
  {
    stmt->executeUpdate(cmd.str());
  }
  catch (odbc::SQLException &e)
  {
    const std::string &errmsg = e.getMessage();
    if (errmsg.find("Cannot insert a duplicate key into unique index") != std::string::npos)
    {
#ifdef VERBOSE
      std::cout << "Identical entry already in DB" << std::endl;
#endif

      iret = 0;
    }
    else
    {
      std::cout << __PRETTY_FUNCTION__ << " DB Error in execute stmt: " << e.getMessage() << std::endl;
      iret = -1;
    }
  }

  return iret;
}

int QADrawDBodbc::GetVar(const std::string &name, const time_t begin, const time_t end, const std::string &varname, std::vector<QADrawDBVar> &DBVars)
{
  if (GetConnection())
  {
    return DBNOCON;
  }
  int iret = 0;
  std::string name_lowercase = name;
  transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), (int (*)(int)) tolower);
  odbc::Statement *query = con->createStatement();
  std::string varnameerr = varname + addvarname[1];
  std::ostringstream cmd;
  cmd << "SELECT runnumber,inserttime,startvaltime,endvaltime,"
      << varname << "," << varnameerr << " FROM " << name_lowercase << " WHERE startvaltime >= " << begin
      << " and startvaltime <= " << end << " order by runnumber, inserttime";  // oldest one comes first so we can fill it into tgraph without resorting

#ifdef VERBOSE
  std::cout << "command: " << cmd.str() << std::endl;
#endif

  odbc::ResultSet *rs;
  try
  {
    rs = query->executeQuery(cmd.str());
  }
  catch (odbc::SQLException &e)
  {
    std::cout << "Exception caught, probably your table "
              << name_lowercase
              << " does not exist" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
    return -1;
  }
  iret = DBNOENTRIES;
  DBVars.clear();
  int lastrun = -1;
  while (rs->next())
  {
    QADrawDBVar newvar;
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

int QADrawDBodbc::GetConnection()
{
  if (con)
  {
    return 0;
  }
  try
  {
    con = odbc::DriverManager::getConnection(dbname.c_str(), dbowner.c_str(), dbpasswd.c_str());
  }
  catch (odbc::SQLException &e)
  {
    std::cout << " Exception caught during DriverManager::getConnection" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
    if (con)
    {
      delete con;
      con = nullptr;
    }
    return -1;
  }
  std::cout << "opened DB connection" << std::endl;
  return 0;
}
