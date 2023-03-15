#include "ScalerDBodbc.h"
#include "Scalers.h"

#include <odbc++/connection.h>
#include <odbc++/errorhandler.h>
#include <odbc++/drivermanager.h>
#include <odbc++/resultset.h>
#include <odbc++/preparedstatement.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

odbc::Connection *con = NULL;

ScalerDBodbc::ScalerDBodbc(const int runno):
  verbosity(0),
  runnumber(runno),
  dbname("daq"),
  dbowner("phnxrc"),
  dbpasswd(""),
  brunixtime(0),
  erunixtime(0)
{
  GetRunInfo();
}

ScalerDBodbc::~ScalerDBodbc()
{
  FlushScalerMap();
  delete con;
  con = NULL;
}

void
ScalerDBodbc::FlushScalerMap()
{
  while( scalermap.begin() != scalermap.end())
    {
      delete scalermap.begin()->second;
      scalermap.erase(scalermap.begin());
    }
  return;
}

void 
ScalerDBodbc::RunNumber(const int runno)
{
  runnumber = runno;
  FlushScalerMap();
  GetRunInfo();
  return;
}

int
ScalerDBodbc::GetConnection()
{
  if (con)
    {
      return 0;
    }
  try
    {
      con = odbc::DriverManager::getConnection(dbname.c_str(), dbowner.c_str(), dbpasswd.c_str());
    }
  catch (odbc::SQLException& e)
    {
      cout << __PRETTY_FUNCTION__
           << " Exception caught during DriverManager::getConnection" << endl;
      cout << "Message: " << e.getMessage() << endl;
      con = NULL;
      return -1;
    }
  return 0;
}

Scalers *
ScalerDBodbc::GetScalers(const std::string &triggername)
{
  map<std::string, Scalers *>::const_iterator iter = scalermap.find(triggername);
  if (iter != scalermap.end())
    {
      return iter->second;
    }
  if (GetConnection())
    {
      return NULL;
    }
  odbc::Statement* query = con->createStatement();
  ostringstream cmd;
  cmd << "SELECT scalergraw,scalerglive,scalergscaled, scalerupdateraw, scalerupdatelive, scalerupdatescaled from trigger where runnumber = "
      << runnumber << " and name= '" << triggername << "'";
  if (verbosity > 0)
    {
      cout << "sql cmd: " << cmd.str() << endl;
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
          scaled =  rs->getLong("scalergscaled");
	  if (verbosity > 0)
	    {
	      cout << "scalergraw: " << raw
		   << "scalerglive: " << live
		   << "scalergscaled: " << scaled
		   << endl;
	    }
        }
      else
        {
          raw = fabs(rs->getLong("scalerupdateraw"));
          live = fabs(rs->getLong("scalerupdatelive"));
          scaled =  fabs(rs->getLong("scalerupdatescaled"));
	  if (verbosity > 0)
	    {
	      cout << "scalerupdateraw: " << raw
		   << "scalerupdatelive: " << live
		   << "scalerupdatescaled: " << scaled
		   << endl;
	    }
        }
      Scalers *scal = new Scalers(raw, live, scaled);
      scalermap[triggername] = scal;
      return scal;
    }
  return NULL;
}

void
ScalerDBodbc::identify()
{
  cout << "DB Name: " << dbname;
  cout << " DB Owner: " << dbowner << endl;
  cout << "runnumber: " << runnumber << ", runtype: " << runtype << ", runstate: " << runstate << endl;
  cout << "Raw: " << ScalerRaw("Clock") << endl;
  return ;
}

int
ScalerDBodbc::GetRunInfo()
{
  if (GetConnection())
    {
      return -1;
    }

  odbc::ResultSet *rs = 0;
  ostringstream cmd;

  odbc::Statement *query = con->createStatement();
  cmd << "SELECT runtype,runstate,eventsinrun,brunixtime,erunixtime,updateunixtime FROM RUN WHERE RUNNUMBER = "
      << runnumber;
  if (verbosity > 0)
    {
      cout << "command: " << cmd.str() << endl;
    }
  try
    {
      rs = query->executeQuery(cmd.str());
    }
  catch (odbc::SQLException& e)
    {
      cout << "Exception caught" << endl;
      cout << "Message: " << e.getMessage() << endl;
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
      cout << " no information for run " << runnumber << endl;
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
