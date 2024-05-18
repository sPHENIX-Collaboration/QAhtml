#include "ScalerDBodbc.h"
#include "Scalers.h"

#include <odbc++/connection.h>
#include <odbc++/drivermanager.h>
#include <odbc++/errorhandler.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/resultset.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

odbc::Connection *con = nullptr;

ScalerDBodbc::ScalerDBodbc(const int runno)
  : verbosity(0)
  , runnumber(runno)
  , dbname("daq")
  , dbowner("phnxrc")
  , dbpasswd("")
  , brunixtime(0)
  , erunixtime(0)
{
  GetRunInfo();
}

ScalerDBodbc::~ScalerDBodbc()
{
  FlushScalerMap();
  delete con;
  con = nullptr;
}

void ScalerDBodbc::FlushScalerMap()
{
  while (scalermap.begin() != scalermap.end())
  {
    delete scalermap.begin()->second;
    scalermap.erase(scalermap.begin());
  }
  return;
}

void ScalerDBodbc::RunNumber(const int runno)
{
  runnumber = runno;
  FlushScalerMap();
  GetRunInfo();
  return;
}

int ScalerDBodbc::GetConnection()
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
    std::cout << __PRETTY_FUNCTION__
              << " Exception caught during DriverManager::getConnection" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
    con = nullptr;
    return -1;
  }
  return 0;
}

Scalers *
ScalerDBodbc::GetScalers(const std::string &triggername)
{
  std::map<std::string, Scalers *>::const_iterator iter = scalermap.find(triggername);
  if (iter != scalermap.end())
  {
    return iter->second;
  }
  if (GetConnection())
  {
    return nullptr;
  }
  odbc::Statement *query = con->createStatement();
  std::ostringstream cmd;
  cmd << "SELECT scalergraw,scalerglive,scalergscaled, scalerupdateraw, scalerupdatelive, scalerupdatescaled from trigger where runnumber = "
      << runnumber << " and name= '" << triggername << "'";
  if (verbosity > 0)
  {
    std::cout << "sql cmd: " << cmd.str() << std::endl;
  }
  odbc::ResultSet *rs = query->executeQuery(cmd.str());
  if (rs && rs->next())
  {
    unsigned long long raw;
    unsigned long long live;
    unsigned long long scaled;
    if (runstate == "ENDED")
    {
      raw = rs->getLong("scalergraw");
      live = rs->getLong("scalerglive");
      scaled = rs->getLong("scalergscaled");
      if (verbosity > 0)
      {
        std::cout << "scalergraw: " << raw
                  << "scalerglive: " << live
                  << "scalergscaled: " << scaled
                  << std::endl;
      }
    }
    else
    {
      raw = std::abs(rs->getLong("scalerupdateraw"));
      live = std::abs(rs->getLong("scalerupdatelive"));
      scaled = std::abs(rs->getLong("scalerupdatescaled"));
      if (verbosity > 0)
      {
        std::cout << "scalerupdateraw: " << raw
                  << "scalerupdatelive: " << live
                  << "scalerupdatescaled: " << scaled
                  << std::endl;
      }
    }
    Scalers *scal = new Scalers(raw, live, scaled);
    scalermap[triggername] = scal;
    return scal;
  }
  return nullptr;
}

void ScalerDBodbc::identify()
{
  std::cout << "DB Name: " << dbname;
  std::cout << " DB Owner: " << dbowner << std::endl;
  std::cout << "runnumber: " << runnumber << ", runtype: " << runtype << ", runstate: " << runstate << std::endl;
  std::cout << "Raw: " << ScalerRaw("Clock") << std::endl;
  return;
}

int ScalerDBodbc::GetRunInfo()
{
  if (GetConnection())
  {
    return -1;
  }

  odbc::ResultSet *rs = nullptr;
  std::ostringstream cmd;

  odbc::Statement *query = con->createStatement();
  cmd << "SELECT runtype,runstate,eventsinrun,brunixtime,erunixtime,updateunixtime FROM RUN WHERE RUNNUMBER = "
      << runnumber;
  if (verbosity > 0)
  {
    std::cout << "command: " << cmd.str() << std::endl;
  }
  try
  {
    rs = query->executeQuery(cmd.str());
  }
  catch (odbc::SQLException &e)
  {
    std::cout << "Exception caught" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
  }
  if (rs && rs->next())
  {
    runtype = rs->getString("runtype");
    runstate = rs->getString("runstate");
    brunixtime = rs->getLong("brunixtime");
    if (runstate == "ENDED")
    {
      erunixtime = rs->getLong("erunixtime");
    }
    else
    {
      erunixtime = rs->getLong("updateunixtime");
    }
  }
  else
  {
    std::cout << " no information for run " << runnumber << std::endl;
    return -1;
  }

  return 0;
}

unsigned long long
ScalerDBodbc::ScalerRaw(const std::string &triggername)
{
  Scalers *scal = GetScalers(triggername);
  if (!scal)
  {
    return 0;
  }
  return scal->ScalerRaw();
}

unsigned long long
ScalerDBodbc::ScalerLive(const std::string &triggername)
{
  Scalers *scal = GetScalers(triggername);
  if (!scal)
  {
    return 0;
  }
  return scal->ScalerLive();
}

unsigned long long
ScalerDBodbc::ScalerScaled(const std::string &triggername)
{
  Scalers *scal = GetScalers(triggername);
  if (!scal)
  {
    return 0;
  }
  return scal->ScalerScaled();
}
